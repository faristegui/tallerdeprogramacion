#pragma once
#include "Arma.h"

class ArmaS : public Arma {
public:
	ArmaS() : Arma(400, "S", 25, "Shotgun", 30) { }

	int ArmaS::DeterminarOffsetBalaX(std::string UnaDireccion) {

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

	int ArmaS::DeterminarOffsetBalaY(std::string UnaDireccion) {
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

	Proyectil* ArmaS::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, Direccion, 10, 10, false);
	}
};
