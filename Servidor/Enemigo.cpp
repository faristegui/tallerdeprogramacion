#include "Enemigo.h"

#include "ArmaEnemigoHumano.h"
#include "ArmaEnemigoFinal1.h"
#include "ArmaEnemigoFinal2.h"
#include "ArmaEnemigoFinal3.h"

Enemigo::Enemigo(std::string unIdSprite, int posX, int posY, int vel, 
				int unaVida, bool esFinal, int UnWidth, int UnHeight,
				std::string UnaDireccion)
{
	IDSprite = unIdSprite;
	x = posX;
	y = posY;
	vida = unaVida;
	velocidad = vel;
	enemigoFinal = esFinal;
	Width = UnWidth;
	Height = UnHeight;
	time_t t = 0;
	time_t* pTiempoDeVida = &t;
	tiempoDeVida = (time(pTiempoDeVida));
	listoParaMorir = false; /*me indica si en el proximo tick ya puedo eliminar al enemigo dandome tiempo para mostrar el sprite de muerte*/
	Nombre = unIdSprite;
	this->estaDisparando = false;

	DeterminarArma();

	if (UnaDireccion == "IZQ") {

		DireccionAparicion = "DER";
		estado = "CAMINA-IZQ";
		Direccion = "IZQUIERDA";
	}
	else {

		DireccionAparicion = "IZQ";
		estado = "CAMINA-DER";
		Direccion = "DERECHA";
	}
}

void Enemigo::DeterminarArma() {

	if (Nombre == "HumanoEnemigo") {

		arma = new ArmaEnemigoHumano();
	}

	if (Nombre == "OvniEnemigo") {

		arma = new ArmaEnemigoHumano();
	}

	if (Nombre == "EnemigoFinal1") {

		arma = new ArmaEnemigoFinal1();
	}
	if (Nombre == "EnemigoFinal2") {

		arma = new ArmaEnemigoFinal2();
	}
	if (Nombre == "EnemigoFinal3") {

		arma = new ArmaEnemigoFinal3();
	}
}

void Enemigo::SacarVida(int Cantidad) {

	if (Nombre == "HumanoEnemigo") {
		if (Direccion == "IZQUIERDA") {
			this->estado = "HERIDO-IZQ";
		} else {
			this->estado = "HERIDO-DER";
		}
	}

	if (Nombre == "OvniEnemigo") {
		this->estado = "HERIDO-ABA";
	}

	this->tiempoDeVida = getTiempoActual();
	tiempoTranscurrido = 0;

	vida -= Cantidad;

}

int Enemigo::GetVida() {
	
	return vida;
}

int Enemigo::getX()
{
	return x;
}

void Enemigo::SetX(int UnX)
{
	x = UnX;
}

void Enemigo::SetY(int UnY)
{
	x = UnY;
}

int Enemigo::GetWidth() {

	return Width;
}

int Enemigo::GetHeight() {

	return Height;
}

std::string Enemigo::getID()
{
	return IDSprite;
}

void Enemigo::MoverEnX(int CantX) {

	x += CantX;
}

