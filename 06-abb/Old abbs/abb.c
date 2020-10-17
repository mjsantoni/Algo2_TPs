#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "abb.h"
#include "pila.h"

/////////////////////////////////////////////////////////////////////
/////////////////////////////STRUCTS/////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef struct abb_nodo {
	char* clave;
	struct abb_nodo* izq;
	struct abb_nodo* der;
	void* dato;
} abb_nodo_t;

struct abb {
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter {
	pila_t* pila;
	abb_nodo_t* nodo;
};

/////////////////////////////////////////////////////////////////////
//////////////////////FUNCIONES AUXILIARES///////////////////////////
/////////////////////////////////////////////////////////////////////

abb_nodo_t* crear_nodo_abb(const char* clave,void* dato) {
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if(!nodo) return NULL;
	char* clave_actual = strdup(clave);
	nodo->clave = clave_actual;
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

/////////////////////////////////////////////////////////////////////
////////////////////////FUNCION DE BUSQUEDA//////////////////////////
/////////////////////////////////////////////////////////////////////

/* 

*/

abb_nodo_t* buscar_padre(abb_nodo_t* nodo,const char* clave,const abb_t* abb) {
	if(!nodo) return NULL;
	abb_comparar_clave_t cmp = abb->cmp;
	if(cmp(nodo->clave,clave) == 0) {
		return NULL; //Siempre me pasan la raíz, si coincide no tiene padre.
	}
	if(cmp(nodo->clave,clave) > 0) { //Tiene que estar a la izquierda de este nodo.
		if(nodo->izq) {
			if(cmp(nodo->izq->clave,clave) == 0) return nodo; //Devuelvo el padre del izquierdo, osea el mismo que me pasaron.
			else return buscar_padre(nodo->izq,clave,abb); //Sigo buscando.
		} return nodo;
	}
	//Sino, tiene que estar a la derecha del nodo.
	if(nodo->der) {
		if(cmp(nodo->der->clave,clave) == 0) return nodo; //Devuelvo el padre del derecho, osea el mismo que me pasaron.
		else return buscar_padre(nodo->der,clave,abb); //Sigo buscando.
	} return nodo;
}

/* 

*/
abb_nodo_t* buscar_nodo(abb_nodo_t* nodo,const char* clave,const abb_t* abb) {
	if(!nodo) return NULL;
	abb_comparar_clave_t cmp = abb->cmp;
	if(cmp(nodo->clave,clave) == 0) {
		return nodo;
	}
	if(cmp(nodo->clave,clave) > 0) { //Tiene que estar a la izquierda de este nodo.
		if(!nodo->izq) return NULL;
		return buscar_nodo(nodo->izq,clave,abb); //Busco a la izquierda.
	}
	//Sino, tiene que estar a la derecha del nodo.
	if(!nodo->der) return NULL;
	return buscar_nodo(nodo->der,clave,abb); //Busco a la derecha.
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void* borrar_nodo(abb_nodo_t* nodo,abb_t* arbol,abb_nodo_t* padre,bool borrar) {
	arbol->cantidad--;
	if(padre) { //Si tengo padre actualizo el puntero a NULL.
		if(padre->izq == nodo) padre->izq = NULL;
		if(padre->der == nodo) padre->der = NULL;
	}
	free(nodo->clave);
	void* dato = nodo->dato;
	if(borrar) if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
	free(nodo);
	return dato;
}

///////////////////////////////////////////////////////////////////////////////////

void* borrar_2_hijos(abb_nodo_t* nodo,abb_t* arbol,abb_nodo_t* padre) {

	//Busco el reemplazante, 1 a la derecha y todo a la izquierda.	
	abb_nodo_t* reemplazante = nodo->der;
	while(reemplazante->izq) {
		reemplazante = reemplazante->izq;
	}
	//Guardo los datos del reemplazante.
	char* clave = strdup(reemplazante->clave);
	void* dato = abb_borrar(arbol,reemplazante->clave); //Llamo para borrarlo y obtener su dato.

	free(nodo->clave); //Libero el nodo en donde voy a poner al reemplazante.
	void* dato_nodo = nodo->dato; //Guardo el dato del nodo que tengo que destruir.

	//Actualizo el nodo que quedo con los datos del reemplazante.
	nodo->clave = clave;
	nodo->dato = dato;
	return dato_nodo; //Devuelvo el dato del nodo a borrar.
}

///////////////////////////////////////////////////////////////////////////////////

void* borrar_1_hijo(abb_nodo_t* nodo,abb_t* arbol,abb_nodo_t* padre) {
	abb_nodo_t* hijo;

 	//Averiguo cual es el hijo que existe
	if(nodo->izq) hijo = nodo->izq;
	else hijo = nodo->der;

	if(padre) { //Si tengo padre actualizo su refenrecia a la de mi hijo
		if(padre->izq == nodo) padre->izq = hijo;
		if(padre->der == nodo) padre->der = hijo;
	} else arbol->raiz = hijo; // Si no tengo padre es porque soy la raiz, asi que la actualizo.
	return borrar_nodo(nodo,arbol,padre,false);
}

/////////////////////////////////////////////////////////////////////
/////////////////////ARBOL BINARIO DE BUSQUEDA///////////////////////
/////////////////////////////////////////////////////////////////////

void* abb_borrar(abb_t *arbol, const char *clave) {
	if(!arbol) return NULL;

	abb_nodo_t* nodo = buscar_nodo(arbol->raiz,clave,arbol);
	if(!nodo) return NULL;
	abb_nodo_t* padre = buscar_padre(arbol->raiz,clave,arbol);

	//CASO SIN HIJOS
	if(!nodo->izq && !nodo->der) {
		void* dato = borrar_nodo(nodo,arbol,padre,false);
		if(!padre) arbol->raiz = NULL;
		return dato;
	}
	//CASO 2 HIJOS
	if(nodo->izq && nodo->der) return borrar_2_hijos(nodo,arbol,padre);

	//CASO 1 HIJO
	return borrar_1_hijo(nodo,arbol,padre);
}


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb) return NULL;

	abb->raiz = NULL;
	abb->cmp = cmp;
	abb->destruir_dato = destruir_dato;
	abb->cantidad = 0;
	return abb;
}

/////////////////////////////////////////////////////////////////////

bool _actualizar_cantidad_arbol(abb_t* arbol) {
	if(!arbol) return false;
	arbol->cantidad++;
	return true;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	
	abb_nodo_t* buscado = buscar_nodo(arbol->raiz,clave,arbol);

	if(!arbol->raiz) {
		abb_nodo_t* nodo = crear_nodo_abb(clave,dato);
		arbol->raiz = nodo;
		return _actualizar_cantidad_arbol(arbol);
	}
	
	if(buscado) {
		if(arbol->destruir_dato) arbol->destruir_dato(buscado->dato);
		buscado->dato = dato; 
		return true;
	}
	else {
		abb_nodo_t* nodo = crear_nodo_abb(clave,dato);
		abb_nodo_t* buscado_padre = buscar_padre(arbol->raiz,clave,arbol);
		if(arbol->cmp(buscado_padre->clave,clave) < 0) buscado_padre->der = nodo;
		else buscado_padre->izq = nodo;
		return _actualizar_cantidad_arbol(arbol);
	}

	
}


/////////////////////////////////////////////////////////////////////

void* abb_obtener(const abb_t *arbol, const char *clave) {
	abb_nodo_t* nodo = buscar_nodo(arbol->raiz,clave,arbol);
	if(!nodo) return NULL;
	return(nodo->dato);
}

/////////////////////////////////////////////////////////////////////

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	abb_nodo_t* buscado = buscar_nodo(arbol->raiz,clave,arbol);
	if(!buscado) return false;
	return true;
}

