#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  /* bool */
#include <stddef.h>	  /* size_t */
#include <sys/types.h> /* ssize_t */
#include "heap.h"

#define TAM_INICIAL 50
#define CTE_AMP_REDUC 2
#define CTE_VERIF_CANTIDAD 4

/////////////////////////////////////////////////////////////////////
/////////////////////////////STRUCTS/////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*
 * Implementación de un TAD cola de prioridad, usando un max-heap.
 *
 * Notar que al ser un max-heap el elemento mas grande será el de mejor
 * prioridad. Si se desea un min-heap, alcanza con invertir la función de
 * comparación.
 */
/* Tipo utilizado para el heap. */
struct heap {
	void** datos;
	size_t cant;
	size_t tam;
	cmp_func_t cmp;
};

/////////////////////////////////////////////////////////////////////
//////////////////////FUNCIONES AUXILIARES///////////////////////////
/////////////////////////////////////////////////////////////////////
//Swapea en el arreglo Los elementos de la posicion Inicio por Fin.
void swap (heap_t* heap, size_t inicio, size_t fin) {
	void* aux = heap->datos[inicio];
	heap->datos[inicio] = heap->datos[fin];
	heap->datos[fin] = aux;
}

/////////////////////////////////////////////////////////////////////
//Redimensiona el heap con el nuevo tamaño pasado por parámetro.
void** heap_redimension(heap_t* heap,size_t nuevo_tam) {
	void** datos_nuevo = realloc(heap->datos, nuevo_tam * sizeof(void*));
	if (!datos_nuevo) return NULL;
	return datos_nuevo;	
}

/////////////////////////////////////////////////////////////////////
//Upheap.
void upheap(heap_t* heap, size_t pos) {
	if(!heap->datos[pos]) return;
	ssize_t pos_padre = (int) (pos-1)/2;
	if(pos_padre < 0) return;
	if(!heap->datos[pos_padre]) return;
	if(heap->cmp(heap->datos[pos], heap->datos[pos_padre]) > 0) {
		swap(heap, pos, pos_padre);
		upheap(heap, pos_padre);
	}
	return;
}

/////////////////////////////////////////////////////////////////////
//Calcula el hijo máximo para hacer downheap.
ssize_t maximo_hijo(heap_t* heap,size_t pos_izq,size_t pos_der) {
	if(pos_izq >= heap->cant) return pos_der;
	if(pos_der >= heap->cant) return pos_izq;
	if(heap->cmp(heap->datos[pos_izq],heap->datos[pos_der]) <= 0) return pos_der;
	return pos_izq;
}

/////////////////////////////////////////////////////////////////////
//Downheap.
void downheap(heap_t* heap, size_t pos) {
	if(!heap->datos[pos]) return;
	size_t pos_izq = pos*2 + 1;
	size_t pos_der = pos*2 + 2;
	if(pos_izq >= heap->cant && pos_der >= heap->cant) return;
	size_t nueva_pos = maximo_hijo(heap,pos_izq,pos_der);
	if(pos_izq < heap->cant || pos_der < heap->cant) {
		if(heap->cmp(heap->datos[pos],heap->datos[nueva_pos]) <= 0) {
			swap(heap,pos,nueva_pos);
			downheap(heap,nueva_pos);
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t* heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;
	heap->datos = calloc(TAM_INICIAL,sizeof(void*));
	if(!heap->datos) {
		free(heap);
		return NULL;
	}
	heap->tam = TAM_INICIAL;
	heap->cant = 0;
	heap->cmp = cmp;
	return heap;
}

/////////////////////////////////////////////////////////////////////
/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/
bool llenar_arr_heapify(void* arreglo[], size_t n, cmp_func_t cmp, heap_t* heap) {
	for(int i = 0; i < n; i++) {
		if (heap->cant == heap->tam) {
			heap->datos = heap_redimension(heap,(heap->tam * CTE_AMP_REDUC));
			heap->tam = heap->tam * CTE_AMP_REDUC;
		}
		if (!heap->datos) return false;
		heap->datos[i] = arreglo[i];
		heap->cant++;
	}
	size_t pos_relativa = heap->cant-1;
	while(pos_relativa >= 0 && pos_relativa <= heap->cant-1) {
		downheap(heap, pos_relativa);
		pos_relativa--;	
	}
	return true;
}

heap_t *heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;
	heap->datos = malloc(sizeof(void*) * TAM_INICIAL);
	if(!heap->datos) {
		free(heap);
		return NULL;
	}
	heap->tam = TAM_INICIAL;
	heap->cant = 0;
	heap->cmp = cmp;
	//Llenar el arreglo que me pasan.
	if(!llenar_arr_heapify(arreglo, n, cmp, heap)) return NULL;
	return heap;
}

/////////////////////////////////////////////////////////////////////
/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t* heap) {
	return heap->cant;
}

/////////////////////////////////////////////////////////////////////
/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t* heap) {
	return heap->cant == 0;
}

/////////////////////////////////////////////////////////////////////
/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t* heap, void* elem) {
	if(elem == NULL) return false;
	//Redimensión.
	if (heap->cant == heap->tam) {
		heap->datos = heap_redimension(heap,(heap->tam * CTE_AMP_REDUC));
		heap->tam = heap->tam * CTE_AMP_REDUC;
	}
	if (!heap->datos) return false;
	//
	heap->datos[heap->cant] = elem;
	upheap(heap,heap->cant); //Le paso posiciones para que lo haga la funcion.
	heap->cant++;
	return true;
}

/////////////////////////////////////////////////////////////////////
/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void* heap_ver_max(const heap_t* heap) {
	return heap->datos[0];
}

/////////////////////////////////////////////////////////////////////
/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void* heap_desencolar(heap_t* heap) {
	if(heap_esta_vacio(heap)) return NULL;
	heap->cant--;
	//Redimension.
	if (heap->cant <= (heap->tam / CTE_VERIF_CANTIDAD) && heap->tam >= TAM_INICIAL * CTE_AMP_REDUC) {
		heap->datos = heap_redimension(heap,(heap->tam / CTE_AMP_REDUC));
		heap->tam = heap->tam / CTE_AMP_REDUC;	
	}
	if (!heap->datos) return NULL;
	//
	void* desencolado = heap->datos[0];
	swap(heap, 0,heap->cant);
	downheap(heap, 0);
	return desencolado;
}

/////////////////////////////////////////////////////////////////////
/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. 
 */
void heap_destruir(heap_t* heap, void destruir_elemento(void* e)) {
	if(destruir_elemento) {
		for(int i = 0;i<heap->cant;i++) {
			destruir_elemento(heap->datos[i]);
		}
	}
	free(heap->datos);
	free(heap);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t* heap = heap_crear_arr(elementos,cant,cmp);
	size_t pos_relativa = heap->cant-1;
	size_t heap_cant = heap->cant;
	while(pos_relativa > 0) {
		swap(heap, 0, pos_relativa);
		pos_relativa--;
		heap->cant--;
		downheap(heap, 0);
	}
	heap->cant = heap_cant;
	for(int i = 0; i < heap->cant; i++) {
		elementos[i] = heap->datos[i];
	}
	heap_destruir(heap,NULL);
}

