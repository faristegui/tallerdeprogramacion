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

		if (!EstaSaltando()) {

			TiempoInicioSaltoY = clock();
			PosicionYInicioSalto = y;

			if (EstaCaminando()) {

				if (this->Estado == "CAMINA-DER") {
					Direccion = "RIGHT";
				} else {
					Direccion = "LEFT";
				}
			}

			this->Estado = "SALTANDO";
		}
		else {
			if ((this->Estado != "SALTANDO-IZQ") || (this->Estado != "SALTANDO-DER")) {

				this->Estado = "SALTANDO";
			}
		}
	}

	if ((Direccion == "RIGHT") && (x <= 710))  {
		
		if (!EstaSaltando()) {

			this->Estado = "CAMINA-DER";
			x += 10;
		}
		else {
			if (this->Estado != "SALTANDO-DER") {

				this->Estado = "SALTANDO-DER";
				TiempoInicioSaltoX = clock();
				PosicionXInicioSalto = x;
			}
		}
	}

	if ((Direccion == "LEFT") && (x >= 20)) {

		if (!EstaSaltando()) {

			this->Estado = "CAMINA-IZQ";
			x -= 10;
		}
		else {
			if (this->Estado != "SALTANDO-IZQ") {

				this->Estado = "SALTANDO-IZQ";
				TiempoInicioSaltoX = clock();
				PosicionXInicioSalto = x;
			}
		}
	}

	if (Direccion == "DOWN") {

		if (this->Estado != "SALTANDO") {
			this->Estado = "AGACHADO";
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

int a(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

int Jugador::GetX() {

	if (EstaSaltando()) {
		float TiempoActual = clock();
		float dif = a(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);

		if (this->Estado == "SALTANDO-DER") {
			x = PosicionXInicioSalto + dif;
		} 
		else {
			if (this->Estado == "SALTANDO-IZQ") {
				x = PosicionXInicioSalto - dif;
			}
		}

		if (x >= 710) {
			this->Estado = "SALTANDO";
			x = 710;
		}
		else {
			if (x <= 20) {
				this->Estado = "SALTANDO";
				x = 20;
			}
		}
	}

	return x;
}

int Jugador::GetY() {

	if (EstaSaltando()) {
		
		float TiempoActual = clock();
		float dif = a(TiempoActual - TiempoInicioSaltoY, 0, 60, 300);
		y = PosicionYInicioSalto - dif;
		if (TiempoActual - TiempoInicioSaltoY >= 500) {
			this->Estado = "QUIETO";
			y = PosicionYInicioSalto;
		}
	}

	return y;
}

bool Jugador::EstaSaltando() {

	return ((this->Estado == "SALTANDO") || (this->Estado == "SALTANDO-DER") || (this->Estado == "SALTANDO-IZQ"));
}

bool Jugador::EstaCaminando() {

	return ((this->Estado == "CAMINA-DER") || (this->Estado == "CAMINA-IZQ"));
}

Jugador::~Jugador()
{
}
