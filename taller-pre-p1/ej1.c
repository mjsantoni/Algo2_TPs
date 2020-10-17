//Ejercicio de composicion de funciones

#include <stdio.h>
#include <stdlib.h>

#include "pila.h"

typedef struct c{
	pila_t* pila;
}comp_t;

comp_t* crear_composicion(){
	comp_t* composicion = malloc(sizeof(comp_t));
	if(!composicion) return NULL;

	composicion->pila = pila_crear();

	if(!composicion->pila){
		free(composicion);
		return NULL;
	}

	return composicion;
}

void composicion_destruir(comp_t* composicion){
	pila_destruir(composicion->pila);
	free(composicion);
}

bool composicion_agregar_funcion(comp_t* composicion, double funcion(double)){
	return pila_apilar(composicion->pila,funcion);
}

double composicion_aplicar(comp_t* composicion, double numero){
	double resultado = numero;

	while(!pila_esta_vacia(composicion->pila)){
		void* funcion = pila_desapilar(composicion->pila);
		resultado = funcion(numero);
	}

	return resultado;
}