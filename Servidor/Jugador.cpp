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

		if (this->Estado != "SALTANDO") {

			this->Estado = "SALTANDO";
			TiempoInicioSalto = clock();
			PosicionYInicioSalto = y;
		}
	}

	if (Direccion == "RIGHT") {

		if (x <= 730) {
			x += 10;
		}

		if (this->Estado != "SALTANDO") {
			this->Estado = "CAMINA";
		}
	}

	if (Direccion == "DOWN") {
		if (this->Estado != "SALTANDO") {
			this->Estado = "AGACHADO";
		}
	}

	if ((Direccion == "LEFT") && (x >= 20)) {
		x -= 10;
		if (this->Estado != "SALTANDO") {
			this->Estado = "CAMINA";
		}
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
		
		float TiempoActual = clock();
		float dif = a(TiempoActual - TiempoInicioSalto, 0, 60, 300);
		y = PosicionYInicioSalto - dif;
		if (TiempoActual - TiempoInicioSalto >= 500) {
			this->Estado = "QUIETO";
			y = PosicionYInicioSalto;
		}
	}

	return y;
}

Jugador::~Jugador()
{
}
