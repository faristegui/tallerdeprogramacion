#include "Pantalla.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 30

Pantalla::Pantalla()
{
	ParamsPedidos = 0;
	COLOR_WHITE.r = 255;
	COLOR_WHITE.g = 255;
	COLOR_WHITE.b = 255;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	// Create an application window with the following settings:
	Window = SDL_CreateWindow("Metal Slug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	if (Window == NULL) {
		// TODO: Log "Could not create window"
	}
}

void Pantalla::WaitFPS(Uint32 starting_tick) {

	if ((1000 / FPS) > SDL_GetTicks() - starting_tick) {
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - starting_tick));
	}

}

void Pantalla::get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
	SDL_Texture **texture, SDL_Rect *rect) {

	int text_width = 0;
	int text_height = 0;
	SDL_Surface *surface;
	SDL_Color textColor = { 255, 255, 255, 0 };

	TTF_Font* Fuente = TTF_OpenFont("start.ttf", 15); //this opens a font style and sets a size

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
	
	SDL_RenderClear(Renderer);
	get_text_and_rect(Renderer, posX, posY, Mensaje.c_str(), &Message, &Message_Rect);
	SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
	Mensaje = "Presione cualquier tecla para continuar...";
	get_text_and_rect(Renderer, 0, 18, Mensaje.c_str(), &Message, &Message_Rect);
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

void Pantalla::MostrarMenu() {
	SDL_Texture *background = IMG_LoadTexture(Renderer, "images/start.bmp");
	SDL_Rect background_Rect;

	background_Rect.x = 0;
	background_Rect.y = 0;
	background_Rect.w = 800;
	background_Rect.h = 600;
	SDL_RenderClear(Renderer);

	SDL_RenderCopy(Renderer, background, NULL, &background_Rect);

	SDL_RenderPresent(Renderer);
}

std::string Pantalla::PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY) {
	std::string UnTexto = ValorXDefecto;
	bool ParamValido = false;
	SDL_Rect Message_Rect;
	SDL_Texture* Message;

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
		
		get_text_and_rect(Renderer, posX, posY, NombreParametro.c_str(), &Message, &Message_Rect);
		SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
		get_text_and_rect(Renderer, posX + Message_Rect.w + 10, posY, UnTexto.c_str(), &Message, &Message_Rect);
		SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);

		WaitFPS(Starting_Tick);
		SDL_RenderPresent(Renderer);
	}

	return UnTexto;
}

void Pantalla::IniciarJuego() {

	SDL_Texture *Ball = IMG_LoadTexture(Renderer, "Sprite.bmp");
	SDL_Rect Ball_Rect;


	Ball_Rect.x = 10;
	Ball_Rect.y = 10;
	Ball_Rect.w = 64;
	Ball_Rect.h = 64;
	bool GameRunning = true;

	while (GameRunning) {

		Starting_Tick = SDL_GetTicks();

		while (SDL_PollEvent(&Event)) {

			if (Event.type == SDL_QUIT) {

				GameRunning = false;
				break;
			}

			if (Event.type == SDL_KEYDOWN) {
				if (Event.key.keysym.sym == SDLK_RIGHT) {
					Ball_Rect.x += 20;
				}
				if (Event.key.keysym.sym == SDLK_LEFT) {
					Ball_Rect.x -= 20;
				}
				if (Event.key.keysym.sym == SDLK_UP) {
					Ball_Rect.y -= 20;
				}
				if (Event.key.keysym.sym == SDLK_DOWN) {
					Ball_Rect.y += 20;
				}
			}

		}

		SDL_RenderClear(Renderer);

		//SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
		SDL_RenderCopy(Renderer, Ball, NULL, &Ball_Rect);

		WaitFPS(Starting_Tick);
		SDL_RenderPresent(Renderer);
	}
}


Pantalla::~Pantalla()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}
