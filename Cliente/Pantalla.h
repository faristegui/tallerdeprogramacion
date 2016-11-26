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
struct SpriteEstado
{
	std::string Nombre;
	int CantFrames;
	int Numero;
};
struct Sprite {
	std::string ID;
	SDL_Texture *Texture;
	int FrameWidth;
	int FrameHeight;
	int Velocidad;
	Lista<SpriteEstado> *Estados;
};
struct MensajeConsola {
	std::string Mensaje;
	int TiempoDibujo;
	int Duracion;
};

struct CapaFondoEscenario {
	std::string nombreImagen;
	int zIndex;
	int ancho;
	int altura;
	SDL_Texture *Texture;
};

struct TextPlayers {
	std::string Nombre;
	SDL_Rect Message_Rect;
	SDL_Texture* Message;
};

class Pantalla
{
public:
	Pantalla(Client* unCliente);
	~Pantalla();

	std::string PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY, char* pathFondo);
	SDL_Rect crearFondo(const char* path, int width, int heigth);
	void MostrarMensaje(std::string Mensaje, int posX, int posY);
	void IniciarJuego();
	Posicion* obtenerPosicion();
private:
	bool PedirRecarga;
	int ParamsPedidos;
	Uint32  Starting_Tick;
	SDL_Renderer *Renderer;
	SDL_Window *Window;
	SDL_Texture *texture;
	SDL_Surface *background;
	SDL_Color COLOR_WHITE;
	SDL_Event Event;
	TTF_Font* FuenteDoce;
	TTF_Font* FuenteDieciseis;
	TTF_Font* FuenteVeinte;
	TTF_Font* FuenteVeintiseis;
	TTF_Font* FuenteVeintiocho;
	TTF_Font* FuenteTreinta;
	Client* cliente;
	Posicion* bolaPos;
	Lista<TextPlayers> *ListaTextPlayers;
	void get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
	SDL_Texture **texture, SDL_Rect *rect,TTF_Font* Fuente);
	void EscribirMensaje(std::string Mensaje, int X, int Y,TTF_Font* fuente, SDL_Renderer *Renderer);
	void EscribirNombreJugador(std::string Nombre, int PosX, int PosY);
	void WaitFPS(Uint32 starting_tick);
	void AgregarSprite(std::string ID, int FrameWidth, int FrameHeight, int Velocidad);
	void CargarSprites();
	void CargarEnemigos();
	char* VerificarRecurso(std::string path);
	Lista<Sprite> *Sprites;
	Lista<CapaFondoEscenario> *CapasFondoEscenario;
	Lista<MensajeConsola> *Mensajes;
	void AgregarMensaje(std::string Mensaje, int Duracion, int TiempoDibujo);
	void MostrarMensajes(int StartingTick);
	void RenderSprite(std::string ID, std::string NombreEstado, Uint32 Ticks, SDL_Renderer *Renderer, int PosX, int PosY);
	SpriteEstado GetEstado(Lista<SpriteEstado> *Estados, std::string Nombre);
	void CargarCapasFondoEscenario();
	void AgregarCapaFondoEscenario(std::string nombreImagen, int zIndex,int ancho,int altura);
	CapaFondoEscenario getCapaFondoEscenario(Lista<CapaFondoEscenario> *CapasFondoEscenario, int zindex);
};