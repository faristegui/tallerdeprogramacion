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

#define bufferSize 512
int opcion;

void solicitarDatos(string puerto, string ip,string user, string clave)
{
	cout << "Ingrese la IP del Servidor (Usar localhost para local): ";
	cin >> ip;
	cout << "Ingrese el puerto de conexion: ";
	cin >> puerto;
	cout << "ingrese usuario: ";
	cin >> user;
	cout << "ingrese clave: ";
	cin >> clave;
}

void ThreadPrincipal(void* pParams)
{
	//Thread principal para enviar y recibir mensajes
	cout << "Lanza el nuevo thread";
}

/*
Esta seria la idea del thread principal
void ThreadPrincipal(void* pParams)
{
	string puerto;
	string ip;
	string user;
	string clave;
	while(true)
	{
			cout << "Ingrese una opci�n entre 1 y 6" << endl <<
			"1- Conectar" << endl <<
		"2-Desconectar" << endl <<
		"3-Salir" << endl <<
		"4-Enviar" << endl <<
		"5-Recibir" << endl <<
		"6-Lore Ipsum" << endl;
		cin >> opcion;
		switch(opcion)
		{
			case 1:
				{
					solicitarDatos(puerto,ip,user,clave);
					Client cliente(AF_INET, SOCK_STREAM, IPPROTO_TCP, ip, puerto, user.c_str(), clave);
					cliente.conectarAServidor();
				}
				//cliente.conectar()
				break;
			case 2:
				//cliente.desconectar()
				break;
			case 3:
				exit(0);
				break;
			case 4:
				//cliente.enviar()
				break;
			case 5:
				//cliente.recibir()
				break;
			case 6:
				//cliente.enviarCiclicamente()
				break;
		}
	}
}
*/

void menuPrincipal()
{
	string puerto;
	string ip;
	string user;
	string clave;

	opcion = 0; //Opci�n por defecto

	while((opcion < 1) || (opcion > 6))
	{
		clear(); //Limpia la pantalla
		cout << "MENU PRINCIPAL" << endl << endl <<
		"1- Conectar" << endl <<
		"2- Desconectar" << endl <<
		"3- Salir" << endl <<
		"4- Enviar" << endl <<
		"5- Recibir" << endl <<
		"6- Lore Ipsum" << endl << endl << "Ingrese una opcion: ";
		cin >> opcion;
	}
	switch(opcion)
	{
		case 1:
			{
				solicitarDatos(puerto,ip,user,clave);
				Client cliente(AF_INET, SOCK_STREAM, IPPROTO_TCP, ip, puerto, user.c_str(), clave);
				cliente.conectarAServidor();
				//falta completar, seria lo de abajo que esta comentado
			}
			//cliente.conectar()
			break;
		case 2:
			//cliente.desconectar()
			break;
		case 3:
			clear();
			cout << "El programa se cerrara.";
			Sleep(3000);
			exit(0);
			break;
		case 4:
			//cliente.enviar()
			break;
		case 5:
			//cliente.recibir()
			break;
		case 6:
			//cliente.enviarCiclicamente()
			break;
	}
}

int main(int argc, char **argv)
{
	while(true)
	{
		_beginthread(ThreadPrincipal, 0, NULL);

		menuPrincipal();
	}

	
	return 0;
}

//system("pause");	
	/*
	int respuestaServer;
	char bufferDeRespuestaServer[bufferSize];
	int bufferSizeCliente = bufferSize;

	solicitarDatos(puerto,ip,user,clave);

	Client cliente(AF_INET, SOCK_STREAM, IPPROTO_TCP, ip, puerto, user.c_str(), clave);

	cliente.conectarAServidor();

	char *mensajeAEnviar = "Hola soy el cliente " ;
	string auxMensajeEnviar = "usuario " + user;
	
	cout << "Mensaje enviado al servidor: " << auxMensajeEnviar << "\n";

	cliente.enviarDatos(auxMensajeEnviar.c_str(), strlen(auxMensajeEnviar.c_str()));


	respuestaServer = cliente.recibirDatos(bufferDeRespuestaServer, bufferSizeCliente);
	
	string auxMensajeRespuesta(bufferDeRespuestaServer);
	cout << "Mensaje recibido desde el servidor: " << auxMensajeRespuesta << "\n";

		



	system("pause");

	cliente.cerrarConexionConServer();
	*/