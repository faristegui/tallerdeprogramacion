#include "Juego.h"

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
	for (int i = 0; i < 6; i++) {

		if (Jugadores[i]->GetNombre() == Usuario) {

			return i;
		}
	}
}

Jugador Juego::GetJugador(std::string Usuario) {
	int i = GetIndexUsuario(Usuario);

	return *Jugadores[i];
}

Juego::~Juego()
{
}
