#include "Juego.h"
#include "Globales.h"

Juego::Juego()
{
	CantJugadores = 0;
}


void Juego::AgregarJugador(std::string UnNombre) {
	Jugadores[CantJugadores] = new Jugador(UnNombre);
	CantJugadores++;
}

void Juego::RecibirEvento(std::string Usuario, std::string Tipo) {
	int i = GetIndexUsuario(Usuario);

	if ((Tipo == "UP") || (Tipo == "RIGHT") || (Tipo == "DOWN") || (Tipo == "LEFT")) {

		Jugadores[i]->Mover(Tipo);
	}

}

int Juego::GetIndexUsuario(std::string Usuario) {
	Usuario = ToLowerCase(Usuario);
	
	for (int i = 0; i < CantJugadores; i++) {

		if (ToLowerCase(Jugadores[i]->GetNombre()) == Usuario) {

			return i;
		}
	}

	return -1;
}

int Juego::GetCantJugadores() {

	return CantJugadores;
}

Jugador Juego::GetJugador(std::string Usuario) {
	int i = GetIndexUsuario(Usuario);

	return *Jugadores[i];
}

Jugador Juego::GetJugador(int Index) {
	
	return *Jugadores[Index];
}


bool Juego::UsuarioYaLogueado(std::string Usuario) {
	return !(GetIndexUsuario(Usuario) == -1);
}

Juego::~Juego()
{
}
