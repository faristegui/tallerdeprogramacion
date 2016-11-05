#include "Juego.h"
#include "Globales.h"

int FuncionCuadratica(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

void FisicaThread(void* arg) {

	int ticks_start = 0;
	int BordeEnXMinCamara = 20;
	int BordeEnXMaxCamara = 710;
	int VelocidadEnX = 10;

	Juego *UnJuego = (Juego*)arg;

	while (true) {
		
		ticks_start = std::clock();

		int MinPosX = -1;
		bool AvanzaCamara = false;
		int CantJugadores = UnJuego->GetCantJugadores();
		int CantEnemigos = UnJuego->GetCantEnemigos();

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

							UnJugador->SetEstadoAnterior("QUIETO-DER");
						}

						UnJugador->SetEstado(UnJugador->GetEstadoAnterior());
						UnJugador->SetEstadoAnterior("");
						UnJugador->SetY(PosicionYInicioSalto);
					}

					if (UnJugador->GetEstado() == "SALTANDO-DER") {

						Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);
						UnJugador->SetX(PosicionXInicioSalto + Diferencia);

						if (UnJugador->GetX() >= BordeEnXMaxCamara) {

							AvanzaCamara = true;
							UnJugador->SetEstado("SALTANDO");
							UnJugador->SetX(BordeEnXMaxCamara);
						}
					}
					else {

						if (UnJugador->GetEstado() == "SALTANDO-IZQ") {

							Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);
							UnJugador->SetX(PosicionXInicioSalto - Diferencia);

							if (UnJugador->GetX() <= BordeEnXMinCamara) {

								UnJugador->SetEstado("SALTANDO");
								UnJugador->SetX(BordeEnXMinCamara);
							}
						}
					}
				}

				if ((MinPosX == -1) || (UnJugador->GetX() < MinPosX)) {

					MinPosX = UnJugador->GetX();
				}
			}
		}

		for (int i = 0; i < CantEnemigos; i++) {

			UnJuego->GetEnemigo(i)->mover();
		}

		if (MinPosX <= BordeEnXMinCamara) {

			AvanzaCamara = false;
		}
		
		if (AvanzaCamara) {

			UnJuego->AvanzarCamara();

			for (int i = 0; i < CantJugadores; i++) {

				bool MoverParaAtras = false;
				Jugador *UnJugador = UnJuego->GetJugador(i);

				if (UnJugador->GetEstaConectado()) {
					if (UnJugador->GetEstado() != "CAMINA-DER") {

						MoverParaAtras = true;
					}
					else {

						if (UnJugador->GetX() < BordeEnXMaxCamara) {

							MoverParaAtras = true;
						}
					}

					if (MoverParaAtras) {

						UnJugador->MoverEnX(-VelocidadEnX);
					}
				}
			}
		}

		int diff_ticks = std::clock() - ticks_start;

		if (diff_ticks >= 50) {
			diff_ticks = 50;
		}

		Sleep(50 - diff_ticks);
	}


}

Juego::Juego()
{
	CantJugadores = 0;
	CantCamaras = 0;
	_beginthread(FisicaThread, 0, this);
}

void Juego::establecerModo(std::string nuevoModo)
{
	modoJuego = stoi(nuevoModo);
}

int Juego::obtenerModo()
{
	return modoJuego;
}

void Juego::AvanzarCamara() {

	for (int i = 0; i < CantCamaras; i++) {

		Camaras[i]->X += Camaras[i]->Velocidad;
		
		if (Camaras[i]->X > Camaras[i]->AnchoImagen)
		{
			Camaras[i]->X = 0;
		}
	}
}

int ObtenerVelocidadParaAncho(int UnAncho) {
	return ((UnAncho * 10) / 2600);
}

void Juego::SetAnchoCamara(int NrCamara, int UnAncho) {
	Camaras[NrCamara]->AnchoImagen = UnAncho;
	Camaras[NrCamara]->Velocidad = ObtenerVelocidadParaAncho(UnAncho);
}

Camara* Juego::GetCamara(int NrCamara)
{
	return Camaras[NrCamara];
}

