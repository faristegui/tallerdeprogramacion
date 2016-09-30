#pragma once
#include "SDL.h"
#include "SDL2-2.0.4\include\SDL_image.h"
#include "SDL2-2.0.4\include\SDL_ttf.h"
#include <string>
#include <iostream>
#include "Client.h"
#include "Lista.h"
struct Posicion
{
	int x;
	int y;
};
struct Sprite {
	std::string ID;
	SDL_Texture *Texture;
};

class Pantalla
{
public:
	Pantalla(Client* unCliente);
	~Pantalla();

	std::string PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY);
	SDL_Rect crearFondo(char* path, int width, int heigth);
	void MostrarMensaje(std::string Mensaje, int posX, int posY);
	void IniciarJuego();
	Posicion* obtenerPosicion();
private:
	int ParamsPedidos;
	Uint32  Starting_Tick;
	SDL_Renderer *Renderer;
	SDL_Window *Window;
	SDL_Texture *texture;
	SDL_Surface *background;
	SDL_Color COLOR_WHITE;
	SDL_Event Event;
	Client* cliente;
	Posicion* bolaPos;
	void get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
		SDL_Texture **texture, SDL_Rect *rect, int fontSize);
	void WaitFPS(Uint32 starting_tick);
	void AgregarSprite(std::string ID);
	void CargarSprites();
	Lista<Sprite> *Sprites;
	SDL_Texture* GetTexture(std::string ID);
};

