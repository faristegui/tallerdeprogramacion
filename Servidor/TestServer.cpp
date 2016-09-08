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
#define ClearScreen() system("cls");
#define pause() system("pause");
#else 
#define ClearScreen() system("clear");
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

	while ((mensaje != "EXIT") && (mensaje != "LOST"))
	{
		mensaje = UnServer.RecibirMensaje(ClientSocket, 4);

		if (mensaje == "AUTH") {

			string UsuarioMsj = UnServer.RecibirMensaje(ClientSocket, 15);
			string PasswordMsj = UnServer.RecibirMensaje(ClientSocket, 15);

			if (ControlUsuarios.ContrasenaValida(UsuarioMsj, PasswordMsj)) {
				Usuario = UsuarioMsj;
				UnServer.EnviarMensaje("000", 3, ClientSocket);
				UnServer.EnviarMensaje("Bienvenido, " + Usuario, 40, ClientSocket);
				UnServer.EscribirLog("Usuario " + Usuario + " logueado correctamente.");
			} else {
				UnServer.EnviarMensaje("401", 3, ClientSocket);
				UnServer.EnviarMensaje("El usuario y la contrasena no coinciden", 40, ClientSocket);
				UnServer.EscribirLog("Fallo de autenticacion de usuario: " + Usuario);
			}

		}
		if (mensaje == "ENVI")
		{

			string todosLosUsuarios = ControlUsuarios.obtenerTodosEnString(";");
			UnServer.EnviarMensajeTamanoVariable(todosLosUsuarios, ClientSocket);

			// Recibo el destinatario y el mensaje
			// Valido destinatario

			string destinatario = UnServer.RecibirMensaje(ClientSocket,15);
			string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);
			if(ControlUsuarios.destinatarioValido(destinatario))
			{
				Mensaje* unMensaje = new Mensaje(Usuario,destinatario,contenidoMensaje);
				UnServer.agregarMensaje(unMensaje);
				UnServer.EnviarMensaje("001",3,ClientSocket);
				UnServer.EnviarMensaje("Mensaje enviado con exito", 30,ClientSocket);
				UnServer.EscribirLog("Mensaje enviado con exito, de: " + Usuario + " a " + destinatario + ". Mensaje: " + contenidoMensaje);
			}
			else
			{
				UnServer.EnviarMensaje("402",3,ClientSocket);
				ClearScreen();
				UnServer.EnviarMensaje("El destinatario no existe",30,ClientSocket);
				UnServer.EscribirLog("Error al enviar mensaje de " + Usuario + " a " + destinatario + "El destinatario no existe.");
			}
		}
		if (mensaje == "OUT") {

			if (Usuario != "") {
				ClearScreen();
				UnServer.EnviarMensaje("Hasta la proxima " + Usuario, 40, ClientSocket);
				UnServer.EscribirLog(Usuario + " desconectado correctamente.");
				Usuario = "";
			} else {
				ClearScreen();
				UnServer.EnviarMensaje("No existe ninguna sesion iniciada.", 40, ClientSocket);
				UnServer.EscribirLog("Fallo intento de Logout. No existe sesion iniciada.");
			}
		}
		if (mensaje == "PING")
		{
			UnServer.EnviarMensaje("OK", 2, ClientSocket);
		}
		if (mensaje == "ENVT")
		{
			string contenidoMensaje = UnServer.RecibirMensaje(ClientSocket,900);
			UnServer.enviarATodos(contenidoMensaje, Usuario);
			UnServer.EnviarMensaje("002",3,ClientSocket);
			//se envia cortado este mensaje
			UnServer.EnviarMensaje("Mensaje enviado a todos los usuarios con exito.",65,ClientSocket);
			UnServer.EscribirLog("Mensaje de " + Usuario + " enviado a todos los usuarios. Mensaje: " + contenidoMensaje);
		}
		if (mensaje == "REC")
		{
			string respuestaServer = "";
			Lista<Mensaje*>* buzon = UnServer.obtenerMensajesPara(Usuario);
			stringstream ss;
			ss << buzon->getTamanio();
			string CantidadMensajes = ss.str();	
			UnServer.EnviarMensaje(CantidadMensajes, 3, ClientSocket);
			
			buzon->iniciarCursor();

			while(buzon->avanzarCursor())
			{
				string UsuarioEmisor = buzon->obtenerCursor()->obtenerEmisor();
				string ContenidoMensaje = buzon->obtenerCursor()->obtenerContenido();

				UnServer.EnviarMensaje(UsuarioEmisor, 15, ClientSocket);
				UnServer.EnviarMensajeTamanoVariable(ContenidoMensaje, ClientSocket);
			}
		}
		if (mensaje == "EXIT") {
			UnServer.EscribirLog("Un cliente se desconecto");
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
