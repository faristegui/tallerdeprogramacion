#include "Pantalla.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FPS 30

Pantalla::Pantalla()
{
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

	TTF_Font* Arial = TTF_OpenFont("Arial.ttf", 24); //this opens a font style and sets a size

	surface = TTF_RenderText_Solid(Arial, UnTexto.c_str(), textColor);
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

void Pantalla::MostrarMensaje(std::string Mensaje) {
	SDL_Rect Message_Rect;
	SDL_Texture* Message;
	std::string tmp;
	bool Sale = false;
	
	SDL_RenderClear(Renderer);
	get_text_and_rect(Renderer, 0, 0, Mensaje.c_str(), &Message, &Message_Rect);
	SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
	Mensaje = "Presione cualquier tecla para continuar...";
	get_text_and_rect(Renderer, 0, 20, Mensaje.c_str(), &Message, &Message_Rect);
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

std::string Pantalla::PedirParametro(std::string NombreParametro, std::string ValorXDefecto) {
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

		get_text_and_rect(Renderer, 0, 0, NombreParametro.c_str(), &Message, &Message_Rect);
		SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
		get_text_and_rect(Renderer, Message_Rect.w + 10, 0, UnTexto.c_str(), &Message, &Message_Rect);
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
