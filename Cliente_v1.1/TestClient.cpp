#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Client.h"

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define bufferSize 512





int main(int argc, char **argv)
{

	string puerto;
	string ip;
	string user;
	string clave;
	int respuestaServer;
	char bufferDeRespuestaServer[bufferSize];
	int bufferSizeCliente = bufferSize;

	cout << "Ingrese la IP del Servidor (Usar localhost para local): ";
	cin >> ip;
	cout << "Ingrese el puerto de conexion: ";
	cin >> puerto;
	cout << "ingrese usuario: ";
	cin >> user;
	cout << "ingrese clave: ";
	cin >> clave;

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

	return 0;
}

