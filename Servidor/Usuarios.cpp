#include "Usuarios.h"

Usuarios::Usuarios()
{
	archivoUsuarios.open("Archivos\\Usuarios.csv", std::ifstream::in);
}

bool Usuarios::contrasenaValida(std::string usuario, std::string contrasena) {
	Usuario unUsuario;
	resetearCursorArchivo();

	//TODO: usuario = tolower(usuario);

	while (hayUsuarios()) {
		unUsuario = getProximoUsuario();

		if (unUsuario.nombre == usuario) {
			return (unUsuario.contrasena == contrasena);
		}
	}

	return false;
}

void Usuarios::resetearCursorArchivo() {
	archivoUsuarios.clear();
	archivoUsuarios.seekg(0, std::ifstream::beg);
}

bool Usuarios::hayUsuarios() {
	return archivoUsuarios.good();
}

Usuario Usuarios::getProximoUsuario()
{
	std::vector<std::string>   result;
	std::string                line;
	std::getline(archivoUsuarios, line);

	std::stringstream          lineStream(line);
	std::string                cell;

	while (std::getline(lineStream, cell, ';'))
	{
		result.push_back(cell);
	}

	Usuario unUsuario;
	unUsuario.nombre = result[0];
	unUsuario.contrasena = result[1];

	return unUsuario;
}

Usuarios::~Usuarios()
{
	archivoUsuarios.close();
}