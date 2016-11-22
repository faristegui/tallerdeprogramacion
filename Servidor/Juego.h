#include "Jugador.h"
#include "Enemigo.h"
#include "Lista.h"
#include "Equipo.h"
#include "Bonus.h"
#include <windows.h>
#include <process.h>

#pragma once

struct DatosSprites
{
	std::string id;
	int width;
	int height;
	int velocidad;
};

struct RectanguloEnemigo {
	int IndexEnLista;
	int X;
	int Y;
	int Width;
	int Height;
	Enemigo* RefEnemigo;
};

struct Posicion {
	int x;
	int y;
};

struct Rectangulo {
	int x;
	int y;
	int w;
	int h;
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
	Camara* GetCamaraObstaculos();
	void establecerModo(std::string modo);
	void SetAnchoCamara(int NrCamara, int UnAncho);
	void BorrarCamaras();
	void AvanzarCamara();
	void AgregarCamara(int UnAncho);
	void AgregarEnemigo(std::string UnIDSprite, int posX, int posY,int velocidad,int vida, bool esFinal,int width,int height);
	Lista<Proyectil *>* GetProyectiles();
	Lista<Enemigo *>* GetTodosLosEnemigos();
	Lista<Enemigo*>* GetEnemigosPantalla();
	void MutexearListaProyectiles();
	void DesmutexearListaProyectiles();
	void MutexearListaEnemigos();
	void DesmutexearListaEnemigos();
	bool enemigosEstanCargados();
	void cargarEnemigos();
	void sumarEnemigo();
	void definirAparicionBonusPower();
	void definirAparicionBonusKilAll();
	int obtenerCantEnemigosAparecidos();
	int getNumeroBonusPower();
	int getNumeroBonusKillAll();
	bool hayBonus();
	Bonus* obtenerBonusPower();
	Bonus* obtenerBonusKillAll();
	void aparecerBonusPower(Bonus* unBonus);
	void aparecerBonusKillAll(Bonus* unBonus);
	void bonusYaMostrado();
	void desaparecerBonusPower();
	void desaparecerBonusKillAll();
	Enemigo* GetEnemigoFinal();
	int GetPisoY();
	void SetListaDatosSprites(Lista<DatosSprites *>* UnaListaSprites);
	DatosSprites* BuscarSpriteEnLista(std::string tipo);
	void AgregarPlataforma(int x, int y, int w, int h);
	bool HayObstaculo(int X, int Y, int W, int H, int &YDelObstaculo, int &HDelObstaculo);
private:
	Lista<DatosSprites *>* ListaSprites;
	Lista<Rectangulo* >* ListaPlataformas;
	bool YaSeAgregoEnemigoFinal;
	int CantJugadores;
	int modoJuego;
	bool empezo;
	bool bonus;
	bool enemigosCargados;
	int numeroEnemigoBonusPower;
	int numeroEnemigoBonusKillAll;
	int cantEnemigosAparecidos;
	Bonus* bonusPower;
	Bonus* bonusKillAll;
	int GetIndexUsuario(std::string Usuario);
	Lista<Proyectil *>* Proyectiles;
	Lista<Enemigo *>* enemigosPantalla;
	Lista<Enemigo*>* todosLosEnemigos;
	Equipo* equipos[4];
	Jugador *Jugadores[5];
	Camara *Camaras[6];
	Enemigo* EnemigoFinal;
	int CantCamaras;
	int NumeroNivel;

	int PisoY;
};

