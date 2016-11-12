#include "Arma.h"



Arma::Arma(std::string UnIDSprite, int UnTiempoEntreDisparos)
{
	IDSprite = UnIDSprite;
	TiempoEntreDisparos = UnTiempoEntreDisparos;
	CodigoArma = "S";
}

std::string Arma::GetCodigoArma() {

	return CodigoArma;
}

bool Arma::PuedeDisparar(float tickActual) {

	return ((tickActual - tick_ultimoDisparo) > TiempoEntreDisparos);
}

Proyectil* Arma::Disparar(int PosX, int PosY, float tickActual, std::string Direccion) {
	int OffsetX = DeterminarOffsetX(Direccion);
	int OffsetY = DeterminarOffsetY(Direccion);

	Proyectil* UnProyectil = new Proyectil("Bala-S", PosX + OffsetX, PosY + OffsetY, 25, Direccion);
	tick_ultimoDisparo = tickActual;

	return UnProyectil;
}

int Arma::DeterminarOffsetX(std::string UnaDireccion) {
	int Offset = 0;

	if (UnaDireccion == "DERECHA") {
		Offset = 44;
	}

	if (UnaDireccion == "IZQUIERDA") {
		Offset = 0;
	}

	if (UnaDireccion == "ARRIBA-DER") {
		Offset = 12;
	}

	if (UnaDireccion == "ARRIBA-IZQ") {
		Offset = 40;
	}

	if (UnaDireccion == "ARRIBA-DER-DIAG") {
		Offset = 44;
	}

	if (UnaDireccion == "ARRIBA-IZQ-DIAG") {
		Offset = 10;
	}

	return Offset;
}

int Arma::DeterminarOffsetY(std::string UnaDireccion) {
	int Offset = 0;

	if (UnaDireccion == "DERECHA") {
		Offset = 55;
	}

	if (UnaDireccion == "IZQUIERDA") {
		Offset = 55;
	}

	if (UnaDireccion == "ARRIBA-DER") {
		Offset = -8;
	}

	if (UnaDireccion == "ARRIBA-IZQ") {
		Offset = -8;
	}

	if (UnaDireccion == "ARRIBA-DER-DIAG") {
		Offset = 0;
	}

	if (UnaDireccion == "ARRIBA-IZQ-DIAG") {
		Offset = 0;
	}

	return Offset;
}

Arma::~Arma()
{
}
