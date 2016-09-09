// MutexTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include <iostream>


using namespace std;
HANDLE mutex; //defino el mutex globalmente para que todos los threads incluyendo al main puedan reconocerlo.

void funcionQuetrataDeLeerElRecursoCompartido(void*)
{
	Sleep(5000);
	std::cout << " thread con id : " << GetCurrentThreadId() << " intenta tomar el recurso para leer." << std::endl;

	WaitForSingleObject(mutex, INFINITE); //aca espera bloqueado hasta que el objeto mutex sea explicitamente liberado cuando otro thread ejecute ReleaseMutex(mutex) 

	std::cout << " thread con id : " << GetCurrentThreadId() << " toma el recurso para leer." << std::endl;
	Sleep(5000);

	ReleaseMutex(mutex);

	std::cout << " thread con id : " << GetCurrentThreadId() << " libera el recurso despues de leerlo." << std::endl;

}

void funcionQuetrataDeEscribirElRecursoCompartido(void*)
{
	Sleep(11000);
	std::cout << " thread con id : " << GetCurrentThreadId() << " intenta tomar el recurso para escribir." << std::endl;

	WaitForSingleObject(mutex, INFINITE); //aca espera hasta que el objeto mutex sea explicitamente liberado cuando otro thread ejecute ReleaseMutex(mutex) 

	std::cout << " thread con id : " << GetCurrentThreadId() << " toma el recurso para escribir." << std::endl;
	

	ReleaseMutex(mutex);

	std::cout << " thread con id : " << GetCurrentThreadId() << " libera el recurso despues de escribirlo." << std::endl;

}

int _tmain(int argc, _TCHAR* argv[])
{

	mutex = CreateMutex(NULL, FALSE, NULL);

	_beginthread(funcionQuetrataDeLeerElRecursoCompartido, 0, NULL);
	_beginthread(funcionQuetrataDeEscribirElRecursoCompartido, 0, NULL);


	std::cout << " thread con id : " << GetCurrentThreadId() << " intenta tomar el recurso para actualizarlo." << std::endl;
	WaitForSingleObject(mutex, INFINITE);//lockeo el recurso compartido

	std::cout << " thread con id : " << GetCurrentThreadId() << " toma el recurso para actualizarlo." << std::endl;
	Sleep(10000);

	ReleaseMutex(mutex);//libera el recurso despues de 10 segundos

	std::cout << " thread con id : " << GetCurrentThreadId() << " libera el recurso despues de actualizarlo." << std::endl;

	Sleep(10000);
	system("pause");
	return 0;
}





