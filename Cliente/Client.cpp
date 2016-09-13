#include "Client.h"
#include <sstream>

#ifdef WIN32 
#define ClearScreen() system("cls");
#define pause() system("pause");
#else 
#define ClearScreen() system("clear");
#define pause() system("pause");
#endif

#define MAX_BUFFER_LENGTH 512
// TODO: El siguiente array creo q hay q crearlo y matarlo dentro
//		 RecibirMensaje().. Como lo mato??
char recvbuf[MAX_BUFFER_LENGTH];

using namespace std;

Client::Client()
{
	logFile.open("logCliente.txt");
}

Client::Client(int log)
{
	//Crea el cliente sin la definici?n del archivo
}

bool Client::ConectarAServidor(string UnaIP, string UnPuerto)
{
	WSADATA wsaData;
	struct addrinfo *result;
	struct addrinfo hints;
	struct addrinfo *ptr;

	this->ClientConnectionSocket = INVALID_SOCKET;

	result = NULL;
	ptr = NULL;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ClearScreen();
		cout << "Ha ocurrido un error inesperado." << endl;
		this->EscribirLog("Error al inicializar el socket del cliente.");
		WSACleanup();
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// configuracion de ip y puerto de conexion con el server
	if (getaddrinfo(UnaIP.c_str(), UnPuerto.c_str(), &(hints), &(result)) != NO_ERROR) {
		ClearScreen();
		cout << "Ha ocurrido un error al enviar la configuracion de conexion." << endl;
		this->EscribirLog("Error al inicializar configuracion de ip y puerto.");
		WSACleanup();
		return false;
	}

	//**************************//
	ptr = result;
	//creacion de socket para conectarse con el servidor
	this->ClientConnectionSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (this->ClientConnectionSocket == INVALID_SOCKET)
	{
		ClearScreen();
		cout << "Ha ocurrido un error inesperado." << endl;
		this->EscribirLog("Error al crear el socket del cliente.");
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}
	// conectarse con la ip del server
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {


		// Conectando al servidor

		if (connect(this->ClientConnectionSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(this->ClientConnectionSocket);
			this->ClientConnectionSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	//freeaddrinfo(result);

	if (this->ClientConnectionSocket == INVALID_SOCKET) {
		ClearScreen();
		cout << "Ha ocurrido un error al intentar conectar con el Servidor." << endl;
		this->EscribirLog("Fallo al intentar conectar con el servidor. Codigo de error: " + WSAGetLastError());
		WSACleanup();
		freeaddrinfo(result);
		return false;
	}

	return true;
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
void Client::EscribirLog(string mens)
{
	string logString = obtenerDateTime() + mens;

	logFile << logString << endl;
}

int Client::EnviarMensaje(string mensaje, int sizeDatos)
{
	const char* datosEnviados = mensaje.c_str();
	int cantidadBytesEnviados;
	cantidadBytesEnviados = send(this->ClientConnectionSocket, datosEnviados, sizeDatos, 0);
	if (cantidadBytesEnviados == SOCKET_ERROR) {
		ClearScreen();
		//Entiendo que la unica posibilidad que se falle al enviar el mensaje sea porque se desconecto el servidor.
		//Agregaria en el log. Debido a perdida de conexion con el servidor. que opinan? [MZ]
		//Adopte esta solucion porque no se cerraba el programa cuando lo pausabamos. 
		this->EscribirLog("Se ha perdido la conexion con el servidor.");
		cout << "Se ha perdido la conexion con el servidor. El programa se cerrara en 5 segundos." << endl << endl;
		Sleep(5000);
		exit(0);
	}
	return cantidadBytesEnviados;
}


int Client::EnviarMensajeTamanoVariable(string mensaje)
{
	// Como long de msj es variable envio:
	// 1: Tamano de string (fijo de 8)
	// 2: Msj (variable)

	// Paso de int a str (TODO: implementar una funcion)
	int Number = (int)mensaje.length();
	string stringLength;
	std::stringstream convert;
	convert << Number;
	stringLength = convert.str();

	EnviarMensaje(stringLength, 8);
	return EnviarMensaje(mensaje, mensaje.length());
}

string Client::RecibirMensaje(int sizeDatos)
{
	int cantidadBytesRecibidos;

	cantidadBytesRecibidos = recv(this->ClientConnectionSocket, recvbuf, sizeDatos, 0);
	recvbuf[cantidadBytesRecibidos] = 0;
	string mensajeCliente(recvbuf);

	memset(recvbuf, 0, sizeof(recvbuf)); // Reseteo el array a 0 para no arrastrar basura

	return mensajeCliente;

}

std::string Client::RecibirMensajeTamanoVariable() {

	std::string mensaje = "";

	int stringLength = atoi(RecibirMensaje(8).c_str());

	if (stringLength > MAX_BUFFER_LENGTH) {

		int BytesALeer = MAX_BUFFER_LENGTH - 1;

		while (stringLength > 0) {

			mensaje = mensaje + RecibirMensaje(BytesALeer);

			stringLength -= BytesALeer;

			if (stringLength > MAX_BUFFER_LENGTH) {
				BytesALeer = MAX_BUFFER_LENGTH - 1;
			}
			else {
				BytesALeer = stringLength;
			}
		}
	}
	else {
		mensaje = RecibirMensaje(stringLength);
	}	

	return mensaje;
}

void Client::cerrarConexionConServer()
{

	// shutdown the connection since no more data will be sent

	if (shutdown(this->ClientConnectionSocket, SD_SEND) == SOCKET_ERROR)
	{
		ClearScreen();
		cout << "Ha ocurrido un error al cerrar la conexion con el Servidor.";
		this->EscribirLog("Fallo al cerrar la conexion con el Servidor.");
		closesocket(this->ClientConnectionSocket);
		WSACleanup();
		exit(0);
	}

}
Client::~Client()
{
	string auxMensajeEnviar = "EXIT";
	EnviarMensaje(auxMensajeEnviar, 4);
	closesocket(this->ClientConnectionSocket);
	this->EscribirLog("Conexion cerrada.");
	logFile.close();
}