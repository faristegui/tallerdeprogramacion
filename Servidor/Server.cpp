#include "Server.h"

using namespace std;

#define DEFAULT_BUFLEN 512
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

Server::Server()
{
	todosLosMensajes = new Lista<Mensaje*>;
}

void Server::agregarMensaje(Mensaje* unMensaje)
{
	todosLosMensajes->agregar(unMensaje);
}

void Server::Abrir(string UnPuerto) {
	// Inicializacion de las variables

	WSADATA wsaData;
	sockaddr_in dirSocketServidor;
	struct addrinfo *result;
	struct addrinfo hints;
	int iSendResult;

	this->ListenSocket = INVALID_SOCKET;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		std::cout << "Error al inicializar el socket del Server: " << WSAGetLastError() << std::endl;
		system("pause");
		WSACleanup();
		exit(0);
	}

	ZeroMemory(&(hints), sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	// Invocacion del getaddrinfo con nuestros parametros del hints
	// Aca ya enviamos el puerto sobre el que vamos a recibir las conexiones
	// (Devuelve por referencia la variable "result")
	if (getaddrinfo(NULL, UnPuerto.c_str(), &(hints), &(result)) != NO_ERROR)
	{
		std::cout << "Error al inicializar configuracion de ip y puerto" << std::endl;
		system("pause");
		WSACleanup();
		exit(0);
	}

	// Creacion del socket 
	this->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET)
	{

		std::cout << "Error al crear el socket del servidor: " << WSAGetLastError() << std::endl;
		system("pause");
		freeaddrinfo(result);
		WSACleanup();
		exit(0);
	}

	// Setup the TCP listening socket	
	if (bind(this->ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{

		std::cout << "fallo al bindear o asociar el socket a la ip y puerto seleccionado" << WSAGetLastError() << std::endl;
		system("pause");
		WSACleanup();
		freeaddrinfo(result);
		closesocket(ListenSocket);
		exit(0);
	}

	freeaddrinfo(result);

	// Aca es la posta de las postas. Aca es donde literalmente se abre la conexion (con el listen).
	// Y el server ya esta listo para recibir conexiones

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Error al escuchar conexiones en socketServer: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		system("pause");
		WSACleanup();
		exit(0);
	}

	std::cout << "Server listo escuchando en el puerto: " << UnPuerto << std::endl;
}

SOCKET Server::RecibirNuevaConexion() {
	SOCKET UnSocketClient = accept(this->ListenSocket, NULL, NULL);
	std::cout << "Conexion entrante" << std::endl;

	return UnSocketClient;
}

string Server::RecibirMensaje(SOCKET ClientSocket, int tam) {
	int cantidadBytesRecibidos;

	cantidadBytesRecibidos = recv(ClientSocket, recvbuf, tam, 0);
	recvbuf[cantidadBytesRecibidos] = 0; // 0 = NULL terminator del string
	string mensajeCliente(recvbuf);
	cout << "Mensaje del cliente: " << mensajeCliente << "\n";

	return mensajeCliente;
}

int Server::EnviarMensaje(string mensaje, int sizeDatos, SOCKET ClientSocket)
{
	const char* datosEnviados = mensaje.c_str();
	// Send an initial buffer
	int respuesta = send(ClientSocket, datosEnviados, (int)strlen(datosEnviados), 0);
	return respuesta;
}

Server::~Server()
{
	closesocket(this->ListenSocket);
	std::cout << "Cerrando server: .." << WSAGetLastError() << std::endl;
	WSACleanup();
}
