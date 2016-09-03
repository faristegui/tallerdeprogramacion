#include "Client.h"

#ifdef WIN32 
#define clear() system("cls");
#define pause() system("pause");
#else 
#define clear() system("clear");
#define pause() system("pause");
#endif

#define DEFAULT_BUFLEN 512
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

using namespace std;

Client::Client()
{
    logFile.open("logCliente.txt");
}

void Client::ConectarAServidor(string UnaIP, string UnPuerto)
{
	WSADATA wsaData;
	struct addrinfo *result;
	struct addrinfo hints;
	struct addrinfo *ptr;

	this->ClientConnectionSocket = INVALID_SOCKET;

	result = NULL;
	ptr = NULL;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

		cout << "Error al inicializar el socket del cliente: " << WSAGetLastError() << endl;
		pause();
		WSACleanup();
		exit(0);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// configuracion de ip y puerto de conexion con el server

	if (getaddrinfo(UnaIP.c_str(), UnPuerto.c_str(), &(hints), &(result)) != NO_ERROR) {

		cout << "Error al inicializar configuracion de ip y puerto" << endl;
		this->EscribirLog("Error al inicializar configuracion de ip y puerto.");
		pause();
		WSACleanup();
		exit(0);
	}

	//**************************//
	ptr = result;
	//creacion de socket para conectarse con el servidor
	this->ClientConnectionSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (this->ClientConnectionSocket == INVALID_SOCKET)
	{
		cout << "Error al crear el socket del cliente: " << WSAGetLastError() << endl;
		pause();
		freeaddrinfo(result);
		WSACleanup();
		exit(0);
	}
	// conectarse con la ip del server
	for (ptr= result; ptr != NULL; ptr = ptr->ai_next) {


		// Conectando al servidor

		if (connect(this->ClientConnectionSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(this->ClientConnectionSocket);
			this->ClientConnectionSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (this->ClientConnectionSocket == INVALID_SOCKET) {
		cout << "Fallo al intentar conectar con el servidor" << WSAGetLastError() << endl;
		this->EscribirLog("Fallo al intentar conectar con el servidor.");
		pause();
		WSACleanup();
		freeaddrinfo(result);
		exit(0);
	}
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
	logFile << obtenerDateTime() << mens << endl;
}

int Client::EnviarMensaje(string mensaje, int sizeDatos)
{
	const char* datosEnviados = mensaje.c_str();
	int cantidadBytesEnviados;
	cantidadBytesEnviados = send(this->ClientConnectionSocket, datosEnviados, sizeDatos, 0);
	if (cantidadBytesEnviados == SOCKET_ERROR) {

		cout << "Fallo al enviar los datos: " << WSAGetLastError() << endl;
		this->EscribirLog("Fallo al enviar los datos.");
		pause();
		exit(0);
	}

	return cantidadBytesEnviados;
}

string Client::RecibirMensaje(int sizeDatos)
{
	int cantidadBytesRecibidos;
	
	cantidadBytesRecibidos = recv(this->ClientConnectionSocket, recvbuf, sizeDatos, 0);
	recvbuf[cantidadBytesRecibidos] = 0;
	string mensajeCliente(recvbuf);

	return mensajeCliente;

}

void Client::cerrarConexionConServer()
{

	// shutdown the connection since no more data will be sent

	if (shutdown(this->ClientConnectionSocket, SD_SEND) == SOCKET_ERROR) {
		cout << "Fallo al cerrar la conexion con el server: " << WSAGetLastError();
		this->EscribirLog("Fallo al cerrar la conexion con el server");
		closesocket(this->ClientConnectionSocket);
		WSACleanup();
		exit(0);
	}

}
Client::~Client()
{
	string auxMensajeEnviar = "logoff";
	EnviarMensaje(auxMensajeEnviar.c_str(), strlen(auxMensajeEnviar.c_str()));
	closesocket(this->ClientConnectionSocket);
	this->EscribirLog("Conexion cerrada.");
	logFile.close();
}
