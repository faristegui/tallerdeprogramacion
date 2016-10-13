#undef UNICODE
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <process.h>

#include "Server.h"
#include "Juego.h"
#include "Usuarios.h"
#include "Mensaje.h"
#include "Globales.h"
#include "tinyxml2.h"

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#ifdef WIN32 
#define ClearScreen() system("cls");
#define pause() system("pause");
#else 
#define ClearScreen() system("clear");
#define pause() system("pause");
#endif

Server UnServer;
Usuarios ControlUsuarios;
#define MAX_CLIENTES 6
int CantidadClientes = 0;

HANDLE ghMutex;
Juego UnJuego;

void MostrarListaComandos() {
	cout << "Ingrese la letra ""q"" si desea apagar el servidor: ";
}

void MainListenThread(void* arg) {
	string Usuario = "";
	string mensaje = "";
	SOCKET ClientSocket = *(SOCKET*)arg;
	bool EsThreadDePing = false;

	while ((mensaje != "EXIT") && (mensaje != "LOST"))
	{
		mensaje = UnServer.RecibirMensaje(ClientSocket, 4);

		if (mensaje == "AUTH") {

			string UsuarioMsj = UnServer.RecibirMensaje(ClientSocket, 15);
			string PasswordMsj = UnServer.RecibirMensaje(ClientSocket, 15);

			string CodigoRespuesta;
			string MensajeRespuesta;

			if (ControlUsuarios.ContrasenaValida(UsuarioMsj, PasswordMsj)) {

				if (!UnJuego.UsuarioYaLogueado(UsuarioMsj)) {

					Usuario = ToLowerCase(UsuarioMsj);

					Lista<std::string>* UsuariosOnline = UnJuego.GetNombresJugadoresOnline();
					std::string Mensaje = Usuario + " se conecto";
					UnServer.enviarATodos(Mensaje, Usuario, UsuariosOnline);

					std::string IDSprite;

					// TODO: Ver como determinar que sprite mandarle a cada player
					if (UnJuego.GetCantJugadores() == 0) {
						IDSprite = "PlayerRed";
					} 
					else {
						IDSprite = "PlayerYellow";
					}

					UnJuego.AgregarJugador(Usuario, IDSprite);

					CodigoRespuesta = "000";
					MensajeRespuesta = "Player: " + Usuario;

					UnServer.EscribirLog("Usuario " + Usuario + " logueado correctamente.", true);
				} else {

					CodigoRespuesta = "410";
					MensajeRespuesta = UsuarioMsj + " ya se encuentra en logueado.";

					UnServer.EscribirLog("Usuario " + Usuario + " logueado correctamente.", true);
				}
			}
			else {

				CodigoRespuesta = "401";
				MensajeRespuesta = "El usuario y la contrasena no coinciden";

				UnServer.EscribirLog("Fallo de autenticacion de usuario: " + Usuario, true);
			}

			UnServer.EnviarMensaje(CodigoRespuesta, 3, ClientSocket);
			UnServer.EnviarMensaje(MensajeRespuesta, 40, ClientSocket);
		}
		if (mensaje == "EVEN") {

			string Tipo = UnServer.RecibirMensajeTamanoVariable(ClientSocket);
			UnJuego.RecibirEvento(Usuario, Tipo);
		}
		if (mensaje == "STAT") {

			int CantJugadores = UnJuego.GetCantJugadores();
			std::string StrCantJugadores = IntAString(CantJugadores);

			Posicion UnaCamara = UnJuego.GetCamara();
			std::string CamaraX = IntAString(UnaCamara.x);
			std::string CamaraY = IntAString(UnaCamara.y);

			std::string GranMensaje = "";

			//UnServer.EnviarMensajeTamanoVariable("0", ClientSocket);
			//UnServer.EnviarMensajeTamanoVariable("0", ClientSocket);

			//Posicion unaCamaraPared = UnJuego.getCamaraPared();

			//UnServer.EnviarMensajeTamanoVariable(IntAString(unaCamaraPared.x),ClientSocket);
			//UnServer.EnviarMensajeTamanoVariable(IntAString(unaCamaraPared.y),ClientSocket);

			//Posicion unaCamaraCielo = UnJuego.getCamaraCielo();

			//UnServer.EnviarMensajeTamanoVariable(IntAString(unaCamaraCielo.x),ClientSocket);
			//UnServer.EnviarMensajeTamanoVariable(IntAString(unaCamaraCielo.y),ClientSocket);

			//UnServer.EnviarMensaje(StrCantJugadores, 1, ClientSocket);
			int IndiceMiJugador = UnJuego.GetIndiceJugador(Usuario);
			Jugador* MiJugador = UnJuego.GetJugador(IndiceMiJugador);

			GranMensaje.append(CamaraX); // CamaraX
			GranMensaje.append(";");
			GranMensaje.append(CamaraY); // CamaraY
			GranMensaje.append(";");
			GranMensaje.append(StrCantJugadores);
			GranMensaje.append(";");
			
			for (int i = 0; i < CantJugadores; i++) {

				if (i != IndiceMiJugador) {

					Jugador* OtroJugador = UnJuego.GetJugador(i);

					string Nombre = OtroJugador->GetNombre();
					string IDSprite = OtroJugador->GetIDSprite();
					string Estado = OtroJugador->GetEstado();
					string PosX = IntAString(OtroJugador->GetX());
					string PosY = IntAString(OtroJugador->GetY());

					GranMensaje.append(Nombre);
					GranMensaje.append(";");
					GranMensaje.append(IDSprite);
					GranMensaje.append(";");
					GranMensaje.append(Estado);
					GranMensaje.append(";");
					GranMensaje.append(PosX);
					GranMensaje.append(";");
					GranMensaje.append(PosY);
					GranMensaje.append(";");
				}
			}
			// Renderizo por ultimo mi jugador para asi aparece al frente

			GranMensaje.append(MiJugador->GetNombre().c_str()); // Nombre
			GranMensaje.append(";");
			GranMensaje.append(MiJugador->GetIDSprite().c_str()); // Sprite
			GranMensaje.append(";");
			GranMensaje.append(MiJugador->GetEstado().c_str()); // Estado
			GranMensaje.append(";");
			GranMensaje.append(IntAString(MiJugador->GetX())); // PosX
			GranMensaje.append(";");
			GranMensaje.append(IntAString(MiJugador->GetY())); // PosY

			UnServer.EnviarMensaje(GranMensaje, 150, ClientSocket);

			// Si hay mensajes para el usuario -> le envio
			/*
			Lista<Mensaje*>* Buzon = UnServer.obtenerMensajesPara(Usuario);
			int CantidadMensajes = Buzon->getTamanio();
			UnServer.EnviarMensaje(IntAString(CantidadMensajes), 8, ClientSocket);
			if (CantidadMensajes > 0) {
				Buzon->iniciarCursor();

				while (Buzon->avanzarCursor())
				{
					string ContenidoMensaje = Buzon->obtenerCursor()->obtenerContenido();
					UnServer.EnviarMensajeTamanoVariable(ContenidoMensaje, ClientSocket);
				}
			}
			*/
		}
		if (mensaje == "ENVI")
		{
			string destinatario = UnServer.RecibirMensaje(ClientSocket, 15);
			string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);

			Mensaje* unMensaje = new Mensaje(Usuario, destinatario, contenidoMensaje);

			WaitForSingleObject(ghMutex, INFINITE);

			UnServer.agregarMensaje(unMensaje);
			
			ReleaseMutex(ghMutex);

			UnServer.EscribirLog("Mensaje enviado con exito, de: " + Usuario + " a " + destinatario + ". Mensaje: " + contenidoMensaje, false);
		}
		if (mensaje == "OUT") {

			if (Usuario != "") {
				UnServer.EnviarMensaje("Hasta la proxima " + Usuario, 40, ClientSocket);
				UnServer.EscribirLog(Usuario + " desconectado correctamente.", true);
				Usuario = "";
			} else {
				UnServer.EnviarMensaje("No existe ninguna sesion iniciada.", 40, ClientSocket);
				UnServer.EscribirLog("Fallo intento de Logout. No existe sesion iniciada.", true);
			}
		}
		if (mensaje == "PING")
		{
			UnServer.EnviarMensaje("OK", 2, ClientSocket);
			EsThreadDePing = true;
		}
		if (mensaje == "ENVT")
		{
			string contenidoMensaje = UnServer.RecibirMensajeTamanoVariable(ClientSocket);
			Lista<std::string>* TodosLosUsuarios = ControlUsuarios.obtenerTodos();

			// Eespera que termine de ejecutar
			WaitForSingleObject(ghMutex, INFINITE);

			UnServer.enviarATodos(contenidoMensaje, Usuario, TodosLosUsuarios);

			ReleaseMutex(ghMutex);

			UnServer.EscribirLog("Mensaje de " + Usuario + " enviado a todos los usuarios. Mensaje: " + contenidoMensaje, true);
		}
		if (mensaje == "REC")
		{
			if (Usuario != "") {
				string respuestaServer = "";
				//Uso del recurso
				WaitForSingleObject(ghMutex, INFINITE); 

				Lista<Mensaje*>* buzon = UnServer.obtenerMensajesPara(Usuario);
				stringstream ss;
				ss << buzon->getTamanio();
				string CantidadMensajes = ss.str();
				UnServer.EnviarMensaje(CantidadMensajes, 8, ClientSocket);

				buzon->iniciarCursor();

				while (buzon->avanzarCursor())
				{
					string UsuarioEmisor = buzon->obtenerCursor()->obtenerEmisor();
					string ContenidoMensaje = buzon->obtenerCursor()->obtenerContenido();

					UnServer.EnviarMensaje(UsuarioEmisor, 15, ClientSocket);
					UnServer.EnviarMensajeTamanoVariable(ContenidoMensaje, ClientSocket);
				}
				ReleaseMutex(ghMutex);
				//Liberacion del recurso
			} else {

				UnServer.EnviarMensaje("NOLOGIN", 8, ClientSocket);
			}
		}
		if (mensaje == "CHKU") {

			if (Usuario == "") {
				UnServer.EnviarMensaje("NO", 2, ClientSocket);
			} else {
				UnServer.EnviarMensaje("SI", 2, ClientSocket);
			}
		}
		// Carga de Sprites
		if (mensaje == "SPRI")
		{
			

			tinyxml2::XMLDocument docu;


			if (docu.LoadFile("Archivos\\escenario.xml") != tinyxml2::XML_ERROR_FILE_NOT_FOUND)
			{

				tinyxml2::XMLElement* elementoEscenario = docu.FirstChildElement();

				tinyxml2::XMLElement* elementoSprites = elementoEscenario->FirstChildElement("SPRITES");

				const char* cantidadSrites = elementoSprites->Attribute("cantidad");
				// Envio cantidad de sprites a cargar
				UnServer.EnviarMensajeTamanoVariable(cantidadSrites, ClientSocket);

				//iterando sobre todos los Sprites 
				for (tinyxml2::XMLElement* elementoSprite = elementoSprites->FirstChildElement("SPRITE"); elementoSprite != NULL; elementoSprite = elementoSprite->NextSiblingElement("SPRITE"))
				{

					// Envio info de los sprites a cargar
					const char* idSprite = elementoSprite->Attribute("id");
					UnServer.EnviarMensajeTamanoVariable(idSprite, ClientSocket);	// ID
					const char* frameWidthSprite = elementoSprite->Attribute("frameWidth");
					UnServer.EnviarMensajeTamanoVariable(frameWidthSprite, ClientSocket);			// FRAME WIDTH
					const char* frameHeightSprite = elementoSprite->Attribute("frameHeight");
					UnServer.EnviarMensajeTamanoVariable(frameHeightSprite, ClientSocket);			// FRAME HEIGHT

					tinyxml2::XMLElement* elementoEstados = elementoSprite->FirstChildElement("ESTADOS");
					const char* cantidadEstados = elementoEstados->Attribute("cantidad");
					UnServer.EnviarMensajeTamanoVariable(cantidadEstados, ClientSocket);			// CANT ESTADOS

					//iterando sobre todos los estados de un Sprite
					for (tinyxml2::XMLElement* elementoEstado = elementoEstados->FirstChildElement("ESTADO"); elementoEstado != NULL; elementoEstado = elementoEstado->NextSiblingElement("ESTADO"))
					{

						const char* idEstado = elementoEstado->Attribute("id");
						UnServer.EnviarMensajeTamanoVariable(idEstado, ClientSocket);		// NOMBRE ESTADO
						const char* cantFramesEstado = elementoEstado->Attribute("cantFrames");
						UnServer.EnviarMensajeTamanoVariable(cantFramesEstado, ClientSocket);			// CANT FRAMES ESTADO
					}

				}

			}
			else std::cout << "error al cargar el archivo de configuracion de escenario.xml " << std::endl;
		}
		if (mensaje == "NEWC") {

			if (CantidadClientes >= MAX_CLIENTES) {
				// Si el server alcanzo el maximo de usuarios -> Envio codigo de error
				UnServer.EnviarMensaje("001", 3, ClientSocket);
				UnServer.EnviarMensaje("El servidor alcanzo el maximo de clientes permitidos, vuelva a intentar mas tarde.", 65, ClientSocket);
			} else {
				// Si esta todo ok -> Envio lista de usuarios del server
				string TodosLosUsuarios = ControlUsuarios.obtenerTodosEnString(";");
				UnServer.EnviarMensaje("000", 3, ClientSocket);
				UnServer.EnviarMensajeTamanoVariable(TodosLosUsuarios, ClientSocket);
				CantidadClientes++;
			}
		}

		if (mensaje == "ESCN")
		{


			tinyxml2::XMLDocument docu;


			if (docu.LoadFile("Archivos\\escenario.xml") != tinyxml2::XML_ERROR_FILE_NOT_FOUND)
			{

				tinyxml2::XMLElement* elementoEscenario = docu.FirstChildElement();

				tinyxml2::XMLElement* elementoFondo = elementoEscenario->FirstChildElement("FONDO");



				//iterando sobre todas las capas del fondo 
				for (tinyxml2::XMLElement* elementoCapas = elementoFondo->FirstChildElement("CAPAS"); elementoCapas != NULL; elementoCapas = elementoCapas->NextSiblingElement("CAPAS"))
				{

					const char* cantidadCapasFondo = elementoCapas->Attribute("cantidad");
					// Envio cantidad de capas a cargar
					UnServer.EnviarMensajeTamanoVariable(cantidadCapasFondo, ClientSocket);



					//iterando sobre todos los estados de un Sprite
					for (tinyxml2::XMLElement* elementoImagen = elementoCapas->FirstChildElement("IMAGEN"); elementoImagen != NULL; elementoImagen = elementoImagen->NextSiblingElement("IMAGEN"))
					{

						const char* nombreImagen = elementoImagen->Attribute("nombre");
						UnServer.EnviarMensajeTamanoVariable(nombreImagen, ClientSocket);		// NOMBRE Imagen de la capa
						const char* zIndex = elementoImagen->Attribute("zIndex");
						UnServer.EnviarMensajeTamanoVariable(zIndex, ClientSocket);			// zIndex de la capa en el fondo
					}

				}

			}
			else std::cout << "error al cargar el archivo de configuracion de escenario.xml " << std::endl;
		}

	}

	if (!EsThreadDePing) { // Cliente se desconecto

		if (Usuario != "") {

			UnJuego.GetJugador(Usuario)->SetEstaConectado(false);
			Lista<std::string>* UsuariosOnline = UnJuego.GetNombresJugadoresOnline();
			std::string Mensaje = Usuario + " se desconecto";
			UnServer.enviarATodos(Mensaje, Usuario, UsuariosOnline);
		}

		CantidadClientes--;

		if (mensaje == "EXIT") {
			UnServer.EscribirLog("Un cliente se desconecto", true);
		}
		else {
			UnServer.EscribirLog("Un cliente perdio la conexion", true);
		}
	}

	closesocket(ClientSocket);
}

