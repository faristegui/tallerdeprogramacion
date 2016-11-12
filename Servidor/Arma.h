#pragma once
#include <string>
#include "Proyectil.h"
class Arma
{
public:
	Arma(int TiempoEntreDisparos, std::string UnCodigoArma, int UnaVelocidadBala);
	Proyectil* Disparar(int PosX, int PosY, float tickActual, std::string Direccion);
	bool PuedeDisparar(float tickActual);
	std::string GetCodigoArma();
	~Arma();
private:
	float tick_ultimoDisparo;
	int TiempoEntreDisparos;
	int Balas;
	int VelocidadBala;
	int DeterminarOffsetBalaX(std::string UnaDireccion);
	int DeterminarOffsetBalaY(std::string UnaDireccion);
	std::string CodigoArma;
};

