#include "Mensaje.h"


Mensaje::Mensaje(std::string emisor, std::string destinatario, std::string contenido)
{
	emisor_ = emisor;
	destinatario_ = destinatario;
	contenido_ = contenido;
}


std::string Mensaje::obtenerContenido()
{
	return contenido_;
}

std::string Mensaje::obtenerEmisor()
{
	return emisor_;
}

std::string Mensaje::obtenerDestinatario()
{
	return destinatario_;
}

Mensaje::~Mensaje(void)
{
}
