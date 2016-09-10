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
bool serverStatus = false;

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

	respuestaServer = UnCliente.RecibirMensaje(3);
	//cout << "Respuesta del servidor: " << respuestaServer << endl;
	UnCliente.EscribirLog("-> " + respuestaServer);
	respuestaServer = UnCliente.RecibirMensaje(65);
	//cout << "Respuesta del servidor: " << respuestaServer << endl;
	UnCliente.EscribirLog("-> " + respuestaServer);
	//pause();
}


void ThreadStatus(void* pParams)
{
	Client ClientePing;
	//Envía mensajes al servidor y setea la variable serverStatus en TRUE o FALSE
	bool status = false;

	EnvioThreadData* datos = (EnvioThreadData*) pParams;

	string ip = datos->destinatario;
	string puerto = datos->mensaje;

	ClientePing.ConectarAServidor(ip, puerto); //Envío primero puerto e IP.

	while (clienteAbierto)
	{
		ClientePing.EnviarMensaje("PING", 4);

		string respuesta = ClientePing.RecibirMensaje(2);

		if(respuesta == "OK")
		{
			status = true;
		}
		else
		{
			status = false;
			ClearScreen();
			cout << "Conexion con el servidor terminada. (Server Offline).";
			ClientePing.EscribirLog("Conexion con el servidor terminada. (Server Offline).");
		}
		serverStatus = status;
		Sleep(30000); // 30 segundos
	}

	ClientePing.EnviarMensaje("EXIT", 4);
}

// TODO: Hay que implementar un metodo que convierta de string separado por ";" a Lista<string>
void MostrarUsuarios(string ListaUsuarios) {
	string UnUsuario;
	int index = ListaUsuarios.find(";");

	cout << "Lista de usuarios: \n\n";

	while (index > -1) {
		UnUsuario = ListaUsuarios.substr(0, index);
		ListaUsuarios = ListaUsuarios.substr(index + 1);
		index = ListaUsuarios.find(";");
		cout << UnUsuario << "\n";
	}

	cout << "\n";
}

void IniciarSesion()
{
	string mensaje = "AUTH";
	UnCliente.EnviarMensaje(mensaje, 4);

	cout << "Ingrese Usuario: ";
	cin >> mensaje;
	UnCliente.EnviarMensaje(mensaje, 15);

	cout << "Ingrese clave: ";
	cin >> mensaje;
	UnCliente.EnviarMensaje(mensaje, 15);

	string respuesta = UnCliente.RecibirMensaje(3);

	UnCliente.EscribirLog("Autorizar usuario. Mensaje del servidor: " + respuesta + ".");
	/* NO HAY QUE HACER NADA DE ACUERDO A CADA CASO?
	if (respuesta == "000") {
		
	}
	else {
		
	}
	*/

	respuesta = UnCliente.RecibirMensaje(40);
	cout << respuesta << endl;
	pause();
}

void CerrarSesion() {
	string mensaje = "OUT";
	UnCliente.EnviarMensaje(mensaje, 4);

	string respuesta = UnCliente.RecibirMensaje(40);
	cout << respuesta << endl;
	UnCliente.EscribirLog("Cerrar sesion. Mensaje del servidor: " + respuesta + ".");
	pause();
}

void menuMensaje(EnvioThreadData* datosOpcionEnvio)
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

	//agregado por sebastian
	
	switch(opcion)
	{
	case 1:
	{
		UnCliente.EnviarMensaje("ENVI", 4);
		string todosLosUsuarios = UnCliente.RecibirMensajeTamanoVariable();
		// Muestro lista de usuarios
		MostrarUsuarios(todosLosUsuarios);
		// Recibo usuario y msj al que el usuario desea enviarle el msj
		cout << "Ingrese nombre de usuario del destinatario:" << endl;
		cin >> destinatario;
		datosOpcionEnvio->destinatario = destinatario;
		cin.ignore();
		cout << "Ingrese el mensaje que desea enviar:" << endl;
		getline(cin,mensaje);
		datosOpcionEnvio->mensaje = mensaje;
	}
		break;
	case 2:
		// Avisa al servidor que el mensaje es para todos
		UnCliente.EnviarMensaje("ENVT", 4);
		// Recibo  msj al que el usuario desea enviarle el msj
		cin.ignore();
		cout << "Ingrese el mensaje que desea enviar a todos los usuarios:" << endl;
		getline(cin,mensaje);
		datosOpcionEnvio->mensaje = mensaje;
		break;
	}
	

}

