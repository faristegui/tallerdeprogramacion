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

#define bufferSize 512


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
	int opcion;
	string puerto;
	string ip;
	string user;
	string clave;
	while(true)
	{
		cout << "Ingrese una opción entre 1 y 6" << endl <<
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
int main(int argc, char **argv)
{
	_beginthread(ThreadPrincipal,0,NULL);

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
	return 0;
}

