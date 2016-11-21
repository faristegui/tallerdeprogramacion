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
	time_t t = 0;
	time_t* pTiempoDeVida = &t;
	tiempoDeVida = (time(pTiempoDeVida));
	listoParaMorir = false; /*me indica si en el proximo tick ya puedo eliminar al enemigo dandome tiempo para mostrar el sprite de muerte*/
}

void Enemigo::SacarVida(int Cantidad) {
	this->estado = "HERIDO-IZQ"; 

	this->tiempoDeVida = getTiempoActual();
	tiempoTranscurrido = 0;

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

		tiempoTranscurrido = getTiempoActual() -tiempoDeVida;

		if ((this->estado == "CAMINA-IZQ" || this->estado == "HERIDO-IZQ") && tiempoTranscurrido > 0.5)
		{
			this->estado = "QUIETO-IZQ";

			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}

		if (this->estado == "QUIETO-IZQ" && tiempoTranscurrido > 1)
		{
			this->estado = "QUIETO-IZQ-DISPARA";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}
		if (this->estado == "QUIETO-IZQ-DISPARA" && tiempoTranscurrido > 1)
		{
			this->estado = "CAMINA-IZQ";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}

		if ((this->estado == "CAMINA-IZQ" || this->estado == "CAMINA-DER"))
		{
			x -= velocidad;
		}

		if (this->estado == "MUERTO-IZQ" && tiempoTranscurrido > 0.8)
		{
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
			this->listoParaMorir = true;
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


time_t Enemigo::GetTiempoDeVida()
{
	return tiempoDeVida;
}
void Enemigo::setTiempoDeVida(time_t valor)

{
	this->tiempoDeVida = valor;

}

time_t Enemigo::getTiempoActual()

{
	time_t tiempo_actual = 0;
	time_t* pTiempo_Actual = &tiempo_actual;
	
	return time(pTiempo_Actual);



}

void Enemigo::muereEnemigo()

{
	if (this->estado == "CAMINA-IZQ" || this->estado == "QUIETO-IZQ" || this->estado == "QUIETO-IZQ-DISPARA" || this->estado == "HERIDO-IZQ" )
	{
		this->estado = "MUERTO-IZQ";
		this->tiempoDeVida = getTiempoActual();
		tiempoTranscurrido = 0;
	}



}

void Enemigo::setListoParaMorir(bool valor)

{
	this->listoParaMorir = valor;

}

bool Enemigo::estaListoParaMorir()
{
	return this->listoParaMorir;

}