#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Client.h"
#include <process.h>
#include <stdio.h>
#include "Lista.h"
// Para ToLower
#include <algorithm>

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#ifdef WIN32 
#define ClearScreen() system("cls");
#define pause() system("pause");
#else 
#define ClearScreen() system("clear");
#define pause() system("pause");
#endif

Client UnCliente;
bool clienteAbierto = true;
bool serverStatus = true;
bool IsUsuarioLogueado = false;
string usuarioLogueado;

Lista<std::string>* TodosLosUsuarios = new Lista<std::string>();

struct EnvioThreadData
{
	int opcion;
	string mensaje;
	string destinatario;
};

void ThreadEnviaMensaje(void* pParams)
{
	int opcion;
	string mensaje;
	string destinatario;
	EnvioThreadData* datos = (EnvioThreadData*)pParams;
	opcion = datos->opcion;
	mensaje = datos->mensaje;
	destinatario = datos->destinatario;

	string respuestaServer;
	if (opcion == 1)
	{
		UnCliente.EnviarMensaje(destinatario, 15);
		UnCliente.EnviarMensajeTamanoVariable(mensaje);
		UnCliente.EscribirLog("Mensaje enviado a " + destinatario + ". Mensaje: " + mensaje);
	}
	if (opcion == 2)
	{
		UnCliente.EnviarMensajeTamanoVariable(mensaje);
		UnCliente.EscribirLog("Mensaje enviado a todos los usuarios. Mensaje: " + mensaje);
	}
}


void ThreadStatus(void* pParams)
{
	Client ClientePing;
	//Envía mensajes al servidor y setea la variable serverStatus en TRUE o FALSE
	bool status = false;
	bool errorYaLogueado = false;
	bool serverCaido = false;
	int resultado = 0;
	EnvioThreadData* datos = (EnvioThreadData*) pParams;

	string ip = datos->destinatario;
	string puerto = datos->mensaje;

	ClientePing.ConectarAServidor(ip, puerto); //Envío primero puerto e IP.

	while (clienteAbierto && !serverCaido)
	{

		if (ClientePing.EnviarMensaje("PING",4) == -1)//si el servidor no nos responde (enviarMensaje retorna -1)
		{
			status = false;
			serverCaido = true;
		}

		else
		{
			string respuesta = ClientePing.RecibirMensaje(2);
			if (respuesta == "OK")
			{
				status = true;
				
			}
			else status = false;
		}
				
		if (!status && !errorYaLogueado)// si el server se cayo lo logueo una sola vez
		{
			ClientePing.EscribirLog("Conexion con el servidor terminada. (Server Offline).");
			errorYaLogueado = true;
		}
		serverStatus = status;
		Sleep(30000);
	}
	if(serverStatus) ClientePing.EnviarMensaje("EXIT", 4); //solo tiene sentido enviar un mensaje de exit del cliente si el server no esta caido!!
	
}


void MostrarListaUsuarios() {

	cout << "Lista de usuarios:" << endl << endl;

	TodosLosUsuarios->iniciarCursor();

	while (TodosLosUsuarios->avanzarCursor())
	{
		cout << TodosLosUsuarios->obtenerCursor() << endl;;
	}

	cout << endl;
}

bool ValidarUsuario(string UnUsuario) {

	// Convierte nombre de usuario a LowerCase
	transform(UnUsuario.begin(), UnUsuario.end(), UnUsuario.begin(), (int(*)(int))tolower);

	TodosLosUsuarios->iniciarCursor();
	while (TodosLosUsuarios->avanzarCursor())
	{
		string OtroUsuario = TodosLosUsuarios->obtenerCursor();
		// Convierte nombre de usuario a LowerCase
		transform(OtroUsuario.begin(), OtroUsuario.end(), OtroUsuario.begin(), (int(*)(int))tolower);

		if (UnUsuario == OtroUsuario) {

			return true;
		}
	}

	cout << "Usuario inexistente, por favor vuelva a intentar" << endl;

	return false;
}

