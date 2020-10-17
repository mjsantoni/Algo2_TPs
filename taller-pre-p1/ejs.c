#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"

bool _son_iguales(int arr[], int inicio, int fin, bool r, int elem) {
	if (inicio > fin) {
		return r;
	}
	int medio = (inicio + fin) / 2;
	if (elem == arr[medio]) {
		bool prim = _son_iguales(arr,inicio,medio-1,r,elem);
		bool segu = _son_iguales(arr,medio+1,fin,r,elem);
		//if (prim != segu) r = false;
	}else{
		r = false;
	}
	
	return r;
}


bool son_iguales(int arr[], int largo) {
	return _son_iguales(arr,0,largo,true,arr[0]);
}


size_t posicion_pico(int v[], size_t inicio, size_t fin) {
	size_t medio = (inicio + fin) / 2;
	if (v[medio] > v[medio+1] && v[medio] > v[medio-1]) {
		return medio;
	}
	if (v[medio] < v[medio+1]) {
		return posicion_pico(v,medio+1,fin);
	}
	return posicion_pico(v,inicio,medio-1);
}


int maximo_pila(pila_t* pila, int max) {
//max inicial = -INFINITY
	if (pila_esta_vacia(pila)) return max;

	int* num = pila_desapilar(pila);
	if (*num > max) {
		max = maximo_pila(pila,*num);
	}
	pila_apilar(pila,num);
	return max;
}

bool lista_eliminar(lista_t* l, void* elem) {
	bool r = false;
	nodo_lista_t* act = l->prim;
	while (act && act->dato == elem) {
		r = true;
		l->prim = act->prox;
		act = l->prim;
		}
	while (act && act->prox) {
		if (act->prox->dato == elem) {
			r = true;
			act->prox = act->prox->prox;
			continue;
		}
		act = act->prox;
	}
	return r;
}

int lista_borrar_menores(lista_t* lista, int valor) {
	int mayor = -1;
	if (!lista->prim) return mayor;
	nodo_lista_t* act = lista->prim;
	while (act && act->dato < valor) {
		if (mayor < act->dato) mayor = act->dato;
		lista->prim = act->prox;
		act = lista->prim;
	}
	while (act && act->prox) {
		if (act->prox->dato < valor) {
			if (mayor < act->prox->dato) mayor = act->prox->dato;
			act->prox = act->prox->prox;
			continue
		}
		act = act->prox;
	}
	return mayor;

}



void main() {
	int arr[] = {1,2,3,5,4,-2};
	//printf("%d\n",son_iguales(arr,8));
	printf("%d\n",posicion_pico(arr,0,6));
}