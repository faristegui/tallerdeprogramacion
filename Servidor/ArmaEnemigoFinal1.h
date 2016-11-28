#pragma once
#include "Arma.h"

class ArmaEnemigoFinal1 : public Arma {
public:
	ArmaEnemigoFinal1() : Arma(1500, "SE1", 10, "MachinegunEnemigoFinal", 999, true) { }

	int ArmaEnemigoFinal1::DeterminarOffsetBalaX(std::string UnaDireccion) {
/*		int Offset = 0;

		if (UnaDireccion == "ABAJO-DER")
		{
			Offset = 120;
		}

		if (UnaDireccion == "ABAJO-IZQ") {
			Offset = 80;
		}*/

		return 120;
	}

	int ArmaEnemigoFinal1::DeterminarOffsetBalaY(std::string UnaDireccion) {
	/*	int Offset = 0;

		if (UnaDireccion == "ABAJO-DER" || UnaDireccion == "ABAJO-IZQ" || UnaDireccion == "ABAJO")
		{
			Offset = 140;
		}*/

		return 140;
	}

	Proyectil* ArmaEnemigoFinal1::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala,"ABAJO-DER", 38, 49, 0, false, false);
	}
};
