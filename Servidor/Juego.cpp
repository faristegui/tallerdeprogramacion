#include "Juego.h"
#include "Globales.h"

int FuncionCuadratica(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

bool HayColision(int X1, int Y1, int W1, int H1,
	int X2, int Y2, int W2, int H2) {

	int X1Inicio = X1;
	int X1Fin = X1 + W1;
	int Y1Inicio = Y1;
	int Y1Fin = Y1 + H1;

	int X2Inicio = X2;
	int X2Fin = X2 + W2;
	int Y2Inicio = Y2;
	int Y2Fin = Y2 + H2;

	if (X1 <= X2 + W2 &&
		X1 + W1 >= X2 &&
		Y1 <= Y2 + H2 &&
		H1 + Y1 >= Y2) {

		return true;
	}

	return false;

}

Lista<Enemigo*>* Juego::GetEnemigosPantalla()
{
	return enemigosPantalla;
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
						UnJugador->SetY(PosicionYInicioSalto);
					}

					if (!UnJugador->EstaSaltandoVertical()) {
						if (UnJugador->GetEstado() == "SALTANDO-DER") {

							Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);
							UnJugador->SetX(PosicionXInicioSalto + Diferencia);

							if (UnJugador->GetX() >= BordeEnXMaxCamara) {

								AvanzaCamara = true;
								
								UnJugador->SetEstaSaltandoVertical(true);
								UnJugador->SetX(BordeEnXMaxCamara);
							}
						}
						else {

							if (UnJugador->GetEstado() == "SALTANDO-IZQ") {

								Diferencia = FuncionCuadratica(TiempoActual - TiempoInicioSaltoX, 0, 120, 600);
								UnJugador->SetX(PosicionXInicioSalto - Diferencia);

								if (UnJugador->GetX() <= BordeEnXMinCamara) {

									UnJugador->SetEstaSaltandoVertical(true);
									UnJugador->SetX(BordeEnXMinCamara);
								}
							}
						}
					}
				}

				if (UnJugador->EstaDisparando()) {

					if (UnJugador->GetArma()->PuedeDisparar(ticks_start)) {

						UnJuego->MutexearListaProyectiles();

						Proyectil* UnProyectil = UnJugador->GetArma()->Disparar(UnJugador->GetNombre(), UnJugador->GetX(),
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

		int VelocidadCamara = 0;
		if (UnJuego->GetCantCamaras() > 0) {
			VelocidadCamara = UnJuego->GetCamara(UnJuego->GetCantCamaras() - 1)->Velocidad;
		}

		Lista<Enemigo *>* todosLosEnemigos = UnJuego->GetTodosLosEnemigos();
		todosLosEnemigos->iniciarCursor();
		//si avanza el juego o recien empieza cargo los enemigos
		if(AvanzaCamara)
		{
			Lista<int>* posiciones = new Lista<int>();
			int indiceAEliminar = 1;
			while(todosLosEnemigos->avanzarCursor())
			{
				int indice = UnJuego->GetCamara(0)->X;
				if(todosLosEnemigos->obtenerCursor()->getX() <= (800+indice))
				{
					UnJuego->MutexearListaEnemigos();
					Lista<Enemigo*>* enemigosVivos = UnJuego->GetEnemigosPantalla();
					enemigosVivos->agregar(todosLosEnemigos->obtenerCursor());
					UnJuego->DesmutexearListaEnemigos();

					posiciones->agregar(indiceAEliminar);
				}
				indiceAEliminar++;
			}

			posiciones->iniciarCursor();
			int indiceCantEliminados = 0;
			while(posiciones->avanzarCursor())
			{
				todosLosEnemigos->remover(posiciones->obtenerCursor()-indiceCantEliminados);
				indiceCantEliminados++;
			}
		}

		int PosicionCursor = 1;

		int CantEnemigos = todosLosEnemigos->getTamanio();
		Lista<RectanguloEnemigo>* RectangulosEnemigos = new Lista<RectanguloEnemigo>();

		UnJuego->MutexearListaEnemigos();
		Lista<Enemigo*>* enemigosVivos = UnJuego->GetEnemigosPantalla();
		enemigosVivos->iniciarCursor();
		while (enemigosVivos->avanzarCursor()) {

			Enemigo* UnEnemigo = enemigosVivos->obtenerCursor();

			UnEnemigo->mover();

			// TODO: if puededisparar -> dispara

			if (AvanzaCamara) {

				UnEnemigo->MoverEnX(-VelocidadCamara);
			}

			if ((UnEnemigo->getX() > 1000) || (UnEnemigo->getX() < -100) ||
				(UnEnemigo->getY() < -100) || (UnEnemigo->getY() > 700)) {

				//UnJuego->eliminarDeListaPrincipal(PosicionCursor);
				enemigosVivos->remover(PosicionCursor);  // TODO: Ver lo de la lista al de remover (que no reinicie el cursor!)
			} else {
			
				RectanguloEnemigo UnRectangulo;

				UnRectangulo.IndexEnLista = PosicionCursor;
				UnRectangulo.X = UnEnemigo->getX();
				UnRectangulo.Y = UnEnemigo->getY();
				UnRectangulo.Width = UnEnemigo->GetWidth();
				UnRectangulo.Height = UnEnemigo->GetHeight();

				RectangulosEnemigos->agregar(UnRectangulo);
			}

			PosicionCursor++;
		}

		UnJuego->DesmutexearListaEnemigos();
		// -----------------------------------------------
		// Proceso proyectiles (y colisiones de los mismos)
		UnJuego->MutexearListaProyectiles();
		Lista<Proyectil *>* Proyectiles = UnJuego->GetProyectiles();
		PosicionCursor = 1;
		Proyectiles->iniciarCursor();
		while (Proyectiles->avanzarCursor()) {

			Proyectil* UnProyectil = Proyectiles->obtenerCursor();

			UnProyectil->Mover();

			if (AvanzaCamara) {


				UnProyectil->MoverEnX(-VelocidadCamara);
			}

			if ((UnProyectil->GetX() > 900) || (UnProyectil->GetX() < -100) ||
				(UnProyectil->GetY() < -100) || (UnProyectil->GetY() > 700)) {

				Proyectiles->remover(PosicionCursor); // TODO: Ver lo de que el cursor vuelve al inicio
			} else {

				RectangulosEnemigos->iniciarCursor();
				while (RectangulosEnemigos->avanzarCursor()) {

					RectanguloEnemigo UnRectangulo = RectangulosEnemigos->obtenerCursor();
					
					if (HayColision(UnProyectil->GetX(), UnProyectil->GetY(), UnProyectil->GetWidth(),
						UnProyectil->GetHeight(), UnRectangulo.X, UnRectangulo.Y,
						UnRectangulo.Width, UnRectangulo.Height)) {

						UnJuego->MutexearListaEnemigos();
						//aca no habria que eliminar al enemigo, solo cuando se queda sin vida
						//UnJuego->eliminarDeListaPrincipal(UnRectangulo.IndexEnLista);
						UnJuego->GetEnemigosPantalla()->remover(UnRectangulo.IndexEnLista);
						UnJuego->DesmutexearListaEnemigos();

						Proyectiles->remover(PosicionCursor); // TODO: Ver lo de que el cursor vuelve al inicio

						// TODO: Marce aca es donde hay que acumularle los puntos al usuario
						// falta restarle vida al enemigo
						UnJuego->GetJugador(UnProyectil->GetIDJugador())->herirEnemigo();
					}
				}
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
HANDLE MutexListaEnemigos;

Juego::Juego()
{
	MutexListaProyectiles = CreateMutex(NULL, FALSE, NULL);
	MutexListaEnemigos = CreateMutex(NULL, FALSE, NULL);
	CantJugadores = 0;
	CantCamaras = 0;
	Proyectiles = new Lista<Proyectil *>();
	enemigosPantalla = new Lista<Enemigo*>();
	todosLosEnemigos = new Lista<Enemigo*>();
	enemigosCargados = false;
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
void Juego::cargarEnemigos()
{
	enemigosCargados = true;
}
bool Juego::enemigosEstanCargados()
{
	return enemigosCargados;
}

Lista<Enemigo *>* Juego::GetTodosLosEnemigos() {

	return todosLosEnemigos;
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

void Juego::AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad,int vida, bool esFinal,int width, int height)
{
	Enemigo* unEnemigo = new Enemigo(UnIDSprite, posX, posY, velocidad, vida, esFinal, width, height);
	todosLosEnemigos->agregar(unEnemigo);
	//MutexearListaEnemigos();
	//Enemigos->agregar(unEnemigo);
	//DesmutexearListaEnemigos();

}

void Juego::AgregarJugador(std::string UnNombre, std::string UnColor) {

	bool JugadorYaSeHabiaConectado = false;

	for (int i = 0; i < CantJugadores; i++) {
		if (Jugadores[i]->GetNombre() == UnNombre) {

			Jugadores[i]->SetEstaConectado(true);
			JugadorYaSeHabiaConectado = true;
			break;
		}
	}

	if (!JugadorYaSeHabiaConectado) {
		Jugadores[CantJugadores] = new Jugador(UnNombre, UnColor);
		//hay un equipo por jugador
		if(modoJuego == 1)
		{
			equipos[CantJugadores] = new Equipo();
			equipos[CantJugadores]->agregarJugador(Jugadores[CantJugadores]);
		}
		//hay 2 equipos de 2 jugadores
		if(modoJuego == 3)
		{
			switch(CantJugadores)
			{
				case 0:
					equipos[0] = new Equipo();
					equipos[0]->agregarJugador(Jugadores[CantJugadores]);
					break;
				case 1:
					equipos[0]->agregarJugador(Jugadores[CantJugadores]);
					break;
				case 2:
					equipos[1] = new Equipo();
					equipos[1]->agregarJugador(Jugadores[CantJugadores]);
					break;
				case 3:
					equipos[1]->agregarJugador(Jugadores[CantJugadores]);
					break;
			}
		}
		CantJugadores++;
	}
}

Equipo* Juego::GetEquipoJugador(std::string NombreJugador) {

	if (equipos[0]->TieneJugador(NombreJugador)) {

		return equipos[0];
	} else {
		
		return equipos[1];
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


			if (Jugadores[IndiceJugador]->EstaApuntandoALaDerecha()) {
				Jugadores[IndiceJugador]->SetEstado("QUIETO-DER");
			}
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

			if (!Jugadores[IndiceJugador]->EstaApuntandoALaDerecha()) {
				Jugadores[IndiceJugador]->SetEstado("QUIETO-IZQ");
			}
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

		if ((!Jugadores[IndiceJugador]->EstaCaminando()) && (!Jugadores[IndiceJugador]->EstaSaltando()) &&
			(!Jugadores[IndiceJugador]->EstaDisparando())) {
			Jugadores[IndiceJugador]->SetEstado(Jugadores[IndiceJugador]->GetEstado() + "-DISPARA");
		}
		else {
			if (Jugadores[IndiceJugador]->EstaApuntandoALaDerecha()) {
				Jugadores[IndiceJugador]->SetEstado("QUIETO-DER-DISPARA");
			} else {
				Jugadores[IndiceJugador]->SetEstado("QUIETO-IZQ-DISPARA");
			}
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

void Juego::MutexearListaEnemigos() {

	WaitForSingleObject(MutexListaEnemigos, INFINITE);
}

void Juego::DesmutexearListaEnemigos() {

	ReleaseMutex(MutexListaEnemigos);
}

int Juego::GetCantJugadores() {

	return CantJugadores;
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
