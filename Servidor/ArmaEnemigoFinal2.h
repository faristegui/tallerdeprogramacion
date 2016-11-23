#pragma once
#include "Arma.h"

class ArmaEnemigoFinal2 : public Arma {
public:
	ArmaEnemigoFinal2() : Arma(350, "SE2", 25, "MachinegunEnemigoFinal", 999,true) { }
	int ArmaEnemigoFinal2::DeterminarOffsetBalaX(std::string UnaDireccion) {

		int Offset = 0;
		if(UnaDireccion == "ABAJO")
		{
			Offset = 0;
		}

		return Offset;
	}

	int ArmaEnemigoFinal2::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;

		if (UnaDireccion == "ABAJO")
		{
			Offset = 10;
		}

		return Offset;
	}

	Proyectil* ArmaEnemigoFinal2::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX("ABAJO");
		int OffsetY = DeterminarOffsetBalaY("ABAJO");

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, "ABAJO", 10, 10, 70, false, false);
	}
};
