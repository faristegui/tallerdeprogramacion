#pragma once
#include <string>

class Proyectil
{
public:
	Proyectil(std::string UnIdSprite, int posX, int posY, int vel, std::string UnaDireccion);
	void Mover();
	std::string GetIDSprite();
	std::string GetEstado();
	int GetX();
	int GetY();
	~Proyectil();
private:
	std::string IDSprite;
	std::string Direccion;
	std::string Estado;
	int x;
	int y;
	int velocidad;
	void DeterminarEstado();
};

