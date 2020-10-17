#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "hash.h"

#define TAM_INICIAL 577
#define CAMBIO_TAM 11
#define FACTOR_REDIMENSION_SUPERIOR 0.7
#define FACTOR_REDIMENSION_INFERIOR 0.10

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// FUNCION DE HASHING BIEN CHINGONA //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//https://codereview.stackexchange.com/questions/85556/simple-string-hashing-algorithm-implementation


static size_t getHash(const char* source,size_t largo)
{
    size_t length = largo;
    size_t hash = 0;
    for(size_t i = 0; i < length; i++) {
        char c = source[i];
        int a = c - '0';
        hash = (hash * 10) + a;     
    } 

    return hash;
}

size_t stringtohash(const char *word, size_t hashTableSize){
  size_t hashAddress;
  hashAddress = getHash(word, strlen(word));
  return (hashAddress%hashTableSize);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum estado { 
  VACIO,OCUPADO,BORRADO 
} estado_t;

typedef struct hash_campo {
    char* clave;
    void* valor;
    estado_t estado; 
} hash_campo_t;

struct hash {
  size_t cantidad;
  size_t largo;
  float carga;
  hash_campo_t* tabla;
  hash_destruir_dato_t destruir_dato;
};

typedef struct hash_iter{
  size_t posicion;
  const hash_t* hash;
} hash_iter_t;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

ssize_t buscar_hash(const hash_t* hash,const char* cadena) {
  size_t largo = hash->largo;
  size_t posicion_ini = stringtohash(cadena,largo);
  for(size_t j = 0;j<largo;j++) {
    size_t i = (j+posicion_ini)%largo; //para iterar circularmente
    if(hash->tabla[i].estado == VACIO) break;
    if(hash->tabla[i].estado == BORRADO) continue;
    char* clave = hash->tabla[i].clave;
    if(strcmp(clave,cadena) == 0) return i;
  }
  return -1;

}

/////////////////////////////////////////////////////////////////////

void destruir_tabla(hash_campo_t* tabla,hash_destruir_dato_t destruir_dato,size_t largo) {
	for(int i = 0;i<largo;i++) {
		if(tabla[i].estado == OCUPADO) {
			free(tabla[i].clave);
			if(destruir_dato) destruir_dato(tabla[i].valor);
		}
	}
	free(tabla);
}

/////////////////////////////////////////////////////////////////////

void actualizar_carga(hash_t* hash) {
	hash->carga =  ((hash->carga * (float) hash->largo) + 1) / (float) hash->largo;
}


/////////////////////////////////////////////////////////////////////

bool redimensionar_tabla(hash_t* hash) {
  float redim = hash->carga;
  float proporcion = (float) hash->cantidad/ (float) hash->largo;
  size_t nuevo_tam;
  if( redim > FACTOR_REDIMENSION_SUPERIOR) {
	if(proporcion < FACTOR_REDIMENSION_INFERIOR && hash->largo > TAM_INICIAL) nuevo_tam = hash->largo/CAMBIO_TAM;
	else nuevo_tam = hash->largo*CAMBIO_TAM;
	if(nuevo_tam < 577) nuevo_tam = TAM_INICIAL;
	
	//guardo los datos utiles.
	hash_campo_t* nueva_tabla = calloc(nuevo_tam,sizeof(hash_campo_t));
	if(!nueva_tabla) return false;
	size_t tam_original = hash->largo;
	hash_campo_t* tabla_original = hash->tabla;
	
	//reseteo el hash como nuevo.
	hash->cantidad = 0;
	hash->carga = 0;
	hash->largo = nuevo_tam;
	hash->tabla = nueva_tabla;
	for(int i = 0; i < tam_original;i++) {
	  if(tabla_original[i].estado == OCUPADO) {
	    char* clave = tabla_original[i].clave;
	    void* dato = tabla_original[i].valor; 
	    hash_guardar(hash,clave,dato);
	    free(tabla_original[i].clave);
	  }
	}
	free(tabla_original);
  }
return true;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/* Crea el hash*/
hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;
	hash_campo_t* tabla = calloc(TAM_INICIAL,sizeof(hash_campo_t));
	if(!tabla) {
		free(hash);
		return NULL;
	}
	hash->cantidad = 0;
	hash->largo = TAM_INICIAL;
	hash->carga = 0;
	hash->tabla = tabla;
	hash->destruir_dato = destruir_dato;
	return hash;
}


/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
  if(!redimensionar_tabla(hash)) return false;
  ssize_t pos = buscar_hash(hash,clave);
  if(pos != -1) { //lo encontro, entonces reemplazo.
  	if(hash->destruir_dato) hash->destruir_dato(hash->tabla[pos].valor);
    hash->tabla[pos].valor = dato;
    return true;
  }
  char* palabra = strdup(clave);
  size_t hashed = stringtohash(clave,hash->largo);
  for(ssize_t j = 0;j<hash->largo;j++) {
    size_t i = (j+hashed)%hash->largo; //para iterar circularmente.
    if(hash->tabla[i].estado == VACIO) {
      hash->tabla[i].clave = palabra;
      hash->tabla[i].valor = dato;
      hash->tabla[i].estado = OCUPADO;
      hash->cantidad++;
      actualizar_carga(hash);
      return true;
    }
  }
  return false;
}
	

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void* hash_borrar(hash_t* hash, const char* clave) {
  if(!redimensionar_tabla(hash)) return false;
  ssize_t pos = buscar_hash(hash,clave);
  if(pos == -1) return NULL; //NO ESTA

  hash->cantidad--;
  void* dato = hash->tabla[pos].valor;

  free(hash->tabla[pos].clave);
  //actualizo los datos
  hash->tabla[pos].clave = NULL;
  hash->tabla[pos].valor = NULL;
  hash->tabla[pos].estado = BORRADO;
  return dato;
}


/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void* hash_obtener(const hash_t* hash, const char* clave) {
  ssize_t pos = buscar_hash(hash,clave);
  if(pos == -1) return NULL; //NO ESTA
  return hash->tabla[pos].valor;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t* hash, const char* clave) {
  ssize_t pos = buscar_hash(hash,clave);
  if(pos == -1) return false; //NO ESTA
  return true;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t* hash) {
  return hash->cantidad;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t* hash) {
  destruir_tabla(hash->tabla,hash->destruir_dato,hash->largo);
  free(hash);
}


/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
  	iter->hash = hash;
	iter->posicion = 0;
	while((iter->posicion < iter->hash->largo) && !iter->hash->tabla[iter->posicion].clave) {
		iter->posicion++;
	}
	return iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter) {
	if(hash_iter_al_final(iter)) return false;
	iter->posicion++;
	while((iter->posicion < iter->hash->largo) && !(iter->hash->tabla[iter->posicion].clave)) {
		iter->posicion++;
	}
	return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if(hash_iter_al_final(iter)) return NULL;
	return iter->hash->tabla[iter->posicion].clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter) {
	return (iter->posicion == iter->hash->largo);
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}
