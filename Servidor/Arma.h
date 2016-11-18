#pragma once
#include <string>
#include "Proyectil.h"
class Arma
{
public:
	Arma(int TiempoEntreDisparos, std::string UnCodigoArma, int UnaVelocidadBala);
	Proyectil* Disparar(std::string idJugador,int PosX, int PosY, float tickActual, std::string Direccion);
	bool PuedeDisparar(float tickActual);
	std::string GetCodigoArma();
	~Arma();
protected:
	int VelocidadBala;
	std::string CodigoArma;
private:
	float tick_ultimoDisparo;
	int TiempoEntreDisparos;
	int Balas;
	virtual int DeterminarOffsetBalaX(std::string UnaDireccion) = 0;
	virtual int DeterminarOffsetBalaY(std::string UnaDireccion) = 0;
	virtual Proyectil* CrearProyectil(std::string NombreJugador, int PosX, int PosY, std::string Direccion) = 0;
};

