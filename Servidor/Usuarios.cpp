#include "Usuarios.h"

Usuarios::Usuarios()
{
	archivoUsuarios.open("Archivos\\Usuarios.csv", std::ifstream::in);
	esFinDeArchivo = false;
}

bool Usuarios::ContrasenaValida(std::string NombreUsuario, std::string Contrasena) {
	
	Usuario UnUsuario;
	std::string TmpNombreUsuario;
	resetearCursorArchivo();

	// Convierte nombre de usuario a LowerCase
	transform(NombreUsuario.begin(), NombreUsuario.end(), NombreUsuario.begin(), (int(*)(int))tolower);

	while (hayUsuarios()) {
		UnUsuario = getProximoUsuario();

		// Convierte nombre de usuario a LowerCase
		TmpNombreUsuario = UnUsuario.nombre;
		transform(TmpNombreUsuario.begin(), TmpNombreUsuario.end(), TmpNombreUsuario.begin(), (int(*)(int))tolower);

		if (TmpNombreUsuario == NombreUsuario) {
			return (UnUsuario.contrasena == Contrasena);
		}
	}

	return false;
}

Lista<std::string>* Usuarios::obtenerTodos()
{
	Lista<std::string>* todosLosUsuarios = new Lista<std::string>;
	Usuario unUsuario;
	resetearCursorArchivo();
	while(hayUsuarios())
	{
		unUsuario = getProximoUsuario();
		todosLosUsuarios->agregar(unUsuario.nombre);
	}
	todosLosUsuarios->remover(todosLosUsuarios->getTamanio());
	todosLosUsuarios->iniciarCursor();
	todosLosUsuarios->avanzarCursor();
	return todosLosUsuarios;
}

std::string Usuarios::obtenerTodosEnString(std::string separador)
{
	std::string todosLosUsuarios = "";
	Usuario unUsuario;
	resetearCursorArchivo();
	while (hayUsuarios())
	{
		unUsuario = getProximoUsuario();
		todosLosUsuarios = todosLosUsuarios + unUsuario.nombre;
		if (hayUsuarios()) {
			todosLosUsuarios = todosLosUsuarios + separador;
		}
	}

	return todosLosUsuarios;
}

bool Usuarios::destinatarioValido(std::string destinatario)
{
	Usuario unUsuario;
	resetearCursorArchivo();
	while(hayUsuarios()) // hay un problema con esta funcion, no funciona bien cuando se terminan los usuario
	{
		unUsuario = getProximoUsuario();
		if(unUsuario.nombre == destinatario)
		{
			return true;
		}
	}
	return false;
}

void Usuarios::resetearCursorArchivo() {
	archivoUsuarios.clear();
	archivoUsuarios.seekg(0, std::ifstream::beg);
	esFinDeArchivo = false;
}

bool Usuarios::hayUsuarios() {

	return !esFinDeArchivo;
}

Usuario Usuarios::getProximoUsuario()
{
	Usuario unUsuario;
	std::vector<std::string>   result;
	std::string                line;
	std::getline(archivoUsuarios, line);
	
	std::stringstream          lineStream(line);
	std::string                cell;
	if(line== "")
	{
		esFinDeArchivo = true;
		unUsuario.nombre = "";
		unUsuario.contrasena = "";
	}
	else
	{
		while (std::getline(lineStream, cell, ';'))
		{
			result.push_back(cell);
		}

		unUsuario.nombre = result[0];
		unUsuario.contrasena = result[1];
	}
	return unUsuario;
}

Usuarios::~Usuarios()
{
	archivoUsuarios.close();
}