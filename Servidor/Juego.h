#include "Jugador.h"
#include "Lista.h"
#include <windows.h>
#include <process.h>

#pragma once
struct Posicion {
	int x;
	int y;
};

struct Camara {
	int X;
	int Y;
	int AnchoImagen;
	int Alto;
	int Velocidad;
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
	int GetCantCamaras();
	Camara* GetCamara(int NrCamara);
	void SetAnchoCamara(int NrCamara, int UnAncho);
	void BorrarCamaras();
	void AvanzarCamara();
	void AgregarCamara(int UnAncho);
private:
	int CantJugadores;
	int GetIndexUsuario(std::string Usuario);
	Jugador *Jugadores[5];
	Camara *Camaras[6];
	int CantCamaras;
};

