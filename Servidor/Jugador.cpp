#include "Jugador.h"
#include <windows.h>

Jugador::Jugador(std::string UnNombre, std::string UnIDSprite)
{
	Nombre = UnNombre;
	IDSprite = UnIDSprite;
	Estado = "QUIETO";
	x = 20;
	y = 455;
	Conectado = true;
}

void Jugador::Mover(std::string Direccion) {

	if (Direccion == "UP") {
		this->Estado = "SALTANDO";
		deltaBeginJump = clock();
		//this->saltoVertical();
	}

	if (Direccion == "RIGHT") {

		if (x <= 730) { //No puede avanzar más allá de la cámara
			x += 10;
		}
		this->Estado = "CAMINA";
	}

	if (Direccion == "DOWN") { //No se mueve para abajo, solo en el salto
		//y += 10;
		this->Estado = "AGACHADO";
	}

	if ((Direccion == "LEFT") && (x >= 20)) { //No permite que vuelva para atrás de la pantalla.
		x -= 10;
		this->Estado = "CAMINA";
	}
}

void Jugador::SetEstaConectado(bool EstaConectado)
{
	this->Conectado = EstaConectado;

	if(!EstaConectado)
	{
		this->Estado = "DESCONECTADO";
		this->x = 0;
	}
	else {
		this->Estado = "QUIETO";
	}
}

void Jugador::saltoVertical()
{
	
	//this->Estado = "QUIETO";
}

bool Jugador::GetEstaConectado() {

	return Conectado;
}

void Jugador::MoverEnX(int UnX) {

	x += UnX;
}

void Jugador::SetEstado(std::string nuevoEstado) {

	this->Estado = nuevoEstado;
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

int a(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

int Jugador::GetY() {

	if (this->Estado == "SALTANDO") {
		
		float newTime = clock();
		float dif = a(newTime - deltaBeginJump, 0, 60, 300);
		y = 455 - dif;
		if (newTime - deltaBeginJump >= 600) {
			this->Estado = "QUIETO";
			y = 455;
		}
	}

	return y;
}

Jugador::~Jugador()
{
}
