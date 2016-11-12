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
	ListaTextPlayers = new Lista<TextPlayers>();

	// Create an application window with the following settings:
	Window = SDL_CreateWindow("Metal Slug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	//Seteo el color del render (negro)
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
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

	TTF_Font* Fuente = NULL;
	
	while (Fuente == NULL) {
		Fuente = TTF_OpenFont(font, fontSize); //this opens a font style and sets a size
	}

	surface = TTF_RenderText_Solid(Fuente, UnTexto.c_str(), textColor);

	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (surface != NULL) {
		text_width = surface->w;
		text_height = surface->h;
	}
	SDL_FreeSurface(surface);
	TTF_CloseFont(Fuente);
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

void Pantalla::EscribirNombreJugador(std::string Nombre, int PosX, int PosY) {
	bool blnEncontro = false;
	ListaTextPlayers->iniciarCursor();

	while (ListaTextPlayers->avanzarCursor()) {
		TextPlayers UnText = ListaTextPlayers->obtenerCursor();
		if (UnText.Nombre == Nombre) {

			UnText.Message_Rect.x = PosX;
			UnText.Message_Rect.y = PosY;
			SDL_RenderCopy(Renderer, UnText.Message, NULL, &UnText.Message_Rect);
			blnEncontro = true;
			break;
		}
	}

	if (!blnEncontro) {
		TextPlayers UnText;
		UnText.Nombre = Nombre;
		get_text_and_rect(Renderer, PosX, PosY, Nombre, &UnText.Message, &UnText.Message_Rect, 12);
		ListaTextPlayers->agregar(UnText);
		SDL_RenderCopy(Renderer, UnText.Message, NULL, &UnText.Message_Rect);
	}
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
	Mensaje = "¡¡Por favor espere..!!";
	get_text_and_rect(Renderer, 170, 370, Mensaje.c_str(), &Message, &Message_Rect, 30);
	SDL_RenderCopy(Renderer, Message, NULL, &Message_Rect);
	SDL_RenderPresent(Renderer);
	while (!Sale) {

		Starting_Tick = SDL_GetTicks();

		while (SDL_PollEvent(&Event)) {
			string respuestaServer = "";
			cliente->EnviarMensaje("LIST",4);
			respuestaServer = cliente->RecibirMensajeTamanoVariable();
			if (respuestaServer == "OK"){
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

std::string Pantalla::PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY, char* pathFondo) {
	std::string UnTexto = ValorXDefecto;
	bool ParamValido = false;
	SDL_Rect Message_Rect;
	SDL_Texture* Message;

	char* start = VerificarRecurso(pathFondo);

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
		// Ingresa el parÃ¡metro
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
		pathDefault = "ClientResources/cross.bmp";
	}
	return strdup(pathDefault.c_str());;
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

			OffsetX = (Starting_Tick / 100) % UnEstado.CantFrames;
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
		int ancho = stoi(cliente->RecibirMensajeTamanoVariable());
		int alto = stoi(cliente->RecibirMensajeTamanoVariable());
		AgregarCapaFondoEscenario(nombreImagen, zIndex, ancho, alto);
	}

}

void Pantalla::AgregarCapaFondoEscenario(std::string nombreImagen, int zIndex,int ancho, int altura)
{
	SDL_Surface *TmpSurface;
	SDL_Texture *TmpTexture;
	CapaFondoEscenario UnaCapa;

	UnaCapa.nombreImagen = nombreImagen;
	UnaCapa.zIndex = zIndex;
	UnaCapa.ancho = ancho;
	UnaCapa.altura = altura;

	TmpSurface = IMG_Load(VerificarRecurso(nombreImagen));
	SDL_SetColorKey(TmpSurface, SDL_TRUE, SDL_MapRGB(TmpSurface->format, 128, 255, 0));
	TmpTexture = SDL_CreateTextureFromSurface(Renderer, TmpSurface);

	UnaCapa.Texture = TmpTexture;
	CapasFondoEscenario->agregar(UnaCapa);
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

SDL_Rect crearCamara(){
	SDL_Rect camara;
	camara.x = 0;
	camara.y = 0;
	camara.w = 800;
	camara.h = 600;

	return camara;
}

void Pantalla::IniciarJuego() {

	CargarSprites();
	CargarCapasFondoEscenario();

	bool sprite = false;
	bool GameRunning = true;

	SDL_Rect background_Rect;
	background_Rect.x = 0;
	background_Rect.y = 0;
	background_Rect.w = 800;
	background_Rect.h = 600;

	SDL_Rect CamaraCorta1 = crearCamara();
	SDL_Rect CamaraDibuja1 = crearCamara();
	SDL_Rect CamaraCorta2 = crearCamara();
	SDL_Rect CamaraDibuja2 = crearCamara();

	int speed = 10;
	
	string eventoAnterior = "";
	while ((GameRunning) && (cliente->TieneConexion()))  {

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
				}
				if (Event.key.keysym.sym == SDLK_LEFT) {
					Evento = "LEFT";
				}
				if (Event.key.keysym.sym == SDLK_SPACE) {

					Evento = "SPACE";
				}
				if (Event.key.keysym.sym == SDLK_DOWN) {

					Evento = "DOWN";
				}
				if (Event.key.keysym.sym == SDLK_UP) {

					Evento = "UP";
				}
				if (Event.key.keysym.sym == SDLK_d) { //Disparar Letra "D"

					Evento = "DISPARA";
				}
				//Eventos de selección de armas
				if (Event.key.keysym.sym == SDLK_h) { //Arma H

					Evento = "ARMA-H";
				}
				if (Event.key.keysym.sym == SDLK_r) { //Arma R

					Evento = "ARMA-R";
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
				if (Event.key.keysym.sym == SDLK_UP) {
					Evento = "SOLTO-UP";
				}
				if (Event.key.keysym.sym == SDLK_DOWN) {
					Evento = "SOLTO-DOWN";
				}
				if (Event.key.keysym.sym == SDLK_SPACE) {
					Evento = "SOLTO-SPACE";
				}
				if (Event.key.keysym.sym == SDLK_r) {

					CargarSprites();
					Evento = "RECARGA";
					CargarCapasFondoEscenario();
				}
				if (Event.key.keysym.sym == SDLK_d) {
					Evento = "SOLTO-DISPARA";
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
		
		std::string respuestaServidor = cliente->RecibirMensajeTamanoVariable();

		if (respuestaServidor != "LOST") {
			
			std::vector<std::string> mensajes = split(respuestaServidor, ';');

			int Indice = 0;
			CapasFondoEscenario->iniciarCursor();
			while (CapasFondoEscenario->avanzarCursor())
			{
				CapaFondoEscenario UnaCapa = CapasFondoEscenario->obtenerCursor();

				int NuevoX = stoi(mensajes[Indice]);

				if (NuevoX > UnaCapa.ancho - 800) {
					CamaraCorta1.x = UnaCapa.ancho - 800;
					CamaraDibuja1.x = UnaCapa.ancho - 800 - NuevoX;

					CamaraDibuja2.x = 800 + CamaraDibuja1.x;
				}
				else {
					CamaraCorta1.x = NuevoX;
					CamaraDibuja1.x = 0;
					CamaraDibuja2.x = 850;
				}

				SDL_RenderCopy(Renderer, UnaCapa.Texture, &CamaraCorta1, &CamaraDibuja1);
				SDL_RenderCopy(Renderer, UnaCapa.Texture, &CamaraCorta2, &CamaraDibuja2);
				Indice++;
			}

			int CantJugadores = stoi(mensajes[Indice]);
			Indice++;

			int PosX = 0;
			int PosY = 0;
		
			for (int i = 0; i < CantJugadores; i++) {

				string Nombre = mensajes[Indice];
				Indice++;
				string IDSprite = mensajes[Indice];
				Indice++;
				string Estado = mensajes[Indice];
				Indice++;
				PosX = stoi(mensajes[Indice]);
				Indice++;
				PosY = stoi(mensajes[Indice]);
				Indice++;

				RenderSprite(IDSprite, Estado, Starting_Tick, Renderer, PosX, PosY);
				EscribirNombreJugador(Nombre, PosX, PosY + 125);
			}

			int CantidadMensajes = stoi(mensajes[Indice]);
			Indice++;

			int CantEnemigos = stoi(mensajes[Indice]);
			Indice++;
			
			for(int i = 0; i < CantEnemigos; i++)
			{
				string nombreEnemigo = mensajes[Indice];
				Indice++;
				int posicionEnemigoX = stoi(mensajes[Indice]);
				Indice++;
				int posicionEnemigoY = stoi(mensajes[Indice]);
				Indice++;
				std::string nombreSprite = mensajes[Indice];
				Indice++;
				//Sleep(40); // Que funcion cumplia esto aca? Trababa todo el juego
				RenderSprite(nombreEnemigo, nombreSprite, Starting_Tick, Renderer, posicionEnemigoX, posicionEnemigoY);
 			}
			
			int CantProyectiles = stoi(mensajes[Indice]);
			Indice++;

			for (int i = 0; i < CantProyectiles; i++) {
				string IDSprite = mensajes[Indice];
				Indice++;
				string EstadoProyectil = mensajes[Indice];
				Indice++;
				int xProyectil = stoi(mensajes[Indice]);
				Indice++;
				int yProyectil = stoi(mensajes[Indice]);
				Indice++;
				RenderSprite(IDSprite, EstadoProyectil, Starting_Tick, Renderer, xProyectil, yProyectil);
			}

			if (CantidadMensajes > 0) {
				for(int i = 0; i < CantidadMensajes;i++)
				{
					string Mensaje = cliente->RecibirMensajeTamanoVariable();
					AgregarMensaje(Mensaje, 5, Starting_Tick);
				}
			}

			MostrarMensajes(Starting_Tick);

			WaitFPS(Starting_Tick);
			SDL_RenderPresent(Renderer);
		}
	}

	if (!cliente->TieneConexion()) {

		EscribirMensaje("Se ha perdido la conexion con el servidor", 0, 5, 12, Renderer);
		SDL_RenderPresent(Renderer);
		Sleep(10000);
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