void MainServerThread(void* arg) {
	ghMutex = CreateMutex(NULL,FALSE,NULL);
	string Puerto = *(string*)arg;
	UnServer.Abrir(Puerto);

	while (true) {
		SOCKET ClientSocket = UnServer.RecibirNuevaConexion();
		_beginthread(MainListenThread, 0, (void*)&ClientSocket);
	}

}

void MenuPrincipal() {
	string comando;

	while (true)
	{
		MostrarListaComandos();
		cin >> comando;
		if (comando == "q")
		{
			cout << "Servidor off\n";
			break;
		}
	}
}

int main()
{

	cout << "Ingrese el puerto donde escuchara conexiones el servidor: ";
	string Puerto;

	std::getline(std::cin, Puerto);

	if (Puerto.empty())
	{
		Puerto = "1000";
		cout << "Se usara el puerto por default: 1000" << endl;
	}

	string NombreArchivoUsuarios = "";

	while (!ControlUsuarios.SetNombreArchivo(NombreArchivoUsuarios)) {

		cout << "Ingrese el nombre del archivo de usuarios: ";
		std::getline(std::cin, NombreArchivoUsuarios);

		if (NombreArchivoUsuarios.empty())
		{
			NombreArchivoUsuarios = "Usuarios.csv";
			cout << "Se usara el archivo por default: Usuarios.csv" << endl;
		}
	}

	// Thread 1: Inicializacion del server

	_beginthread(MainServerThread, 0, (void*)&Puerto);

	// Thread 0: Handler de eventos del server
	Sleep(1000);
	MenuPrincipal();

	return 0;
}
