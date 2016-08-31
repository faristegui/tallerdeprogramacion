#include "Server.h"
#include "Usuarios.h"

using namespace std;

#define DEFAULT_BUFLEN 512
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

Server::Server(std::string puerto)
{

	// Inicializacion de las variables

	this->puerto = puerto;
	this->result = NULL;
	this->ListenSocket = INVALID_SOCKET;

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
	this->ListenSocket = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET)
	{

		std::cout << "Error al crear el socket del servidor: " << WSAGetLastError() << std::endl;
		system("pause");
		freeaddrinfo(result);
		WSACleanup();
		exit(0);
	}
	
}

void Server::abrir() {
	binding(AF_INET);
	escucharConexiones();
}


void Server::binding(int af)
{
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

}

void Server::escucharConexiones()
{

	// Aca es la posta de las postas. Aca es donde literalmente se abre la conexion (con el listen).
	// Y el server ya esta listo para recibir conexiones

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Error al escuchar conexiones en socketServer: " <<  WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		system("pause");
		WSACleanup();
		exit(0);
	}

	std::cout << "Server listo escuchando en el puerto: " << this->puerto << std::endl;


	this->ClientSocket = accept(this->ListenSocket, NULL, NULL);
	/*aceptamos una conexiones*/
	std::cout << "conexion entrante" << std::endl;

	closesocket(this->ListenSocket); // TODO: Armar un while, sacarlo y dejar afuera del while (para aceptar multiples conexiones)

	/******leemos los datos desde el socket*/
	char consultaCliente[100];
	memset(consultaCliente, 0, 99);
	int cantBitesLeidos;
	int respuestaEnviarDatos;

	do {
		string tipoMensaje;

		cantBitesLeidos = recibirMensaje(tipoMensaje);
		
		/*preparamos respuesta al cliente*/

		string auxRespuestaServidor = "hola ";

		/***enviamos la respuesta****/
		respuestaEnviarDatos = enviarDatos(auxRespuestaServidor.c_str(), strlen(auxRespuestaServidor.c_str()));
		std::cout << "Se envia respuesta al cliente: " << auxRespuestaServidor << std::endl;

		if (respuestaEnviarDatos == SOCKET_ERROR) {
			std::cout << "fallo al enviar los datos: " << WSAGetLastError() << std::endl;
			system("pause");
			exit(0);
		}

		/*consulto si hubo algun error*/
		if (this->ListenSocket == INVALID_SOCKET)
		{
			std::cout << "fallo al aceptar conexiones con error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

	} while (cantBitesLeidos > 0);


	if (cantBitesLeidos == 0)
	{
		std::cout << "cerrando conexion del servidor.." << std::endl;
		exit(0);
	}

}

int Server::recibirMensaje(string& mensaje)
{
	int cantidadBytesRecibidos;

	cantidadBytesRecibidos = recv(this->ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
	if (cantidadBytesRecibidos > 0) {

		recvbuf[cantidadBytesRecibidos] = 0; // 0 = NULL terminator del string
		string mensajeCliente(recvbuf);
		cout << "Mensaje del cliente: " << mensajeCliente << "\n";
	}

	return cantidadBytesRecibidos;
}

int Server::enviarDatos(const char* datosEnviados, int sizeDatos)
{
		// Send an initial buffer
		int respuesta = send(this->ClientSocket, datosEnviados, (int)strlen(datosEnviados), 0);
		return respuesta;
}

void Server::autenticar(string usuario, string password) {
	Usuarios usuarios;
	
	if (usuarios.contrasenaValida(usuario, password)) {
		// TODO: Mandar msj credenciales validas
	}
	else {
		// TODO: Mandar msj credenciales invalidas
	}
}

Server::~Server()
{

	std::cout << "cerrando server: .." << WSAGetLastError() << std::endl;
	closesocket(this->ClientSocket);
	WSACleanup();
}