void Juego::AgregarCamara(int UnAncho) {

	Camaras[CantCamaras] = new Camara;
	Camaras[CantCamaras]->X = 0;
	Camaras[CantCamaras]->Y = 0;
	SetAnchoCamara(CantCamaras, UnAncho);

	CantCamaras++;
}

Enemigo* Juego::GetEnemigo(int posicion)
{
	return enemigos[posicion];

}

void Juego::AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad)
{
	Enemigo* unEnemigo = new Enemigo(UnIDSprite, posX, posY, velocidad);
	enemigos[cantidadEnemigos] = unEnemigo;
	cantidadEnemigos++;

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
		//hay un equipo por jugador
		if(modoJuego == 1)
		{
			equipos[CantJugadores] = new Equipo();
			equipos[CantJugadores]->agregarJugador(Jugadores[CantJugadores]);
		}
		//hay un solo equipo para todos los jugadorees
		if (modoJuego == 2)
		{
			if(CantJugadores==0)
			{
				equipos[CantJugadores] = new Equipo();
			}
			equipos[0]->agregarJugador(Jugadores[CantJugadores]);
		}
		//hay 2 equipos de 2 jugadores
		if(modoJuego == 3)
		{
			switch(CantJugadores)
			{
				case 0:
					equipos[0] = new Equipo();
					equipos[0]->agregarJugador(Jugadores[CantJugadores]);
				case 1:
					equipos[0]->agregarJugador(Jugadores[CantJugadores]);
				case 2:
					equipos[1] = new Equipo();
					equipos[1]->agregarJugador(Jugadores[CantJugadores]);
				case 3:
					equipos[1]->agregarJugador(Jugadores[CantJugadores]);
			}
		}
		CantJugadores++;
	}
}

void Juego::RecibirEvento(std::string Usuario, std::string Tipo) {

	int IndiceJugador = GetIndexUsuario(Usuario);

	if ((Tipo == "UP") || (Tipo == "RIGHT") || (Tipo == "DOWN") || (Tipo == "LEFT")) {

		Jugadores[IndiceJugador]->Mover(Tipo);
	}

	if(Tipo == "SOLTO-RIGHT")
	{
		if (Jugadores[IndiceJugador]->EstaCaminando()) {

			Jugadores[IndiceJugador]->SetEstado("QUIETO-DER");
		}
		else {			
			if (Jugadores[IndiceJugador]->EstaSaltando()) {

				Jugadores[IndiceJugador]->SetEstadoAnterior("QUIETO-DER");
			}
		}
	}

	if(Tipo == "SOLTO-LEFT")
	{
		if (Jugadores[IndiceJugador]->EstaCaminando()) {

			Jugadores[IndiceJugador]->SetEstado("QUIETO-IZQ");
		}
		else {			
			if (Jugadores[IndiceJugador]->EstaSaltando()) {

				Jugadores[IndiceJugador]->SetEstadoAnterior("QUIETO-IZQ");
			}
		}
	}

	if (Tipo == "SOLTO-DOWN") {
		if (Jugadores[IndiceJugador]->GetEstado() == "MUERTO") {

			Jugadores[IndiceJugador]->SetEstado("QUIETO-DER");
		}
	}

	if(Tipo == "DISPARA")
	{
		if ((Jugadores[IndiceJugador]->GetEstado() == "QUIETO-DER") || (Jugadores[IndiceJugador]->GetEstado() == "SALTANDO-DER")) {

			Jugadores[IndiceJugador]->SetEstado("DISPARA-DER");
		}
		else
		{
			Jugadores[IndiceJugador]->SetEstado("DISPARA-IZQ");
		}
	}

	if(Tipo == "SOLTO-DISPARA")
	{
		if (Jugadores[IndiceJugador]->GetEstado() == "DISPARA-DER") {

			Jugadores[IndiceJugador]->SetEstado("QUIETO-DER");
		}
		else
		{
			Jugadores[IndiceJugador]->SetEstado("QUIETO-IZQ");
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

int Juego::GetCantEnemigos() {

	return cantidadEnemigos;
}

int Juego::GetCantCamaras() {

	return CantCamaras;
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

void Juego::BorrarCamaras() {

	CantCamaras = 0;
}

Juego::~Juego()
{
}
