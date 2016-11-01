#include "Jugador.h"
#include <string>
#pragma once
class Equipo
{
public:
	Equipo(void);
	~Equipo(void);
	void agregarJugador(Jugador* nuevoJugador);
	int obtenerPuntaje();
private:
	Jugador* Jugadores[4];
	int puntaje;
	int cantJugadores;
};

