#include "Proyectil.h"

Proyectil::Proyectil(std::string UnIdSprite, int posX, int posY, int vel, std::string UnaDireccion)
{
	IDSprite = UnIdSprite;
	x = posX;
	y = posY;
	velocidad = vel;
	Direccion = UnaDireccion;
}

Proyectil::~Proyectil()
{
}
