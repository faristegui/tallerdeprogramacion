#include "Jugador.h"
#include "Enemigo.h"
#include "Lista.h"
#include "Equipo.h"
#include <windows.h>
#include <process.h>

#pragma once
struct RectanguloEnemigo {
	int IndexEnLista;
	int X;
	int Y;
	int Width;
	int Height;
};

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
	Equipo* GetEquipoJugador(std::string NombreJugador);
	int obtenerModo();
	Camara* GetCamara(int NrCamara);
	void establecerModo(std::string modo);
	void SetAnchoCamara(int NrCamara, int UnAncho);
	void BorrarCamaras();
	void AvanzarCamara();
	void AgregarCamara(int UnAncho);
	void AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad,int vida, bool esFinal);
	Lista<Proyectil *>* GetProyectiles();
	Lista<Enemigo *>* GetEnemigos();
	void MutexearListaProyectiles();
	void DesmutexearListaProyectiles();
	void MutexearListaEnemigos();
	void DesmutexearListaEnemigos();
private:
	int CantJugadores;
	int modoJuego;
	int GetIndexUsuario(std::string Usuario);
	Lista<Proyectil *>* Proyectiles;
	Lista<Enemigo *>* Enemigos;
	Equipo* equipos[4];
	Jugador *Jugadores[5];
	Camara *Camaras[6];
	int CantCamaras;
};

