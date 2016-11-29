#include "Juego.h"
#include "Globales.h"

void Juego::definirAparicionBonusPower()
{
	numeroEnemigoBonusPower = rand()% todosLosEnemigos->getTamanio();
	//rango entre 1 y 11
	numeroEnemigoBonusPower++;
}

void Juego::definirAparicionBonusKilAll()
{
	//la idea es que los bonus no los dropee el mismos personaje
	numeroEnemigoBonusKillAll = rand()% todosLosEnemigos->getTamanio();
	while(numeroEnemigoBonusKillAll +1 == numeroEnemigoBonusPower)
	{
		numeroEnemigoBonusKillAll = rand()% todosLosEnemigos->getTamanio();
	}
	//rango entre 1 y 11
	numeroEnemigoBonusKillAll++;
}

bool HayColision(int X1, int Y1, int W1, int H1,
	int X2, int Y2, int W2, int H2) {

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

void Juego::sumarEnemigo()
{
	cantEnemigosAparecidos++;
}

int Juego::getNumeroBonusPower()
{
	return numeroEnemigoBonusPower;
}

int Juego::getNumeroBonusKillAll()
{
	return numeroEnemigoBonusKillAll;
}

Bonus* Juego::obtenerBonusPower()
{
	return bonusPower;
}

Bonus* Juego::obtenerBonusKillAll()
{
	return bonusKillAll;
}

void Juego::desaparecerBonusKillAll()
{
	bonusKillAll = 0;
}
Lista<Bonus*>* Juego::getRepuestosArma()
{
	return repuestosArma;
}
void Juego::agregarRepuestoArma(int posX, int posY)
{
	Bonus* unBonus = new Bonus(posX,posY,"b","NC");
	unBonus->mostrar();
	MutexearListaRepuestos();
	repuestosArma->agregar(unBonus);
	DesmutexearListaRepuestos();
	MutexearListaTodosBonus();
	todosLosBonus->agregar(unBonus);
	DesmutexearListaTodosBonus();
}

void FisicaThread(void* arg) {

	float ticks_start = 0;
	int BordeEnXMinCamara = 20;
	int BordeEnXMaxCamara = 700;
	int VelocidadEnX = 10;

	Juego *UnJuego = (Juego*)arg;

	while (true) {
		ticks_start = std::clock();
		int MinPosX = -1;
		bool AvanzaCamara = false;
		int CantJugadores = UnJuego->GetCantJugadores();

		bool hayJugadoresEnLinea = false;

		for (int i = 0; i < CantJugadores; i++) {
			if(UnJuego->GetJugador(i)->GetEstaConectado())
			{
				hayJugadoresEnLinea = true;
				break;
			}
		}

		if (!UnJuego->GetEnemigoFinalMurio() && UnJuego->GetEmpezoElJuego() && hayJugadoresEnLinea) {

		// -----------------------------------------------
		// Proceso jugador (estados y eventos)
		for (int i = 0; i < CantJugadores; i++) {

			Jugador *UnJugador = UnJuego->GetJugador(i);

			if (UnJugador->GetEstaConectado()) {

				if (UnJugador->EstaCaminando()) {

					if (UnJugador->GetEstado() == "CAMINA-DER") {

						if (UnJugador->GetX() > BordeEnXMaxCamara) {

							AvanzaCamara = true;
						}
					}
				}

				if (UnJugador->EstaDisparando()) {

					if (UnJugador->GetArmaEnUso()->PuedeDisparar(ticks_start)) {

						UnJuego->MutexearListaProyectiles();

						Proyectil* UnProyectil = UnJugador->GetArmaEnUso()->Disparar(UnJugador->GetNombre(), UnJugador->GetX(),
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

			// El ultimo fondo determina el ancho total del juego (el fondo de los obstaculos)
			UnJuego->MutexearListaCamaras();
			Camara* UnaCamara = UnJuego->GetCamaraObstaculos();
			UnJuego->DesmutexearListaCamaras();
			VelocidadCamara = UnaCamara->Velocidad;

			if (UnaCamara->X + 800 >= UnaCamara->AnchoImagen) {

				// Aca llega al fin del nivel -> no avanza mas!
				
				Enemigo* EnemigoFinal = UnJuego->GetEnemigoFinal();
				if (EnemigoFinal != NULL) {
					UnJuego->MutexearListaEnemigos();
					Lista<Enemigo*>* enemigosVivos = UnJuego->GetEnemigosPantalla();

					// if direccion = izq
					EnemigoFinal->SetX(850);	// Si camina para la izquierda -> lo hago aparecer desde el borde derecho
					// else if direccion = der
					// Si camina para la der -> lo hago aparecer desde el borde izq
					// UnEnemigo->SetX(-50)

					enemigosVivos->agregar(EnemigoFinal);
					UnJuego->DesmutexearListaEnemigos();
				}

				AvanzaCamara = false;
			}
		}

		Lista<Enemigo *>* todosLosEnemigos = UnJuego->GetTodosLosEnemigos();
		todosLosEnemigos->iniciarCursor();
		//si avanza el juego o recien empieza cargo los enemigos
		if(AvanzaCamara || UnJuego->GetCamaraObstaculos()->X == 1800)
		{
			Lista<int>* posiciones = new Lista<int>();
			int indiceAEliminar = 1;
			while(todosLosEnemigos->avanzarCursor())
			{
				UnJuego->MutexearListaCamaras();
				int indice = UnJuego->GetCamaraObstaculos()->X;
				UnJuego->DesmutexearListaCamaras();
				if(todosLosEnemigos->obtenerCursor()->getX() <= (800+indice))
				{
					UnJuego->MutexearListaEnemigos();
					Lista<Enemigo*>* enemigosVivos = UnJuego->GetEnemigosPantalla();
					Enemigo* UnEnemigo = todosLosEnemigos->obtenerCursor();
					UnEnemigo->getEstado();

					if (UnEnemigo->getX() > 850 || UnEnemigo->getEspecial()) {
						
						if (UnEnemigo->GetDireccionAparicion() == "DER") {
							if (UnEnemigo->getEspecial())
							{
								UnEnemigo->SetX(595);
							}
							else
							{
								UnEnemigo->SetX(850);
							}
						} else {
							UnEnemigo->SetX(-20);
						}
					}
					else {
						if (UnEnemigo->getX() < 0) {
							UnEnemigo->SetX(-20);
						}
					}

					enemigosVivos->agregar(UnEnemigo);
					UnEnemigo->setIndexEnListaOriginal(UnJuego->obtenerCantEnemigosAparecidos());
					UnJuego->sumarEnemigo();
					UnJuego->DesmutexearListaEnemigos();

					posiciones->agregar(indiceAEliminar);
				}
				indiceAEliminar++;
			}

			posiciones->iniciarCursor();
			int indiceCantEliminados = 0;
			//podria ir un mutex
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
			if (UnEnemigo->EstaDisparando()) {

				if (UnEnemigo->getArmaEnUso()->PuedeDisparar(ticks_start)) {

					UnJuego->MutexearListaProyectiles();

					Proyectil* UnProyectil = UnEnemigo->getArmaEnUso()->Disparar(UnEnemigo->getNombre(), UnEnemigo->getX(),
						UnEnemigo->getY(),
						ticks_start,
						UnEnemigo->getDireccion());

					UnJuego->GetProyectiles()->agregar(UnProyectil);

					UnJuego->DesmutexearListaProyectiles();
				}
			}

			/****************************************************************************************************/

			if (AvanzaCamara ) {

				UnEnemigo->MoverEnX(-VelocidadCamara);
			}

			if ((UnEnemigo->getX() > 1000) || (UnEnemigo->getX() < -100) ||
				(UnEnemigo->getY() < -100) || (UnEnemigo->getY() > 700) ||  UnEnemigo->estaListoParaMorir()) {

				//UnJuego->eliminarDeListaPrincipal(PosicionCursor);
				enemigosVivos->remover(PosicionCursor);  // TODO: Ver lo de la lista al de remover (que no reinicie el cursor!)
			} else {
			
				RectanguloEnemigo UnRectangulo;

				UnRectangulo.IndexEnLista = PosicionCursor;
				UnRectangulo.X = UnEnemigo->getX();
				UnRectangulo.Y = UnEnemigo->getY();
				UnRectangulo.Width = UnEnemigo->GetWidth();
				UnRectangulo.Height = UnEnemigo->GetHeight();
				UnRectangulo.RefEnemigo = UnEnemigo;
				RectangulosEnemigos->agregar(UnRectangulo);
			}
			//hacer aparecer bonus
			UnJuego->MutexearListaTodosBonus();
			UnJuego->getTodosLosBonus()->iniciarCursor();
			UnJuego->DesmutexearListaTodosBonus();
			PosicionCursor++;
		}

		UnJuego->DesmutexearListaEnemigos();
		UnJuego->MutexearListaTodosBonus();
		while(UnJuego->getTodosLosBonus()->avanzarCursor())
			{

				if(UnJuego->getTodosLosBonus()->obtenerCursor()!= NULL && AvanzaCamara)
				{
					UnJuego->getTodosLosBonus()->obtenerCursor()->moverEnX(-VelocidadCamara);
				}
				/*
				if(UnJuego->getRepuestosArma()->obtenerCursor()->getX() <= 1000+UnJuego->GetCamaraObstaculos()->X)
				{
					if(UnJuego->getRepuestosArma()->obtenerCursor()->getX() > 850)
					{
						UnJuego->getRepuestosArma()->obtenerCursor()->setX(700);
					}
				}
				*/
			}
		UnJuego->DesmutexearListaTodosBonus();
		// -----------------------------------------------
		// Proceso proyectiles (y colisiones de los mismos)
		UnJuego->MutexearListaProyectiles();
		Lista<Proyectil *>* Proyectiles = UnJuego->GetProyectiles();
		PosicionCursor = 1;
		Proyectiles->iniciarCursor();
		int TmpY, TmpX;
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
						UnRectangulo.Width, UnRectangulo.Height)) 
					{

						if (UnProyectil->EsDePersonaje()) //si el proyectil que impacta en enemigo es disparado por un pesonaje y no por otro enemigo entonces hiero al enemigo sino no!!
						{
						
							bool hayObstaculo = UnJuego->HayObstaculo(UnProyectil->GetX(), UnProyectil->GetY(), UnProyectil->GetWidth(), UnProyectil->GetHeight(), TmpY, TmpX);

							if(!hayObstaculo)
							{
			
								UnJuego->MutexearListaEnemigos();

								UnRectangulo.RefEnemigo->SacarVida(UnProyectil->GetDanio());
								if (UnRectangulo.RefEnemigo->GetVida() <= 0) 
								{
								//chequeos por bonus
									if(UnJuego->getNumeroBonusPower() == UnRectangulo.RefEnemigo->getIndexEnListaOriginal())
									{
										Bonus* bonusPower = new Bonus(UnRectangulo.X,UnRectangulo.Y,"p",UnProyectil->GetIDJugador());
										bonusPower->mostrar();
										UnJuego->aparecerBonusPower(bonusPower);
										UnJuego->MutexearListaTodosBonus();
										UnJuego->getTodosLosBonus()->agregar(bonusPower);
										UnJuego->DesmutexearListaTodosBonus();
									}		
									if(UnJuego->getNumeroBonusKillAll() == UnRectangulo.RefEnemigo->getIndexEnListaOriginal())
									{
										Bonus* bonusKillAll = new Bonus(UnRectangulo.X,UnRectangulo.Y,"ka",UnProyectil->GetIDJugador());
										bonusKillAll->mostrar();
										UnJuego->aparecerBonusKillAll(bonusKillAll);
										UnJuego->MutexearListaTodosBonus();
										UnJuego->getTodosLosBonus()->agregar(bonusKillAll);
										UnJuego->DesmutexearListaTodosBonus();
									}

							
									/**/
									UnRectangulo.RefEnemigo->muereEnemigo();

									/*si al tratar de mover al enemigo se encuentra con un estado de muerte, y transcurrio n segundos, entonces internamente
									se setea el atributo listoParaMorir en true*/

									if (UnRectangulo.RefEnemigo->estaListoParaMorir())
									{
										UnJuego->GetEnemigosPantalla()->remover(UnRectangulo.IndexEnLista);
									}
									//eliminar enemigo de pantalla
									/*UnJuego->GetEnemigosPantalla()->remover(UnRectangulo.IndexEnLista);*/

									if (UnRectangulo.RefEnemigo->esEnemigoFinal()) {

										UnJuego->SetEnemigoFinalMurio(true);
										if(UnJuego->GetNivel() == 3) {

											UnJuego->SetFinJuego(true);
										}
									}
								}
								UnJuego->DesmutexearListaEnemigos();

								Proyectiles->remover(PosicionCursor); // TODO: Ver lo de que el cursor vuelve al inicio

								UnJuego->GetJugador(UnProyectil->GetIDJugador())->herirEnemigo();
							}
						}
					}
					//muchachos el width y el height de la imagen del bonus la hardcodeo, de ultima si hacemos a tiempo lo cambiamos.
				}

				if (UnJuego->HayObstaculo(UnProyectil->GetX(), UnProyectil->GetY(), UnProyectil->GetWidth(), UnProyectil->GetHeight(), TmpY, TmpX)) {
					Proyectiles->remover(PosicionCursor);

					if (UnProyectil->GetIDSprite() == "Bala-SE1")
					{
						int velCaminata = (rand()% 5) + 4; //Random entre 4 y 8
						Enemigo* unEnemigo = new Enemigo("HumanoEnemigo", UnProyectil->GetX(), 390, velCaminata, 150, false, 38, 49, "IZQ");
						unEnemigo->setEspecial(true);
						UnJuego->GetTodosLosEnemigos()->agregar(unEnemigo);
					}
				}
				/*avanzo sobre los rectangulos de los personajes y chequeo sus colisiones contra el proyectil*/
				
				UnJuego->getRectangulosPersonajes()->iniciarCursor();
				while (UnJuego->getRectangulosPersonajes()->avanzarCursor()) {

					RectanguloPersonaje UnRectanguloPersonaje = UnJuego->getRectangulosPersonajes()->obtenerCursor();

					if (HayColision(UnProyectil->GetX(), UnProyectil->GetY(), UnProyectil->GetWidth(),
						UnProyectil->GetHeight(), UnRectanguloPersonaje.RefJugador->GetX() + 8, UnRectanguloPersonaje.RefJugador->GetY() + 8,
						UnRectanguloPersonaje.RefJugador->GetWidth() - 15, UnRectanguloPersonaje.RefJugador->GetHeight() - 15))
					{

  						if (!UnProyectil->EsDePersonaje()) //si el proyectil que impacta sobre el  personaje es disparado por un enemigo y no por otro personaje entonces hiero al personaje!!
						{
							UnJuego->MutexearListaJugadores();

							UnRectanguloPersonaje.RefJugador->SacarVida(UnProyectil->GetDanio());
							if (UnRectanguloPersonaje.RefJugador->GetVida() <= 0)
							{

								UnRectanguloPersonaje.RefJugador->muereJugador();
								int indiceJugador = UnRectanguloPersonaje.IndexEnLista;

								UnJuego->getRectangulosPersonajes()->remover(indiceJugador);
								UnJuego->removerJugador(indiceJugador-1);

								

							}



							UnJuego->DesmutexearListaJugadores();

							Proyectiles->remover(PosicionCursor); // TODO: Ver lo de que el cursor vuelve al inicio


						}

					}
				}
			}
			PosicionCursor++;
		}
		UnJuego->DesmutexearListaProyectiles();
		
	int cantRepuestosEliminados = 0;
		//chequeo si algun jugador tomo algun bonus o repuesto de arma
		for(int i =0; i < UnJuego->GetCantJugadores();i++)
		{
			if(UnJuego->obtenerBonusPower() != NULL 
				&& HayColision(UnJuego->obtenerBonusPower()->getX(),UnJuego->obtenerBonusPower()->getY(),25,28,UnJuego->GetJugador(i)->GetX(),UnJuego->GetJugador(i)->GetY(),44,61))
			{
				UnJuego->obtenerBonusPower()->encontrar(UnJuego);
				UnJuego->desaparecerBonusPower();
			}
			if(UnJuego->obtenerBonusKillAll() != NULL 
				&& HayColision(UnJuego->obtenerBonusKillAll()->getX(),UnJuego->obtenerBonusKillAll()->getY(),25,28,UnJuego->GetJugador(i)->GetX(),UnJuego->GetJugador(i)->GetY(),44,61))
			{
				UnJuego->obtenerBonusKillAll()->encontrar(UnJuego);
				UnJuego->desaparecerBonusKillAll();
			}
			UnJuego->MutexearListaRepuestos();
			UnJuego->getRepuestosArma()->iniciarCursor();
			while(UnJuego->getRepuestosArma()->avanzarCursor())
			{
				Bonus* unRepuestoArma = UnJuego->getRepuestosArma()->obtenerCursor();
				if(HayColision(unRepuestoArma->getX(),unRepuestoArma->getY(),25,28,UnJuego->GetJugador(i)->GetX(),UnJuego->GetJugador(i)->GetY(),44,61))
				{
					UnJuego->GetJugador(i)->sumarBalas();
					unRepuestoArma->marcarParaEliminar(unRepuestoArma);
					cantRepuestosEliminados++;

				}
			}
			UnJuego->DesmutexearListaRepuestos();
		}
		//eliminar repuestos marcados
		int pos;
		for(int j = 0;j < cantRepuestosEliminados;j++)
		{
			pos = 1;
			UnJuego->MutexearListaRepuestos();
			UnJuego->getRepuestosArma()->iniciarCursor();
			while(UnJuego->getRepuestosArma()->avanzarCursor())
			{
				if(UnJuego->getRepuestosArma()->obtenerCursor()->getDireccion() == UnJuego->getRepuestosArma()->obtenerCursor())
				{
					UnJuego->getRepuestosArma()->remover(pos);
					break;
				}
				pos++;
			}
			UnJuego->DesmutexearListaRepuestos();
		}
		
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

						UnJugador->MoverEnX(-VelocidadCamara);
					}
				}
			}
		}
		for (int i = 0; i < CantJugadores; i++) {

			Jugador *UnJugador = UnJuego->GetJugador(i);

			if (UnJugador->GetEstaConectado()) {

				int XAnterior = UnJugador->GetX();
				int YAnterior = UnJugador->GetY();

				UnJugador->UpdatePos();

				int YDelObstaculo;
				int HDelObstaculo;

				if (UnJuego->HayObstaculo(UnJugador->GetX(), UnJugador->GetY(), UnJugador->GetWidth(), UnJugador->GetHeight(), YDelObstaculo, HDelObstaculo)) {

					if (UnJugador->EstaSaltando()) {
						if ((UnJugador->EstaCayendo())) {

							if (UnJugador->GetY() + UnJugador->GetHeight() >= YDelObstaculo) {

								UnJugador->SetEstaEnPiso(true);
								UnJugador->SetEstaSaltando(false);
								UnJugador->SetVelocidadY(0);
								UnJugador->SetY(YDelObstaculo - UnJugador->GetHeight());
							}
						} else {

							UnJugador->SetVelocidadY(0);
							UnJugador->SetY(YAnterior);
							UnJugador->SetVelocidadX(0);
							UnJugador->SetX(XAnterior);
						}
					} else {
						if (UnJugador->EstaCaminando()) {
							if (UnJugador->EstaApuntandoALaDerecha()) {
								UnJugador->SetVelocidadX(VelocidadEnX);
							} else {
								UnJugador->SetVelocidadX(-VelocidadEnX);
							}
						}
					}
				} else {

					UnJugador->SetEstaEnPiso(false);
					UnJugador->SetEstaSaltando(true);
				}
			}
		}

		// -----------------------------------------------
		// Fin procesos

		}
		float diff_ticks = std::clock() - ticks_start;

		if (diff_ticks >= 50) {
			diff_ticks = 50;
		}

		Sleep(50 - diff_ticks);
	}
	delete UnJuego->getRectangulosPersonajes();
}

