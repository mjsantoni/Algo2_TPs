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
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))
#define mix(a,b,c) \
{ \
  a -= c;  a ^= rot(c, 4);  c += b; \
  b -= a;  b ^= rot(a, 6);  a += c; \
  c -= b;  c ^= rot(b, 8);  b += a; \
  a -= c;  a ^= rot(c,16);  c += b; \
  b -= a;  b ^= rot(a,19);  a += c; \
  c -= b;  c ^= rot(b, 4);  b += a; \
}
#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c,4);  \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

uint32_t lookup3 (const void *key,size_t length,uint32_t initval) {
  uint32_t  a,b,c;
  const uint8_t  *k;
  const uint32_t *data32Bit;
  data32Bit = key;
  a = b = c = 0xdeadbeef + (((uint32_t)length)<<2) + initval;
  while (length > 12) {
    a += *(data32Bit++);
    b += *(data32Bit++);
    c += *(data32Bit++);
    mix(a,b,c);
    length -= 12;
  }

  k = (const uint8_t *)data32Bit;
  switch (length) {
    case 12: c += ((uint32_t)k[11])<<24;
    case 11: c += ((uint32_t)k[10])<<16;
    case 10: c += ((uint32_t)k[9])<<8;
    case 9 : c += k[8];
    case 8 : b += ((uint32_t)k[7])<<24;
    case 7 : b += ((uint32_t)k[6])<<16;
    case 6 : b += ((uint32_t)k[5])<<8;
    case 5 : b += k[4];
    case 4 : a += ((uint32_t)k[3])<<24;
    case 3 : a += ((uint32_t)k[2])<<16;
    case 2 : a += ((uint32_t)k[1])<<8;
    case 1 : a += k[0];
             break;
    case 0 : return c;
  }
  final(a,b,c);
  return c;
}

size_t stringtohash(char *word, size_t hashTableSize){
  unsigned int initval;
  unsigned int hashAddress;
  initval = 12345;
  hashAddress = lookup3(word, strlen(word), initval);
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
  char* palabra = strdup(cadena);
  size_t posicion_ini = stringtohash(palabra,largo);
  free(palabra);
  for(size_t i = 0;i<largo;i++) {
    i = (i+posicion_ini)%largo; //para iterar circularmente
    char* clave = hash->tabla[i].clave;
    if(!hash->tabla[i].clave) clave = "\n";
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


bool llenar_tabla_hash(hash_campo_t* tabla,size_t largo) {
  for(int i = 0; i < largo;i++) {
  	tabla[i].valor = NULL;
  	tabla[i].clave = NULL;
  	tabla[i].estado = VACIO;
  }
  return true;
}


/////////////////////////////////////////////////////////////////////

bool redimensionar_tabla(hash_t* hash) {
  float redim = (float) hash->carga/ (float) hash->largo;
  if( redim > FACTOR_REDIMENSION) {
    hash_campo_t* nueva_tabla = malloc(sizeof(hash_campo_t) * primos[REDIMENSION_GLOBAL]);
    if(!nueva_tabla) return false;
    llenar_tabla_hash(nueva_tabla,primos[REDIMENSION_GLOBAL]);

    //guardo los datos utiles.
    size_t tam_original = hash->largo;
    hash_campo_t* tabla_original = hash->tabla;
    
    //reseteo el hash como nuevo.
    hash->cantidad = 0;
    hash->carga = 0;
    hash->largo = primos[REDIMENSION_GLOBAL];
    hash->tabla = nueva_tabla;

    for(int i = 0; i < tam_original;i++) {
      if(tabla_original[i].clave) {
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
	hash_campo_t* tabla = malloc(sizeof(hash_campo_t)*TAM_INICIAL);
	if(!tabla) {
		free(hash);
		return NULL;
	}
	hash->cantidad = 0;
	hash->largo = TAM_INICIAL;
	hash->carga = 0;
	llenar_tabla_hash(tabla,TAM_INICIAL);
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
  size_t hashed = stringtohash(palabra,hash->largo);
  for(ssize_t i = 0;i<hash->largo;i++) {
    i = (i+hashed)%hash->largo; //para iterar circularmente.
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
  char* palabra = strdup(clave);
  ssize_t pos = buscar_hash(hash,palabra);
  free(palabra);
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
