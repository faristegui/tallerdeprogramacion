#include "Enemigo.h"


Enemigo::Enemigo(std::string unIdSprite, int posX, int posY, int vel, 
				int unaVida, bool esFinal, int UnWidth, int UnHeight)
{
	IDSprite = unIdSprite;
	estado = "CAMINA-IZQ";
	x = posX;
	y = posY;
	vida = unaVida;
	velocidad = vel;
	enemigoFinal = esFinal;
	Width = UnWidth;
	Height = UnHeight;
}
int Enemigo::getX()
{
	return x;
}

int Enemigo::GetWidth() {

	return Width;
}

int Enemigo::GetHeight() {

	return Height;
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
		x-= velocidad;
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