void Juego::desaparecerBonusPower()
{
	bonusPower = 0;
}

int Juego::GetPisoY() {

	return PisoY;
}

void Juego::aparecerBonusPower(Bonus* unBonus)
{
	bonusPower = unBonus;
}

void Juego::aparecerBonusKillAll(Bonus* unBonus)
{
	bonusKillAll = unBonus;
}

bool Juego::hayBonus()
{
	return bonus;
}

Lista<Bonus*>* Juego::getTodosLosBonus()
{
	return todosLosBonus;
}

HANDLE MutexListaProyectiles;
HANDLE MutexListaEnemigos;
HANDLE MutexListaRepuestos;
HANDLE MutexListaJugadores;
HANDLE MutexListaTodosBonus;
HANDLE MutexListaCamaras;
Juego::Juego()
{
	MutexListaProyectiles = CreateMutex(NULL, FALSE, NULL);
	MutexListaEnemigos = CreateMutex(NULL, FALSE, NULL);
	MutexListaRepuestos = CreateMutex(NULL,FALSE,NULL);
	CantJugadores = 0;
	CantCamaras = 0;
	NumeroNivel = 1;
	YaSeAgregoEnemigoFinal = false;
	Proyectiles = new Lista<Proyectil *>();
	enemigosPantalla = new Lista<Enemigo*>();
	todosLosEnemigos = new Lista<Enemigo*>();
	enemigosCargados = false;
	cantEnemigosAparecidos = 1;
	bonusPower = 0;
	bonusKillAll = 0;
	bonus = false;
	_beginthread(FisicaThread, 0, this);
	PisoY = 486;
	ListaPlataformas = new Lista<Rectangulo *>();
	repuestosArma = new Lista<Bonus*>();
	todosLosBonus = new Lista<Bonus*>();
	RectangulosPersonajes = new Lista<RectanguloPersonaje>();
	EmpezoElJuego = false;
	FinJuego = false;
}

