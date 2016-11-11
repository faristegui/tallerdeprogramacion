#include "Arma.h"



Arma::Arma(std::string UnIDSprite, int UnTiempoEntreDisparos)
{
	IDSprite = UnIDSprite;
	TiempoEntreDisparos = UnTiempoEntreDisparos;
}

bool Arma::PuedeDisparar(float tickActual) {

	return ((tickActual - tick_ultimoDisparo) > TiempoEntreDisparos);
}

Proyectil* Arma::Disparar(int PosX, int PosY, float tickActual, std::string Direccion) {

	Proyectil* UnProyectil = new Proyectil("Bala_H", PosX, PosY + 15, 25, Direccion);
	tick_ultimoDisparo = tickActual;

	return UnProyectil;
}

Arma::~Arma()
{
}
