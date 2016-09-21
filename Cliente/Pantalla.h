#pragma once
#include "SDL.h"
#include "SDL2-2.0.4\include\SDL_image.h"
#include "SDL2-2.0.4\include\SDL_ttf.h"
#include <string>
#include <iostream>

class Pantalla
{
public:
	Pantalla();
	~Pantalla();

	std::string PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY);
	SDL_Rect crearFondo(char* path);
	void MostrarMensaje(std::string Mensaje, int posX, int posY);
	void IniciarJuego();
private:
	int ParamsPedidos;
	Uint32  Starting_Tick;
	SDL_Renderer *Renderer;
	SDL_Window *Window;
	SDL_Texture *texture;
	SDL_Surface *background;
	SDL_Color COLOR_WHITE;
	SDL_Event Event;

	void get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
		SDL_Texture **texture, SDL_Rect *rect, int fontSize);
	void WaitFPS(Uint32 starting_tick);
};

