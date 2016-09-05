#include "Server.h"

#ifdef WIN32 
#define clear() system("cls");
#define pause() system("pause");
#else 
#define clear() system("clear");
#define pause() system("pause");
#endif

using namespace std;

#define DEFAULT_BUFLEN 512
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

Server::Server()
{
	todosLosMensajes = new Lista<Mensaje*>;
	logFile.open("logServidor.txt");
}

void Server::agregarMensaje(Mensaje* unMensaje)
{
	todosLosMensajes->agregar(unMensaje);
}

void Server::enviarATodos(string contenidoMensaje, string emisor)
{
	Usuarios* instanciaUsuarios = new Usuarios();
	Lista<string>* todosLosUsuarios = instanciaUsuarios->obtenerTodos();
	todosLosUsuarios->iniciarCursor();
	while(todosLosUsuarios->avanzarCursor())
	{
		Mensaje* unMensaje = new Mensaje(emisor,todosLosUsuarios->obtenerCursor(),contenidoMensaje);
		todosLosMensajes->agregar(unMensaje);
	}
	
}

Lista<Mensaje*>* Server::obtenerMensajesPara(string destinatario)
{
	Lista<Mensaje*>* buzon = new Lista<Mensaje*>;
	int posicion = 1;
	int vuelta = 0;
	Lista<int>* posicionesAEliminar = new Lista<int>();
	todosLosMensajes->iniciarCursor();
	
	while(todosLosMensajes->avanzarCursor())
	{
		if(todosLosMensajes->obtenerCursor()->obtenerDestinatario() == destinatario)
		{
			buzon->agregar(todosLosMensajes->obtenerCursor());
			posicionesAEliminar->agregar(posicion);
		}
		posicion++;
	}
	posicionesAEliminar->iniciarCursor();
	while(posicionesAEliminar->avanzarCursor())
	{
		todosLosMensajes->remover(posicionesAEliminar->obtenerCursor()-vuelta);
		vuelta++;
	}
	return buzon;
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
		clear();
		cout << "Ha ocurrido  un error inesperado." << endl;
		this->EscribirLog("Error al inicializar el socket del Server. " + WSAGetLastError());
		pause();
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
		clear();
		cout << "Ha ocurrido un error con la inicializacion de la IP y el Puerto de conexion." << endl;
		this->EscribirLog("Error al inicializar configuracion de ip y puerto.");
		pause();
		WSACleanup();
		exit(0);
	}

	// Creacion del socket 
	this->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET)
	{
		clear();
		cout << "Ha ocurrido un error inesperado." << endl;
		this->EscribirLog("Error al crear el socket del servidor: " + WSAGetLastError());
		pause();
		freeaddrinfo(result);
		WSACleanup();
		exit(0);
	}

	// Setup the TCP listening socket	
	if (bind(this->ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		clear();
		cout << "Ha ocurrido un error inesperado." << endl;
		this->EscribirLog("Error al asociar el Socket a la IP y Puerto seleccionado" + WSAGetLastError());
		pause();
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
		clear();
		cout << "Ha ocurrido un error inesperado." << endl;
		this->EscribirLog("Error al escuchar conexiones en socketServer: " + WSAGetLastError());
		closesocket(ListenSocket);
		pause();
		WSACleanup();
		exit(0);
	}

	cout << "Server activado escuchando en el puerto: " << UnPuerto << endl;
	this->EscribirLog("Servidor activado. Puerto: " + UnPuerto);
}

SOCKET Server::RecibirNuevaConexion() {
	SOCKET UnSocketClient = accept(this->ListenSocket, NULL, NULL);
	cout << "Conexion entrante" << endl;
	return UnSocketClient;
}

const string obtenerDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y - %X - ", &tstruct);

    return buf;
}

//Log del cliente
void Server::EscribirLog(string mens)
{
	string logString = obtenerDateTime() + mens;

	if(logString.size() > 128)
	{
		logString.resize (125); // Lo corto en 125 para que no exceda los 128 caracteres con los 3 puntos.
		logString = logString + "...";
	}

	logFile << logString << endl;
}

string Server::RecibirMensaje(SOCKET ClientSocket, int tam) {
	int cantidadBytesRecibidos;

	cantidadBytesRecibidos = recv(ClientSocket, recvbuf, tam, 0);
	recvbuf[cantidadBytesRecibidos] = 0; // 0 = NULL terminator del string
	string mensajeCliente(recvbuf);
	cout << "Mensaje del cliente: " << mensajeCliente << endl;

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
	cout << "Cerrando server: .." << WSAGetLastError() << endl;
	this->EscribirLog("Servidor cerrado.");
	WSACleanup();
	logFile.close();
}
