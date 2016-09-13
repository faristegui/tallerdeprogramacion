#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <string>
#include <iostream>

using namespace std;

class Client
{

private:
	SOCKET ClientConnectionSocket;
	ofstream logFile;  //Archivo para el log
public:
	Client();
	Client(int log);
	int EnviarMensaje(string, int sizeDatos);
	int EnviarMensajeTamanoVariable(string);
	bool ConectarAServidor(string ip, string puerto);
	std::string RecibirMensaje(int sizeDatos);
	std::string RecibirMensajeTamanoVariable();
	void cerrarConexionConServer();
	void EscribirLog(string mensaje);
	~Client();
};
