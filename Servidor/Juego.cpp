#include "Juego.h"
#include "Globales.h"

Juego::Juego()
{
	CantJugadores = 0;
}


void Juego::AgregarJugador(std::string UnNombre, std::string UnIDSprite) {
	Jugadores[CantJugadores] = new Jugador(UnNombre, UnIDSprite);
	CantJugadores++;
}

Posicion Juego::GetCamara() {

	return Camara;
}

void Juego::RecibirEvento(std::string Usuario, std::string Tipo) {

	int MinX = -1;
	bool PuedeAvanzarCamara = true;
	int IndiceJugador = 0;

	for (int i = 0; i < CantJugadores; i++) {

		if (ToLowerCase(Jugadores[i]->GetNombre()) == Usuario) {
			
			IndiceJugador = i;

			if (Jugadores[i]->GetX() < 710) {

				PuedeAvanzarCamara = false;
			}
		}

		if ((Jugadores[i]->GetX() < MinX) || (MinX == -1)) {

			MinX = Jugadores[i]->GetX();
		}
	}

	if (MinX < 20) {
		PuedeAvanzarCamara = false;
	}

	if ((Tipo == "UP") || (Tipo == "RIGHT") || (Tipo == "DOWN") || (Tipo == "LEFT")) {

		Jugadores[IndiceJugador]->Mover(Tipo);

		if (Tipo == "RIGHT") {

			if (PuedeAvanzarCamara) {

				Camara.x += 10;
				
				for (int i = 0; i < CantJugadores; i++) {

					if (i != IndiceJugador) {

						Jugadores[i]->MoverAdelante(-10);
					}
				}
			}
		}
	}

}

int Juego::GetIndexUsuario(std::string Usuario) {
	Usuario = ToLowerCase(Usuario);
	
	for (int i = 0; i < CantJugadores; i++) {

		if (ToLowerCase(Jugadores[i]->GetNombre()) == Usuario) {

			return i;
		}
	}

	return -1;
}

int Juego::GetCantJugadores() {

	return CantJugadores;
}

Jugador Juego::GetJugador(std::string Usuario) {
	int i = GetIndexUsuario(Usuario);

	return *Jugadores[i];
}

Jugador Juego::GetJugador(int Index) {
	
	return *Jugadores[Index];
}


bool Juego::UsuarioYaLogueado(std::string Usuario) {
	return !(GetIndexUsuario(Usuario) == -1);
}

Juego::~Juego()
{
}
