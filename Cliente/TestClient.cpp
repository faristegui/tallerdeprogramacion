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
#define clear() system("cls");
#define pause() system("pause");
#else 
#define clear() system("clear");
#define pause() system("pause");
#endif

Client UnCliente;

Client ClientePing;

bool serverStatus = false;

struct EnvioThreadData
{
	int opcion;
	string mensaje;
	string destinatario;
};

void ThreadStatus(void* pParams)
{
	//Envía mensajes al servidor y setea la variable serverStatus en TRUE o FALSE
	bool status = false;

	EnvioThreadData* datos = (EnvioThreadData*) pParams;

	ClientePing.ConectarAServidor(datos->destinatario,datos->mensaje); //Envío primero puerto e IP.

	while(true)
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
			clear();
			cout << "Conexion con el servidor terminada. (Server Offline).";
			ClientePing.EscribirLog("Conexion con el servidor terminada. (Server Offline).");
		}
		serverStatus = status;
		Sleep(30000); // 30 segundos
	}
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
		clear();
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
		// Avisa al servidor que va a mandar un mensaje
		UnCliente.EnviarMensaje("ENVI", 4);
		// Recibe todos los usuarios
		// Como la cantidad es variable. Primero recibo 
		int stringLength = atoi(UnCliente.RecibirMensaje(4).c_str());
		string todosLosUsuarios = UnCliente.RecibirMensaje(stringLength);
		// Muestro lista de usuarios
		MostrarUsuarios(todosLosUsuarios);
		// Recibo usuario y msj al que el usuario desea enviarle el msj
		cout << "Ingrese nombre de usuario del destinatario:" << endl;
		cin >> destinatario;
		datosOpcionEnvio->destinatario = destinatario;
		cout << "Ingrese el mensaje que desea enviar:" << endl;
		cin >> mensaje;
		datosOpcionEnvio->mensaje = mensaje;
	}
		break;
	case 2:
		// Avisa al servidor que el mensaje es para todos
		UnCliente.EnviarMensaje("ENVT", 4);
		// Recibo  msj al que el usuario desea enviarle el msj
		cout << "Ingrese el mensaje que desea enviar a todos los usuarios:" << endl;
		cin >> mensaje;
		datosOpcionEnvio->mensaje = mensaje;



		break;
	}


}

void enviarMensaje(void* pParams)
{
	int opcion;
	string mensaje;
	string destinatario;
	EnvioThreadData* datos = (EnvioThreadData*) pParams;
	opcion = datos->opcion;
	mensaje = datos->mensaje;
	destinatario = datos->destinatario;

	string respuestaServer;
	if(opcion == 1)
	{
		UnCliente.EnviarMensaje(destinatario,15);
		UnCliente.EscribirLog("Mensaje enviado a " + destinatario + ". Mensaje: " + mensaje);
		UnCliente.EnviarMensaje(mensaje,mensaje.length());
	}
	if(opcion == 2)
	{
		UnCliente.EnviarMensaje(mensaje,mensaje.length());
		UnCliente.EscribirLog("Mensaje enviado a todos los usuarios. Mensaje: " + mensaje);
	}
	
	respuestaServer = UnCliente.RecibirMensaje(3);
	cout << "Respuesta del servidor: " << respuestaServer << endl;
	UnCliente.EscribirLog("-> " + respuestaServer);
	respuestaServer = UnCliente.RecibirMensaje(30);
	cout << "Respuesta del servidor: " << respuestaServer << endl;
	UnCliente.EscribirLog("-> " + respuestaServer);
	pause();
}

void recibirMensajes(void* pParams)
{
	string cantMensajes, respuestaServer,emisor,contenidoMensaje;
	string tamanioEmisor,tamanioMensaje;
	UnCliente.EnviarMensaje("REC",3);

	//El servidor responde la cantidad de mensajes
	respuestaServer = UnCliente.RecibirMensaje(3); 
	cantMensajes = respuestaServer;

	cout << "Usted tiene " << cantMensajes << " mensajes" << endl << endl;
	UnCliente.EscribirLog(cantMensajes + " mensajes recibidos.");

	if(stoi(cantMensajes) > 0)
	{
		//se reciben los mensajes (incluye cadena de emisor con contenido de cada uno)
		//puse 512 porque es el maximo pero habria que encadenar varias respuestas del server
		//en caso que exceda el maximo de 512 que definimos de tamaño de buffer
		
		respuestaServer = UnCliente.RecibirMensaje(900);
	
		while(respuestaServer!= "")
		{
			emisor = respuestaServer.substr(0,respuestaServer.find(';'));
			respuestaServer.erase(0,respuestaServer.find(';')+1);
		
			contenidoMensaje = respuestaServer.substr(0,respuestaServer.find(";;"));
			respuestaServer.erase(0,respuestaServer.find(";;") +2);

			cout << "De: " << emisor << endl;
			cout << "Mensaje: " << contenidoMensaje << endl << endl;
			UnCliente.EscribirLog("Mensaje de " + emisor + " recibido.");
		}
	}
	pause();
}

void MenuPrincipal()
{
	int opcion = 0;
	Sleep(3000); // Por que este sleep? [LD]
	//es para sincronizar el thread de envio con el principal y que no se superpongan, pero
	//creo que ya lo soluciono seba. estaria al pedo. [MZ10]
	while((opcion < 1) || (opcion > 6))
	{
		clear();
		cout << "MENU PRINCIPAL" << endl << endl <<
		"1- Iniciar Sesion" << endl <<
		"2- Cerrar Sesion" << endl <<
		"3- Salir" << endl <<
		"4- Enviar" << endl <<
		"5- Recibir" << endl <<
		"6- Lore Ipsum" << endl << endl << "Ingrese una opcion: ";
		cin >> opcion;
	}
	switch(opcion)
	{
		case 1:
			IniciarSesion();
			break;
		case 2:
			CerrarSesion();
			break;
		case 3:
			clear();
			cout << "El programa se cerrara.";
			UnCliente.EscribirLog("Programa cerrado por el usuario.");
			Sleep(3000);
			exit(0);
			break;
		case 4:
			{
				EnvioThreadData* datosOpcionEnvio = new EnvioThreadData;
				menuMensaje(datosOpcionEnvio);
				//sebastian: primero llamo al menuMensaje para obtener todos los datos del envio, osea si es envio a uno , a todos y cual es el mensaje a enviar

				//sebastian: aca si lanzo el thread ya que ahora tengo todos los datos del menu de envio completos dentro de la estructura datosOpcionEnvio
				_beginthread(enviarMensaje, 0, (void*)datosOpcionEnvio); //lanzo el hilo y asincronicamente enviara el mensaje al server, ya puedo entonces volver al menu principal del usuario.
				break;
			}
		case 5:
			recibirMensajes(NULL);
			break;
		case 6:
			//cliente.enviarCiclicamente()
			break;
	}
	MenuPrincipal();
}

int main(int argc, char **argv)
{
	string ip;
	string puerto;

	cout << "Ingrese la IP del Servidor (Usar localhost para local): ";
	cin >> ip;
	cout << "Ingrese el puerto de conexion: ";
	cin >> puerto;

	UnCliente.EscribirLog("Programa Cliente iniciado.");
	
	UnCliente.ConectarAServidor(ip, puerto);

	// Thread de status del server.
	EnvioThreadData* datosPing = new EnvioThreadData;
	//Envio los datos de Puerto e IP al thread.
	datosPing->destinatario = ip;
	datosPing->mensaje = puerto;

	_beginthread(ThreadStatus, 0, (void*)datosPing);

	MenuPrincipal();

	return 0;
}