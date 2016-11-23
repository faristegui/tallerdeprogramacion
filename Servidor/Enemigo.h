#pragma once
#include <string>
#include <time.h>
#include "Arma.h"
class Enemigo
{
public:
	Enemigo(std::string unIdSprite, int posX, int posY, int vel, int unaVida, bool esFinal, 
			int UnWidth, int UnHeight, std::string UnaDireccion);
	int getX();
	int getY();
	void SetX(int UnX);
	void SetY(int UnY);
	void mover();
	void disparar();
	std::string getEstado();
	std::string getID();
	bool esEnemigoFinal();
	~Enemigo(void);
	int GetWidth();
	int GetHeight();
	void MoverEnX(int CantX);
	void SacarVida(int Cantidad);
	int GetVida();
	time_t GetTiempoDeVida();
	void setTiempoDeVida(time_t valor);
	time_t getTiempoActual();
	void muereEnemigo();
	bool estaListoParaMorir();
	void setListoParaMorir(bool valor);
	int getIndexEnListaOriginal();
	void setIndexEnListaOriginal(int indice);
	bool EstaDisparando();
	Arma* getArmaEnUso();
	std::string  getNombre();
	std::string getDireccion();
	std::string GetDireccionAparicion();
private:
	std::string IDSprite;
	std::string estado;
	bool enemigoFinal;
	int vida;
	int velocidad;
	int x;
	int y;
	int Width;
	int indexEnListaOriginal;
	int Height;
	time_t tiempoDeVida;
	time_t tiempoTranscurrido;
	bool listoParaMorir;
	Arma* arma;
	std::string Nombre;
	std::string DireccionAparicion;
	std::string Direccion;
	bool estaDisparando;
};

