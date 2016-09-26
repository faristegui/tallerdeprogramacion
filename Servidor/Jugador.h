#pragma once
#include <string>
class Jugador
{
public:
	Jugador(std::string UnNombre);
	~Jugador();
	void Mover(std::string Direccion);
	std::string GetNombre();
	int GetX();
	int GetY();
private:
	std::string Nombre;
	int x;
	int y;
};

