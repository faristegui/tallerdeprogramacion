#include "Bonus.h"
#include "Juego.h"

Bonus::Bonus(int X, int Y, std::string letra,std::string idJugador)
{
	posX = X;
	posY = Y;
	inicial = letra;
	enPantalla = false;
	IDJugador = idJugador;
}

std:: string Bonus::getIdJugador()
{
	return IDJugador;
}

int Bonus::getX()
{
	return posX;
}
void Bonus::setX(int numero)
{
	posX = numero;
}

Bonus* Bonus::getDireccion()
{
	return bonusDireccion;
}

void Bonus::marcarParaEliminar(Bonus* unaDireccion)
{
	bonusDireccion = unaDireccion;
}

void Bonus::mostrar()
{
	enPantalla=true;
}


void Bonus::encontrar(Juego* unJuego)
{
	enPantalla = false;
	if(inicial == "p")
	{
		for(int i =0;i<unJuego->GetCantJugadores();i++)
		{
			unJuego->GetJugador(i)->reestablecerVida();
		}
	}
	else
	{
		for(int i =0; i < unJuego->GetEnemigosPantalla()->getTamanio();i++)
		{
			unJuego->GetEnemigosPantalla()->remover(1);
		}
	}

}

bool Bonus::getEstado()
{
	return enPantalla;
}

std::string Bonus::getInicial()
{
	return inicial;
}

int Bonus::getY()
{
	return posY;
}


Bonus::~Bonus(void)
{
}
