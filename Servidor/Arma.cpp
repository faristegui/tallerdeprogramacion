#include "Arma.h"



Arma::Arma(std::string UnIDSprite, std::string UnaDireccion)
{
	IDSprite = UnIDSprite;
	Direccion = UnaDireccion;
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

Proyectil* Arma::Disparar(int PosX, int PosY) {

	Proyectil* UnProyectil = new Proyectil("Bala_H", PosX, PosX, 10, Direccion);

	return UnProyectil;
}

Arma::~Arma()
{
}
