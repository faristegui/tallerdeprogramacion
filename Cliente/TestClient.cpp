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

	while(true)
	{
		UnCliente.EnviarMensaje("PING", 4);

		string respuesta = UnCliente.RecibirMensaje(2);

		if(respuesta == "OK")
		{
			status = true;
		}
		else
		{
			status = false;
			clear();
			cout << "Conexion con el servidor terminada. (Server Offline).";
			UnCliente.EscribirLog("Conexion con el servidor terminada. (Server Offline).");
		}
		serverStatus = status;
		Sleep(30000); // 30 segundos
	}
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
		
		cout << "Ingrese nombre de usuario del destinatario:" << endl;
		cin >> destinatario;
		datosOpcionEnvio->destinatario = destinatario;
		cout << "Ingrese el mensaje que desea enviar:" << endl;
		cin >> mensaje;
		datosOpcionEnvio->mensaje = mensaje;
		break;
	case 2:
		
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
		//Avisa al servidor que va a mandar un mensaje
		UnCliente.EnviarMensaje("ENVI",4);




		UnCliente.EnviarMensaje(destinatario,15);
		UnCliente.EscribirLog("Mensaje enviado a " + destinatario + ".");
		UnCliente.EnviarMensaje(mensaje,60);
	}
	if(opcion == 2)
	{
		//Avisa al servidor que el mensaje es para todos
		UnCliente.EnviarMensaje("ENVT",4);

		UnCliente.EnviarMensaje(mensaje,60);
	}
	
	respuestaServer = UnCliente.RecibirMensaje(3);
	cout << "Respuesta del servidor: " << respuestaServer << endl;
	respuestaServer = UnCliente.RecibirMensaje(30);
	cout << "Respuesta del servidor: " << respuestaServer << endl;
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
		respuestaServer = UnCliente.RecibirMensaje(512);
	
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
	Sleep(3000);
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
				int opcionEnvio;
				string mensaje;

				EnvioThreadData* datosOpcionEnvio = new EnvioThreadData;
				//sebastian: primero llamo al menuMensaje para obtener todos los datos del envio, osea si es envio a uno , a todos y cual es el mensaje a enviar.
				menuMensaje(datosOpcionEnvio);

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
	_beginthread(ThreadStatus, 0, NULL);

	MenuPrincipal();

	return 0;
}