#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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
	bool contrasenaValida(string usuario, string contrasena);
	~Usuarios();
private:
	Usuario getProximoUsuario();
	bool hayUsuarios();
	void resetearCursorArchivo();
};
