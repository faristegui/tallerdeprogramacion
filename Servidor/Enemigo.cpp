#include "Enemigo.h"


Enemigo::Enemigo(std::string unIdSprite, int posX, int posY, int vel)
{
	IDSprite = unIdSprite;
	estado = "CAMINA-IZQ";
	x = posX;
	y = posY;
	velocidad = vel;
}
int Enemigo::getX()
{
	return x;
}

std::string Enemigo::getID()
{
	return IDSprite;
}

void Enemigo::mover()
{
	if(x > -30) //Para que salga afuera de la pantalla
	{
		x-= velocidad;
	}
	else
	{
		x = 800;
	}
}

int Enemigo::getVelocidad()
{
	return velocidad;
}

void Enemigo::disparar()
{
	estado = "DISPARA-DER";
}

std::string Enemigo::getEstado()
{
	return estado;
}


int Enemigo::getY()
{
	return y;
}
Enemigo::~Enemigo(void)
{
}
