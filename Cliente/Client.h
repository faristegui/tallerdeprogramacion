#pragma once


#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>


class Client
{

private:
	std::string puerto;
	std::string ipServer;
	WSADATA wsaData;
	SOCKET ClientConnectionSocket;
	struct addrinfo *result;
	struct addrinfo hints;
	struct addrinfo *ptr;
	const char* usuario;
	std::string clave;
		


public:
	Client(int af, int tipo, int protocolo, std::string ip, std::string puerto,const char* usuario,std::string clave);
	int enviarDatos(const char* datosEnviados, int sizeDatos);
	void conectarAServidor();
	int recibirDatos(char* datosRecibidos, int sizeDatos);
	const char* getUserClientName();
	void cerrarConexionConServer();
	~Client();
};

