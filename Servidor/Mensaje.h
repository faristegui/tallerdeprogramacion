#include <string>
#pragma once
//using namespace std;
class Mensaje
{
public:
	Mensaje(std::string emisor, std::string destinatario, std::string contenido);
	std::string obtenerContenido();
	std::string obtenerEmisor();
	std::string obtenerDestinatario();
	~Mensaje(void);
private:
	std::string emisor_;
	std::string destinatario_;
	std::string contenido_;
};

