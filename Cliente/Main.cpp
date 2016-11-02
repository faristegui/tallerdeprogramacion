#include "Pantalla.h"
#include "Client.h"

Client UnCliente2;
Pantalla UnaPantalla(&UnCliente2);

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct ParametrosThread
{
	int Entero1;
	int Entero2;
	std::string String1;
	std::string String2;
};

void ThreadStatusx(void* pParams)
{
	Client ClientePing(0);
	bool status = false;
	bool serverCaido = false;
	int resultado = 0;
	ParametrosThread* Parametros = (ParametrosThread*)pParams;

	string IP = Parametros->String1;
	string Puerto = Parametros->String2;

	ClientePing.ConectarAServidor(IP, Puerto); //Envio primero puerto e IP.

	while (!serverCaido)
	{

		if (ClientePing.EnviarMensaje("PING", 4) == -1)//si el servidor no nos responde (enviarMensaje retorna -1)
		{
			status = false;
			serverCaido = true;
		}

		else
		{
			string respuesta = ClientePing.RecibirMensaje(2);
			if (respuesta == "OK")
			{
				status = true;

			}
			else status = false;
		}

		if (!status)// si el server se cayo lo logueo una sola vez
		{
			UnCliente2.EscribirLog("Conexion con el servidor terminada. (Server Offline).");
			cout << "Se ha perdido la conexion con el servidor. El programa se cerrara en 5 segundos." << endl << endl;
			Sleep(5000);
			exit(0);
		}
		//serverStatus = status;
		Sleep(30000);
	}
}

void PedirParametrosConexion() {

	bool ConexionOk = false;
	std::string IP;
	std::string Puerto;
	std::string Respuesta;
	std::string entrar;

	char* fondo = "start.bmp";

	while (!ConexionOk) {

		IP = UnaPantalla.PedirParametro("Ingrese IP", "localhost", 190, 270, fondo);
		Puerto = UnaPantalla.PedirParametro("Ingrese Puerto", "1000", 190, 270, fondo);

		ConexionOk = UnCliente2.ConectarAServidor(IP,Puerto);

		if (!ConexionOk) {
			UnaPantalla.MostrarMensaje("Ha habido un error al intentar conectarse.",100,250);
		}
	}

	UnCliente2.EnviarMensaje("NEWC", 4);
	Respuesta = UnCliente2.RecibirMensaje(3);

	if (Respuesta == "000") {

		string UsuariosEnString = UnCliente2.RecibirMensajeTamanoVariable();
		//ArmarListaUsuarios(UsuariosEnString);

		// Lanzo thread de status del server.
		ParametrosThread* ParametrosPing = new ParametrosThread;
		ParametrosPing->String1 = IP;
		ParametrosPing->String2 = Puerto;
		_beginthread(ThreadStatusx, 0, (void*)ParametrosPing);
	}
	else {

		string MensajeError = UnCliente2.RecibirMensaje(65);
		// TODO: Mostrar MensajeError;
	}
}


bool UsuarioYPassValidos() {
	std::string Parametro;
	std::string Mensaje;
	std::string Respuesta;
	std::string CodigoRespuesta;
	
	Mensaje = "AUTH";
	UnCliente2.EnviarMensaje(Mensaje, 4);

	char* fondo = "start.bmp";

	// Pido usuario
	Parametro = "";
	while (Parametro == "") {
		Parametro = UnaPantalla.PedirParametro("Usuario", "", 230, 270, fondo);
	}
	UnCliente2.EnviarMensaje(Parametro, 15);
	// Pido contrasena
	Parametro = "";
	while (Parametro == "") {
		Parametro = UnaPantalla.PedirParametro("Contrasena", "", 230, 270, fondo);
	}
	UnCliente2.EnviarMensaje(Parametro, 15);

	CodigoRespuesta = UnCliente2.RecibirMensaje(3);
	UnCliente2.EscribirLog("Autorizar usuario. Mensaje del servidor: " + Respuesta + ".");
	Respuesta = UnCliente2.RecibirMensaje(40);
	UnaPantalla.MostrarMensaje(Respuesta,190,300);

	return (CodigoRespuesta == "000");
}

void PedirModoDeJuego()
{
	std::string opcion = "0";
	char* fondo = "modojuego.bmp";
	while(opcion != "1" && opcion != "2" && opcion != "3")
	{
		opcion = UnaPantalla.PedirParametro("Ingrese una opcion","",250,450, fondo);
	}
	UnCliente2.EnviarMensaje("MODO",4);
	UnCliente2.EnviarMensaje(opcion,1);
}

int main(int argc, char* args[])
{
	
	PedirParametrosConexion();
	//pregunto si el juego tiene establecido un modo
	//el modo lo establece el primer jugador que se conecta
	/*UnCliente2.EnviarMensaje("MODE",4);
	std::string respuestaServer = UnCliente2.RecibirMensaje(2);
	if(respuestaServer=="NO")
	{
		PedirModoDeJuego();
	}
	*/
	if (UsuarioYPassValidos()) {

		UnaPantalla.IniciarJuego();
	}
	
	return 0;
}