#undef UNICODE
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

#include "Server.h"
#include "Usuarios.h"
#include "Mensaje.h"

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#ifdef WIN32 
#define clear() system("cls");
#define pause() system("pause");
#else 
#define clear() system("clear");
#define pause() system("pause");
#endif

Server UnServer;
Usuarios ControlUsuarios;

void MostrarListaComandos() {
	cout << "Ingrese la letra ""q"" si desea apagar el servidor: ";
}

void MainListenThread(void* arg) {
	string Usuario = "";
	string mensaje = "";
	SOCKET ClientSocket = *(SOCKET*)arg;

	while (mensaje != "logoff") 
	{
		mensaje = UnServer.RecibirMensaje(ClientSocket, 4);

		if (mensaje == "AUTH") {

			string UsuarioMsj = UnServer.RecibirMensaje(ClientSocket, 15);
			string PasswordMsj = UnServer.RecibirMensaje(ClientSocket, 15);

			if (ControlUsuarios.ContrasenaValida(UsuarioMsj, PasswordMsj)) {
				Usuario = UsuarioMsj;
				UnServer.EnviarMensaje("000", 3, ClientSocket);
				UnServer.EnviarMensaje("Bienvenido, " + Usuario, 40, ClientSocket);
			} else {
				UnServer.EnviarMensaje("401", 3, ClientSocket);
				UnServer.EnviarMensaje("El usuario y la contrasena no coinciden", 40, ClientSocket);
			}

		}
		if(mensaje == "ENVI")
		{
			string destinatario = UnServer.RecibirMensaje(ClientSocket,15);
			string contenidoMensaje = UnServer.RecibirMensaje(ClientSocket,60);
			if(ControlUsuarios.destinatarioValido(destinatario))
			{
				Mensaje* unMensaje = new Mensaje(Usuario,destinatario,contenidoMensaje);
				UnServer.agregarMensaje(unMensaje);
				UnServer.EnviarMensaje("001",3,ClientSocket);
				UnServer.EnviarMensaje("Mensaje enviado con exito", 30,ClientSocket);
				//faltaria informar en el log
			}
			else
			{
				UnServer.EnviarMensaje("402",3,ClientSocket);
				UnServer.EnviarMensaje("El destinatario no existe",30,ClientSocket);
			}
		}
		if (mensaje == "OUT") {

			if (Usuario != "") {
				UnServer.EnviarMensaje("Hasta la proxima " + Usuario, 40, ClientSocket);
				Usuario = "";
			} else {
				UnServer.EnviarMensaje("No tenia ninguna sesion iniciada", 40, ClientSocket);
			}
		}
		if(mensaje == "PING")
		{
			UnServer.EnviarMensaje("OK", 2, ClientSocket);
		}
		if(mensaje == "ENVT")
		{
			string contenidoMensaje = UnServer.RecibirMensaje(ClientSocket,60);
			UnServer.enviarATodos(contenidoMensaje, Usuario);
			UnServer.EnviarMensaje("002",3,ClientSocket);
			//se envia cortado este mensaje
			UnServer.EnviarMensaje("Mensaje enviado a todos los usuarios con exito",65,ClientSocket);
		}
		if(mensaje=="REC")
		{
			Lista<Mensaje*>* buzon = UnServer.obtenerMensajesPara(Usuario);
			stringstream ss;
			ss << buzon->getTamanio();
			string str = ss.str();	
			UnServer.EnviarMensaje(str,3,ClientSocket);
			if(buzon->getTamanio() > 0 )
			{
				buzon->iniciarCursor();
				while(buzon->avanzarCursor())
				{
					UnServer.EnviarMensaje(buzon->obtenerCursor()->obtenerEmisor()+";",15,ClientSocket);
					//uso ;; para separar el contenido del mensaje del emisor del siguiente mensaje.
					//habria que prohibir el uso de ;; en el contenido del mensaje
					UnServer.EnviarMensaje(buzon->obtenerCursor()->obtenerContenido()+";;",60,ClientSocket);
				}
			}
		}
	}

	closesocket(ClientSocket);
}

void MainServerThread(void* arg) {

	string Puerto = *(string*)arg;
	UnServer.Abrir(Puerto);

	while (true) {
		SOCKET ClientSocket = UnServer.RecibirNuevaConexion();
		_beginthread(MainListenThread, 0, (void*)&ClientSocket);
	}

}

void MenuPrincipal() {
	string comando;

	while (true)
	{
		MostrarListaComandos();
		cin >> comando;
		if (comando == "q")
		{
			cout << "Servidor off\n";
			pause();
			break;
		}
	}
}

int main()
{

	string puerto;
	cout << "Ingrese el puerto donde escuchara conexiones el servidor: ";
	cin >> puerto;	

	// Thread 1: Inicializacion del server

	_beginthread(MainServerThread, 0, (void*)&puerto);

	// Thread 0: Handler de eventos del server
	Sleep(1000);
	MenuPrincipal();

	return 0;
}
