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

/**/
struct RectanguloPersonaje {
int IndexEnLista;
int X;
int Y;
int Width;
int Height;
Jugador* RefJugador;
};
/**/
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
	void AgregarJugador(std::string UnNombre, std::string UnIDSprite, bool EsDios);
	void RecibirEvento(std::string Usuario, std::string Tipo);
	bool UsuarioYaLogueado(std::string Usuario);
	Jugador* GetJugador(std::string UnNombre);
	Jugador* GetJugador(int Index);
	Lista<std::string>* GetNombresJugadoresOnline();
	int GetIndiceJugador(std::string Usuario);
	int GetCantJugadores();
	int GetCantCamaras();
	Equipo* GetEquipo(int NumeroEquipo);
	Equipo* GetEquipoJugador(std::string NombreJugador);
	int obtenerModo();
	Camara* GetCamara(int NrCamara);
	Camara* GetCamaraObstaculos();
	void establecerModo(std::string modo);
	void SetAnchoCamara(int NrCamara, int UnAncho);
	void BorrarCamaras();
	void AvanzarCamara();
	void AgregarCamara(int UnAncho);
	void AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad, int vida, bool esFinal, int width, 
						int height, std::string UnaDireccion);
	Lista<Proyectil *>* GetProyectiles();
	Lista<Enemigo *>* GetTodosLosEnemigos();
	Lista<Enemigo*>* GetEnemigosPantalla();
	Lista<Bonus*>* getTodosLosBonus();
	void MutexearListaProyectiles();
	void DesmutexearListaProyectiles();
	void MutexearListaEnemigos();
	void DesmutexearListaEnemigos();
	void MutexearListaRepuestos();
	void DesmutexearListaRepuestos();
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
	Lista<Bonus*>* getRepuestosArma();
	void agregarRepuestoArma(int posX, int posY);
	void BorrarEnemigos();
	void BorrarPlataformas();
	void SetEnemigoFinalMurio(bool UnMurio);
	bool GetEnemigoFinalMurio();
	void MutexearListaJugadores();
	void DesmutexearListaJugadores();
	Lista<RectanguloPersonaje>* getRectangulosPersonajes();
	void removerJugador(int indice);
	void EmpezarElJuego();
	bool GetEmpezoElJuego();
private:
	bool EmpezoElJuego;
	Lista<DatosSprites *>* ListaSprites;
	Lista<Rectangulo* >* ListaPlataformas;
	Lista<Bonus*>* todosLosBonus;
	bool YaSeAgregoEnemigoFinal;
	bool EnemigoFinalMurio;
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
	Lista<Bonus*>* repuestosArma;
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
	Lista<RectanguloPersonaje>* RectangulosPersonajes;
};

