#include "cola.h"
#include <stdlib.h>


typedef struct nodo_cola {
	void* dato;
	struct nodo_cola* prox;
} nodo_cola_t;

nodo_cola_t* crear_nodo_cola(void* valor) {
	nodo_cola_t* nodo = malloc(sizeof(nodo_cola_t));
	if(!nodo) return NULL;
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

struct cola {
	nodo_cola_t* prim;
	nodo_cola_t* ult;
};

////////////////////////////////////////////////////////////////////////////////////////////////

// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t* cola_crear(void) {
	cola_t* cola = malloc(sizeof(cola_t));
	if(!cola) return NULL;
	cola->prim = NULL;
	cola->ult = NULL;
	return cola;
}


// Devuelve verdadero o falso, según si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola) {
	if (cola->prim == NULL) return true;
	return false;
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor) {
	nodo_cola_t* nodo = crear_nodo_cola(valor);
	if(!nodo) return false;
	if(cola_esta_vacia(cola)) {
		cola->prim = nodo;
		cola->ult = nodo;
		return true;
	}
	cola->ult->prox = nodo;
	cola->ult = nodo;
	return true;
}

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void* cola_ver_primero(const cola_t *cola) {
	if(cola_esta_vacia(cola)) return NULL;
	return cola->prim->dato;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_desencolar(cola_t *cola) {
	if(cola_esta_vacia(cola)) return NULL;
	void* valor = cola->prim->dato;
	void* proximo = cola->prim->prox;
	free(cola->prim);
	cola->prim = proximo;
	if(cola->prim == NULL) cola->ult = NULL;
	return valor;
}

// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
	bool destruir = true;
	if (destruir_dato == NULL) destruir = false;
	while(!cola_esta_vacia(cola)) {
		void* dato = cola_desencolar(cola);
		if(destruir) destruir_dato(dato);
	}
	free(cola);
}

