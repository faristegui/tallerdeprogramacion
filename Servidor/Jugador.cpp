#include "Jugador.h"

Jugador::Jugador(std::string UnNombre, std::string UnIDSprite)
{
	Nombre = UnNombre;
	IDSprite = UnIDSprite;
	x = 0;
	y = 0;
}

void Jugador::Mover(std::string Direccion) {

	if (Direccion == "UP") {
		y -= 10;
	}

	if (Direccion == "RIGHT") {
		x += 10;
	}

	if (Direccion == "DOWN") {
		y += 10;
	}

	if (Direccion == "LEFT") {
		x -= 10;
	}
}

std::string Jugador::GetNombre() {

	return Nombre;
}

std::string Jugador::GetIDSprite() {

	return IDSprite;
}

int Jugador::GetX() {

	return x;
}

int Jugador::GetY() {

	return y;
}

Jugador::~Jugador()
{
}
