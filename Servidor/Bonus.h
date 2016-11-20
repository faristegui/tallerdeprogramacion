#pragma once
#include <string>
class Juego;
class Bonus
{
public:
	Bonus(int X, int Y,std::string letra,std::string idJugador);
	int getX();
	int getY();
	std::string getIdJugador();
	bool getEstado();
	void mostrar();
	void encontrar(Juego* unJuego);
	std::string getInicial();
	~Bonus(void);
private:
	int posX;
	int posY;
	std::string IDJugador;
	bool enPantalla;
	std::string inicial;

};


