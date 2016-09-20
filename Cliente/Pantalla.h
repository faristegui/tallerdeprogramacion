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

	std::string PedirParametro(std::string NombreParametro, std::string ValorXDefecto);
	void MostrarMensaje(std::string Mensaje);
	void IniciarJuego();
private:
	int ParamsPedidos = 0;
	Uint32  Starting_Tick;
	SDL_Renderer *Renderer;
	SDL_Window *Window;
	SDL_Color COLOR_WHITE = { 255, 255, 255 };
	SDL_Event Event;

	void get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
		SDL_Texture **texture, SDL_Rect *rect);
	void WaitFPS(Uint32 starting_tick);
};

