#pragma once
#include "Arma.h"

class ArmaEnemigoHumano : public Arma {
public:
	ArmaEnemigoHumano() : Arma(500, "SEH", 25, "ShotgunEnemigoHumano", 100, true) { }

	int ArmaEnemigoHumano::DeterminarOffsetBalaX(std::string UnaDireccion) {

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

	int ArmaEnemigoHumano::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;

		if (UnaDireccion == "ABAJO-DER") {
			Offset = 60;
		}

		if (UnaDireccion == "DERECHA") {
			Offset = 55;
		}

		if (UnaDireccion == "IZQUIERDA") {
			Offset = 35;
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

	Proyectil* ArmaEnemigoHumano::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, Direccion, 10, 10, 70, false, false);
	}
};
