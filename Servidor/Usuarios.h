#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Lista.h"

// Para ToLower
#include <algorithm>
#include <iostream>

struct Usuario {
	std::string nombre;
	std::string contrasena;
};

class Usuarios
{
	std::ifstream archivoUsuarios;
public:
	Usuarios();
	bool ContrasenaValida(std::string usuario, std::string contrasena);
	 Lista<std::string>* obtenerTodos();
	 std::string obtenerTodosEnString(std::string separador);
	 bool destinatarioValido(std::string destinatario);
	~Usuarios();
private:
	Usuario getProximoUsuario();
	 bool hayUsuarios();
	 bool esFinDeArchivo;
	void resetearCursorArchivo();
};

