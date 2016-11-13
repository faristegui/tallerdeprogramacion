#include "Equipo.h"


Equipo::Equipo(void)
{
	puntaje = 0;
	cantJugadores = 0;
}
int Equipo::obtenerPuntaje()
{
	int Puntaje = 0;

	for (int i = 0; i < cantJugadores; i++) {
		
		Puntaje += Jugadores[i]->getPuntaje();
	}

	return Puntaje;
}

std::string Equipo::GetNombresJugadores() {

	std::string NombresJugadores = "";

	for (int i = 0; i < cantJugadores; i++) {

		NombresJugadores = NombresJugadores + Jugadores[i]->GetNombre();

		if (i + 1 != cantJugadores) {

			NombresJugadores = NombresJugadores + ", ";
		}
	}

	return NombresJugadores;
}

bool Equipo::TieneJugador(std::string NombreJugador) {

	for (int i = 0; i < cantJugadores; i++) {
		if (Jugadores[i]->GetNombre() == NombreJugador) {

			return true;
		}
	}

	return false;
}

void Equipo::agregarJugador(Jugador* nuevoJugador)
{
	Jugadores[cantJugadores] = nuevoJugador;
	cantJugadores++;
}

Equipo::~Equipo(void)
{
}
