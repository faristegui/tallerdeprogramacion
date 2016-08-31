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
	string Mensaje = "";
	SOCKET ClientSocket = *(SOCKET*)arg;

	while (Mensaje != "logoff") {

		Mensaje = UnServer.RecibirMensaje(ClientSocket, 4);

		if (Mensaje == "AUTH") {

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

		} else {

			if (Mensaje == "OUT") {

				if (Usuario != "") {
					UnServer.EnviarMensaje("Hasta la proxima " + Usuario, 40, ClientSocket);
					Usuario = "";
				} else {
					UnServer.EnviarMensaje("No tenia ninguna sesion iniciada", 40, ClientSocket);
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
