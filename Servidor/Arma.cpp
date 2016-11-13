#include "Arma.h"



Arma::Arma(int UnTiempoEntreDisparos, std::string UnCodigoArma, int UnaVelocidadBala)
{
	TiempoEntreDisparos = UnTiempoEntreDisparos;
	CodigoArma = UnCodigoArma;
	VelocidadBala = UnaVelocidadBala;
}

std::string Arma::GetCodigoArma() {

	return CodigoArma;
}

bool Arma::PuedeDisparar(float tickActual) {

	return ((tickActual - tick_ultimoDisparo) > TiempoEntreDisparos);
}

Proyectil* Arma::Disparar(std::string NombreJugador, int PosX, int PosY, float tickActual, std::string Direccion) {
	int OffsetX = DeterminarOffsetBalaX(Direccion);
	int OffsetY = DeterminarOffsetBalaY(Direccion);


	// TODO: Ver si el width y el height se pueden leer del xml, si no por cada arma establecer los tamanios
	Proyectil* UnProyectil = new Proyectil(NombreJugador, "Bala-" + CodigoArma, 
											PosX + OffsetX, PosY + OffsetY, VelocidadBala, Direccion, 15, 15);
	tick_ultimoDisparo = tickActual;

	return UnProyectil;
}

int Arma::DeterminarOffsetBalaX(std::string UnaDireccion) {
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

int Arma::DeterminarOffsetBalaY(std::string UnaDireccion) {
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
