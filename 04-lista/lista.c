#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
struct nodo;
typedef struct nodo {
	void* dato;
	struct nodo* prox;
}nodo_t;

nodo_t* crear_nodo(void* valor) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

typedef struct lista_iter {
	lista_t* lista;
	nodo_t* act;
	nodo_t* ant;
}lista_iter_t;

typedef struct lista {
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
}lista_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista) {
	return (lista->prim == NULL);
}

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al comienzo
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nuevo_nodo = crear_nodo(dato);
	if (nuevo_nodo == NULL) return false;
	if (lista_esta_vacia(lista)) lista->ult = nuevo_nodo;
	else nuevo_nodo->prox = lista->prim;
	lista->prim = nuevo_nodo;
	lista->largo++;
	return true;
}

// Agrega un nuevo elemento al final de la lista (mismo funcionamiento que funcion append). Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nuevo_nodo = crear_nodo(dato);
	if (nuevo_nodo == NULL) return false;
	if (lista_esta_vacia(lista)) lista->prim = nuevo_nodo;
	else lista->ult->prox = nuevo_nodo;
	lista->ult = nuevo_nodo;
	lista->largo++;
	return true;
}

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento en la lista. La lista ahora
// contiene un elemento menos, si la lista no estaba vacía.
void* lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	void* dato = lista->prim->dato;
	nodo_t* borrado = lista->prim;
	lista->prim = borrado->prox;
	free(borrado);
	lista->largo--;
	return dato;
}

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void* lista_ver_primero(const lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void* lista_ver_ultimo(const lista_t* lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ult->dato;
}

//Devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
	while(!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato) destruir_dato(dato);
	}
	free(lista);
}

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

//Itera de manera interna la lista.
//Pre: la lista fue creada y las funciones necesarias para ejecutar este iterador también.
//Pos: La lista fue iterada.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t* actual = lista->prim;
	if (!actual) return;
	bool r = visitar(actual->dato,extra);
	actual = actual->prox;
	while(actual != NULL && r == true) {
		r = visitar(actual->dato,extra);
		actual = actual->prox;
	}
}

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Devuelve un iterador.
//Post: devuelve un nuevo iterador apuntando al primer elemento de la lista relacionada a él.
lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;
	iter->lista = lista;
	iter->act = iter->lista->prim;
	iter->ant = NULL;
	return iter;
}

//El iterador avanza un lugar en la lista.
//Pre: el iterador fue creado.
//Post: el iterador apunta al siguiente elemento en la lista.
bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return false;
	iter->ant = iter->act;
	iter->act = iter->act->prox;
	return true;
}

//Obtiene el valor del elemento al cual el iterador apunta.
//Si la lista tiene elementos, se devuelve el valor del que apunta el iterador.
//Si está vacía devuelve NULL. Si llegó al final devuelve NULL.
//Pre: el iterador fue creado.
//Post: se devolvió el elemento de la lista al cual el iterador apunta, cuando no está vacía o al final.
void *lista_iter_ver_actual(const lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return NULL;
	return iter->act->dato;
}

//Devuelve True al llegar al final de la lista.
//Pre: el iterador fue creado.
//Post: el iterador recorrió completamente la lista y llegó al final.
bool lista_iter_al_final(const lista_iter_t *iter) {
	return iter->act == NULL;
}

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
}

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra entre 
// el anterior y el actual al que apuntaba el iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	nodo_t* nuevo_nodo = crear_nodo(dato);
	if (nuevo_nodo == NULL) return false;
	if (iter->ant == NULL) 	iter->lista->prim = nuevo_nodo;
	else iter->ant->prox = nuevo_nodo;
	if (iter->act == NULL) iter->lista->ult = nuevo_nodo;
	nuevo_nodo->prox = iter->act;
	iter->act = nuevo_nodo;
	iter->lista->largo++;
	return true;
}

// Saca el elemento el cual apunta el iterador. Si la lista tiene elementos, se quita el
// apuntado por el iterador y se devuelve su valor, si está vacía o apunta al final, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del elemento en la lista apuntado por el iterador. La lista ahora
// contiene un elemento menos, si la lista no estaba vacía.
// El actual apunta al próximo del borrado.
void *lista_iter_borrar(lista_iter_t *iter) {
	if (lista_esta_vacia(iter->lista)) return NULL;
	if (iter->act == NULL) return NULL;
	void* dato = iter->act->dato;
	nodo_t* borrado = iter->act;
	if (iter->ant == NULL) iter->lista->prim = borrado->prox;
	else iter->ant->prox = iter->act->prox;
	if (iter->act->prox == NULL) iter->lista->ult = iter->ant;
	iter->act = iter->act->prox;
	free(borrado);
	iter->lista->largo--;
	return dato;
}
