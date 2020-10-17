#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stddef.h>



/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista_iter lista_iter_t;
typedef struct lista lista_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al comienzo
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista (mismo funcionamiento que funcion append). Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento en la lista. La lista ahora
// contiene un elemento menos, si la lista no estaba vacía.
void* lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void* lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void* lista_ver_ultimo(const lista_t *lista);

//Devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void*));

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

//Itera de manera interna la lista.
//Pre: la lista fue creada y las funciones necesarias para ejecutar este iterador también.
//Pos: La lista fue iterada.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Devuelve un iterador.
//Post: devuelve un nuevo iterador apuntando al primer elemento de la lista relacionada a él.
lista_iter_t* lista_iter_crear(lista_t *lista);

//El iterador avanza un lugar en la lista.
//Pre: el iterador fue creado.
//Post: el iterador apunta al siguiente elemento en la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

//Obtiene el valor del elemento al cual el iterador apunta.
//Si la lista tiene elementos, se devuelve el valor del que apunta el iterador.
//Si está vacía devuelve NULL. Si llegó al final devuelve NULL.
//Pre: el iterador fue creado.
//Post: se devolvió el elemento de la lista al cual el iterador apunta, cuando no está vacía o al final.
void* lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve True al llegar al final de la lista.
//Pre: el iterador fue creado.
//Post: el iterador recorrió completamente la lista y llegó al final.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra entre 
// el anterior y el actual al que apuntaba el iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Saca el elemento el cual apunta el iterador. Si la lista tiene elementos, se quita el
// apuntado por el iterador y se devuelve su valor, si está vacía o apunta al final, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del elemento en la lista apuntado por el iterador. La lista ahora
// contiene un elemento menos, si la lista no estaba vacía.
// El actual apunta al próximo del borrado.
void* lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);


#endif // LISTA_H