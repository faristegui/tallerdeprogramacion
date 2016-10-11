#include "Jugador.h"
#include "Lista.h"

#pragma once
struct Posicion {
	int x;
	int y;
};

class Juego
{
public:
	Juego();
	~Juego();
	void AgregarJugador(std::string UnNombre, std::string UnIDSprite);
	void RecibirEvento(std::string Usuario, std::string Tipo);
	bool UsuarioYaLogueado(std::string Usuario);
	Jugador* GetJugador(std::string UnNombre);
	Jugador* GetJugador(int Index);
	Lista<std::string>* GetNombresJugadoresOnline();
	int GetIndiceJugador(std::string Usuario);
	int GetCantJugadores();
	Posicion GetCamara();
private:
	int CantJugadores;
	int GetIndexUsuario(std::string Usuario);
	Jugador *Jugadores[5];
	Posicion Camara;
};

