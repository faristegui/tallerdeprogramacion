#pragma once
#ifndef _Server_h_
#define _Server_h_

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>


class Server
{




private:
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	sockaddr_in dirSocketServidor;
	struct addrinfo *result;
	struct addrinfo hints;
	int iSendResult;
	std::string puerto;

	void binding(int af);
	void escucharConexiones();
	int recibirMensaje(std::string& mensaje);
	void autenticar(std::string usuario, std::string password);
public:
	Server(std::string puerto);
	void abrir();
	int enviarDatos(const char* datosEnviados, int sizeDatos); // A que cliente le manda los datos? Creo que debiera ser private si es solo para echo
	~Server();
};

#endif