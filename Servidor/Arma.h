#pragma once
#include <string>
#include "Proyectil.h"
class Arma
{
public:
	Arma(std::string UnIDSprite, std::string UnaDireccion);
	Proyectil* Disparar(int PosX, int PosY);
	void CambiarDireccion(std::string UnaDireccion);
	~Arma();
private:
	std::string IDSprite;
	std::string Direccion;
	int Balas;
};

