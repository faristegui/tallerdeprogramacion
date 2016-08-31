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
#include <process.h>


class Server
{
private:
	SOCKET ListenSocket;
public:
	void Abrir(std::string UnPuerto);
	SOCKET RecibirNuevaConexion();
	std::string RecibirMensaje(SOCKET ClientSocket);
	Server();
	int EnviarMensaje(const char* datosEnviados, int sizeDatos, SOCKET ClientSocket);
	~Server();
};

#endif