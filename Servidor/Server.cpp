#include "Server.h"
#include <WinBase.h>
#include <Windows.h>

#ifdef WIN32 
#define ClearScreen() system("cls");
#define pause() system("pause");
#else 
#define ClearScreen() system("clear");
#define pause() system("pause");
#endif

//#include <mutex>
//using namespace std;
#define MAX_BUFFER_LENGTH 512
//std::mutex UnMutex;
//using namespace System::Threading;
Server::Server()
{
	todosLosMensajes = new Lista<Mensaje*>;
	logFile.open("logServidor.txt");
}

void Server::agregarMensaje(Mensaje* unMensaje)
{

	//UnMutex.lock();
	//std::cout << "El contenido es: " << unMensaje->obtenerContenido() << std::endl << std::endl;
	todosLosMensajes->agregar(unMensaje);
	//UnMutex.unlock();
}

void Server::enviarATodos(std::string contenidoMensaje, std::string emisor, Lista<std::string>* TodosLosUsuarios)
{
	Usuarios* instanciaUsuarios = new Usuarios();
	TodosLosUsuarios->iniciarCursor();
	while (TodosLosUsuarios->avanzarCursor())
	{
		Mensaje* unMensaje = new Mensaje(emisor, TodosLosUsuarios->obtenerCursor(), contenidoMensaje);
		//UnMutex.lock();
		todosLosMensajes->agregar(unMensaje);
		//UnMutex.unlock();
	}

}

Lista<Mensaje*>* Server::obtenerMensajesPara(std::string destinatario)
{
	//UnMutex.lock();

	Lista<Mensaje*>* buzon = new Lista<Mensaje*>;
	int posicion = 1;
	int vuelta = 0;
	Lista<int>* posicionesAEliminar = new Lista<int>();
	todosLosMensajes->iniciarCursor();
	std::string TmpNombreUsuario;

	// Convierte nombre de usuario a LowerCase
	transform(destinatario.begin(), destinatario.end(), destinatario.begin(), (int(*)(int))tolower);

	while (todosLosMensajes->avanzarCursor())
	{
		TmpNombreUsuario = todosLosMensajes->obtenerCursor()->obtenerDestinatario();
		// Convierte nombre de usuario a LowerCase
		transform(TmpNombreUsuario.begin(), TmpNombreUsuario.end(), TmpNombreUsuario.begin(), (int(*)(int))tolower);

		if (TmpNombreUsuario == destinatario)
		{
			buzon->agregar(todosLosMensajes->obtenerCursor());
			posicionesAEliminar->agregar(posicion);
		}
		posicion++;
	}
	posicionesAEliminar->iniciarCursor();
	while (posicionesAEliminar->avanzarCursor())
	{
		todosLosMensajes->remover(posicionesAEliminar->obtenerCursor() - vuelta);
		vuelta++;
	}


	//UnMutex.unlock();
	return buzon;
}


void Server::Abrir(std::string UnPuerto) {
	// Inicializacion de las variables

	WSADATA wsaData;
	//sockaddr_in dirSocketServidor; La comento porque est? sin referencia
	struct addrinfo *result;
	struct addrinfo hints;
	//int iSendResult; La comento porque est? sin referencia

	this->ListenSocket = INVALID_SOCKET;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		ClearScreen();
		std::cout << "Ha ocurrido  un error inesperado." << std::endl;
		this->EscribirLog("Error al inicializar el socket del Server. " + WSAGetLastError(), true);
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
		ClearScreen();
		std::cout << "Ha ocurrido un error con la inicializacion de la IP y el Puerto de conexion." << std::endl;
		this->EscribirLog("Error al inicializar configuracion de ip y puerto.", true);
		pause();
		WSACleanup();
		exit(0);
	}

	// Creacion del socket 
	this->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET)
	{
		ClearScreen();
		std::cout << "Ha ocurrido un error inesperado." << std::endl;
		this->EscribirLog("Error al crear el socket del servidor: " + WSAGetLastError(), true);
		pause();
		freeaddrinfo(result);
		WSACleanup();
		exit(0);
	}

	// Setup the TCP listening socket	
	if (bind(this->ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		ClearScreen();
		std::cout << "Ha ocurrido un error inesperado." << std::endl;
		this->EscribirLog("Error al asociar el Socket a la IP y Puerto seleccionado" + WSAGetLastError(), true);
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
		ClearScreen();
		std::cout << "Ha ocurrido un error inesperado." << std::endl;
		this->EscribirLog("Error al escuchar conexiones en socketServer: " + WSAGetLastError(), true);
		closesocket(ListenSocket);
		pause();
		WSACleanup();
		exit(0);
	}

	std::cout << "Server activado escuchando en el puerto: " << UnPuerto << std::endl;
	this->EscribirLog("Servidor activado. Puerto: " + UnPuerto, true);
}

