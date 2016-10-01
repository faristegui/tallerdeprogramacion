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
	
	SDL_Rect back = this->crearFondo("ClientResources/partida.bmp",800,600);

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

SDL_Rect Pantalla::crearFondo(char* path, int width, int heigth) {
	background = SDL_LoadBMP(path);

	SDL_Rect background_Rect;

	background_Rect.x = 0;
	background_Rect.y = 0;
	background_Rect.w = width;
	background_Rect.h = heigth;
	
	texture = SDL_CreateTextureFromSurface(Renderer,background);

	return background_Rect;
}

std::string Pantalla::PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY) {
	std::string UnTexto = ValorXDefecto;
	bool ParamValido = false;
	SDL_Rect Message_Rect;
	SDL_Texture* Message;

	SDL_Rect back = this->crearFondo("ClientResources/start.bmp",800,600);

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

void Pantalla::AgregarSprite(std::string ID, int FrameWidth, int FrameHeight) {
	SDL_Surface *TmpSurface;
	SDL_Texture *TmpTexture;
	Sprite UnSprite;
	SpriteEstado UnEstado;

	int CantEstados = stoi(cliente->RecibirMensajeTamanoVariable());
	UnSprite.Estados = new Lista<SpriteEstado>();

	for (int i = 0; i < CantEstados; i++) {

		UnEstado.Nombre = cliente->RecibirMensajeTamanoVariable();
		UnEstado.CantFrames = stoi(cliente->RecibirMensajeTamanoVariable());
		UnEstado.Numero = i;

		UnSprite.Estados->agregar(UnEstado);
	}

	UnSprite.ID = ID;
	UnSprite.FrameWidth = FrameWidth;
	UnSprite.FrameHeight = FrameHeight;

	ID = "ClientResources/" + ID + ".bmp";

	TmpSurface = IMG_Load(ID.c_str());
	SDL_SetColorKey(TmpSurface, SDL_TRUE, SDL_MapRGB(TmpSurface->format, 128, 255, 0));
	TmpTexture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);

	UnSprite.Texture = TmpTexture;

	Sprites->agregar(UnSprite);
}
void Pantalla::CargarSprites() {

	Sprites = new Lista<Sprite>();

	cliente->EnviarMensaje("SPRI", 4);
	int CantidadSprites = stoi(cliente->RecibirMensajeTamanoVariable());

	for (int i = 0; i < CantidadSprites; i++) {

		std::string ID = cliente->RecibirMensajeTamanoVariable();
		int FrameWidth = stoi(cliente->RecibirMensajeTamanoVariable());
		int FrameHeight = stoi(cliente->RecibirMensajeTamanoVariable());

		AgregarSprite(ID, FrameWidth, FrameHeight);
	}
}

SpriteEstado Pantalla::GetEstado(Lista<SpriteEstado> *Estados, std::string Nombre) {

	Estados->iniciarCursor();

	while (Estados->avanzarCursor()) {

		if (Nombre == Estados->obtenerCursor().Nombre) {

			return Estados->obtenerCursor();
		}
	}

}

void Pantalla::RenderSprite(std::string ID, std::string NombreEstado, Uint32 Ticks, SDL_Renderer *Renderer,
							int PosX, int PosY) {

	SpriteEstado UnEstado;
	Sprite UnSprite;
	Uint32 OffsetX;
	Uint32 OffsetY;
	SDL_Rect Crop_Rect;
	SDL_Rect Posicion_Rect;

	Sprites->iniciarCursor();

	while (Sprites->avanzarCursor())
	{
		if (ID == Sprites->obtenerCursor().ID) {

			UnSprite = Sprites->obtenerCursor();
			UnEstado = GetEstado(UnSprite.Estados, NombreEstado);

			OffsetX = (Starting_Tick / 200) % UnEstado.CantFrames;
			OffsetY = UnEstado.Numero * UnSprite.FrameHeight;

			Crop_Rect.x = OffsetX * UnSprite.FrameWidth;
			Crop_Rect.y = OffsetY * UnSprite.FrameHeight;
			Crop_Rect.w = UnSprite.FrameWidth;
			Crop_Rect.h = UnSprite.FrameHeight;

			Posicion_Rect.x = PosX;
			Posicion_Rect.y = PosY;
			Posicion_Rect.w = UnSprite.FrameWidth * 2;
			Posicion_Rect.h = UnSprite.FrameHeight * 2;

			SDL_RenderCopy(Renderer, UnSprite.Texture, &Crop_Rect, &Posicion_Rect);

			break;
		}
	}
}

void Pantalla::IniciarJuego() {

	CargarSprites();

	SDL_Rect Back_Rect = crearFondo("ClientResources/escenario.bmp", 800, 600); // Imagen para el escenario del juego

	bool sprite = false;
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

			string IDSprite = cliente->RecibirMensajeTamanoVariable();
			string Estado = cliente->RecibirMensajeTamanoVariable();
			int PosX = stoi(cliente->RecibirMensaje(4));
			int PosY = stoi(cliente->RecibirMensaje(4));
			
			RenderSprite(IDSprite, Estado, Starting_Tick, Renderer, PosX, PosY);
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