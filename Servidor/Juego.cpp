#include "Juego.h"
#include "Globales.h"

Juego::Juego()
{
	CantJugadores = 0;
}


void Juego::AgregarJugador(std::string UnNombre, std::string UnIDSprite) {

	bool JugadorYaSeHabiaConectado = false;

	for (int i = 0; i < CantJugadores; i++) {
		if (Jugadores[i]->GetNombre() == UnNombre) {

			Jugadores[i]->SetEstaConectado(true);
			JugadorYaSeHabiaConectado = true;
			break;
		}
	}

	if (!JugadorYaSeHabiaConectado) {
		Jugadores[CantJugadores] = new Jugador(UnNombre, UnIDSprite);
		CantJugadores++;
	}
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

			if (Jugadores[i]->GetEstaConectado()) {
				MinX = Jugadores[i]->GetX();
			}
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

				if(Camara.x > 1000)
				{
					Camara.x = 0;
				}
				
				for (int i = 0; i < CantJugadores; i++) {

					if ((i != IndiceJugador) && (Jugadores[i]->GetEstaConectado())) {

						Jugadores[i]->MoverEnX(-10);
					}
				}
			}
		}
	}
	if((Tipo == "SOLTO-RIGHT") || (Tipo == "SOLTO-LEFT") || (Tipo == "SOLTO-DOWN"))
	{
		Jugadores[IndiceJugador]->SetEstado("QUIETO");
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

Jugador* Juego::GetJugador(std::string Usuario) {
	int i = GetIndexUsuario(Usuario);

	return Jugadores[i];
}

Jugador* Juego::GetJugador(int Index) {
	
	return Jugadores[Index];
}

int Juego::GetIndiceJugador(std::string Usuario) {

	return GetIndexUsuario(Usuario);
}

bool Juego::UsuarioYaLogueado(std::string Usuario) {
	int i = GetIndexUsuario(Usuario);

	if (i > -1) {

		return Jugadores[i]->GetEstaConectado();
	}

	return false;
}

Juego::~Juego()
{
}