void ArmarListaUsuarios(string TodosLosUsuariosEnString) {
	string UnUsuario;
	int index = TodosLosUsuariosEnString.find(";");

	while (index > -1) {
		UnUsuario = TodosLosUsuariosEnString.substr(0, index);
		TodosLosUsuariosEnString = TodosLosUsuariosEnString.substr(index + 1);
		index = TodosLosUsuariosEnString.find(";");
		TodosLosUsuarios->agregar(UnUsuario);
	}
}

void IniciarSesion()
{
	string Mensaje;
	string Respuesta;

	Mensaje = "CHKU";
	UnCliente.EnviarMensaje(Mensaje, 4);
	Respuesta = UnCliente.RecibirMensaje(2);

	if (Respuesta == "NO") {

		Mensaje = "AUTH";
		UnCliente.EnviarMensaje(Mensaje, 4);

		cout << "Ingrese Usuario: ";
		cin >> Mensaje;
		usuarioLogueado = Mensaje;
		UnCliente.EnviarMensaje(Mensaje, 15);

		cout << "Ingrese clave: ";
		cin >> Mensaje;
		UnCliente.EnviarMensaje(Mensaje, 15);

		Respuesta = UnCliente.RecibirMensaje(3); // Codigo de respuesta de la autenticacion
		if (Respuesta == "000")
		{
			IsUsuarioLogueado = true;
			UnCliente.EscribirLog("Usuario autorizado por el servidor.");
		}

		Respuesta = UnCliente.RecibirMensaje(40);
		cout << Respuesta << endl;
		pause();

	}
	else {

		cout << "Ya se encuentra loggeado al servidor" << endl;
		pause();
	}

}

void CerrarSesion() {
	string mensaje = "OUT";
	UnCliente.EnviarMensaje(mensaje, 4);

	string respuesta = UnCliente.RecibirMensaje(40);
	cout << respuesta << endl;
	UnCliente.EscribirLog("Cerrar sesion. Mensaje del servidor: " + respuesta + ".");
	IsUsuarioLogueado = false;
	pause();
}

void MenuDestinatarioMensaje(EnvioThreadData* datosOpcionEnvio)
{
	int opcion = 0;
	string destinatario;
	string mensaje;

	while((opcion < 1) || (opcion > 2))
	{
		ClearScreen();
		cout << "Seleccione destinatario" << endl << endl
		<< "1- Destinatario unico" << endl
		<< "2- Todos los usuarios" << endl;
		cin >> opcion;
		datosOpcionEnvio->opcion = opcion;
	}
	
	switch(opcion)
	{
	case 1:
	{
		bool UsuarioValido = false;
		UnCliente.EnviarMensaje("ENVI", 4);
		
		MostrarListaUsuarios();

		while (!UsuarioValido) {
			cout << "Ingrese nombre de usuario del destinatario:" << endl;
			cin >> destinatario;
			UsuarioValido = ValidarUsuario(destinatario);
		}

		datosOpcionEnvio->destinatario = destinatario;
		cin.ignore();
		cout << "Ingrese el mensaje que desea enviar:" << endl;
		getline(cin, mensaje);
		datosOpcionEnvio->mensaje = mensaje;

		break;

	}
	case 2:

		UnCliente.EnviarMensaje("ENVT", 4);
		
		cin.ignore();
		cout << "Ingrese el mensaje que desea enviar a todos los usuarios:" << endl;
		getline(cin, mensaje);
		datosOpcionEnvio->mensaje = mensaje;

		break;
	}
}

void EnviarMensaje() {

	string Mensaje = "CHKU";
	UnCliente.EnviarMensaje(Mensaje, 4);
	string Respuesta = UnCliente.RecibirMensaje(2);

	if (Respuesta == "SI") {

		EnvioThreadData* datosOpcionEnvio = new EnvioThreadData;
		MenuDestinatarioMensaje(datosOpcionEnvio);

		_beginthread(ThreadEnviaMensaje, 0, (void*)datosOpcionEnvio);
		cout << "Mensaje enviado." << endl;
		pause();
	}
	else {
		cout << "Debe iniciar sesion para poder enviar mensajes" << endl;
		pause();
	}


}

