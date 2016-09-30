#pragma once
#include <string>
class Jugador
{
public:
	Jugador(std::string UnNombre, std::string UnIDSprite);
	~Jugador();
	void Mover(std::string Direccion);
	std::string GetNombre();
	std::string GetIDSprite();
	int GetX();
	int GetY();
private:
	std::string Nombre;
	std::string IDSprite;
	int x;
	int y;
};

