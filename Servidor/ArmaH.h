#pragma once
#include "Arma.h"

class ArmaH : public Arma {
public:
	ArmaH() : Arma(150, "H", 25, "Heavy Machinegun", 200) { }

	int ArmaH::DeterminarOffsetBalaX(std::string UnaDireccion) {

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

	int ArmaH::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;

		if (UnaDireccion == "DERECHA") {
			Offset = 65;
		}

		if (UnaDireccion == "IZQUIERDA") {
			Offset = 65;
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
	
	Proyectil* ArmaH::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, Direccion, 10, 10, 20, false);
	}
};
