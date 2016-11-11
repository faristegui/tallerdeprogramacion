#include "Proyectil.h"

Proyectil::Proyectil(std::string UnIdSprite, int posX, int posY, int vel, std::string UnaDireccion)
{
	IDSprite = UnIdSprite;
	x = posX;
	y = posY;
	velocidad = vel;
	Direccion = UnaDireccion;
	DeterminarEstado();
}

void Proyectil::DeterminarEstado() {

	Estado = "HORIZONTAL";

	if ((Direccion == "DERECHA") || (Direccion == "IZQUIERDA")) {

		Estado = "HORIZONTAL";
	}

	if ((Direccion == "ARRIBA-DER") || (Direccion == "ARRIBA-IZQ") ||
		(Direccion == "ABAJO-DER") || (Direccion == "ABAJO-IZQ")) {

		Estado = "VERTICAL";
	}

	if ((Direccion == "ARRIBA-DER-DIAG") || (Direccion == "ABAJO-IZQ-DIAG")) {

		Estado = "DIAGONAL-DER";
	}

	if ((Direccion == "ARRIBA-IZQ-DIAG") || (Direccion == "ABAJO-DER-DIAG")) {

		Estado = "DIAGONAL-IZQ";
	}
}

void Proyectil::Mover() {

	if (Direccion == "DERECHA") {

		x += velocidad;
	}
	else {
		if (Direccion == "IZQUIERDA") {

			x -= velocidad;
		} else {
			if ((Direccion == "ARRIBA-DER") || (Direccion == "ARRIBA-IZQ")) {

				y -= velocidad;
			} else {
				if ((Direccion == "ABAJO-DER") || (Direccion == "ABAJO-IZQ")) {

					y += velocidad;
				}
				else {
					// TODO: if puedeDispararDiagonal
					if (Direccion == "ARRIBA-DER-DIAG") {

						x += velocidad;
						y -= velocidad;
					}
					else {
						if (Direccion == "ARRIBA-IZQ-DIAG") {

							x -= velocidad;
							y -= velocidad;
						}
						else {
							if (Direccion == "ABAJO-IZQ-DIAG") {

								x -= velocidad;
								y += velocidad;
							}
							else {
								if (Direccion == "ABAJO-DER-DIAG") {

									x += velocidad;
									y += velocidad;
								}
							}
						}
					}
				}
			}
		}
	}
}
std::string Proyectil::GetIDSprite() {

	return IDSprite;
}

std::string Proyectil::GetEstado() {

	return Estado;
}

int Proyectil::GetX() {

	return x;
}

int Proyectil::GetY() {

	return y;
}

Proyectil::~Proyectil()
{
}
