#pragma once
#ifndef _Server_h_
#define _Server_h_

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <process.h>
#include "Lista.h"
#include "Mensaje.h"
#include "Usuarios.h"

class Server
{
private:
	SOCKET ListenSocket;
	Lista<Mensaje*>* todosLosMensajes;
	std::ofstream logFile;  //Archivo para el log
public:
	void Abrir(std::string UnPuerto);
	SOCKET RecibirNuevaConexion();
	std::string RecibirMensaje(SOCKET ClientSocket, int tam);
	std::string RecibirMensajeTamanoVariable(SOCKET ClientSocket);
	Server();
	void agregarMensaje(Mensaje* unMensaje);
	int EnviarMensaje(std::string mensaje, int sizeDatos, SOCKET ClientSocket);
	int EnviarMensajeTamanoVariable(std::string mensaje, SOCKET ClientSocket);
	void EscribirLog(std::string mensaje, bool cortar);
	void enviarATodos(std::string contenidoMensaje, std::string emisor, Lista<std::string>* TodosLosUsuarios);
	Lista<Mensaje*>* obtenerMensajesPara(std::string destinatario);
	~Server();
};

#endif