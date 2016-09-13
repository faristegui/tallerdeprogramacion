#ifndef NodoSimplementeEnlazado_H
#define NodoSimplementeEnlazado_H
#include <cstddef>
template <class T>

// Falta hacer el override del operador de igualdad y desigualdad.

class NodoSimplementeEnlazado{

	// Atributos
	private:
		T dato;
		NodoSimplementeEnlazado<T>* siguiente;

	// Metodos
	public:
		NodoSimplementeEnlazado(T nuevoDato);
		T getDato();
		void setDato( T nuevoDato);
		NodoSimplementeEnlazado <T> * getSiguiente();
		void setSiguiente(NodoSimplementeEnlazado <T> * nuevoSiguiente);

};

// Implementacion de metodos -------------------------------------------

template <class T> NodoSimplementeEnlazado<T>::NodoSimplementeEnlazado (T nuevoDato){
	this->dato = nuevoDato;
	this->siguiente = NULL;
}

template <class T> T NodoSimplementeEnlazado<T>::getDato(){
	return this->dato;
}

template <class T> void NodoSimplementeEnlazado<T>::setDato(T nuevoDato){
	this->dato = nuevoDato;
}

template <class T> NodoSimplementeEnlazado <T>* NodoSimplementeEnlazado<T>::getSiguiente(){
	return this->siguiente;
}

template <class T> void NodoSimplementeEnlazado<T>::setSiguiente(NodoSimplementeEnlazado <T>* nuevoSiguiente){
	this-> siguiente = nuevoSiguiente;
}

#endif // NodoSimplementeEnlazado_H
