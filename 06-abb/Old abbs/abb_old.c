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

struct abb_nodo {
	char* clave;
	abb_nodo_t* izq;
	abb_nodo_t* der;
	void* dato;
};

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

abb_nodo_t* buscar_nodo(abb_nodo_t* nodo,const char* clave,bool* encontro,const abb_t* abb,abb_nodo_t** padre) {
	if(!nodo) return NULL;
	abb_comparar_clave_t cmp = abb->cmp;
	if(cmp(nodo->clave,clave) == 0) {
		*encontro = true;
		return nodo;
	}
	if(padre) *padre = nodo;
	if(cmp(nodo->clave,clave) > 0) {
		if(!nodo->izq) { //Devuelve el padre (el hijo va a la izq). Si se pasa "padre" devuelve la posicion del mismo.
			*encontro = false;
			return nodo;
		}	
		return buscar_nodo(nodo->izq,clave,encontro,abb,padre); //Busco a la izquierda.
	}

	if(!nodo->der) { //Devuelve el padre (el hijo va a la der). Si se pasa "padre" devuelve la posicion del mismo.
		*encontro = false;
		return nodo;
	}
	return buscar_nodo(nodo->der,clave,encontro,abb,padre); //Busco a la derecha
}

///////////////////////////////////////////////////////////////////////////////////

void* borrar_nodo(abb_nodo_t* nodo,abb_t* arbol,abb_nodo_t* padre) {
	arbol->cantidad--;
	if(padre) {
		if(padre->izq == nodo) padre->izq = NULL;
		if(padre->der == nodo) padre->der = NULL;
	}
	free(nodo->clave);
	void* dato = nodo->dato;
	if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
	free(nodo);
	return dato;
}

///////////////////////////////////////////////////////////////////////////////////

void* borrar_2_hijos(abb_nodo_t* nodo,abb_t* arbol,abb_nodo_t* padre) {
	abb_nodo_t* reemplazante = nodo->der;
	while(reemplazante->izq) {
		reemplazante = reemplazante->izq;
	}
	void* dato = reemplazante->dato;
	char* clave = strdup(reemplazante->clave);
	abb_borrar(arbol,reemplazante->clave);
	free(nodo->clave);
	void* dato_nodo = nodo->dato;
	if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
	nodo->clave = clave;
	nodo->dato = dato;
	return dato_nodo;
}

///////////////////////////////////////////////////////////////////////////////////

void* borrar_1_hijo(abb_nodo_t* nodo,abb_t* arbol,abb_nodo_t* padre) {
	abb_nodo_t* hijo;

	if(nodo->izq) hijo = nodo->izq;
	else hijo = nodo->der;

	if(padre) {
		if(padre->izq == nodo) padre->izq = hijo;
		if(padre->der == nodo) padre->der = hijo;
	} else arbol->raiz = hijo;
	return borrar_nodo(nodo,arbol,padre);
}

/////////////////////////////////////////////////////////////////////
/////////////////////ARBOL BINARIO DE BUSQUEDA///////////////////////
/////////////////////////////////////////////////////////////////////

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

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	abb_nodo_t* nodo = crear_nodo_abb(clave,dato);
	if(!nodo) return false;
	if(!arbol->raiz) {
		arbol->raiz = nodo;
		arbol->cantidad++;
		return true;
	}
	bool encontro = false;
	abb_nodo_t* buscado = buscar_nodo(arbol->raiz,clave,&encontro,arbol,NULL);
	if(encontro) { 
		buscado->dato = dato;
		free(nodo);
	}
	// si "encontro" es falso -> buscado es el padre de donde iria el nodo.
	else if(arbol->cmp(buscado->clave,clave) < 0) buscado->der = nodo;
	else buscado->izq = nodo;
	arbol->cantidad++;
	return true;
}

/////////////////////////////////////////////////////////////////////

void* abb_borrar(abb_t *arbol, const char *clave) {
	if(!arbol) return NULL;
	bool encontro = false;
	abb_nodo_t* padre = NULL;
	abb_nodo_t* nodo = buscar_nodo(arbol->raiz,clave,&encontro,arbol,&padre);
	if(!encontro || !nodo) return NULL;

	//CASO SIN HIJOS
	if(!nodo->izq && !nodo->der) {
		void* dato = borrar_nodo(nodo,arbol,padre);
		if(!padre) arbol->raiz = NULL;
		return dato;
	}
	//CASO 2 HIJOS
	if(nodo->izq && nodo->der) return borrar_2_hijos(nodo,arbol,padre);

	//CASO 1 HIJO
	return borrar_1_hijo(nodo,arbol,padre);
}

/////////////////////////////////////////////////////////////////////

void* abb_obtener(const abb_t *arbol, const char *clave) {
	bool encontro = false;
	abb_nodo_t* nodo = buscar_nodo(arbol->raiz,clave,&encontro,arbol,NULL);
	if(!encontro || !nodo) return NULL;
	return(nodo->dato);
}

/////////////////////////////////////////////////////////////////////

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	bool encontro = false;
	buscar_nodo(arbol->raiz,clave,&encontro,arbol,NULL);
	return encontro;
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
	free(nodo->clave);
	if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
	free(nodo);
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

void _abb_in_order(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
	if(!nodo) return;
	_abb_in_order(nodo->izq, visitar, extra);
	if(!visitar(nodo->clave,nodo->dato,extra)) return;
	_abb_in_order(nodo->der, visitar, extra);
}
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if(!arbol) return;
	abb_nodo_t* nodo = arbol->raiz;
	if(!nodo) return;
	_abb_in_order(nodo, visitar, extra);
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

//Veo el tope de la pila.
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
