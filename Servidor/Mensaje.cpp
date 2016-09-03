#include "Mensaje.h"


Mensaje::Mensaje(string emisor, string destinatario, string contenido)
{
	emisor_ = emisor;
	destinatario_ = destinatario;
	contenido_ = contenido;
}


string Mensaje::obtenerContenido()
{
	return contenido_;
}

string Mensaje::obtenerEmisor()
{
	return emisor_;
}

Mensaje::~Mensaje(void)
{
}
