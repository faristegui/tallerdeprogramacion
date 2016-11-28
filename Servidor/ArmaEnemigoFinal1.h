#pragma once
#include "Arma.h"

class ArmaEnemigoFinal1 : public Arma {
public:
	ArmaEnemigoFinal1() : Arma(3500, "SE1", 10, "MachinegunEnemigoFinal", 999, true) { }

	int ArmaEnemigoFinal1::DeterminarOffsetBalaX(std::string UnaDireccion) {
		return 120;
	}

	int ArmaEnemigoFinal1::DeterminarOffsetBalaY(std::string UnaDireccion) {
		return 140;
	}

	Proyectil* ArmaEnemigoFinal1::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala,"ABAJO-DER", 33, 90, 0, false, false);
	}
};
