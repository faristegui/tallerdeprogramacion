#include "Jugador.h"
#include <windows.h>

Jugador::Jugador(std::string UnNombre, std::string UnIDSprite)
{
	Nombre = UnNombre;
	IDSprite = UnIDSprite;
	Estado = "QUIETO-DER";
	EstadoAnterior = Estado;
	Direccion = "DERECHA";
	x = 20;
	y = 405;
	vida = 100;
	Conectado = true;
	Saltando = false;
	SaltandoVertical = false;
	UnArma = new Arma("Arma_H", 200);
}

std::string Jugador::GetDireccion() {

	return Direccion;
}

void Jugador::Mover(std::string Tecla) {

	if (Tecla == "SPACE") {

		SaltandoVertical = true;

		if (!EstaSaltando()) {

			TiempoInicioSaltoY = clock();
			PosicionYInicioSalto = y;

			EstadoAnterior = this->Estado;

			if (EstaCaminando()) {
				if (this->EstaApuntandoALaDerecha()) {
					Tecla = "RIGHT";
				}
				else {
					Tecla = "LEFT";
				}
			}

			SetEstaSaltando(true);
		}
	}

	if (Tecla == "RIGHT")  {
		
		if (!EstaSaltando()) {

			this->Estado = "CAMINA-DER";
		}
		else {

			if (!((!SaltandoVertical) && (this->Estado == "SALTANDO-DER"))) {

				SaltandoVertical = false;
				this->Estado = "SALTANDO-DER";
				SetEstadoAnterior("CAMINA-DER");
				TiempoInicioSaltoX = clock();
				PosicionXInicioSalto = x;
			}
		}

		// TODO: Setear los estados corresp
		if (this->EstaApuntandoAbajo()) {
			Direccion = "ABAJO-DER-DIAG";
		}
		else {
			if (this->EstaApuntandoArriba()) {
				Direccion = "ARRIBA-DER-DIAG";
			}
			else {
				Direccion = "DERECHA";
			}
		}
	}

	if (Tecla == "LEFT") {

		if (!EstaSaltando()) {

			this->Estado = "CAMINA-IZQ";
		}  
		else {
			if (!((!SaltandoVertical) && (this->Estado == "SALTANDO-IZQ"))) {

				SaltandoVertical = false;
				this->Estado = "SALTANDO-IZQ";
				SetEstadoAnterior("CAMINA-IZQ");
				TiempoInicioSaltoX = clock();
				PosicionXInicioSalto = x;
			}
		}

		// TODO: Setear los estados corresp
		if (this->EstaApuntandoAbajo()) {
			Direccion = "ABAJO-IZQ-DIAG";
		}
		else {
			if (this->EstaApuntandoArriba()) {
				Direccion = "ARRIBA-IZQ-DIAG";
			} else {
				Direccion = "IZQUIERDA";
			}
		}
	}

	if (Tecla == "UP") {

		if (EstaSaltando()) {
			SaltandoVertical = true;
		}

		if (this->EstaApuntandoALaDerecha()) {
			Direccion = "ARRIBA-DER";	// TODO: this->Estado
		} else {
			Direccion = "ARRIBA-IZQ";	// TODO: this->Estado
		}

		if (this->EstaCaminando()) {
			Direccion = Direccion + "-DIAG"; // TODO: this->Estado
		}
	}

	if (Tecla == "DOWN") {
		
		if (EstaSaltando()) {
			SaltandoVertical = true;
		}

		if (this->EstaApuntandoALaDerecha()) {
			Direccion = "ABAJO-DER";	// TODO: this->Estado
		}
		else {
			Direccion = "ABAJO-IZQ";	// TODO: this->Estado
		}

		if (this->EstaCaminando()) {
			Direccion = Direccion + "-DIAG"; // TODO: this->Estado
		}
	}
}

bool Jugador::EstaApuntandoAbajo() {

	return ((Direccion == "ABAJO") || (Direccion == "ABAJO-IZQ") || (Direccion == "ABAJO-DER"));
}

