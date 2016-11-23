#pragma once
#include "Arma.h"

class ArmaEnemigoFinal2 : public Arma {
public:
	ArmaEnemigoFinal2() : Arma(800, "SE2", 25, "MachinegunEnemigoFinal", 999,true) { }
	int ArmaEnemigoFinal2::DeterminarOffsetBalaX(std::string UnaDireccion) {

		int Offset = 0;

		if (UnaDireccion == "ABAJO-DER")
		{
			Offset = 120;
		}

		if (UnaDireccion == "ABAJO-IZQ") {
			Offset = 80;
		}

		return Offset;
	}

	int ArmaEnemigoFinal2::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;

		if (UnaDireccion == "ABAJO-DER" || UnaDireccion == "ABAJO-IZQ" || UnaDireccion == "ABAJO")
		{
			Offset = 140;
		}

		return Offset;
	}

	Proyectil* ArmaEnemigoFinal2::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, "ABAJO-DER", 10, 10, 70, false, false);
	}
};
