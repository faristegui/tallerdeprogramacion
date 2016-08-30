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
	SOCKET serverListenSocket;
	SOCKET backupListenSocket;
	SOCKET acceptSocket;
	sockaddr_in dirSocketServidor;
	struct addrinfo *result;
	struct addrinfo hints;


	int iSendResult;


	std::string puerto;

	void binding(int af);
	void escucharConexiones();
public:
	Server();

	/*af=AF_INET  The Internet Protocol version 4 (IPv4) address family.
	tipo=SOCK_STREAM  A socket type that provides sequenced, reliable, two-way, connection-based byte streams
	protocolo=IPPROTO_TCP*/
	Server(std::string puerto);

	void aceptarConexion();
	int enviarDatos(const char* datosEnviados, int sizeDatos);
	int recibirDatos(char* datosRecibidos, int sizeDatos);
	bool aceptaConexion();
	void cerrarConexion();
	bool isSocketInvalido();
	~Server();
};

#endif