bool Jugador::EstaApuntandoArriba() {

	return ((Direccion == "ARRIBA") || (Direccion == "ARRIBA-IZQ") || (Direccion == "ARRIBA-DER"));
}

Arma* Jugador::GetArma() 
{

	return UnArma;
}
void Jugador::SetEstaConectado(bool EstaConectado)
{
	this->Conectado = EstaConectado;

	if(!EstaConectado)
	{
		this->Estado = "DESCONECTADO";
		this->x = 20;
		this->y = 405;
	}
	else {
		this->Estado = "QUIETO-DER";
	}
}

bool Jugador::EstaDisparando() {

	return ((this->Estado == "DISPARA-DER") || (this->Estado == "DISPARA-IZQ"));
}

void Jugador::SetEstaSaltando(bool UnEstaSaltando) {

	Saltando = UnEstaSaltando;

	if (Saltando) {
		if (EstaApuntandoALaDerecha()) {

			this->Estado = "SALTANDO-DER";
		}
		else {
			this->Estado = "SALTANDO-IZQ";		
		}
	}
	else {
		this->Estado = this->GetEstadoAnterior();
	}
}

bool Jugador::GetEstaConectado() {

	return Conectado;
}

void Jugador::MoverEnX(int UnX) {

	x += UnX;
}

void Jugador::SetEstado(std::string nuevoEstado) {

	this->Estado = nuevoEstado;
}

void Jugador::SetEstadoAnterior(std::string nuevoEstado) {

	this->EstadoAnterior = nuevoEstado;
}

std::string Jugador::GetNombre() {

	return Nombre;
}

std::string Jugador::GetIDSprite() {

	return IDSprite;
}

std::string Jugador::GetEstado() {

	return Estado;
}

int Jugador::GetX() {
	
	return x;
}

int Jugador::GetY() {

	return y;
}

bool Jugador::EstaSaltando() {

	return Saltando;
}

bool Jugador::EstaSaltandoVertical() {

	return SaltandoVertical;
}

void Jugador::SueltaTeclaDireccion() {
	if (this->EstaCaminando()) {
		if (this->EstaApuntandoALaDerecha()) {

			this->SetDireccion("DERECHA");
			this->SetEstado("CAMINA-DER");
		}
		else {

			this->SetDireccion("IZQUIERDA");
			this->SetEstado("CAMINA-IZQ");
		}
	}
	else {
		if (this->EstaApuntandoALaDerecha()) {

			this->SetDireccion("DERECHA");
			this->SetEstado("QUIETO-DER");
		}
		else {

			this->SetDireccion("IZQUIERDA");
			this->SetEstado("QUIETO-IZQ");
		}
		// TODO: if EstaSaltando then setEstadoANTERIOR
	}
}

bool Jugador::EstaCaminando() {

	return ((this->Estado == "CAMINA-DER") || (this->Estado == "CAMINA-IZQ"));
}

bool Jugador::EstaApuntandoALaDerecha() {

	return ((this->Direccion == "DERECHA") || (this->Direccion == "ARRIBA-DER") ||
			(this->Direccion == "ARRIBA-DER-DIAG") || (this->Direccion == "ABAJO-DER") ||
			(this->Direccion == "ABAJO-DER-DIAG"));
}

float Jugador::GetTiempoInicioSaltoY() {

	return TiempoInicioSaltoY;
}
float Jugador::GetTiempoInicioSaltoX() {

	return TiempoInicioSaltoX;
}

int Jugador::GetPosicionYInicioSalto() {

	return PosicionYInicioSalto;
}

int Jugador::GetPosicionXInicioSalto() {

	return PosicionXInicioSalto;
}

std::string Jugador::GetEstadoAnterior() {

	return EstadoAnterior;
}

void Jugador::SetX(int UnX) {

	x = UnX;
}

void Jugador::SetDireccion(std::string UnaDireccion) {

	Direccion = UnaDireccion;
}

void Jugador::SetY(int UnY) {

	y = UnY;
}

Jugador::~Jugador()
{
}
