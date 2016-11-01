#pragma once
#include <string>
class Enemigo
{
public:
	Enemigo(std::string unIdSprite, int posX, int posY, int vel);
	int getX();
	int getY();
	void mover(int velocidad);
	void disparar();
	std::string getEstado();
	std::string getID();
	int getVelocidad();
	~Enemigo(void);
private:
	std::string IDSprite;
	std::string estado;
	int vida;
	int velocidad;
	int x;
	int y;
};

