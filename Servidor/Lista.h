#include "NodoSimplementeEnlazado.h"
#include <cstddef>
#include <iostream>
#ifndef _LISTA_H
#define _LISTA_H
using namespace std;

template <class T>
class Lista{

	// Atributos
	private:
		unsigned int tamanio;
		NodoSimplementeEnlazado<T>* primerElemento;
		NodoSimplementeEnlazado<T>* cursor;
	// Metodos
	public:
        	Lista();
            ~Lista();
        	unsigned int getTamanio();
        	bool estaVacia();

        	void agregar(T elemento);
        	void remover(unsigned int posicion);

        	void iniciarCursor();
        	bool avanzarCursor();

        	T obtenerCursor();
        	T obtenerUltimo();

	private:
            NodoSimplementeEnlazado<T>* obtenerNodo(unsigned int posicion);

};

// Implementacion de metodos

template <class T> Lista<T>::Lista(){
    this->primerElemento = NULL;
    this->tamanio = 0;
    this->cursor = NULL;
}

template <class T> unsigned int Lista<T>::getTamanio(){

	return ( this-> tamanio );

}

template <class T> bool Lista<T>::estaVacia(){

	return ( this-> tamanio == 0 );

}
//Agrega elemento, el contenido esta ordenado de menor a mayor.
template <class T> void Lista<T>::agregar(T elemento){

    NodoSimplementeEnlazado<T>* nuevoElemento = new NodoSimplementeEnlazado<T>(elemento);
   tamanio++;
    if(tamanio > 1){
		
        obtenerNodo(tamanio-1)->setSiguiente(nuevoElemento);
        
    } else {
        primerElemento = nuevoElemento;
    }


}


template<class T> Lista<T>::~Lista() {
	
    while (this -> tamanio != 0) {
		
        NodoSimplementeEnlazado<T>* aBorrar = this -> primerElemento;
        this -> primerElemento = this -> primerElemento -> getSiguiente();

        delete aBorrar;
        this -> tamanio--;
        
    }
}

template <class T> void Lista<T>::remover(unsigned int posicion){

      if ((posicion > 0) && (posicion <= this->tamanio)){

        NodoSimplementeEnlazado<T>* removido;

        if (posicion == 1) {

            removido = this->primerElemento;
            this->primerElemento = removido->getSiguiente();

        } else {

            NodoSimplementeEnlazado<T>* anterior = this->obtenerNodo(posicion - 1);
            removido = anterior->getSiguiente();
            anterior->setSiguiente(removido->getSiguiente());
        }

        delete removido;
        this->tamanio--;

        /* cualquier recorrido actual queda invalidado */
        this->iniciarCursor();
    }

}
//ojo que al iniciar el cursor el puntero no se posiciona en el primer elemento
//hay que avanzar y ahi si estas posicionado en el primero.
template <class T> void Lista<T>::iniciarCursor(){

	cursor = 0;

}

// Cambia el cursor al elemento siguiente.
// Devuelve verdadero si el contador de elementos no supero el tamanio.
// Lo ultimo es para no preguntar por nulo.
template <class T> bool Lista<T>::avanzarCursor(){
	  if (this->cursor == 0) {
        this->cursor = this->primerElemento;

    } else {
        this->cursor = this->cursor->getSiguiente();
    }
    /* pudo avanzar si el cursor ahora apunta a un nodo */
    return (this->cursor != NULL);
}

template <class T> T Lista<T>::obtenerCursor(){

    T elemento;

    if (this->cursor != NULL) {

        elemento = this->cursor->getDato();
    }

    return elemento;

}

// 1<=posicion<=tamanio
template <class T> NodoSimplementeEnlazado<T>* Lista<T>::obtenerNodo(unsigned int posicion){

    NodoSimplementeEnlazado<T>* actual = this-> primerElemento;
    for (unsigned int i = 1; i < posicion; i++) {

        actual = actual->getSiguiente();
    }

    return actual;
}

template <class T> T Lista<T>::obtenerUltimo(){

    return obtenerNodo(tamanio)->getDato();

}

#endif // LISTA_H

