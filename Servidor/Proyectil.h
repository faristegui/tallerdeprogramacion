#pragma once
#include <string>

class Proyectil
{
public:
	Proyectil(std::string UnIdSprite, int posX, int posY, int vel, std::string UnaDireccion);
	~Proyectil();
private:
	std::string IDSprite;
	std::string Direccion;
	int x;
	int y;
	int velocidad;
};