/////////////////////////////////////////////////////////////////////

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

/////////////////////////////////////////////////////////////////////

void _abb_destruir_nodo(abb_nodo_t* nodo,abb_t* arbol) {
	//POSTORDER SI O SI PARA PASAR POR TODOS LOS NODOS
	if(!nodo) return;
	_abb_destruir_nodo(nodo->izq,arbol);
	_abb_destruir_nodo(nodo->der,arbol);
	borrar_nodo(nodo,arbol,NULL,true);
}

/////////////////////////////////////////////////////////////////////

void abb_destruir(abb_t *arbol) {
	_abb_destruir_nodo(arbol->raiz,arbol);
	free(arbol);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////ITERADOR INTERNO////////////////////////////
/////////////////////////////////////////////////////////////////////

//Un iterador interno, que funciona usando la función de callback “visitar” que recibe la clave, 
//el valor y un puntero extra, y devuelve true si se debe seguir iterando, 
//false en caso contrario.

void _abb_in_order(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra,bool* seguir) {
	if(!nodo) return;
	if(!*seguir) return;
	_abb_in_order(nodo->izq, visitar, extra,seguir);
	if(!*seguir) return;
	if(!visitar(nodo->clave,nodo->dato,extra)) {
		*seguir = false;
		return;
	}
	if(!*seguir) return;
	_abb_in_order(nodo->der, visitar, extra,seguir);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if(!arbol) return;
	bool seguir = true;
	_abb_in_order(arbol->raiz, visitar, extra,&seguir);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////ITERADOR EXTERNO////////////////////////////
/////////////////////////////////////////////////////////////////////

//Función auxiliar.
void _agregar_a_pila(abb_nodo_t* nodo,pila_t* pila) {
	if(!nodo) return;
	pila_apilar(pila,nodo);
	_agregar_a_pila(nodo->izq,pila);
	
}

//Apilo raíz y todos los hijos izquierdos.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	if(!arbol) return NULL;
	abb_iter_t* iter = calloc(1,sizeof(abb_iter_t));
	if(!iter) return NULL;
	pila_t* pila = pila_crear();
	if(!pila) {
		free(iter);
		return NULL; 
	}
	iter->pila = pila;

	if(arbol->raiz) pila_apilar(iter->pila, arbol->raiz);
	else return iter;
	_agregar_a_pila(arbol->raiz->izq,iter->pila);
	iter->nodo = (abb_nodo_t*) pila_ver_tope(iter->pila);
	return iter;
}

//Desapilo. Apilo hijo derecho del desapilado (si existe) y todos los hijos izquierdos.
bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if(abb_iter_in_al_final(iter)) return false;
	abb_nodo_t* desapilado = pila_desapilar(iter->pila);
	if(desapilado->der) {
		_agregar_a_pila(desapilado->der,iter->pila);
	}
	iter->nodo = (abb_nodo_t*) pila_ver_tope(iter->pila);
	return true;
}

//Devuelvo la clave del nodo actual (que es el tope de la pila).
const char* abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if(!iter->nodo) return NULL;
	return iter->nodo->clave;
}

//Verifico si pila_esta_vacia.
bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

//Destruyo la pila.
void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}
