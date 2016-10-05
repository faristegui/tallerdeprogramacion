#include "Jugador.h"

Jugador::Jugador(std::string UnNombre, std::string UnIDSprite)
{
	Nombre = UnNombre;
	IDSprite = UnIDSprite;
	Estado = "QUIETO";
	x = 20;
	y = 455;
}

void Jugador::Mover(std::string Direccion) {

	if (Direccion == "UP") {
		y -= 10;
	}

	if (Direccion == "RIGHT") {

		if (x <= 730) { //No puede avanzar más allá de la cámara
			x += 10;
		}
	}

	if ((Direccion == "DOWN") && (y <= 450)) { //No permite que se vaya para abajo del escenario
		y += 10;
	}

	if ((Direccion == "LEFT") && (x >= 20)) { //No permite que vuelva para atrás de la pantalla.
		x -= 10;
	}
}

void Jugador::MoverAdelante(int UnX) {

	x += UnX;
}

std::string Jugador::GetNombre() {

	return Nombre;
}

std::string Jugador::GetIDSprite() {

	return IDSprite;
}

std::string Jugador::GetEstado() {

	return Estado;
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
