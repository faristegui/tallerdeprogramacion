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
	// TODO: Ver si lo siguiente esta ok:
	int FrameWidth;
	int FrameHeight;
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
};

class Pantalla
{
public:
	Pantalla(Client* unCliente);
	~Pantalla();

	std::string PedirParametro(std::string NombreParametro, std::string ValorXDefecto, int posX, int posY);
	SDL_Rect crearFondo(const char* path, int width, int heigth);
	void MostrarMensaje(std::string Mensaje, int posX, int posY);
	void IniciarJuego();
	Posicion* obtenerPosicion();
private:
	int ParamsPedidos;
	Uint32  Starting_Tick;
	SDL_Renderer *Renderer;
	SDL_Window *Window;
	SDL_Texture *texture;
	SDL_Surface *background;
	SDL_Color COLOR_WHITE;
	SDL_Event Event;
	Client* cliente;
	Posicion* bolaPos;
	void get_text_and_rect(SDL_Renderer *renderer, int x, int y, std::string UnTexto,
		SDL_Texture **texture, SDL_Rect *rect, int fontSize);
	void EscribirMensaje(std::string Mensaje, int X, int Y, int Tamano, SDL_Renderer *Renderer);
	void WaitFPS(Uint32 starting_tick);
	void AgregarSprite(std::string ID, int FrameWidth, int FrameHeight);
	void CargarSprites();
	char* VerificarRecurso(std::string path);
	Lista<Sprite> *Sprites;
	Lista<MensajeConsola> *Mensajes;
	void AgregarMensaje(std::string Mensaje, int Duracion, int TiempoDibujo);
	void MostrarMensajes(int StartingTick);
	void RenderSprite(std::string ID, std::string NombreEstado, Uint32 Ticks, SDL_Renderer *Renderer, int PosX, int PosY);
	SpriteEstado GetEstado(Lista<SpriteEstado> *Estados, std::string Nombre);
	void CargarCapasFondoEscenario();
	Lista<CapaFondoEscenario> *CapasFondoEscenario;
	void AgregarCapaFondoEscenario(std::string nombreImagen, int zIndex,int ancho,int altura);
	CapaFondoEscenario getCapaFondoEscenario(Lista<CapaFondoEscenario> *CapasFondoEscenario, int zindex);
};