#pragma once
#include "Arma.h"
#include <string>
#include <ctime>

class Jugador
{
public:
	Jugador(std::string UnNombre, std::string UnIDSprite);
	~Jugador();
	void Mover(std::string Direccion);
	std::string GetNombre();
	std::string GetIDSprite();
	std::string GetEstado();
	void SetEstado(std::string nuevoEstado);
	void SetEstadoAnterior(std::string nuevoEstado);
	void SetEstaConectado(bool EstaConectado);
	bool GetEstaConectado();
	void SetX(int UnX);
	void SetY(int UnY);
	std::string GetEstadoAnterior();
	float GetTiempoInicioSaltoY();
	float GetTiempoInicioSaltoX();
	int GetPosicionYInicioSalto();
	int GetPosicionXInicioSalto();
	int GetX();
	int GetY();
	Proyectil* Disparar();
	void MoverEnX(int UnX);
	bool EstaCaminando();
	bool EstaSaltando();
private:
	std::string Nombre;
	std::string IDSprite;
	std::string Estado;
	std::string EstadoAnterior;
	float TiempoInicioSaltoY;
	float TiempoInicioSaltoX;
	int PosicionYInicioSalto;
	int PosicionXInicioSalto;
	int x;
	int y;
	int vida;
	bool Conectado;
	Arma *UnArma;
};