void Enemigo::mover()
{
	tiempoTranscurrido = getTiempoActual() - tiempoDeVida;

	// Como no usamos herencia para los enemigos entonces en esta rutina definimos 
	// el comportamiento de cada uno, segun su nombre

	if (Nombre == "EnemigoFinal1") {
		if (x > 480)
		{
			x -= velocidad;
		}
		else
		{
			if ((this->estado == "CAMINA-IZQ") || (this->estado == "HERIDO-IZQ"))
			{
				this->estado = "DISPARA";
				this->estaDisparando = true;
				this->tiempoDeVida = getTiempoActual();
				tiempoTranscurrido = 0;
			}

			if (this->estado == "DISPARA" && tiempoTranscurrido > 0.8)
			{
				this->estado = "QUIETO-IZQ";
				this->estaDisparando = false;
				this->tiempoDeVida = getTiempoActual();
				tiempoTranscurrido = 0;
			}

			if (this->estado == "QUIETO-IZQ" && tiempoTranscurrido > 0.3)
			{
				this->estado = "DISPARA";
				this->estaDisparando = true;
				this->tiempoDeVida = getTiempoActual();
				tiempoTranscurrido = 0;	
			}

			if (((this->estado == "MUERTO-DER") && tiempoTranscurrido > 0.8))
			{
				this->tiempoDeVida = getTiempoActual();
				tiempoTranscurrido = 0;
				this->listoParaMorir = true;
			}
		}
	}
	if (Nombre == "EnemigoFinal2") 
	{
		if(this->estado == "CAMINA-IZQ")
		{
			this->estaDisparando = true;
			x-=velocidad;
			Direccion = "ABAJO-IZQ";
		}
		if (x < 10)
		{
			this->estado = "CAMINA-DER";
			Direccion = "ABAJO-DER";
		}
		if(this->estado=="CAMINA-DER")
		{
			x+=velocidad;
			if(x >= 500)
			{
				this->estaDisparando = true;
				this->estado="CAMINA-IZQ";
				Direccion = "ABAJO-IZQ";
			}
		}

	}
	if (Nombre == "EnemigoFinal3") 
	{
		//this->estaDisparando = true;

		if(this->estado == "CAMINA-IZQ")
		{
			x-=velocidad;
		}
		if (x < 10 && (this->estado != "QUIETO-DER"))
		{
			this->estado = "QUIETO-DER";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}
		
		if ((this->estado == "QUIETO-DER") && tiempoTranscurrido > 10)
		{
			this->estado = "CAMINA-DER";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}

		if ((this->estado == "QUIETO-IZQ") && tiempoTranscurrido > 10)
		{
			this->estado = "CAMINA-IZQ";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}


		if(this->estado=="CAMINA-DER")
		{
			x+=velocidad;
			if(x >= 500)
			{
				this->estado = "QUIETO-IZQ";
				this->tiempoDeVida = getTiempoActual();
				tiempoTranscurrido = 0;
			}
		}

	}

	if (Nombre == "HumanoEnemigo") {

		if (velocidad == 0) {
			if (this->estado == "CAMINA-IZQ") {

				this->estado = "QUIETO-IZQ";
			} else {
				if (this->estado == "CAMINA-DER") {

					this->estado = "QUIETO-DER";
				}
			}
		}

		if ((this->estado == "CAMINA-IZQ" || this->estado == "HERIDO-IZQ") && tiempoTranscurrido > 0.5)
		{
			this->estado = "QUIETO-IZQ";

			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}

		if (this->estado == "HERIDO-DER" && tiempoTranscurrido > 0.5)
		{
			this->estado = "CAMINA-DER";

			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}

		if (this->estado == "QUIETO-IZQ" && tiempoTranscurrido > 0.2)
		{
			this->estado = "QUIETO-IZQ-DISPARA";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
			this->estaDisparando = true;
		}

		if (this->estado == "QUIETO-DER" && tiempoTranscurrido > 0.2)
		{
			this->estado = "CAMINA-DER";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
		}

		if (this->estado == "QUIETO-IZQ-DISPARA" && tiempoTranscurrido > 0.8)
		{
			this->estado = "CAMINA-IZQ";
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
			this->estaDisparando = false;
		}

		if (this->estado == "CAMINA-IZQ")
		{
			x -= velocidad;
		}
		else {
			if (this->estado == "CAMINA-DER") {
				x += velocidad;
			}
		}

		if ((this->estado == "MUERTO-IZQ" || (this->estado == "MUERTO-DER") && tiempoTranscurrido > 0.8))
		{
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
			this->listoParaMorir = true;
		}
	}

	if (Nombre == "OvniEnemigo") {

		this->estaDisparando = true;
		this->estado = "QUIETO-IZQ-DISPARA";

		Direccion = "ABAJO-DER";

		if (DireccionAparicion == "DER") {
			x -= velocidad;
		} else {
			x += velocidad;
		}

		if (((this->estado == "MUERTO-DER" || this->estado == "MUERTO-IZQ") && tiempoTranscurrido > 0.8))
		{
			this->tiempoDeVida = getTiempoActual();
			tiempoTranscurrido = 0;
			this->listoParaMorir = true;
		}
	}
}

bool Enemigo::esEnemigoFinal()
{
	return enemigoFinal;
}

void Enemigo::disparar()
{
	estado = "DISPARA-DER";
}

std::string Enemigo::getEstado()
{
	return estado;
}

int Enemigo::getIndexEnListaOriginal()
{
	return indexEnListaOriginal;
}

void Enemigo::setIndexEnListaOriginal(int indice)
{
	indexEnListaOriginal = indice;
}

int Enemigo::getY()
{
	return y;
}
Enemigo::~Enemigo(void)
{
}

time_t Enemigo::GetTiempoDeVida()
{
	return tiempoDeVida;
}
void Enemigo::setTiempoDeVida(time_t valor)

{
	this->tiempoDeVida = valor;

}

time_t Enemigo::getTiempoActual()

{
	time_t tiempo_actual = 0;
	time_t* pTiempo_Actual = &tiempo_actual;
	
	return time(pTiempo_Actual);



}

void Enemigo::muereEnemigo()

{
	if (this->estado == "CAMINA-IZQ" || this->estado == "QUIETO-IZQ" || this->estado == "QUIETO-IZQ-DISPARA" || this->estado == "HERIDO-IZQ" )
	{
		this->estado = "MUERTO-IZQ";
		this->tiempoDeVida = getTiempoActual();
		tiempoTranscurrido = 0;
	}
	else {
		this->estado = "MUERTO-DER";
		this->tiempoDeVida = getTiempoActual();
		tiempoTranscurrido = 0;
	}
}

void Enemigo::setListoParaMorir(bool valor)

{
	this->listoParaMorir = valor;

}

bool Enemigo::estaListoParaMorir()
{
	return this->listoParaMorir;

}
bool Enemigo::EstaDisparando()
{

	return this->estaDisparando;
}

Arma* Enemigo::getArmaEnUso()
{

	return arma;

}
std::string  Enemigo::getNombre()
{

	return this->Nombre;
}

std::string Enemigo::GetDireccionAparicion() {

	return DireccionAparicion;
}

std::string Enemigo::getDireccion()
{
	return this->Direccion;
}