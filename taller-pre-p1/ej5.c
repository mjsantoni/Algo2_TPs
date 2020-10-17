#include "cola.h"
#include <stdio.h>


//Este es una primitiva
void** cola_multiprimeros(const cola_t* cola,size_t k){
	void** resultado = malloc(k*sizeof(void*));

	if(!resultado){
		return;
	}

	nodo_cola_t* actual = cola->prim;

	for(size_t i =0;i<k;i++){
		if(actual){
			resultado[i] = actual->dato;
			actual = actual->proxi;
			continue
		}
		resultado[i] = NULL;
	}

	return resultado;
}


//Este es una funcion
void** cola_multiprimeros_func(cola_t* cola,size_t k){
	void** resultado = malloc(k*sizeof(void*));

	if(!resultado){
		return NULL;
	}

	cola_t* cola_aux = cola_crear();

	if(!cola_aux){
		free(resultado);
		return NULL; 
	}

	size_t i =0;

	for(size_t i=0;i<k;i++){

		desencolado = cola_desencolar(cola);
		resultado[i] = desencolado;

		if(desencolado){
			cola_encolar(cola_aux,desencolado);
		}
	}

	while(!cola_esta_vacia(cola)){
		cola_encolar(cola_aux,cola_desencolar(cola));
	}

	while(!cola_esta_vacia(cola_aux)){
		cola_encolar(cola,cola_desencolar(cola_aux));
	}

	return resultado;

}