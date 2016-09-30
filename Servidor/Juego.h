#include "Jugador.h"

#pragma once
class Juego
{
public:
	Juego();
	~Juego();
	void AgregarJugador(std::string UnNombre, std::string UnIDSprite);
	void RecibirEvento(std::string Usuario, std::string Tipo);
	bool UsuarioYaLogueado(std::string Usuario);
	Jugador GetJugador(std::string UnNombre);
	Jugador GetJugador(int Index);
	int GetCantJugadores();
private:
	int CantJugadores;
	int GetIndexUsuario(std::string Usuario);
	Jugador *Jugadores[5];
};

