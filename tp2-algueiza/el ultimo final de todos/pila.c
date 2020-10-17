#include "pila.h"
#include <stdlib.h>
#define TAM_INICIAL 10
#define AUMENTO +tamanio/2  //Si pongo 1.5 explota al multiplicar, lo solucione asi.


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

bool pila_redimensionar(pila_t *pila,size_t tam) {
    void* pila_nueva = realloc(pila->datos, tam * sizeof(void*));
    if (!pila_nueva) return false;
    pila->capacidad = tam;
    pila->datos = pila_nueva;
    return true;
} 


/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear(void) {
    pila_t* pila = malloc(sizeof(pila_t));
    if (!pila) return NULL;
    pila->cantidad = 0;
    pila->capacidad = TAM_INICIAL;
    pila->datos = malloc(sizeof(void*)*pila->capacidad);
    if(pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila) {
    return (pila->cantidad == 0);
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor) {
    size_t elementos = pila->cantidad;
    size_t tamanio = pila->capacidad;
    if (elementos == tamanio) {
		if (!pila_redimensionar(pila,tamanio*AUMENTO)) return false;
	}
    pila->datos[elementos] = valor;
    pila->cantidad++;
    return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;
    return pila->datos[pila->cantidad-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila) {
    if(pila_esta_vacia(pila)) return NULL;
    void* valor;
    valor = pila->datos[pila->cantidad-1];
    pila->cantidad--;
    if (pila->capacidad/4 > pila->cantidad && pila->capacidad/4 > TAM_INICIAL/4) pila_redimensionar(pila,pila->capacidad/2);
    if (pila->capacidad/4 > pila->cantidad && pila->capacidad/4 < TAM_INICIAL/4) pila_redimensionar(pila,TAM_INICIAL);
    return valor;
}


