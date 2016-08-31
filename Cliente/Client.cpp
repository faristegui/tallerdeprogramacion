#include "Client.h"

#ifdef WIN32 
#define clear() system("cls");
#define pause() system("pause");
#else 
#define clear() system("clear");
#define pause() system("pause");
#endif

Client::Client(int af, int tipo, int protocolo, std::string ip, std::string puerto,const char* usuario, std::string clave)
{
	this->ClientConnectionSocket = INVALID_SOCKET;


	this->result = NULL;
	this->puerto = puerto;
	this->ipServer = ip;
	this->ptr = NULL;
	this->usuario = usuario;
	this->clave = clave;

	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

		std::cout << "Error al inicializar el socket del cliente: " << WSAGetLastError() << std::endl;
		pause();
		WSACleanup();
		exit(0);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = af;
	hints.ai_socktype = tipo;
	hints.ai_protocol = protocolo;

	// configuracion de ip y puerto de conexion con el server

	if (getaddrinfo(this->ipServer.c_str(), this->puerto.c_str(), &(this->hints), &(this->result)) != NO_ERROR) {
		
		std::cout << "Error al inicializar configuracion de ip y puerto" << std::endl;
		pause();
		WSACleanup();
		exit(0);
	}

	//**************************//
	this->ptr = this->result; 
	//creacion de socket para conectarse con el servidor
	this->ClientConnectionSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);
	if (this->ClientConnectionSocket == INVALID_SOCKET) 
	{
		std::cout << "Error al crear el socket del cliente: " << WSAGetLastError() << std::endl;
		pause();
		freeaddrinfo(result);
		WSACleanup();
		exit(0);
	}

}

void Client::conectarAServidor()
{
	// conectarse con la ip del server
	for (this->ptr= this->result; this->ptr != NULL; this->ptr = this->ptr->ai_next) {


		// Conectando al servidor

		if (connect(this->ClientConnectionSocket, this->ptr->ai_addr, (int)this->ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(this->ClientConnectionSocket);
			this->ClientConnectionSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(this->result);

	if (this->ClientConnectionSocket == INVALID_SOCKET) {
		std::cout << "fallo al intentar conectar con el servidor" << WSAGetLastError() << std::endl;
		pause();
		WSACleanup();
		freeaddrinfo(this->result);
		exit(0);


	}


}


int Client::enviarDatos(const char* datosEnviados, int sizeDatos)
{
	int cantidadBytesEnviados;
	cantidadBytesEnviados = send(this->ClientConnectionSocket, datosEnviados, sizeDatos, 0);
	if (cantidadBytesEnviados == SOCKET_ERROR) {

		std::cout << "fallo al enviar los datos: " << WSAGetLastError() << std::endl;
		pause();
		exit(0);
	}

	std::cout << "Bytes enviados al Server: " << cantidadBytesEnviados << std::endl;
	return cantidadBytesEnviados;
}

int Client::recibirDatos(char* datosRecibidos, int sizeDatos)
{
	int cantidadBytesRecibidos;

	
	cantidadBytesRecibidos = recv(this->ClientConnectionSocket, datosRecibidos, sizeDatos, 0);
	if (cantidadBytesRecibidos > 0) {
		std::cout << "Bytes recibidos: " << cantidadBytesRecibidos << std::endl;
		datosRecibidos[cantidadBytesRecibidos] = 0;

	}
	else
		if(cantidadBytesRecibidos == 0)
		std::cout << "Bytes recibidos: " << cantidadBytesRecibidos << " la conexion esta cerrada" <<  std::endl;
		else
			std::cout << "fallo con error: " <<  WSAGetLastError();

	return cantidadBytesRecibidos;

}
const char* Client::getUserClientName()
{
	return this->usuario;

}
void Client::cerrarConexionConServer()
{

	// shutdown the connection since no more data will be sent

	if (shutdown(this->ClientConnectionSocket, SD_SEND) == SOCKET_ERROR) {
		std::cout << "fallo al cerrar la conexion con el server: " << WSAGetLastError();
		closesocket(this->ClientConnectionSocket);
		WSACleanup();
		exit(0);
	}

}
Client::~Client()
{
	closesocket(this->ClientConnectionSocket);
}
