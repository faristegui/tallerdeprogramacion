#include "Arma.h"



Arma::Arma(std::string UnIDSprite, std::string UnaDireccion, int UnTiempoEntreDisparos)
{
	IDSprite = UnIDSprite;
	Direccion = UnaDireccion;
	TiempoEntreDisparos = UnTiempoEntreDisparos;
}

void Arma::CambiarDireccion(std::string UnaDireccion) {

	// Direcciones Admitidas:
	//
	// DERECHA
	// ARRIBA-DERECHA
	// ARRIBA
	// ARRIBA-IZQUIERDA
	// IZQUIERDA
	// ABAJO-IZQUIERDA
	// ABAJO
	// ABAJO-DERECHA

	Direccion = UnaDireccion;
}

bool Arma::PuedeDisparar(float tickActual) {

	return ((tickActual - tick_ultimoDisparo) > TiempoEntreDisparos);
}

Proyectil* Arma::Disparar(int PosX, int PosY, float tickActual) {

	Proyectil* UnProyectil = new Proyectil("Bala_H", PosX, PosY + 15, 25, Direccion);
	tick_ultimoDisparo = tickActual;

	return UnProyectil;
}

Arma::~Arma()
{
}