void EnviarMensaje() {
	EnvioThreadData* datosOpcionEnvio = new EnvioThreadData;
	menuMensaje(datosOpcionEnvio);
	_beginthread(ThreadEnviaMensaje, 0, (void*)datosOpcionEnvio);
	cout << "Mensaje enviado" << endl;
	pause();
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
	string todosLosUsuarios = UnCliente.RecibirMensajeTamanoVariable();
	// Recibo lista de usuarios
	string UnUsuario;
	int index = todosLosUsuarios.find(";");

	while (index > -1) {
		UnUsuario = todosLosUsuarios.substr(0, index);
		todosLosUsuarios = todosLosUsuarios.substr(index + 1);
		index = todosLosUsuarios.find(";");
	}
	pause();
	destinatario = UnUsuario; //Estoy devolviendo siempre el primero. Después reviso como hacer para enviar a otros.

	return destinatario;
}

void LoremIpsum()
{
	int frecuencia;
	int cantidad;
	int milisegundos;
	char* lectura;
	int tamanio = 0;

	srand(time(NULL)); 
	tamanio = rand()%201; //Random entre 1 y 200, tamaño máximo del mensaje a enviar.

    lectura = new char[tamanio];

	string destinatario = obtenerDestinatario();

	UnCliente.EscribirLog("Secuencia de envio automatico iniciada.");

	FILE *archivoLoremIpsum;
	archivoLoremIpsum = fopen("loremIpsum.txt", "rt");

	ClearScreen();
	
	cout << "Se enviaran los mensajes al usuario " << destinatario << endl; 

	cout << "Ingrese la frecuencia de envio de mensajes: ";
	cin >> frecuencia;
	cout << "Ingrese la cantidad de mensajes a enviar: ";
	cin >> cantidad;

	milisegundos = (1000/frecuencia);

	cout << "Se enviara un mensaje cada " << milisegundos << " milisegundos." << endl;

	for(int i = 0; i < cantidad; i = i + 1)
	{
		if(!feof(archivoLoremIpsum))
		{
			fgets(lectura,tamanio + 1,archivoLoremIpsum);
			Sleep(milisegundos);
			UnCliente.EnviarMensaje("ENVI", 4);
			UnCliente.EnviarMensaje(destinatario, 15);
			UnCliente.EnviarMensajeTamanoVariable(lectura);
			UnCliente.EscribirLog("Mensaje enviado a " + destinatario + ". Mensaje: " + lectura);
		}
		else
		{
			rewind(archivoLoremIpsum);
		}
	}

	fclose(archivoLoremIpsum);

	pause();
}

void CerrarPrograma() {
	clienteAbierto = false;
	UnCliente.EscribirLog("Programa cerrado por el usuario.");
	exit(0);
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
		cout << "MENU PRINCIPAL" << endl << endl <<
			"1- Iniciar Sesion" << endl <<
			"2- Cerrar Sesion" << endl <<
			"3- Salir" << endl <<
			"4- Enviar" << endl <<
			"5- Recibir" << endl <<
			"6- Lorem Ipsum" << endl << endl << "Ingrese una opcion: ";
		cin >> opcion;
	}

	switch (opcion)
	{
	case 1:
		IniciarSesion();
		break;
	case 2:
		CerrarSesion();
		break;
	case 3:
		CerrarPrograma();
		break;
	case 4:
		EnviarMensaje();
		break;
	case 5:
		RecibirMensajes();
		break;
	case 6:
		/*
		string letra;
		cin >> letra;
		for (int i = 0; i < 400; i++) {
			UnCliente.EnviarMensaje("ENVI", 4);
			UnCliente.RecibirMensajeTamanoVariable();
			UnCliente.EnviarMensaje("Leandro", 15);
			UnCliente.EnviarMensajeTamanoVariable(letra + std::to_string(i));
			UnCliente.RecibirMensaje(3);
			UnCliente.RecibirMensaje(30);
		}
		cout << "LISTO!";
		pause();
		*/
		LoremIpsum();
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
			pause();
		}
	}

	
	UnCliente.EscribirLog("Programa Cliente iniciado.");

	// Thread de status del server.
	EnvioThreadData* datosPing = new EnvioThreadData;
	//Envio los datos de Puerto e IP al thread.
	datosPing->destinatario = ip;
	datosPing->mensaje = puerto;

	_beginthread(ThreadStatus, 0, (void*)datosPing);
	
	MenuPrincipal();
	
	return 0;
}