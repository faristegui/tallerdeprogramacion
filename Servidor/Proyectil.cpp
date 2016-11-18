#include "Proyectil.h"
#include <string>
Proyectil::Proyectil(std::string idJugador,std::string UnIdSprite, int posX, int posY, int vel, 
					std::string UnaDireccion, int UnWidth, int UnHeight, bool UnTieneDireccion)
{
	IDSprite = UnIdSprite;
	IDJugador = idJugador.substr(0,idJugador.find('-'));
	x = posX;
	y = posY;
	velocidad = vel;
	Direccion = UnaDireccion;
	TieneDireccion = UnTieneDireccion;
	DeterminarEstado();
	Width = UnWidth;
	Height = UnHeight;
}

std::string Proyectil::GetIDJugador()
{
	return IDJugador;
}

void Proyectil::MoverEnX(int CantX) {

	x += CantX;
}

void Proyectil::DeterminarEstado() {

	Estado = "HORIZONTAL";

	if (!TieneDireccion) {

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
	else {

		if (Direccion == "DERECHA") Estado = "HORIZONTAL-DER";
		if (Direccion == "IZQUIERDA") Estado = "HORIZONTAL-IZQ";
		if ((Direccion == "ARRIBA-DER") || (Direccion == "ARRIBA-IZQ")) Estado = "VERTICAL";
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

int Proyectil::GetWidth() {

	return Width;
}

int Proyectil::GetHeight() {

	return Height;
}

Proyectil::~Proyectil()
{
}
