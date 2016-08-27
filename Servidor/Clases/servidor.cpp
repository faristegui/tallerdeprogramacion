#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

// Importacion dinamica de libreria Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

// Tamano max del buffer. Por ej, en el recv() todos los recvbuf llegan con esta leng (aunque el mensaje sea mas corto)
#define DEFAULT_BUFLEN 512
// Puerto de escucha del server (puede ser cualquiera)
#define DEFAULT_PORT "27015"


int main()
{
	WSADATA wsaData;
	int iResult;

	// Inicializacion de las variables vacias
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// -----------------------------------------------------------------------------------------------------------
	// Inicializa el objeto WinSock. Los parametros van a ir fijos siempre estos.
	// El primero indica el numero de version de la DLL. Y el segundo es un puntero a la data que va a manejar (bien bien no se que es)
	// Explicacion de parametros: https://msdn.microsoft.com/en-us/library/windows/desktop/ms742213(v=vs.85).aspx
	// Este constructor devuelve un codigo de error. 0 = no error

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
		return 1;
	}

	// -----------------------------------------------------------------------------------------------------------
	// hints es un struct de tipo addrinfo que es necesario enviar a la funcion getaddrinfo (traduce los nombres de host al formato de
	// direccion aceptado por winsocket.
	//
	// Tiene informacion de como sera la conexion
	// Hay que especificar los siguientes parametros (no cambiar al menos que lo indiquen los profesores)
	// Todos los parametros existentes: https://msdn.microsoft.com/en-us/library/windows/desktop/ms740506(v=vs.85).aspx
	// Struct completo: https://msdn.microsoft.com/en-us/library/windows/desktop/ms737530(v=vs.85).aspx

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Invocacion del getaddrinfo con nuestros parametros del hints
	// Aca ya enviamos el puerto sobre el que vamos a recibir las conexiones
	// (Devuelve por referencia la variable "result")
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		system("pause");
		return 1;
	}

	// Creacion del objeto de tipo "socket" (usando de parametros los valores obtenidos antes)
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	// (Esto no se que hace, pero como todo lo demas.. supongo que no cambiara)
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	// Aca es la posta de las postas. Aca es donde literalmente se abre la conexion (con el listen).
	// Y el server ya esta listo para recibir conexiones
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server listo escuchando en el puerto: 27015");

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	// Aca el thread queda suspendido hasta que recibe una conexion
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);
	
	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Esto no esta ok pero funciona
			recvbuf[iResult] = 0;
			std::string str(recvbuf);
			std::cout << "Mensaje del cliente: " << str << "\n";

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
