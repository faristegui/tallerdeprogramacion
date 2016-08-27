#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct Usuario {
	std::string nombre;
	std::string contrasena;
};

class Usuarios
{
	std::ifstream archivoUsuarios;
public:
	Usuarios();
	bool contrasenaValida(std::string usuario, std::string contrasena);
	~Usuarios();
private:
	Usuario getProximoUsuario();
	bool hayUsuarios();
	void resetearCursorArchivo();
};

