#undef UNICODE
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Server.h"


using namespace std;

// Importacion dinamica de libreria Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

// Tamano max del buffer. Por ej, en el recv() todos los recvbuf llegan con esta leng (aunque el mensaje sea mas corto)


int main()
{

	string puerto;
	cout << "Ingrese el puerto donde escuchar conexiones el servidor: ";
	cin >> puerto;	

	Server server(puerto /*"8090"*/);

	while (server.aceptaConexion())
	{
		/*aceptamos una conexiones*/
		std::cout << "conexion aceptada" << std::endl;

		
		/******leemos los datos desde el socket*/
		char consultaCliente[100];
		memset(consultaCliente, 0, 99);
		int cantBitesLeidos;
		int respuestaEnviarDatos;

		do{
			cantBitesLeidos = server.recibirDatos(consultaCliente, sizeof(consultaCliente));


			string mensajeCliente(consultaCliente);
			cout << "Mensaje del cliente: " << mensajeCliente << "\n";

			/*preparamos respuesta al cliente*/
			
			string auxRespuestaServidor = "hola " + mensajeCliente;

			/***enviamos la respuesta****/
			respuestaEnviarDatos = server.enviarDatos(auxRespuestaServidor.c_str(), strlen(auxRespuestaServidor.c_str()));
			std::cout << "Se envia respuesta al cliente: " << auxRespuestaServidor << std::endl;

			if (respuestaEnviarDatos == SOCKET_ERROR) {
				std::cout << "fallo al enviar los datos: " << WSAGetLastError() << std::endl;
				system("pause");
				exit(0);
			}

			/*consulto si hubo algun error*/
			if (server.isSocketInvalido())
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

	
	} /*end while*/


	return 0;
}
