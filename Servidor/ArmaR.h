#pragma once
#include "Arma.h"

class ArmaR : public Arma {
public:
	ArmaR() : Arma(750, "R", 25, "RocketLauncher", 30, false) { }

	int ArmaR::DeterminarOffsetBalaX(std::string UnaDireccion) {

		int Offset = 0;

		if (UnaDireccion == "DERECHA") {
			Offset = 35;
		}

		if (UnaDireccion == "IZQUIERDA") {
			Offset = -64;
		}

		if (UnaDireccion == "ARRIBA-DER") {
			Offset = -40;
		}

		if (UnaDireccion == "ARRIBA-IZQ") {
			Offset = -5;
		}


		return Offset;

	}

	int ArmaR::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;

		if (UnaDireccion == "DERECHA") {
			Offset = 25;
		}

		if (UnaDireccion == "IZQUIERDA") {
			Offset = 25;
		}

		if (UnaDireccion == "ARRIBA-DER") {
			Offset = -65;
		}

		if (UnaDireccion == "ARRIBA-IZQ") {
			Offset = -65;
		}

		return Offset;
	}

	Proyectil* ArmaR::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, Direccion, 64, 64, 80, true);
	}
};
