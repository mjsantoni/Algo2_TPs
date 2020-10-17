#include "cola.h"
#include <stdlib.h>
#include <stdio.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La cola está planteada como una cola de punteros genéricos. */

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

struct cola {
	nodo_t* prim;
	nodo_t* ult;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t* cola_crear(void) {
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL) return NULL;
	cola->prim = NULL;
	cola->ult = NULL;
	return cola;
}
// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
	while(!cola_esta_vacia(cola)) {
		void* dato = cola_desencolar(cola);
		if (destruir_dato) destruir_dato(dato);
	}
	free(cola);
}

// Devuelve verdadero o falso, según si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola) {
	return (cola->prim == NULL);
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor) {
	nodo_t* nuevo_nodo = crear_nodo(valor);
	if (nuevo_nodo == NULL) return false;
	if (!cola_esta_vacia(cola)) {
		cola->ult->prox = nuevo_nodo;
		cola->ult = nuevo_nodo;
		return true;
	}
	cola->prim = nuevo_nodo;
	cola->ult = cola->prim;
	return true;
}


// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void* cola_ver_primero(const cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;
	return cola->prim->dato;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_desencolar(cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;
	void* valor = cola->prim->dato;
	nodo_t* nodo = cola->prim;
	cola->prim = nodo->prox;
	free(nodo);
	return valor;
}


