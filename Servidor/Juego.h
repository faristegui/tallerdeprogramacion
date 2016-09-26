#include "Jugador.h"

#pragma once
class Juego
{
public:
	Juego();
	~Juego();
	void AgregarJugador(std::string UnNombre);
	void RecibirEvento(std::string Usuario, std::string Tipo);
	Jugador GetJugador(std::string UnNombre);
private:
	int CantJugadores;
	int GetIndexUsuario(std::string Usuario);
	Jugador *Jugadores[5];
};

