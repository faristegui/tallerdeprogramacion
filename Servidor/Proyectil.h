#pragma once
#include <string>

class Proyectil
{
public:
	Proyectil(std::string idJugador,std::string UnIdSprite, int posX, int posY, int vel, std::string UnaDireccion, int UnWidth, int UnHeight);
	void Mover();
	std::string GetIDJugador();
	std::string GetIDSprite();
	std::string GetEstado();
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	~Proyectil();
private:
	std::string IDJugador;
	std::string IDSprite;
	std::string Direccion;
	std::string Estado;
	int x;
	int y;
	int velocidad;
	void DeterminarEstado();
	int Width;
	int Height;
};

