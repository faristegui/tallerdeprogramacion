#pragma once
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
	void SetEstaConectado(bool EstaConectado);
	bool GetEstaConectado();
	int GetX();
	int GetY();
	void MoverEnX(int UnX);
	bool EstaCaminando();
private:
	std::string Nombre;
	std::string IDSprite;
	std::string Estado;
	float TiempoInicioSaltoY;
	float TiempoInicioSaltoX;
	int PosicionYInicioSalto;
	int PosicionXInicioSalto;
	int x;
	int y;
	bool Conectado;
	bool EstaSaltando();
};

