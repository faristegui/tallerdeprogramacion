#include "Equipo.h"


Equipo::Equipo(void)
{
	puntaje = 0;
	cantJugadores = 0;
}
int Equipo::obtenerPuntaje()
{
	return puntaje;
}
void Equipo::agregarJugador(Jugador* nuevoJugador)
{
	Jugadores[cantJugadores] = nuevoJugador;
	cantJugadores++;
}

Equipo::~Equipo(void)
{
}
