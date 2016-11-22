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
	void marcarParaEliminar(Bonus* unaDireccion);
	void encontrar(Juego* unJuego);
	bool debeSerEliminado();
	void setX(int numero);
	Bonus* getDireccion();
	std::string getInicial();
	~Bonus(void);
private:
	int posX;
	int posY;
	std::string IDJugador;
	bool enPantalla;
	Bonus* bonusDireccion;
	std::string inicial;

};


