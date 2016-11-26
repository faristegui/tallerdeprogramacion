#pragma once
#include "Arma.h"

class ArmaEnemigoFinal1 : public Arma {
public:
	ArmaEnemigoFinal1() : Arma(1500, "SE1", 10, "MachinegunEnemigoFinal", 999, true) { }

	int ArmaEnemigoFinal1::DeterminarOffsetBalaX(std::string UnaDireccion) {

		int Offset = 0;

		if (UnaDireccion == "DERECHA") {
			Offset = 44;
		}

		if (UnaDireccion == "IZQUIERDA") {
			Offset = 10;
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

		if (UnaDireccion == "ABAJO-DER") {
			Offset = 40;
		}

		return Offset;
	}

	int ArmaEnemigoFinal1::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;

		if (UnaDireccion == "ABAJO-DER") {
			Offset = 60;
		}

		if (UnaDireccion == "DERECHA") {
			Offset = 55;
		}

		if (UnaDireccion == "IZQUIERDA") {
			Offset = 150;
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

	Proyectil* ArmaEnemigoFinal1::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, "IZQUIERDA", 10, 10, 70, false, false);
	}
};
