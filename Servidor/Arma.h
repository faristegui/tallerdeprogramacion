#pragma once
#include <string>
#include "Proyectil.h"
class Arma
{
public:
	Arma(std::string UnIDSprite, std::string UnaDireccion, int TiempoEntreDisparos);
	Proyectil* Disparar(int PosX, int PosY, float tickActual);
	void CambiarDireccion(std::string UnaDireccion);
	bool PuedeDisparar(float tickActual);
	~Arma();
private:
	std::string IDSprite;
	std::string Direccion;
	float tick_ultimoDisparo;
	int TiempoEntreDisparos;
	int Balas;
};