SOCKET Server::RecibirNuevaConexion() {
	SOCKET UnSocketClient = accept(this->ListenSocket, NULL, NULL);
	std::cout << "Conexion entrante" << std::endl;
	return UnSocketClient;
}

const std::string obtenerDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%d-%m-%Y - %X - ", &tstruct);

	return buf;
}

//Log del cliente
void Server::EscribirLog(std::string mens, bool cortar)
{
	std::string logString = obtenerDateTime() + mens;

	if (logString.size() > 128 && cortar)
	{
		logString.resize(125); // Lo corto en 125 para que no exceda los 128 caracteres con los 3 puntos.
		logString = logString + "...";
	}
	logFile << logString << std::endl;
}

std::string Server::RecibirMensaje(SOCKET ClientSocket, int tam) {
	char recvbuf[MAX_BUFFER_LENGTH];
	int cantidadBytesRecibidos;
	std::string mensajeCliente;

	cantidadBytesRecibidos = recv(ClientSocket, recvbuf, tam, 0);
	if (cantidadBytesRecibidos > 0) {
		recvbuf[cantidadBytesRecibidos] = 0; // 0 = NULL terminator del std::string
		std::string tmpMsj(recvbuf);
		mensajeCliente = tmpMsj;
	}
	else {
		mensajeCliente = "LOST";
	}

	memset(recvbuf, 0, sizeof(recvbuf)); // Reseteo el array a 0 para no arrastrar basura

	std::cout << "Mensaje del cliente: " << mensajeCliente << std::endl;

	return mensajeCliente;
}

std::string Server::RecibirMensajeTamanoVariable(SOCKET ClientSocket) {

	int stringLength = atoi(RecibirMensaje(ClientSocket, 8).c_str());

	if (stringLength > MAX_BUFFER_LENGTH) {

		// TODO: Traer de a pedazos
	}

	std::string mensaje = RecibirMensaje(ClientSocket, stringLength);

	return mensaje;
}

int Server::EnviarMensajeTamanoVariable(std::string mensaje, SOCKET ClientSocket)
{
	// Como long de msj es variable envio:
	// 1: Tamano de string (fijo de 8)
	// 2: Msj (variable)

	// Paso de int a str (TODO: implementar una funcion)
	int Number = (int)mensaje.length();
	std::string stringLength;
	std::stringstream convert;
	convert << Number;
	stringLength = convert.str();

	EnviarMensaje(stringLength, 8, ClientSocket);
	return EnviarMensaje(mensaje, mensaje.length(), ClientSocket);
}

int Server::EnviarMensaje(std::string mensaje, int sizeDatos, SOCKET ClientSocket)
{
	const char* datosEnviados = mensaje.c_str();
	// Send an initial buffer
	// int respuesta = send(ClientSocket, datosEnviados, (int)strlen(datosEnviados), 0);
	int respuesta = send(ClientSocket, datosEnviados, sizeDatos, 0);
	return respuesta;
}

Server::~Server()
{
	closesocket(this->ListenSocket);
	//std::cout << "Cerrando server: .." << WSAGetLastError() << std::endl;
	this->EscribirLog("Servidor cerrado.", true);
	WSACleanup();
	logFile.close();
}