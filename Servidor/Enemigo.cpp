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

void Enemigo::SacarVida(int Cantidad) {

	vida -= Cantidad;
}

int Enemigo::GetVida() {
	
	return vida;
}

int Enemigo::getX()
{
	return x;
}

void Enemigo::SetX(int UnX)
{
	x = UnX;
}

void Enemigo::SetY(int UnY)
{
	x = UnY;
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

void Enemigo::MoverEnX(int CantX) {

	x += CantX;
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

int Enemigo::getIndexEnListaOriginal()
{
	return indexEnListaOriginal;
}

void Enemigo::setIndexEnListaOriginal(int indice)
{
	indexEnListaOriginal = indice;
}

int Enemigo::getY()
{
	return y;
}
Enemigo::~Enemigo(void)
{
}
