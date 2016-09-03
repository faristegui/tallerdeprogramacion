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
#include "Lista.h"
#include "Mensaje.h"
#include "Usuarios.h"

class Server
{
private:
	SOCKET ListenSocket;
	Lista<Mensaje*>* todosLosMensajes;
public:
	void Abrir(std::string UnPuerto);
	SOCKET RecibirNuevaConexion();
	std::string RecibirMensaje(SOCKET ClientSocket, int tam);
	Server();
	void agregarMensaje(Mensaje* unMensaje);
	int EnviarMensaje(std::string mensaje, int sizeDatos, SOCKET ClientSocket);
	void enviarATodos(std::string contenidoMensaje, std::string emisor);
	Lista<Mensaje*>* obtenerMensajesPara(std::string destinatario);
	~Server();
};

#endif