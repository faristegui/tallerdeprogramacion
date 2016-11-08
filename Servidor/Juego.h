#include "Jugador.h"
#include "Enemigo.h"
#include "Lista.h"
#include "Equipo.h"
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
	int GetCantEnemigos();
	int GetCantCamaras();
	int obtenerModo();
	Camara* GetCamara(int NrCamara);
	void establecerModo(std::string modo);
	void SetAnchoCamara(int NrCamara, int UnAncho);
	void BorrarCamaras();
	void AvanzarCamara();
	void AgregarCamara(int UnAncho);
	void AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad,int vida, bool esFinal);
	Enemigo* GetEnemigo(int posicion);
	Lista<Proyectil *>* GetProyectiles();
private:
	int CantJugadores;
	int cantidadEnemigos;
	int modoJuego;
	int GetIndexUsuario(std::string Usuario);
	Enemigo* enemigos[10];
	Lista<Proyectil *>* Proyectiles;
	Equipo* equipos[4];
	Jugador *Jugadores[5];
	Camara *Camaras[6];
	int CantCamaras;
};

