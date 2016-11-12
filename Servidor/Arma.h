#pragma once
#include <string>
#include "Proyectil.h"
class Arma
{
public:
	Arma(std::string UnIDSprite, int TiempoEntreDisparos);
	Proyectil* Disparar(int PosX, int PosY, float tickActual, std::string Direccion);
	bool PuedeDisparar(float tickActual);
	std::string GetCodigoArma();
	~Arma();
private:
	std::string IDSprite;
	float tick_ultimoDisparo;
	int TiempoEntreDisparos;
	int Balas;
	int DeterminarOffsetX(std::string UnaDireccion);
	int DeterminarOffsetY(std::string UnaDireccion);
	std::string CodigoArma;
};

