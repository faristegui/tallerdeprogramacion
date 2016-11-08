#include "Enemigo.h"


Enemigo::Enemigo(std::string unIdSprite, int posX, int posY, int vel, int unaVida, bool esFinal)
{
	IDSprite = unIdSprite;
	estado = "CAMINA-IZQ";
	x = posX;
	y = posY;
	vida = unaVida;
	velocidad = vel;
	enemigoFinal = esFinal;
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
	//Solo enemigos finales
	if(this->esEnemigoFinal())
	{
		if(x > 500) //Se posiciona para disparar. No se mueve
		{
			x-= velocidad;
		}
		else
		{
			this->estado = "DISPARA";
		}
	}
	else
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
}

bool Enemigo::esEnemigoFinal()
{
	return enemigoFinal;
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
