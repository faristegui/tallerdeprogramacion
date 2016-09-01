#include <string>
#pragma once
using namespace std;
class Mensaje
{
public:
	Mensaje(string emisor, string destinatario, string contenido);
	string obtenerContenido();
	~Mensaje(void);
private:
	string emisor_;
	string destinatario_;
	string contenido_;
};

