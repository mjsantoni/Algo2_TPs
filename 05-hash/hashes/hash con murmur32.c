#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "hash.h"
#define TAM_INICIAL 557
#define FACTOR_REDIMENSION 0.93
int REDIMENSION_GLOBAL = 0;

int primos [] = {2503,6007,15013,50021,120011,350003,700001,1500007,5000011};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// FUNCION DE HASHING BIEN CHINGONA //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/14409466/simple-hash-functions
uint32_t murmur3_32(const char* key, size_t len, uint32_t seed) {
  uint32_t h = seed;
  if (len > 3) {
    const uint32_t* key_x4 = (const uint32_t*) key;
    size_t i = len >> 2;
    do {
      uint32_t k = *key_x4++;
      k *= 0xcc9e2d51;
      k = (k << 15) | (k >> 17);
      k *= 0x1b873593;
      h ^= k;
      h = (h << 13) | (h >> 19);
      h = (h * 5) + 0xe6546b64;
    } while (--i);
    key = (const char*) key_x4;
  }
  if (len & 3) {
    size_t i = len & 3;
    uint32_t k = 0;
    key = &key[i - 1];
    do {
      k <<= 8;
      k |= *key--;
    } while (--i);
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    h ^= k;
  }
  h ^= (uint32_t) len;
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}
size_t stringtohash(char *word, size_t hashTableSize){
  unsigned int initval;
  unsigned int hashAddress;
  initval = 12345;
  hashAddress = murmur3_32(word, strlen(word), initval);
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
  size_t carga;
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

bool redimensionar_tabla(hash_t* hash) {
  float redim = (float) hash->carga/ (float) hash->largo;
  if( redim > FACTOR_REDIMENSION) {
    hash_campo_t* nueva_tabla = calloc(primos[REDIMENSION_GLOBAL],sizeof(hash_campo_t));
    if(!nueva_tabla) return false;

    //guardo los datos utiles.
    size_t tam_original = hash->largo;
    hash_campo_t* tabla_original = hash->tabla;
    
    //reseteo el hash como nuevo.
    hash->cantidad = 0;
    hash->carga = 0;
    hash->largo = primos[REDIMENSION_GLOBAL];
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
    REDIMENSION_GLOBAL++;
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
  if(!hash) return false;
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
      hash->carga++;
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
  if(!hash) return NULL;
  ssize_t pos = buscar_hash(hash,clave);
  if(pos == -1) return NULL; //NO ESTA

  hash->cantidad--;
  void* dato = hash->tabla[pos].valor;

  free(hash->tabla[pos].clave);
  if(hash->destruir_dato) hash->destruir_dato(hash->tabla[pos].valor);
  
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
  if(!hash) return NULL;
  ssize_t pos = buscar_hash(hash,clave);
  if(pos == -1) return NULL; //NO ESTA

  return hash->tabla[pos].valor;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t* hash, const char* clave) {
  if(!hash) return false;
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
  REDIMENSION_GLOBAL = 0;
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