void RecibirMensajes()
{
	int CantMensajes;
	string respuestaServer;
	string tamanioEmisor,tamanioMensaje;
	UnCliente.EnviarMensaje("REC", 3);

	//El servidor responde la cantidad de mensajes
	respuestaServer = UnCliente.RecibirMensaje(8);

	if (respuestaServer != "NOLOGIN") {
		CantMensajes = stoi(respuestaServer);

		cout << "Usted tiene " << CantMensajes << " mensajes" << endl << endl;
		UnCliente.EscribirLog(CantMensajes + " mensajes recibidos.");

		while (CantMensajes > 0)
		{
			string UsuarioEmisor = UnCliente.RecibirMensaje(15);
			string ContenidoMensaje = UnCliente.RecibirMensajeTamanoVariable();

			cout << "De: " << UsuarioEmisor << endl;
			cout << "Mensaje: " << ContenidoMensaje << endl << endl;
			UnCliente.EscribirLog("Mensaje de " + UsuarioEmisor + " recibido.");

			CantMensajes--;
		}
	} else {
		cout << "Tiene que iniciar sesion para poder recibir mensajes" << endl;
	}

	pause();
}

string obtenerDestinatario()
{
	string destinatario;
	
	UnCliente.EnviarMensaje("USER", 4);
	destinatario = UnCliente.RecibirMensaje(15);

	return destinatario;
}

string replace(char* lectura, const std::string& from, const std::string& to) {
	string str(lectura);
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return str;
	str.replace(start_pos, from.length(), to);
	return str;
}

void LoremIpsum()
{
	int frecuencia;
	int cantidad;
	int milisegundos;
	char* lectura;
	int tamanio = 0;

	string Mensaje;
	string Respuesta;

	Mensaje = "CHKU";
	UnCliente.EnviarMensaje(Mensaje, 4);
	Respuesta = UnCliente.RecibirMensaje(2);

	if (Respuesta == "SI") {

		srand(time(NULL));
		tamanio = rand() % 201; //Random entre 1 y 200, tamaño máximo del mensaje a enviar.

		lectura = new char[tamanio];

		string destinatario = obtenerDestinatario();

		UnCliente.EscribirLog("Secuencia de envio automatico iniciada.");

		FILE *archivoLoremIpsum;
		archivoLoremIpsum = fopen("loremIpsum.txt","rb"); //el directorio actual era tallerdeprogramacion/Cliente. Ahi hay que tirar el archivo

		if (archivoLoremIpsum) {

			ClearScreen();

			cout << "Se enviaran los mensajes al usuario " << destinatario << endl;

			cout << "Ingrese la frecuencia de envio de mensajes: ";
			cin >> frecuencia;
			cout << "Ingrese la cantidad de mensajes a enviar: ";
			cin >> cantidad;

			milisegundos = (1000 / frecuencia);

			cout << "Se enviara un mensaje cada " << milisegundos << " milisegundos." << endl;

			int i = 0;
			while (i < cantidad) {
				if (!feof(archivoLoremIpsum))
				{
					fgets(lectura, tamanio + 1, archivoLoremIpsum);
					string linea = replace(lectura, "\n", "");
					Sleep(milisegundos);
					UnCliente.EnviarMensaje("ENVI", 4);
					UnCliente.EnviarMensaje(destinatario, 15);
					UnCliente.EnviarMensajeTamanoVariable(linea);
					UnCliente.EscribirLog("Mensaje enviado a " + destinatario + ". Mensaje: " + linea);
					i++;
				}
				else
				{
					rewind(archivoLoremIpsum);
				}
			}

			fclose(archivoLoremIpsum);

			pause();
		}
		
		else {

			cout << "Archivo loremIpsum.txt no encontrado" << endl;
			pause();
		}
	}
	else {

		cout << "Debe iniciar sesion para poder enviar mensajes" << endl;
		pause();
	}

}

void CerrarPrograma() {
	clienteAbierto = false;
	UnCliente.EscribirLog("Programa cerrado por el usuario.");
	exit(0);
}

