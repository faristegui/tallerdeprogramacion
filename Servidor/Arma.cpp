#include "Arma.h"



Arma::Arma(int UnTiempoEntreDisparos, std::string UnCodigoArma, int UnaVelocidadBala, std::string UnNombre)
{
	TiempoEntreDisparos = UnTiempoEntreDisparos;
	CodigoArma = UnCodigoArma;
	VelocidadBala = UnaVelocidadBala;
	NombreArma = UnNombre;
}

std::string Arma::GetCodigoArma() {

	return CodigoArma;
}

bool Arma::PuedeDisparar(float tickActual) {

	return ((tickActual - tick_ultimoDisparo) > TiempoEntreDisparos);
}

Proyectil* Arma::Disparar(std::string NombreJugador, int PosX, int PosY, float tickActual, std::string Direccion) {

	tick_ultimoDisparo = tickActual;

	Proyectil* UnProyectil = CrearProyectil(NombreJugador, PosX, PosY, Direccion);
	return UnProyectil;
}

Arma::~Arma()
{
}
