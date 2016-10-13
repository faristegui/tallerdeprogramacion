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
		
		int MinPosX = -1;
		bool AvanzaCamara = false;
		int CantJugadores = UnJuego->GetCantJugadores();

		for (int i = 0; i < CantJugadores; i++) {

			Jugador *UnJugador = UnJuego->GetJugador(i);

			if (UnJugador->GetEstaConectado()) {

				if (UnJugador->EstaCaminando()) {

					if (UnJugador->GetEstado() == "CAMINA-DER") {

						if (UnJugador->GetX() <= BordeEnXMaxCamara) {

							UnJugador->MoverEnX(VelocidadEnX);
						} else {
							
							AvanzaCamara = true;
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

					if (UnJugador->GetX() >= BordeEnXMaxCamara) {

						UnJugador->SetEstado("SALTANDO");
						UnJugador->SetX(BordeEnXMaxCamara);
					}
					else {
						if (UnJugador->GetX() <= BordeEnXMinCamara) {

							UnJugador->SetEstado("SALTANDO");
							UnJugador->SetX(BordeEnXMinCamara);
						}
					}
				}

				if ((MinPosX == -1) || (UnJugador->GetX() < MinPosX)) {

					MinPosX = UnJugador->GetX();
				}
			}
		}

		if (MinPosX <= BordeEnXMinCamara) {

			AvanzaCamara = false;
		}

		if (AvanzaCamara) {

			UnJuego->AvanzarCamara();

			for (int i = 0; i < CantJugadores; i++) {

				bool MoverParaAtras = false;
				Jugador *UnJugador = UnJuego->GetJugador(i);

				if (UnJugador->GetEstado() != "CAMINA-DER") {

					MoverParaAtras = true;
				} else {
					
					if (UnJugador->GetX() < BordeEnXMaxCamara) {

						MoverParaAtras = true;
					}
				}

				if (MoverParaAtras) {

					UnJugador->MoverEnX(-VelocidadEnX);
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

void Juego::AvanzarCamara() {

	Camara.x += 10;
	if(Camara.x > 1800)
	{
		Camara.x = 0;
	}
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

	int IndiceJugador = GetIndexUsuario(Usuario);

	if ((Tipo == "UP") || (Tipo == "RIGHT") || (Tipo == "DOWN") || (Tipo == "LEFT")) {

		Jugadores[IndiceJugador]->Mover(Tipo);
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
