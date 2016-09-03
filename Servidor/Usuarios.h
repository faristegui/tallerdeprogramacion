#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Lista.h"
using namespace std;

struct Usuario {
	string nombre;
	string contrasena;
};

class Usuarios
{
	ifstream archivoUsuarios;
public:
	Usuarios();
	bool ContrasenaValida(string usuario, string contrasena);
	 Lista<string>* obtenerTodos();
	 bool destinatarioValido(string destinatario);
	~Usuarios();
private:
	Usuario getProximoUsuario();
	 bool hayUsuarios();
	 bool esFinDeArchivo;
	void resetearCursorArchivo();
};

