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

			if (Usuario == "") {

				UnServer.EnviarMensaje("000", 3, ClientSocket);

				string UsuarioMsj = UnServer.RecibirMensaje(ClientSocket, 15);
				string PasswordMsj = UnServer.RecibirMensaje(ClientSocket, 15);

				if (ControlUsuarios.ContrasenaValida(UsuarioMsj, PasswordMsj)) {
					Usuario = UsuarioMsj;
					UnServer.EnviarMensaje("000", 3, ClientSocket);
					UnServer.EnviarMensaje("Bienvenido, " + Usuario, 40, ClientSocket);
					UnServer.EscribirLog("Usuario " + Usuario + " logueado correctamente.");
				}
				else {
					UnServer.EnviarMensaje("401", 3, ClientSocket);
					UnServer.EnviarMensaje("El usuario y la contrasena no coinciden", 40, ClientSocket);
					UnServer.EscribirLog("Fallo de autenticacion de usuario: " + Usuario);
				}
			}
			else {
				UnServer.EnviarMensaje("001", 3, ClientSocket);
				UnServer.EnviarMensaje("Ya se encuentra loggeado en el servidor", 40, ClientSocket);
			}


		}
		if (mensaje == "USER")
		{
			string todosLosUsuarios = ControlUsuarios.obtenerTodosEnString(";");
			UnServer.EnviarMensajeTamanoVariable(todosLosUsuarios, ClientSocket);
		}
		if (mensaje == "ENVI")
		{

			if (Usuario != "") {
				UnServer.EnviarMensaje("000", 3, ClientSocket);

				// El mutex aca parece que va OK
				WaitForSingleObject(ghMutex, INFINITE);

				string destinatario = UnServer.RecibirMensaje(ClientSocket, 15);
				string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);

				Mensaje* unMensaje = new Mensaje(Usuario, destinatario, contenidoMensaje);

				UnServer.agregarMensaje(unMensaje);

				UnServer.EscribirLog("Mensaje enviado con exito, de: " + Usuario + " a " + destinatario + ". Mensaje: " + contenidoMensaje);

				ReleaseMutex(ghMutex);
			}
			else {
				UnServer.EnviarMensaje("001", 3, ClientSocket);
				UnServer.EnviarMensaje("No existe ninguna sesion iniciada.", 40, ClientSocket);
			}
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
			if (Usuario != "") {
				UnServer.EnviarMensaje("000", 3, ClientSocket);

				string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);

				// Eespera que termine de ejecutar
				WaitForSingleObject(ghMutex, INFINITE);

				UnServer.enviarATodos(contenidoMensaje, Usuario);

				ReleaseMutex(ghMutex);

				UnServer.EscribirLog("Mensaje de " + Usuario + " enviado a todos los usuarios. Mensaje: " + contenidoMensaje);
			} else {

				UnServer.EnviarMensaje("001", 3, ClientSocket);
				UnServer.EnviarMensaje("No existe ninguna sesion iniciada.", 40, ClientSocket);
			}
		}
		if (mensaje == "REC")
		{
			if (Usuario != "") {
				string respuestaServer = "";
				//En teoria espera que termine de ejecutar o como maximo los 5 segundos y libera el recurso [MZ]
				WaitForSingleObject(ghMutex, INFINITE); //se apodera del recurso, puse 5 segundos por poner algo.

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
			} else {

				UnServer.EnviarMensaje("NOLOGIN", 8, ClientSocket);
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
			UnServer.EscribirLog("Un cliente perdio la conexion");
		}
	}

	closesocket(ClientSocket);
}

void MainServerThread(void* arg) {
	int contador =0;
	HANDLE manejador[14]; 
	//LD: Puse 12 (2 por cada cliente por el ping) y le agregue uno mas por si entra un 7mo cliente para que no pinche.
	//    No pasa nada porque mas adelante le dice que no se puede conectar!
	//    TODO: Cada vez que una conexion se cierre hay que disminuir el contador pq si no va a pinchar (PROBAR MAS ADELANTE)

	ghMutex = CreateMutex(NULL,FALSE,NULL);
	string Puerto = *(string*)arg;
	UnServer.Abrir(Puerto);

	while (true) {
		SOCKET ClientSocket = UnServer.RecibirNuevaConexion();
		//En teoria cada vez que se conecta un nuevo cliente pasa por aca? No parece pasar eso.
		//El codigo siguiente esta aplicado con esa logica
		//La idea es ir acumulando los threads(clientes) y tener una lista para manejarlos con el mutex
		manejador[contador] = (HANDLE) _beginthread(MainListenThread, 0, (void*)&ClientSocket);
		contador++;
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
