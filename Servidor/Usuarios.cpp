#include "Usuarios.h"

bool Usuarios::ContrasenaValida(std::string NombreUsuario, std::string Contrasena) {
	
	bool EsValida = false;
	Usuario UnUsuario;
	std::string TmpNombreUsuario;

	AbrirArchivo();

	// Convierte nombre de usuario a LowerCase
	transform(NombreUsuario.begin(), NombreUsuario.end(), NombreUsuario.begin(), (int(*)(int))tolower);

	while (hayUsuarios()) {
		UnUsuario = getProximoUsuario();

		// Convierte nombre de usuario a LowerCase
		TmpNombreUsuario = UnUsuario.nombre;
		transform(TmpNombreUsuario.begin(), TmpNombreUsuario.end(), TmpNombreUsuario.begin(), (int(*)(int))tolower);

		if (TmpNombreUsuario == NombreUsuario) {
			EsValida = (UnUsuario.contrasena == Contrasena);
			break;
		}
	}

	CerrarArchivo();
	return EsValida;
}

Lista<std::string>* Usuarios::obtenerTodos()
{
	Lista<std::string>* todosLosUsuarios = new Lista<std::string>;
	Usuario unUsuario;

	AbrirArchivo();

	while(hayUsuarios())
	{
		unUsuario = getProximoUsuario();
		todosLosUsuarios->agregar(unUsuario.nombre);
	}

	todosLosUsuarios->remover(todosLosUsuarios->getTamanio());
	todosLosUsuarios->iniciarCursor();
	todosLosUsuarios->avanzarCursor();

	CerrarArchivo();
	return todosLosUsuarios;
}

std::string Usuarios::obtenerTodosEnString(std::string separador)
{
	std::string todosLosUsuarios = "";
	Usuario unUsuario;

	AbrirArchivo();

	while (hayUsuarios())
	{
		unUsuario = getProximoUsuario();
		todosLosUsuarios = todosLosUsuarios + unUsuario.nombre;
		if (hayUsuarios()) {
			todosLosUsuarios = todosLosUsuarios + separador;
		}
	}

	CerrarArchivo();
	return todosLosUsuarios;
}

bool Usuarios::destinatarioValido(std::string destinatario)
{
	Usuario unUsuario;
	bool ExisteUsuario = false;
	std::string TmpNombreUsuario;

	// Convierte nombre de usuario a LowerCase
	transform(destinatario.begin(), destinatario.end(), destinatario.begin(), (int(*)(int))tolower);

	AbrirArchivo();

	while(hayUsuarios())
	{
		unUsuario = getProximoUsuario();

		TmpNombreUsuario = unUsuario.nombre;
		// Convierte nombre de usuario a LowerCase
		transform(TmpNombreUsuario.begin(), TmpNombreUsuario.end(), TmpNombreUsuario.begin(), (int(*)(int))tolower);

		if(TmpNombreUsuario == destinatario)
		{
			ExisteUsuario = true;
		}
	}

	CerrarArchivo();
	return ExisteUsuario;
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


void Usuarios::AbrirArchivo()
{
	archivoUsuarios.open("Archivos\\Usuarios.csv", std::ifstream::in);
	esFinDeArchivo = false;
}

void Usuarios::CerrarArchivo()
{
	archivoUsuarios.close();
}