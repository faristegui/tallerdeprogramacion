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
	SOCKET ClientConnectionSocket;
public:
	Client();
	int EnviarMensaje(std::string, int sizeDatos);
	void ConectarAServidor(std::string ip, std::string puerto);
	std::string RecibirMensaje(int sizeDatos);
	void cerrarConexionConServer();
	~Client();
};

