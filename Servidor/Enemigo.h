#pragma once
#include <string>
class Enemigo
{
public:
	Enemigo(std::string unIdSprite, int posX, int posY, int vel, int unaVida, bool esFinal, int UnWidth, int UnHeight);
	int getX();
	int getY();
	void SetX(int UnX);
	void SetY(int UnY);
	void mover();
	void disparar();
	std::string getEstado();
	std::string getID();
	int getVelocidad();
	bool esEnemigoFinal();
	~Enemigo(void);
	int GetWidth();
	int GetHeight();
	void MoverEnX(int CantX);
	void SacarVida(int Cantidad);
	int GetVida();
	int getIndexEnListaOriginal();
	void setIndexEnListaOriginal(int indice);
private:
	std::string IDSprite;
	std::string estado;
	bool enemigoFinal;
	int vida;
	int velocidad;
	int x;
	int y;
	int Width;
	int indexEnListaOriginal;
	int Height;
};

