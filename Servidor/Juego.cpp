#include "Juego.h"
#include "Globales.h"

int FuncionCuadratica(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

void FisicaThread(void* arg) {

	float ticks_start = 0;
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

		// -----------------------------------------------
		// Proceso jugador (estados y eventos)
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

						UnJugador->SetEstaSaltando(false);
						UnJugador->SetEstadoAnterior("");
						UnJugador->SetY(PosicionYInicioSalto);
					}

					if (!UnJugador->EstaSaltandoVertical()) {
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
				}

				if (UnJugador->EstaDisparando()) {

					if (UnJugador->GetArma()->PuedeDisparar(ticks_start)) {

						UnJuego->MutexearListaProyectiles();

						Proyectil* UnProyectil = UnJugador->GetArma()->Disparar(UnJugador->GetX(),
																				UnJugador->GetY(),
																				ticks_start, 
																				UnJugador->GetDireccion());

						UnJuego->GetProyectiles()->agregar(UnProyectil);

						UnJuego->DesmutexearListaProyectiles();
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
		// -----------------------------------------------
		// Proceso enemigos
		for (int i = 0; i < CantEnemigos; i++) {

			UnJuego->GetEnemigo(i)->mover();

			if (AvanzaCamara) {
				UnJuego->GetEnemigo(i)->mover();
			}
		}
		// -----------------------------------------------
		// Proceso proyectiles (y colisiones de los mismos)
		UnJuego->MutexearListaProyectiles();
		Lista<Proyectil *>* Proyectiles = UnJuego->GetProyectiles();
		int PosicionCursor = 1;
		Proyectiles->iniciarCursor();
		while (Proyectiles->avanzarCursor()) {

			Proyectil* UnProyectil = Proyectiles->obtenerCursor();

			UnProyectil->Mover();

			if (AvanzaCamara) {

				// TODO: Mover proyectil para izquierda a velocidad de camara
			}

			if ((UnProyectil->GetX() > 800) || (UnProyectil->GetX() < 0) ||
				(UnProyectil->GetY() < 0) || (UnProyectil->GetY() > 600)) {

				Proyectiles->remover(PosicionCursor);
			}
			PosicionCursor++;
		}
		UnJuego->DesmutexearListaProyectiles();
		// -----------------------------------------------
		// Proceso camara (y avance/retroceso de todos los sprites)
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

		// -----------------------------------------------
		// Fin procesos
		float diff_ticks = std::clock() - ticks_start;

		if (diff_ticks >= 50) {
			diff_ticks = 50;
		}

		Sleep(50 - diff_ticks);
	}


}

HANDLE MutexListaProyectiles;

Juego::Juego()
{
	MutexListaProyectiles = CreateMutex(NULL, FALSE, NULL);
	CantJugadores = 0;
	CantCamaras = 0;
	Proyectiles = new Lista<Proyectil *>();
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

Lista<Proyectil *>* Juego::GetProyectiles() {
	
	return Proyectiles;
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

void Juego::AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad,int vida, bool esFinal)
{
	Enemigo* unEnemigo = new Enemigo(UnIDSprite, posX, posY, velocidad,vida, esFinal);
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

	if ((Tipo == "SPACE") || (Tipo == "RIGHT") || (Tipo == "DOWN") || (Tipo == "LEFT") || (Tipo == "UP")) {

		Jugadores[IndiceJugador]->Mover(Tipo);
	}

	if(Tipo == "SOLTO-RIGHT")
	{
		if (Jugadores[IndiceJugador]->EstaCaminando()) {

			Jugadores[IndiceJugador]->SetEstado("QUIETO-DER");
		}
		else {			
			if ((Jugadores[IndiceJugador]->EstaSaltando()) || (Jugadores[IndiceJugador]->EstaDisparando())) {

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
			if ((Jugadores[IndiceJugador]->EstaSaltando()) || (Jugadores[IndiceJugador]->EstaDisparando())) {

				Jugadores[IndiceJugador]->SetEstadoAnterior("QUIETO-IZQ");
			}
		}
	}

	if (Tipo == "SOLTO-DOWN") {
		Jugadores[IndiceJugador]->SueltaTeclaDireccion();
	}

	if (Tipo == "SOLTO-UP") {
		Jugadores[IndiceJugador]->SueltaTeclaDireccion();
	}

	if(Tipo == "DISPARA")
	{
		if (!Jugadores[IndiceJugador]->EstaSaltando()) {
			Jugadores[IndiceJugador]->SetEstadoAnterior(Jugadores[IndiceJugador]->GetEstado());
		}

		if (Jugadores[IndiceJugador]->EstaApuntandoALaDerecha()) {

			Jugadores[IndiceJugador]->SetEstado("DISPARA-DER");
		}
		else
		{

			Jugadores[IndiceJugador]->SetEstado("DISPARA-IZQ");
		}
	}

	if(Tipo == "SOLTO-DISPARA")
	{
		if (!Jugadores[IndiceJugador]->EstaSaltando()) {
			Jugadores[IndiceJugador]->SetEstado(Jugadores[IndiceJugador]->GetEstadoAnterior());
		} else {
			Jugadores[IndiceJugador]->SetEstaSaltando(true);
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

void Juego::MutexearListaProyectiles() {
	
	WaitForSingleObject(MutexListaProyectiles, INFINITE);
}

void Juego::DesmutexearListaProyectiles() {

	ReleaseMutex(MutexListaProyectiles);
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
