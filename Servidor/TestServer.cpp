#undef UNICODE
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <process.h>

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
#define MAX_CLIENTES 6
int CantidadClientes = 0;

HANDLE ghMutex;

void MostrarListaComandos() {
	cout << "Ingrese la letra ""q"" si desea apagar el servidor: ";
}

void MainListenThread(void* arg) {
	string Usuario = "";
	string mensaje = "";
	SOCKET ClientSocket = *(SOCKET*)arg;
	bool EsThreadDePing = false;

	while ((mensaje != "EXIT") && (mensaje != "LOST"))
	{
		mensaje = UnServer.RecibirMensaje(ClientSocket, 4);

		if (mensaje == "AUTH") {

			string UsuarioMsj = UnServer.RecibirMensaje(ClientSocket, 15);
			string PasswordMsj = UnServer.RecibirMensaje(ClientSocket, 15);

			string CodigoRespuesta;
			string MensajeRespuesta;

			if (ControlUsuarios.ContrasenaValida(UsuarioMsj, PasswordMsj)) {

				Usuario = UsuarioMsj;

				CodigoRespuesta = "000";
				MensajeRespuesta = "Player: " + Usuario;

				UnServer.EscribirLog("Usuario " + Usuario + " logueado correctamente.", true);
			}
			else {

				CodigoRespuesta = "401";
				MensajeRespuesta = "El usuario y la contrasena no coinciden";

				UnServer.EscribirLog("Fallo de autenticacion de usuario: " + Usuario, true);
			}

			UnServer.EnviarMensaje(CodigoRespuesta, 3, ClientSocket);
			UnServer.EnviarMensaje(MensajeRespuesta, 40, ClientSocket);
		}
		if (mensaje == "ENVI")
		{
			string destinatario = UnServer.RecibirMensaje(ClientSocket, 15);
			string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);

			Mensaje* unMensaje = new Mensaje(Usuario, destinatario, contenidoMensaje);

			WaitForSingleObject(ghMutex, INFINITE);

			UnServer.agregarMensaje(unMensaje);
			
			ReleaseMutex(ghMutex);

			UnServer.EscribirLog("Mensaje enviado con exito, de: " + Usuario + " a " + destinatario + ". Mensaje: " + contenidoMensaje, false);
		}
		if (mensaje == "OUT") {

			if (Usuario != "") {
				UnServer.EnviarMensaje("Hasta la proxima " + Usuario, 40, ClientSocket);
				UnServer.EscribirLog(Usuario + " desconectado correctamente.", true);
				Usuario = "";
			} else {
				UnServer.EnviarMensaje("No existe ninguna sesion iniciada.", 40, ClientSocket);
				UnServer.EscribirLog("Fallo intento de Logout. No existe sesion iniciada.", true);
			}
		}
		if (mensaje == "PING")
		{
			UnServer.EnviarMensaje("OK", 2, ClientSocket);
			EsThreadDePing = true;
		}
		if (mensaje == "ENVT")
		{
			string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);
			Lista<std::string>* TodosLosUsuarios = ControlUsuarios.obtenerTodos();

			// Eespera que termine de ejecutar
			WaitForSingleObject(ghMutex, INFINITE);

			UnServer.enviarATodos(contenidoMensaje, Usuario, TodosLosUsuarios);

			ReleaseMutex(ghMutex);

			UnServer.EscribirLog("Mensaje de " + Usuario + " enviado a todos los usuarios. Mensaje: " + contenidoMensaje, true);
		}
		if (mensaje == "REC")
		{
			if (Usuario != "") {
				string respuestaServer = "";
				//Uso del recurso
				WaitForSingleObject(ghMutex, INFINITE); 

				Lista<Mensaje*>* buzon = UnServer.obtenerMensajesPara(Usuario);
				stringstream ss;
				ss << buzon->getTamanio();
				string CantidadMensajes = ss.str();
				UnServer.EnviarMensaje(CantidadMensajes, 8, ClientSocket);

				buzon->iniciarCursor();

				while (buzon->avanzarCursor())
				{
					string UsuarioEmisor = buzon->obtenerCursor()->obtenerEmisor();
					string ContenidoMensaje = buzon->obtenerCursor()->obtenerContenido();

					UnServer.EnviarMensaje(UsuarioEmisor, 15, ClientSocket);
					UnServer.EnviarMensajeTamanoVariable(ContenidoMensaje, ClientSocket);
				}
				ReleaseMutex(ghMutex);
				//Liberacion del recurso
			} else {

				UnServer.EnviarMensaje("NOLOGIN", 8, ClientSocket);
			}
		}
		if (mensaje == "CHKU") {

			if (Usuario == "") {
				UnServer.EnviarMensaje("NO", 2, ClientSocket);
			} else {
				UnServer.EnviarMensaje("SI", 2, ClientSocket);
			}
		}
		if (mensaje == "NEWC") {

			if (CantidadClientes >= MAX_CLIENTES) {
				// Si el server alcanzo el maximo de usuarios -> Envio codigo de error
				UnServer.EnviarMensaje("001", 3, ClientSocket);
				UnServer.EnviarMensaje("El servidor alcanzo el maximo de clientes permitidos, vuelva a intentar mas tarde.", 65, ClientSocket);
			} else {
				// Si esta todo ok -> Envio lista de usuarios del server
				string TodosLosUsuarios = ControlUsuarios.obtenerTodosEnString(";");
				UnServer.EnviarMensaje("000", 3, ClientSocket);
				UnServer.EnviarMensajeTamanoVariable(TodosLosUsuarios, ClientSocket);
				CantidadClientes++;
			}
		}
	}

	if (!EsThreadDePing) {

		CantidadClientes--;

		if (mensaje == "EXIT") {
			UnServer.EscribirLog("Un cliente se desconecto", true);
		}
		else {
			UnServer.EscribirLog("Un cliente perdio la conexion", true);
		}
	}

	closesocket(ClientSocket);
}

void MainServerThread(void* arg) {
	ghMutex = CreateMutex(NULL,FALSE,NULL);
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
			break;
		}
	}
}

int main()
{

	cout << "Ingrese el puerto donde escuchara conexiones el servidor: ";
	string Puerto;

	std::getline(std::cin, Puerto);

	if (Puerto.empty())
	{
		Puerto = "1000";
		cout << "Se usara el puerto por default: 1000" << endl;
	}

	string NombreArchivoUsuarios = "";

	while (!ControlUsuarios.SetNombreArchivo(NombreArchivoUsuarios)) {

		cout << "Ingrese el nombre del archivo de usuarios: ";
		std::getline(std::cin, NombreArchivoUsuarios);

		if (NombreArchivoUsuarios.empty())
		{
			NombreArchivoUsuarios = "Usuarios.csv";
			cout << "Se usara el archivo por default: Usuarios.csv" << endl;
		}
	}

	// Thread 1: Inicializacion del server

	_beginthread(MainServerThread, 0, (void*)&Puerto);

	// Thread 0: Handler de eventos del server
	Sleep(1000);
	MenuPrincipal();

	return 0;
}
