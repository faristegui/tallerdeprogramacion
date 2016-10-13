#include "Juego.h"
#include "Globales.h"

int FuncionCuadratica(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

void FisicaThread(void* arg) {

	int BordeEnXMinCamara = 20;
	int BordeEnXMaxCamara = 710;
	int VelocidadEnX = 10;

	Juego *UnJuego = (Juego*)arg;

	while (true) {

		bool PuedeAvanzarCamara = true;

		int CantJugadores = UnJuego->GetCantJugadores();

		for (int i = 0; i < CantJugadores; i++) {

			Jugador *UnJugador = UnJuego->GetJugador(i);

			if (UnJugador->GetX() < BordeEnXMinCamara) {

				PuedeAvanzarCamara = false;
				break;
			}
		}

		for (int i = 0; i < CantJugadores; i++) {

			Jugador *UnJugador = UnJuego->GetJugador(i);

			if (UnJugador->GetEstaConectado()) {

				if (UnJugador->EstaCaminando()) {

					if (UnJugador->GetEstado() == "CAMINA-DER") {

						if (UnJugador->GetX() <= BordeEnXMaxCamara) {

							UnJugador->MoverEnX(VelocidadEnX);
						} else {
							
							// TODO: Logica avanzar camara
						}
					}
					else {

						if (UnJugador->GetX() >= BordeEnXMinCamara) {

							UnJugador->MoverEnX(-VelocidadEnX);
						}
					}
				}

				if (UnJugador->EstaSaltando()) {

					float TiempoActual = clock();
					float TiempoInicioSaltoX = UnJugador->GetTiempoInicioSaltoX();
					float TiempoInicioSaltoY = UnJugador->GetTiempoInicioSaltoY();
					int PosicionXInicioSalto = UnJugador->GetPosicionXInicioSalto();
					int PosicionYInicioSalto = UnJugador->GetPosicionYInicioSalto();

					float Diferencia;

					Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoY, 0, 60, 300);
					UnJugador->SetY(PosicionYInicioSalto - Diferencia);

					if (TiempoActual - TiempoInicioSaltoY >= 500) {

						if (UnJugador->GetEstadoAnterior() == "") {

							UnJugador->SetEstadoAnterior("QUIETO");
						}

						UnJugador->SetEstado(UnJugador->GetEstadoAnterior());
						UnJugador->SetEstadoAnterior("");
						UnJugador->SetY(PosicionYInicioSalto);
					}

					if (UnJugador->GetEstado() == "SALTANDO-DER") {

						Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);
						UnJugador->SetX(PosicionXInicioSalto + Diferencia);
					}
					else {

						if (UnJugador->GetEstado() == "SALTANDO-IZQ") {
							Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);
							UnJugador->SetX(PosicionXInicioSalto - Diferencia);
						}
					}

					// TODO: Pensar logica p avanzar camara si puede (si pasar a saltando)
					/*
					if (x >= 710) {
						this->Estado = "SALTANDO";
						x = 710;
					}
					else {
						if (x <= 20) {
							this->Estado = "SALTANDO";
							x = 20;
						}
					}*/
				}
			}
		}

		Sleep(50);
	}


}

Juego::Juego()
{
	CantJugadores = 0;
	_beginthread(FisicaThread, 0, this);
}

Posicion Juego::getCamaraPared()
{
	return camaraPared;
}

Posicion Juego::getCamaraCielo()
{
		return camaraCielo;
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
	int IndiceJugador = -1;

	for (int i = 0; i < CantJugadores; i++) {

		if (ToLowerCase(Jugadores[i]->GetNombre()) == Usuario) {
			
			IndiceJugador = i;

			if (Jugadores[i]->GetX() <= 710) {

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

				if((Camara.x == 1800))
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
	if((Tipo == "SOLTO-RIGHT") || (Tipo == "SOLTO-LEFT"))
	{
		if (Jugadores[IndiceJugador]->EstaCaminando()) {

			Jugadores[IndiceJugador]->SetEstado("QUIETO");
		}
		else {			
			if (Jugadores[IndiceJugador]->EstaSaltando()) {

				Jugadores[IndiceJugador]->SetEstadoAnterior("QUIETO");
			}
		}
	}

	if (Tipo == "SOLTO-DOWN") {
		if (Jugadores[IndiceJugador]->GetEstado() == "AGACHADO") {

			Jugadores[IndiceJugador]->SetEstado("QUIETO");
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

Lista<std::string>* Juego::GetNombresJugadoresOnline() {
	Lista<std::string>* JugadoresOnline = new Lista<std::string>();

	for (int i = 0; i < CantJugadores; i++) {

		if (Jugadores[i]->GetEstaConectado()) {

			JugadoresOnline->agregar(Jugadores[i]->GetNombre());
		}
	}

	return JugadoresOnline;
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
