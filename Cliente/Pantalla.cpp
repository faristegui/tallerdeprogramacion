#include "Pantalla.h"
#include <sstream>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 30

using namespace std;

Pantalla::Pantalla(Client* unCliente)
{
	ParamsPedidos = 0;
	COLOR_WHITE.r = 255;
	COLOR_WHITE.g = 255;
	COLOR_WHITE.b = 255;
	bolaPos =  new Posicion();
	cliente = unCliente;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	// Create an application window with the following settings:
	Window = SDL_CreateWindow("Metal Slug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	//Seteo el color del render (negro)
	SDL_SetRenderDrawColor(Renderer,0,0,0,255);
	if (Window == NULL) {
		// TODO: Log "Could not create window"
	}
	CargarSpritesJugadores();
}

void Pantalla::WaitFPS(Uint32 starting_tick) {

	if ((1000 / FPS) > SDL_GetTicks() - starting_tick) {
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - starting_tick));
	}

}

void Pantalla::get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
	SDL_Texture **texture, SDL_Rect *rect, int fontSize) {

	int text_width = 0;
	int text_height = 0;
	SDL_Surface *surface;
	SDL_Color textColor = { 255, 255, 255, 0 };

	TTF_Font* Fuente = TTF_OpenFont("ClientResources/start.ttf", fontSize); //this opens a font style and sets a size

	surface = TTF_RenderText_Solid(Fuente, UnTexto.c_str(), textColor);

	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (surface != NULL) {
		text_width = surface->w;
		text_height = surface->h;
	}
	SDL_FreeSurface(surface);
	rect->x = x;
	rect->y = y;
	rect->w = text_width;
	rect->h = text_height;
}

void Pantalla::MostrarMensaje(std::string Mensaje, int posX, int posY) {
	SDL_Rect Message_Rect;
	SDL_Texture* Message;
	std::string tmp;
	bool Sale = false;
	
	SDL_Rect back = this->crearFondo("ClientResources/partida.bmp");

	SDL_RenderClear(Renderer);

	SDL_RenderCopy(Renderer, texture, NULL, &back);

	get_text_and_rect(Renderer, posX, posY, Mensaje.c_str(), &Message, &Message_Rect, 26);
	SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
	Mensaje = "¡¡Press START!!";
	get_text_and_rect(Renderer, 170, 370, Mensaje.c_str(), &Message, &Message_Rect, 30);
	SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
	SDL_RenderPresent(Renderer);

	while (!Sale) {

		Starting_Tick = SDL_GetTicks();

		while (SDL_PollEvent(&Event)) {

			if (Event.type == SDL_KEYDOWN) {
				Sale = true;
				break;
			}
		}
	}
}

SDL_Rect Pantalla::crearFondo(char* path) {
	background = SDL_LoadBMP(path);

	SDL_Rect background_Rect;

	background_Rect.x = 0;
	background_Rect.y = 0;
	background_Rect.w = 800;
	background_Rect.h = 600;
	
	texture = SDL_CreateTextureFromSurface(Renderer,background);

	return background_Rect;
}

std::string Pantalla::PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY) {
	std::string UnTexto = ValorXDefecto;
	bool ParamValido = false;
	SDL_Rect Message_Rect;
	SDL_Texture* Message;

	SDL_Rect back = this->crearFondo("ClientResources/start.bmp");

	NombreParametro = NombreParametro + ":";

	while (!ParamValido) {

		Starting_Tick = SDL_GetTicks();

		while (SDL_PollEvent(&Event)) {

			if (Event.type == SDL_QUIT) {
				exit(0);
			}

			if (Event.type == SDL_KEYDOWN) {

				if ((Event.key.keysym.sym == SDLK_BACKSPACE) && (UnTexto.length() > 0)) {
					UnTexto = UnTexto.substr(0, UnTexto.length() - 1);
				}
				if ((Event.key.keysym.sym == SDLK_RETURN) && (UnTexto.length() > 0)) {
					ParamValido = true;
					break;
				}
			}
			else {
				if (Event.type == SDL_TEXTINPUT) {
					UnTexto = UnTexto + Event.text.text;
				}
			}
		}

		SDL_RenderClear(Renderer);
		
		SDL_RenderCopy(Renderer, texture, NULL, &back);

		get_text_and_rect(Renderer, posX, posY, NombreParametro.c_str(), &Message, &Message_Rect, 16);
		SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
		// Ingresa el parámetro
		get_text_and_rect(Renderer, posX + Message_Rect.w + 10, posY, UnTexto.c_str(), &Message, &Message_Rect, 16);
		SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);

		WaitFPS(Starting_Tick);

		SDL_RenderPresent(Renderer);
	}

	return UnTexto;
}

