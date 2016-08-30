#include "Server.h"

Server::Server(std::string puerto)
{

	// Inicializacion de las variables

	this->puerto = puerto;
	this->result = NULL;
	this->serverListenSocket = INVALID_SOCKET;


		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
		{
			std::cout << "Error al inicializar el socket del Server: " << WSAGetLastError() << std::endl;
			system("pause");
			WSACleanup();
			exit(0);
		}

		ZeroMemory(&(this->hints), sizeof(this->hints));
		this->hints.ai_family = AF_INET;
		this->hints.ai_socktype = SOCK_STREAM;
		this->hints.ai_protocol = IPPROTO_TCP;
		this->hints.ai_flags = AI_PASSIVE;


		// Invocacion del getaddrinfo con nuestros parametros del hints
		// Aca ya enviamos el puerto sobre el que vamos a recibir las conexiones
		// (Devuelve por referencia la variable "result")
		if (getaddrinfo(NULL, puerto.c_str(), &(this->hints), &(this->result)) != NO_ERROR)
		{
			std::cout << "Error al inicializar configuracion de ip y puerto" << std::endl;
			system("pause");
			WSACleanup();
			exit(0);
		}

		// Creacion del socket 
		this->serverListenSocket = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
		if (this->serverListenSocket == INVALID_SOCKET)
		{

			std::cout << "Error al crear el socket del servidor: " << WSAGetLastError() << std::endl;
			system("pause");
			freeaddrinfo(result);
			WSACleanup();
			exit(0);
		}

		this->backupListenSocket = this->serverListenSocket;

		binding(AF_INET);
		escucharConexiones();

}


void Server::binding(int af)
{
	// Setup the TCP listening socket

	
	if (bind(this->serverListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{

		std::cout << "fallo al bindear o asociar el socket a la ip y puerto seleccionado" << WSAGetLastError() << std::endl;
		system("pause");
		WSACleanup();
		freeaddrinfo(result);
		closesocket(serverListenSocket);
		exit(0);
	}

	freeaddrinfo(result);

}

void Server::escucharConexiones()
{

	// Aca es la posta de las postas. Aca es donde literalmente se abre la conexion (con el listen).
	// Y el server ya esta listo para recibir conexiones

	if (listen(serverListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{


		std::cout << "Error al escuchar conexiones en socketServer: " <<  WSAGetLastError() << std::endl;
		closesocket(serverListenSocket);
		system("pause");
		WSACleanup();
		exit(0);
	}

	std::cout << "Server listo escuchando en el puerto: " << this->puerto << std::endl;


}



bool Server::aceptaConexion()
{
	return ((this->acceptSocket = accept(this->backupListenSocket, NULL, NULL)) != INVALID_SOCKET);
}

int Server::recibirDatos(char* datosRecibidos, int sizeDatos)
{
	int cantidadBytesRecibidos;

	cantidadBytesRecibidos = recv(this->acceptSocket, datosRecibidos, sizeDatos, 0);
	if (cantidadBytesRecibidos > 0) {
		std::cout << "Bytes recibidos: " << cantidadBytesRecibidos << std::endl;
		datosRecibidos[cantidadBytesRecibidos] = 0;
		
		}

	return cantidadBytesRecibidos;
}



int Server::enviarDatos(const char* datosEnviados, int sizeDatos)
{
		// Send an initial buffer
		int respuesta = send(this->acceptSocket, datosEnviados, (int)strlen(datosEnviados), 0);


		return respuesta;
	

}
void Server::cerrarConexion()
{
	closesocket(this->serverListenSocket);
}

bool Server::isSocketInvalido()
{
	return (this->serverListenSocket == INVALID_SOCKET);

}
Server::~Server()
{

	std::cout << "cerrando server: .." << WSAGetLastError() << std::endl;
	closesocket(this->acceptSocket);
	WSACleanup();
}