void mostrarMensajeServerNoDisponible(string operacion)
{
	ClearScreen();
	cout << "El server se encuentra inaccesible por el momento"  << endl;
	cout << "No se puede ejecutar la operacion de " << operacion << endl;
	cout << endl;
	cout << "Cierre el cliente e intente conectarse nuevamente con el server"  << endl;
	system("pause");
	UnCliente.EscribirLog("Server inaccesible: No se puede ejecutar la operacion de " + operacion);
}

void MenuPrincipal()
{
	int opcion = 0;

	while (((opcion < 1) || (opcion > 6)) || cin.fail())
	{
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(256, '\n');
		}

		ClearScreen();
		if (IsUsuarioLogueado)
		{
				cout << "Usuario Logueado: " << usuarioLogueado << endl << endl <<
				"2- Cerrar Sesion" << endl <<
				"3- Salir" << endl <<
				"4- Enviar" << endl <<
				"5- Recibir" << endl <<
				"6- Lorem Ipsum" << endl << endl;
				MostrarListaUsuarios();
				cout << "Ingrese una opcion: " << endl;
		}
		else
		{
			cout << "MENU PRINCIPAL" << endl << endl <<
				"1- Iniciar Sesion" << endl <<
				"2- Cerrar Sesion" << endl <<
				"3- Salir" << endl <<
				"4- Enviar" << endl <<
				"5- Recibir" << endl <<
				"6- Lorem Ipsum" << endl << endl;
				MostrarListaUsuarios();
				cout << "Ingrese una opcion: " << endl;

		}

		cin >> opcion;
	}

	switch (opcion)
	{
	case 1:
		if (serverStatus)//si el server no esta caido puedo ejecutar la operacion de inciarSesion
			IniciarSesion();
		else
			mostrarMensajeServerNoDisponible("Iniciar Sesion");
		break;
	case 2:
		if (serverStatus)//si el server no esta caido puedo ejecutar la operacion de cerrarSesion
			CerrarSesion();
		else
			mostrarMensajeServerNoDisponible("Cerrar Sesion");
		break;
	case 3:
		CerrarPrograma();
		break;
	case 4:
		if (serverStatus)//si el server no esta caido puedo ejecutar la operacion de enviarMensaje
			EnviarMensaje();
		else
			mostrarMensajeServerNoDisponible("Enviar");
		break;
	case 5:
		if (serverStatus)//si el server no esta caido puedo ejecutar la operacion de recibierMensaje
			RecibirMensajes();
		else
			mostrarMensajeServerNoDisponible("Recibir");
		break;
	case 6:
		if (serverStatus)//si el server no esta caido puedo ejecutar la operacion de LoremIpsum
			LoremIpsum();
		else
			mostrarMensajeServerNoDisponible("Lorem Ipsum");
		break;
	}

	MenuPrincipal();
}

int main(int argc, char **argv)
{
	
	bool conexionOk = false;
	string ip;
	string puerto;

	while (!conexionOk) {
		ClearScreen();
		cout << "Ingrese la IP del Servidor (Usar localhost para local): ";
		cin >> ip;
		cout << "Ingrese el puerto de conexion: ";
		cin >> puerto;

		conexionOk = UnCliente.ConectarAServidor(ip, puerto);

		if (!conexionOk) {
			cout << "Vuelva intentarlo\n";
			UnCliente.EscribirLog("Fallo el intento de conexion con servidor.");
			pause();
		}
	}

	UnCliente.EnviarMensaje("NEWC", 4);

	string Respuesta = UnCliente.RecibirMensaje(3);

	if (Respuesta == "000") {

		string UsuariosEnString = UnCliente.RecibirMensajeTamanoVariable();
		ArmarListaUsuarios(UsuariosEnString);

		UnCliente.EscribirLog("Programa Cliente iniciado.");

		// Thread de status del server.
		EnvioThreadData* datosPing = new EnvioThreadData;
		//Envio los datos de Puerto e IP al thread.
		datosPing->destinatario = ip;
		datosPing->mensaje = puerto;

		_beginthread(ThreadStatus, 0, (void*)datosPing);

		MenuPrincipal();
	} else {

		string MensajeError = UnCliente.RecibirMensaje(65);		
		cout << MensajeError << endl;
		pause();
	}
	
	return 0;
}