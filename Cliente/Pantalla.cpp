#include "Pantalla.h"
#include <sstream>
#include <string>
#include <vector>
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
	Mensajes = new Lista<MensajeConsola>();

	// Create an application window with the following settings:
	Window = SDL_CreateWindow("Metal Slug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	//Seteo el color del render (negro)
	SDL_SetRenderDrawColor(Renderer,0,0,0,255);
	if (Window == NULL) {
		// TODO: Log "Could not create window"
	}
}

void Pantalla::AgregarMensaje(std::string Mensaje, int Duracion, int TiempoDibujo) {
	MensajeConsola UnMensaje;
	
	UnMensaje.Mensaje = Mensaje;
	UnMensaje.Duracion = Duracion;
	UnMensaje.TiempoDibujo = TiempoDibujo;

	Mensajes->agregar(UnMensaje);
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

	char* font = VerificarRecurso("start.ttf");

	TTF_Font* Fuente = TTF_OpenFont(font, fontSize); //this opens a font style and sets a size

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

void Pantalla::EscribirMensaje(std::string Mensaje, int PosX, int PosY, int Tamano, SDL_Renderer *Renderer) {
	SDL_Rect Message_Rect;
	SDL_Texture* Message;

	get_text_and_rect(Renderer, PosX, PosY, Mensaje.c_str(), &Message, &Message_Rect, Tamano);
	SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
}

void Pantalla::MostrarMensaje(std::string Mensaje, int posX, int posY) {
	SDL_Rect Message_Rect;
	SDL_Texture* Message;
	std::string tmp;
	bool Sale = false;

	char* fondo = VerificarRecurso("partida.bmp");
	
	SDL_Rect back = this->crearFondo(fondo,800,600);

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

SDL_Rect Pantalla::crearFondo(const char* path, int width, int heigth) {

	SDL_Rect background_Rect;

	SDL_Surface *TmpSurface;

	background_Rect.x = 0;
	background_Rect.y = 0;
	background_Rect.w = width;
	background_Rect.h = heigth;

	TmpSurface = SDL_LoadBMP(path);
	SDL_SetColorKey(TmpSurface, SDL_TRUE, SDL_MapRGB(TmpSurface->format, 128, 255, 0));
	texture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);

	return background_Rect;
}

std::string Pantalla::PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY) {
	std::string UnTexto = ValorXDefecto;
	bool ParamValido = false;
	SDL_Rect Message_Rect;
	SDL_Texture* Message;

	char* start = VerificarRecurso("start.bmp");

	SDL_Rect back = this->crearFondo(start,800,600);

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

	ID = ID + ".bmp";

	TmpSurface = IMG_Load(VerificarRecurso(ID));
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

char* Pantalla::VerificarRecurso(std::string path)
{
	std::string pathDefault = "ClientResources/";

	if(std::ifstream(pathDefault + path))
	{
		pathDefault = "ClientResources/" + path;
	}
	else
	{
		pathDefault = "Default/" + path;
	}
	return strdup(pathDefault.c_str());;
}

SpriteEstado Pantalla::GetEstado(Lista<SpriteEstado> *Estados, std::string Nombre) {

	int PosSep = Nombre.find("-");

	if (PosSep > -1) {

		Nombre = Nombre.substr(0, PosSep);
	}

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
			Crop_Rect.y = OffsetY;
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


void Pantalla::CargarCapasFondoEscenario()
{

	CapasFondoEscenario = new Lista<CapaFondoEscenario>();

	cliente->EnviarMensaje("ESCN", 4);
	int CantidadCapasXFondoEscenario = stoi(cliente->RecibirMensajeTamanoVariable());

	for (int i = 0; i < CantidadCapasXFondoEscenario; i++) {

		std::string nombreImagen = cliente->RecibirMensajeTamanoVariable();
		int zIndex = stoi(cliente->RecibirMensajeTamanoVariable());


		AgregarCapaFondoEscenario(nombreImagen, zIndex);
	}

}

void Pantalla::AgregarCapaFondoEscenario(std::string nombreImagen, int zIndex)
{
	CapaFondoEscenario unaCapa;
	unaCapa.nombreImagen = nombreImagen;
	unaCapa.zIndex = zIndex;
	CapasFondoEscenario->agregar(unaCapa);

}

CapaFondoEscenario Pantalla::getCapaFondoEscenario(Lista<CapaFondoEscenario> *CapasFondoEscenario, int zindex)
{

	CapasFondoEscenario->iniciarCursor();

	while (CapasFondoEscenario->avanzarCursor()) {

		if (zindex == CapasFondoEscenario->obtenerCursor().zIndex) {

			return CapasFondoEscenario->obtenerCursor();
		}
	}

}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void Pantalla::IniciarJuego() {

	CargarSprites();

	CargarCapasFondoEscenario();
	
	//Verifica que el path exista y si no usa una carpeta Default

	char* escenario = VerificarRecurso(getCapaFondoEscenario(CapasFondoEscenario, 1).nombreImagen);
	char* cielo = VerificarRecurso(getCapaFondoEscenario(CapasFondoEscenario, 2).nombreImagen);
	char* fondo = VerificarRecurso(getCapaFondoEscenario(CapasFondoEscenario, 3).nombreImagen);

	SDL_Rect Back_Rect = crearFondo(escenario, 800, 600);// Escenario movible

	SDL_Surface* fondoCielo = SDL_LoadBMP(cielo);
	fondoCielo->w = 2600;
	fondoCielo->h = 600;
	SDL_Texture* texturaCielo = SDL_CreateTextureFromSurface(Renderer, fondoCielo);
	
	SDL_Surface* fondoEscenario = SDL_LoadBMP(fondo);
	fondoEscenario->w = 2600;
	fondoEscenario->h = 600;
	SDL_SetColorKey(fondoEscenario, SDL_TRUE, SDL_MapRGB(fondoEscenario->format, 128, 255, 0));
	SDL_Texture* texturaFondoEscenario = SDL_CreateTextureFromSurface(Renderer, fondoEscenario);
	bool sprite = false;
	bool GameRunning = true;
	SDL_Rect camara;
	camara.x = 0;
	camara.y = 0;
	camara.w = 800;
	camara.h = 600;
	int speed = 10;

	SDL_Rect camaraPared;
	camaraPared.x = 0;
	camaraPared.y = 0;
	camaraPared.w = 800;
	camaraPared.h = 600;

	SDL_Rect camaraCielo;
	camaraCielo.x = 0;
	camaraCielo.y = 0;
	camaraCielo.w = 800;
	camaraCielo.h = 600;
	
	string eventoAnterior = "";
	while (GameRunning) {

		Starting_Tick = SDL_GetTicks();
		string Evento = "";
		while (SDL_PollEvent(&Event)) {

			if (Event.type == SDL_QUIT) {

				GameRunning = false;
				break;
			}
			
			if (Event.type == SDL_KEYDOWN) {
				if (Event.key.keysym.sym == SDLK_RIGHT) {
					Evento = "RIGHT";
					camaraPared.x += 10;
					//Loop de la pared
					if(camaraPared.x > 800)
					{
						camaraPared.x = 0;
					}
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
			}
			if(Event.type == SDL_KEYUP)
			{
				if (Event.key.keysym.sym == SDLK_RIGHT) {
					Evento = "SOLTO-RIGHT";
				}
				if (Event.key.keysym.sym == SDLK_LEFT) {
					Evento = "SOLTO-LEFT";
				}
				if (Event.key.keysym.sym == SDLK_DOWN) {
					Evento = "SOLTO-DOWN";
				}
				if (Event.key.keysym.sym == SDLK_UP) {
					Evento = "SOLTO-UP";
				}
			}

			if ((Evento != "") && (eventoAnterior != Evento)) {
				cliente->EnviarMensaje("EVEN", 4);
				cliente->EnviarMensajeTamanoVariable(Evento);
				eventoAnterior = Evento;
			}
		}

		SDL_RenderClear(Renderer);

		cliente->EnviarMensaje("STAT", 4);
		
		std::string respuestaServidor = cliente->RecibirMensaje(150);

		std::vector<std::string> mensajes = split(respuestaServidor, ';');

		camara.x = stoi(mensajes[0]);
		camara.y = stoi(mensajes[1]);

		/*camara.x = stoi(cliente->RecibirMensajeTamanoVariable());
		
		camara.y = stoi(cliente->RecibirMensajeTamanoVariable());

		camaraPared.x = stoi(cliente->RecibirMensajeTamanoVariable());
		camaraPared.y = stoi(cliente->RecibirMensajeTamanoVariable());

		*/
		SDL_RenderCopy(Renderer, texturaCielo,&camaraCielo,NULL);
		SDL_RenderCopy(Renderer, texturaFondoEscenario,&camaraPared,NULL);
		SDL_RenderCopy(Renderer, texture, &camara, &Back_Rect);

		//string StrCantJugadores = cliente->RecibirMensaje(1);
		//int CantJugadores = stoi(StrCantJugadores);
		int CantJugadores = stoi(mensajes[2]);

		int PosX = 0;
		int PosY = 0;

		for (int i = 0; i < CantJugadores*5; i+=5) {

			string Nombre = mensajes[i + 3]; //cliente->RecibirMensaje(15);
			string IDSprite = mensajes[i + 4]; //cliente->RecibirMensajeTamanoVariable();
			string Estado = mensajes[i + 5]; //cliente->RecibirMensajeTamanoVariable();
			PosX = stoi(mensajes[i + 6]);//stoi(cliente->RecibirMensaje(4));
			PosY = stoi(mensajes[i + 7]);//stoi(cliente->RecibirMensaje(4));
			
			RenderSprite(IDSprite, Estado, Starting_Tick, Renderer, PosX, PosY);
			EscribirMensaje(Nombre, PosX, PosY + 85, 12, Renderer);
		}
		/*
		int CantidadMensajes = stoi(cliente->RecibirMensaje(8));
		if (CantidadMensajes > 0) {
			string Mensaje = cliente->RecibirMensajeTamanoVariable();
			AgregarMensaje(Mensaje, 5, Starting_Tick);
		}

		MostrarMensajes(Starting_Tick);
		*/

		//Loop del cielo.
		camaraCielo.x += 1;
		if(camaraCielo.x > 1800)
		{
			camaraCielo.x = 0;
		}

		WaitFPS(Starting_Tick);
		SDL_RenderPresent(Renderer);
	}
}

void Pantalla::MostrarMensajes(int StartingTick) {
	int OffsetY = 5;
	int i = 0;
	Mensajes->iniciarCursor();

	while (Mensajes->avanzarCursor()) {
		
		MensajeConsola UnMensaje = Mensajes->obtenerCursor();

		if (StartingTick - UnMensaje.TiempoDibujo < UnMensaje.Duracion * 1000) {

			EscribirMensaje(UnMensaje.Mensaje, 0, OffsetY, 20, Renderer);
			OffsetY += 20;
		}
		else {

			Mensajes->remover(i); //TODO: No esta borrando
		}
		i++;
	}
}

Pantalla::~Pantalla()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}