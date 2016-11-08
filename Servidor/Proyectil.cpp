#include "Proyectil.h"

Proyectil::Proyectil(std::string UnIdSprite, int posX, int posY, int vel, std::string UnaDireccion)
{
	IDSprite = UnIdSprite;
	x = posX;
	y = posY;
	velocidad = vel;
	Direccion = UnaDireccion;
	DeterminarEstado();
}

void Proyectil::DeterminarEstado() {

	if ((Direccion == "DERECHA") || (Direccion == "IZQUIERDA")) {

		Estado = "HORIZONTAL";
	}

	if ((Direccion == "ARRIBA") || (Direccion == "ABAJO")) {

		Estado = "VERTICAL";
	}

	// TODO: Completar con estados faltantes (los diagonales)
}

void Proyectil::Mover() {

	if (Direccion == "DERECHA") {

		x += velocidad;
	}
	else {
		if (Direccion == "IZQUIERDA") {

			x -= velocidad;
		} else {

			// TODO: Todas las direcciones
		}
	}
}
std::string Proyectil::GetIDSprite() {

	return IDSprite;
}

std::string Proyectil::GetEstado() {

	return Estado;
}

int Proyectil::GetX() {

	return x;
}

int Proyectil::GetY() {

	return y;
}

Proyectil::~Proyectil()
{
}