Posicion* Pantalla::obtenerPosicion()
{
	return bolaPos;
}

void Pantalla::CargarSpritesJugadores() {
	SDL_Surface *TmpSurface;
	// Cargo bola roja
	TmpSurface = IMG_Load("ClientResources/PlayerRed.bmp");
	SDL_SetColorKey(TmpSurface, SDL_TRUE, SDL_MapRGB(TmpSurface->format, 128, 255, 0));
	PlayerRed = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
	// Cargo bola azul
	TmpSurface = IMG_Load("ClientResources/PlayerBlue.bmp");
	SDL_SetColorKey(TmpSurface, SDL_TRUE, SDL_MapRGB(TmpSurface->format, 128, 255, 0));
	PlayerBlue = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
	// Cargo bola amarilla
	TmpSurface = IMG_Load("ClientResources/PlayerYellow.bmp");
	SDL_SetColorKey(TmpSurface, SDL_TRUE, SDL_MapRGB(TmpSurface->format, 128, 255, 0));
	PlayerYellow = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
}

void Pantalla::IniciarJuego() {
	SDL_Rect Player_Rect;
	Player_Rect.w = 64;
	Player_Rect.h = 64;

	SDL_Rect Back_Rect = crearFondo("ClientResources/escenario.bmp"); // Imagen para el escenario del juego
	
	bool GameRunning = true;

	while (GameRunning) {

		Starting_Tick = SDL_GetTicks();

		while (SDL_PollEvent(&Event)) {

			if (Event.type == SDL_QUIT) {

				GameRunning = false;
				break;
			}

			if (Event.type == SDL_KEYDOWN) {

				string Evento = "";

				if (Event.key.keysym.sym == SDLK_RIGHT) {

					Evento = "RIGHT";
				}
				if (Event.key.keysym.sym == SDLK_LEFT) {

					Evento = "LEFT";
				}
				if (Event.key.keysym.sym == SDLK_UP) {

					Evento = "UP";
				}
				if (Event.key.keysym.sym == SDLK_DOWN) {

					Evento = "DOWN";
				}

				if (Evento != "") {
					cliente->EnviarMensaje("EVEN", 4);
					cliente->EnviarMensajeTamanoVariable(Evento);
				}
			}

		}

		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, texture, NULL, &Back_Rect); // Fondo

		cliente->EnviarMensaje("STAT", 4);
		string StrCantJugadores = cliente->RecibirMensaje(1);
		int CantJugadores = stoi(StrCantJugadores);

		for (int i = 0; i < CantJugadores; i++) {

			string x = cliente->RecibirMensaje(4);
			string y = cliente->RecibirMensaje(4);

			Player_Rect.x = stoi(x);
			Player_Rect.y = stoi(y);

			// TODO: El server indica cual es el sprite a renderizar dependiendo el jugador

			if (i == 0) {
				SDL_RenderCopy(Renderer, PlayerRed, NULL, &Player_Rect);
			} else {
				if (i == 1) {
					SDL_RenderCopy(Renderer, PlayerBlue, NULL, &Player_Rect);
				} else {
					SDL_RenderCopy(Renderer, PlayerYellow, NULL, &Player_Rect);
				}
			}
		}
		
		WaitFPS(Starting_Tick);
		SDL_RenderPresent(Renderer);
	}
}


Pantalla::~Pantalla()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}