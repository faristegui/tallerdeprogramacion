#include "Arma.h"



Arma::Arma(int UnTiempoEntreDisparos, std::string UnCodigoArma, int UnaVelocidadBala, 
			std::string UnNombre, int CantBalasInicial, bool UnApuntaEnDiagonal)
{
	TiempoEntreDisparos = UnTiempoEntreDisparos;
	CodigoArma = UnCodigoArma;
	VelocidadBala = UnaVelocidadBala;
	NombreArma = UnNombre;
	Balas = CantBalasInicial;
	ApuntaEnDiagonal = UnApuntaEnDiagonal;
}

bool Arma::PuedeApuntarEnDiagonal() {

	return ApuntaEnDiagonal;
}

std::string Arma::GetCodigoArma() {

	return CodigoArma;
}

bool Arma::PuedeDisparar(float tickActual) {

	return (((tickActual - tick_ultimoDisparo) > TiempoEntreDisparos) && (Balas > 0));
}

Proyectil* Arma::Disparar(std::string NombreJugador, int PosX, int PosY, float tickActual, std::string Direccion) {

	tick_ultimoDisparo = tickActual;
	Balas--;

	Proyectil* UnProyectil = CrearProyectil(NombreJugador, PosX, PosY, Direccion);
	return UnProyectil;
}

std::string Arma::GetNombre() {

	return NombreArma;
}

int Arma::GetBalas() {

	return Balas;
}

void Arma::agregarBalas()
{
	if(CodigoArma == "S")
	{
		Balas+=10;
	}
	if(CodigoArma == "H")
	{
		Balas+=150;
	}
	if(CodigoArma == "R")
	{
		Balas+=15;
	}
}

Arma::~Arma()
{
}
