#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Client.h"
#include <process.h>
#include <stdio.h>

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#ifdef WIN32 
#define clear() system("cls");
#define pause() system("pause");
#else 
#define clear() system("clear");
#define pause() system("pause");
#endif

Client UnCliente;

void ThreadPrincipal(void* pParams)
{
	//Thread principal para enviar y recibir mensajes
	cout << "Lanza el nuevo thread";
}

void IniciarSesion()
{
	string mensaje = "AUTH";
	UnCliente.EnviarMensaje(mensaje, 4);

	cout << "Ingrese Usuario: ";
	cin >> mensaje;
	UnCliente.EnviarMensaje(mensaje, 15);

	cout << "Ingrese clave: ";
	cin >> mensaje;
	UnCliente.EnviarMensaje(mensaje, 15);

	string respuesta = UnCliente.RecibirMensaje(3);

	/* NO HAY QUE HACER NADA DE ACUERDO A CADA CASO?
	if (respuesta == "000") {
		
	}
	else {
		
	}
	*/

	respuesta = UnCliente.RecibirMensaje(40);
	cout << respuesta << "\n";
	pause();
}

void CerrarSesion() {
	string mensaje = "OUT";
	UnCliente.EnviarMensaje(mensaje, 4);

	string respuesta = UnCliente.RecibirMensaje(40);
	cout << respuesta << "\n";
	pause();
}

void MenuPrincipal()
{
	int opcion = 0;

	while((opcion < 1) || (opcion > 6))
	{
		clear();
		cout << "MENU PRINCIPAL" << endl << endl <<
		"1- Iniciar Sesion" << endl <<
		"2- Cerrar Sesion" << endl <<
		"3- Salir" << endl <<
		"4- Enviar" << endl <<
		"5- Recibir" << endl <<
		"6- Lore Ipsum" << endl << endl << "Ingrese una opcion: ";
		cin >> opcion;
	}
	switch(opcion)
	{
		case 1:
			IniciarSesion();
			break;
		case 2:
			CerrarSesion();
			break;
		case 3:
			clear();
			cout << "El programa se cerrara.";
			Sleep(3000);
			exit(0);
			break;
		case 4:
			//cliente.enviar()
			break;
		case 5:
			//cliente.recibir()
			break;
		case 6:
			//cliente.enviarCiclicamente()
			break;
	}
	MenuPrincipal();
}

int main(int argc, char **argv)
{
	string ip;
	string puerto;

	cout << "Ingrese la IP del Servidor (Usar localhost para local): ";
	cin >> ip;
	cout << "Ingrese el puerto de conexion: ";
	cin >> puerto;

	UnCliente.ConectarAServidor(ip, puerto);

	_beginthread(ThreadPrincipal, 0, NULL);

	MenuPrincipal();
	
	return 0;
}