void Juego::EmpezarElJuego() {
	EmpezoElJuego = true;
}


void Juego::AgregarPlataforma(int x, int y, int w, int h) {

	Rectangulo* UnRectangulo = new Rectangulo();
	UnRectangulo->x = x;
	UnRectangulo->y = y;
	UnRectangulo->w = w;
	UnRectangulo->h = h;

	ListaPlataformas->agregar(UnRectangulo);
}

int Juego::obtenerCantEnemigosAparecidos()
{
	return cantEnemigosAparecidos;
}

void Juego::bonusYaMostrado()
{
	bonus = false;
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

int Juego::GetNivel() {

	return NumeroNivel;
}

bool Juego::GetFinJuego() {

	return FinJuego;
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

void Juego::SetFinJuego(bool UnBool) {

	FinJuego = UnBool;
}

Enemigo* Juego::GetEnemigoFinal() {
	
	if (YaSeAgregoEnemigoFinal) {

		return NULL;
	}
	else {

		YaSeAgregoEnemigoFinal = true;
		return EnemigoFinal;
	}
	
}

bool Juego::GetEmpezoElJuego() {

	return EmpezoElJuego;
}

void Juego::BorrarEnemigos() {
	delete todosLosEnemigos;
	todosLosEnemigos = new Lista<Enemigo *>();
	delete enemigosPantalla;
	enemigosPantalla = new Lista<Enemigo *>();
}

void Juego::BorrarPlataformas() {
	delete ListaPlataformas;
	ListaPlataformas = new Lista<Rectangulo *>();
}

void Juego::AgregarEnemigo(std::string UnIDSprite, int posX, int posY, int velocidad,int vida, bool esFinal,int width, int height,
						   std::string UnaDireccion)
{
	Enemigo* unEnemigo = new Enemigo(UnIDSprite, posX, posY, velocidad, vida, esFinal, width, height, UnaDireccion);
	if (!esFinal) {
		todosLosEnemigos->agregar(unEnemigo);
	}
	else {
		EnemigoFinal = new Enemigo(UnIDSprite, posX, posY, velocidad, vida, esFinal, width, height, UnaDireccion);
	}
}

Camara* Juego::GetCamaraObstaculos() {

	if (CantCamaras > 0) {

		return Camaras[CantCamaras - 1];
	} else {
	
		std::cout << "CHEQUEAR PRIMERO Q NO ESTE VACIO EL ARRAY DE CAMARAS";
		return NULL;
	}
}

bool Juego::HayObstaculo(int X, int Y, int W, int H, int &YDelObstaculo, int &HDelObstaculo) {

	YDelObstaculo = 0;
	HDelObstaculo = 0;

	if (Y + H >= PisoY) {

		YDelObstaculo = PisoY;
		return true;
	}

	ListaPlataformas->iniciarCursor();
	MutexearListaCamaras();
	Camara* CamaraObstaculos = GetCamaraObstaculos();
	DesmutexearListaCamaras();
	while (ListaPlataformas->avanzarCursor()) {

		Rectangulo* UnRectangulo = ListaPlataformas->obtenerCursor();

		if (HayColision(X, Y, W, H, UnRectangulo->x - CamaraObstaculos->X, UnRectangulo->y, UnRectangulo->w, UnRectangulo->h)) {

			YDelObstaculo = UnRectangulo->y;
			HDelObstaculo = UnRectangulo->h;

			return true;
		}
	}

	return  false;
}

void Juego::SetListaDatosSprites(Lista<DatosSprites *>* UnaListaSprites) {

	ListaSprites = UnaListaSprites;
}

DatosSprites* Juego::BuscarSpriteEnLista(std::string tipo) {
	DatosSprites* UnSprite;
	ListaSprites->iniciarCursor();

	// TODO: Mutexear?

	while (ListaSprites->avanzarCursor()) {

		UnSprite = ListaSprites->obtenerCursor();

		if (UnSprite->id == tipo) {

			return UnSprite;
		}
	}

	std::cout << "error: mutexear esta lista";
	return NULL;
}

void Juego::AgregarJugador(std::string UnNombre, std::string UnColor, bool EsDios) {

	bool JugadorYaSeHabiaConectado = false;

	for (int i = 0; i < CantJugadores; i++) {
		if (Jugadores[i]->GetNombre() == UnNombre) {

			Jugadores[i]->SetEstaConectado(true);
			JugadorYaSeHabiaConectado = true;
			break;
		}
	}

	if (!JugadorYaSeHabiaConectado) {


		// En nuestro caso, todos los sprites de cada arma tienen mismo ancho y alto (y los estados tambien)
		// Si esto fuera a cambiar entonces la siguiente linea no serviria para nada
		// Se podria hacer una lsita de sprites correspondientes a un color en particular y enviarsela como parametro al constructor de Jugador
		// (que tenga todas las armas y estados)

		//DatosSprites* DatosSprite = BuscarSpriteEnLista("Player" + UnColor + "-S");
		//int Width = DatosSprite->width;
		//int Height = DatosSprite->height;


		// TODO: fix hardcodeada
		int Width = 68;
		int Height = 122;

		Jugador* UnJugador = new Jugador(UnNombre, UnColor, Width, Height, EsDios);
		Jugadores[CantJugadores] = UnJugador;

		RectanguloPersonaje unRectanguloPersonaje;

		unRectanguloPersonaje.IndexEnLista = CantJugadores+1;
		unRectanguloPersonaje.X = UnJugador->GetX();
		unRectanguloPersonaje.Y = UnJugador->GetY();
		unRectanguloPersonaje.Width = UnJugador->GetWidth();
		unRectanguloPersonaje.Height = UnJugador->GetHeight();
		unRectanguloPersonaje.RefJugador = UnJugador;
		this->RectangulosPersonajes->agregar(unRectanguloPersonaje);
		
		
		
		
		
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


Equipo* Juego::GetEquipo(int NumeroEquipo) {

	return equipos[NumeroEquipo];
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

		if (Tipo == "RIGHT") {
			Jugadores[IndiceJugador]->SetVelocidadX(10);
		}

		if (Tipo == "LEFT") {
			Jugadores[IndiceJugador]->SetVelocidadX(-10);
		}
	}

	if (Tipo == "ARMA-S") {
		Jugadores[IndiceJugador]->ArmaSiguiente();
	}

	if (Tipo == "RECARGA") {

		if (EnemigoFinalMurio) {
			for (int i = 0; i < CantJugadores; i++) {
				Jugadores[i]->SetX(20);
				Jugadores[i]->SetY(365);
				Jugadores[i]->SetEstado("QUIETO-DER");
				Jugadores[i]->SetVelocidadX(0);
				Jugadores[i]->SetVelocidadY(0);
				delete Proyectiles;
				Proyectiles = new Lista<Proyectil *>();
			}
			NumeroNivel += 1;
		}

		EnemigoFinalMurio = false;
		YaSeAgregoEnemigoFinal = false;
	}

	if (Tipo == "ARMA-A") {
		Jugadores[IndiceJugador]->ArmaAnterior();
	}

	if(Tipo == "SOLTO-RIGHT")
	{

		Jugadores[IndiceJugador]->SetVelocidadX(0);

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

		Jugadores[IndiceJugador]->SetVelocidadX(0);

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

void Juego::SetEnemigoFinalMurio(bool UnMurio) {

	EnemigoFinalMurio = UnMurio;
}

bool Juego::GetEnemigoFinalMurio() {

	return EnemigoFinalMurio;
}

void Juego::MutexearListaProyectiles() {
	
	WaitForSingleObject(MutexListaProyectiles, INFINITE);
}

void Juego::MutexearListaCamaras() {
	
	WaitForSingleObject(MutexListaCamaras, INFINITE);
}
void Juego::DesmutexearListaCamaras() {
	
	ReleaseMutex(MutexListaCamaras);
}

void Juego::MutexearListaRepuestos()
{
	WaitForSingleObject(MutexListaRepuestos, INFINITE);
}

void Juego::DesmutexearListaRepuestos()
{
	ReleaseMutex(MutexListaRepuestos);
}

void Juego::DesmutexearListaProyectiles() {

	ReleaseMutex(MutexListaProyectiles);
}

void Juego::MutexearListaEnemigos() {

	WaitForSingleObject(MutexListaEnemigos, 1200);
}

void Juego::DesmutexearListaEnemigos() {

	ReleaseMutex(MutexListaEnemigos);
}

void Juego::MutexearListaJugadores(){
	
	WaitForSingleObject(MutexListaJugadores, INFINITE);

}

void Juego::DesmutexearListaJugadores() {

	ReleaseMutex(MutexListaJugadores);
}

void Juego::MutexearListaTodosBonus()
{
	WaitForSingleObject(MutexListaTodosBonus,INFINITE);
}

void Juego::DesmutexearListaTodosBonus()
{
	ReleaseMutex(MutexListaTodosBonus);
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

Lista<RectanguloPersonaje>* Juego::getRectangulosPersonajes(){

	return RectangulosPersonajes;
}

void Juego::removerJugador(int indice){

	/*delete Jugadores[indice];*/
	Jugadores[indice]->SetEstaConectado(false);
	/*CantJugadores--;*/

}
Juego::~Juego()
{
}
