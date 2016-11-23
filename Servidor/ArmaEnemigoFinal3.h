#pragma once
#include "Arma.h"

class ArmaEnemigoFinal3 : public Arma {
public:
	ArmaEnemigoFinal3() : Arma(500, "SE3", 25, "MachinegunEnemigoFinal", 999, true) { }
	int ArmaEnemigoFinal3::DeterminarOffsetBalaX(std::string UnaDireccion) {

		int Offset = 0;

		if (UnaDireccion == "DERECHA")
		{
			Offset = 230;
		}

		if (UnaDireccion == "ARRIBA-DER-DIAG")
		{
			Offset = 190;
		}

		if (UnaDireccion == "ARRIBA-IZQ-DIAG")
		{
			Offset = 20;
		}

		if (UnaDireccion == "IZQUIERDA")
		{
			Offset = 0;
		}

		return Offset;
	}

	int ArmaEnemigoFinal3::DeterminarOffsetBalaY(std::string UnaDireccion) {
		int Offset = 0;


		if (UnaDireccion == "DERECHA")
		{
			Offset = 45;
		}

		if (UnaDireccion == "ARRIBA-DER-DIAG")
		{
			Offset = 10;
		}

		if (UnaDireccion == "ARRIBA-IZQ-DIAG")
		{
			Offset = 10;
		}

		if (UnaDireccion == "IZQUIERDA")
		{
			Offset = 45;
		}

		return Offset;
	}

	Proyectil* ArmaEnemigoFinal3::CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) {

		int OffsetX = DeterminarOffsetBalaX(Direccion);
		int OffsetY = DeterminarOffsetBalaY(Direccion);

		return new Proyectil(NombreJugador, "Bala-" + CodigoArma,
			PosX + OffsetX, PosY + OffsetY, VelocidadBala, Direccion, 10, 10, 70, true, false);
	}
};
