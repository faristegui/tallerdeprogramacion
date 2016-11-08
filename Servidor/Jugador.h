#pragma once
#include <string>
#include <ctime>
#include "Arma.h"
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
	bool EstaApuntandoALaDerecha();
	float GetTiempoInicioSaltoY();
	float GetTiempoInicioSaltoX();
	int GetPosicionYInicioSalto();
	int GetPosicionXInicioSalto();
	int GetX();
	int GetY();
	Arma* GetArma();
	void MoverEnX(int UnX);
	bool EstaCaminando();
	bool EstaSaltando();
	bool EstaDisparando();
private:
	bool Saltando;
	std::string Direccion;
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

