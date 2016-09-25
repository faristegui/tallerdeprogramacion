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

	TTF_Font* Fuente = TTF_OpenFont("start.ttf", fontSize); //this opens a font style and sets a size

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
	
	SDL_Rect back = this->crearFondo("images/partida.bmp");

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

	SDL_Rect back = this->crearFondo("images/start.bmp");

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

void Pantalla::IniciarJuego() {

	SDL_Surface *SurfaceBall = IMG_Load("Sprite.bmp");
	SDL_SetColorKey(SurfaceBall, SDL_TRUE, SDL_MapRGB(SurfaceBall->format, 128, 255, 0));
	SDL_Texture *Ball = SDL_CreateTextureFromSurface(Renderer, SurfaceBall); //IMG_LoadTexture(Renderer, "Sprite.bmp");
	SDL_Rect Ball_Rect;
	string respuestaServer = "1";
	int movimiento = -1;
	// Imagen para el escenario del juego
	SDL_Rect back = this->crearFondo("images/escenario.bmp");

	

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
					movimiento = 1;
				}
				if (Event.key.keysym.sym == SDLK_LEFT) {
					Ball_Rect.x -= 20;
					movimiento = 2;
				}
				if (Event.key.keysym.sym == SDLK_UP) {
					Ball_Rect.y -= 20;
					movimiento = 3;
				}
				if (Event.key.keysym.sym == SDLK_DOWN) {
					Ball_Rect.y += 20;
					movimiento = 4;
				}
				//Envio posicion actual
				cliente->EnviarMensaje("ENVI",4);
				cliente->EnviarMensaje("fulano", 15);

				std::stringstream convertir;
				convertir << movimiento;
				cliente->EnviarMensajeTamanoVariable(convertir.str());
				/*
				bolaPos->x = Ball_Rect.x;
				bolaPos->y = Ball_Rect.y;
				Probablemente haga falta NO BORRAR
				string coordenadas = "";
				std::stringstream convertir;
				convertir << bolaPos->x;
				coordenadas = convertir.str();
				convertir.str("");
				coordenadas += " ";

				convertir << bolaPos->y;
				
				coordenadas += convertir.str();
				*/
				
			}

		}

		//Actualizacion posicion rival
			cliente->EnviarMensaje("REC",3);
			respuestaServer = cliente->RecibirMensaje(8);
			if(stoi(respuestaServer) > 0)
			{
				string UsuarioEmisor = cliente->RecibirMensaje(15);
				string ContenidoMensaje = cliente->RecibirMensajeTamanoVariable();

				switch (stoi(ContenidoMensaje))
				{
					case 1:
						Ball_Rect.x += 20;
						break;
					case 2:
						Ball_Rect.x -= 20;
						break;
					case 3:
						Ball_Rect.y -= 20;
						break;
					case 4: 
						Ball_Rect.y += 20;
						break;
					default:
						break;
				}

			}
			respuestaServer = "";
			
		
		SDL_RenderClear(Renderer);

		//SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
		SDL_RenderCopy(Renderer, texture, NULL, &back);
		
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