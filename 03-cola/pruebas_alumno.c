#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_crear() {
	printf("---INICIO PRUEBAS DE CREACIÓN---\n");
	cola_t* cola = cola_crear();
	print_test("Se crea la cola vacía", cola != NULL);
	print_test("Está vacía es True", cola_esta_vacia(cola) == true);
	print_test("El primero concuerda y es NULL", cola_ver_primero(cola) == NULL);
	print_test("Intenta desencolar pero recibe NULL", cola_desencolar(cola) == NULL);

	cola_destruir(cola,NULL);
	print_test("Se destruye la cola", true);
}

void prueba_encolar_heap() {
	cola_t* cola = cola_crear();
	printf("---INICIO PRUEBAS ENCOLAR POCOS EN HEAP---\n");
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		int* dato = malloc(sizeof(int));
		*dato = i;
		ok &= cola_encolar(cola,dato);
	}
	print_test("Se pudieron encolar todos los elementos", ok);
	cola_destruir(cola,free);
	print_test("Se destruye la cola y sus datos", true);
}

void prueba_encolar_desencolar() {
	printf("---INICIO PRUEBAS DE ENCOLAR Y DESENCOLAR POCOS---\n");
	cola_t* cola = cola_crear();
	int* vector = malloc(30*sizeof(int));
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		vector[i] = i;
		ok &= cola_encolar(cola,&vector[i]);
		ok &= (cola_ver_primero(cola) == &vector[0]);
	}
	print_test("Se apilaron y se verificaron los primeros correctamente", ok);

	for(int i = 0; i < 30; i++) {
		ok &= cola_desencolar(cola) == &vector[i];
	}
	print_test("Se desapilaron y se verificaron los primeros correctamente", ok);
	cola_destruir(cola,NULL);
	print_test("Se destruye la cola", true);
	free(vector);
}

void prueba_individual() {
	printf("---INICIO PRUEBAS INDIVIDUALES---\n");
	void* puntero_null = NULL;
	cola_t* cola = cola_crear();
	print_test("Se crea la cola", true);
	int* vector = malloc(11*sizeof(int));
	for(int i = 0; i < 10; i++) {
		vector[i] = i;
	}

	print_test("Está vacía es True", cola_esta_vacia(cola) == true);
	print_test("Encolo un numero", cola_encolar(cola,&vector[0]));
	print_test("Veo si es el primero", cola_ver_primero(cola) == &vector[0]);
	print_test("Está vacía es Falso", cola_esta_vacia(cola) == false);
	print_test("Desencolo el número", cola_desencolar(cola) == &vector[0]);
	print_test("Está vacía es True", cola_esta_vacia(cola) == true);
	print_test("Encolo un número", cola_encolar(cola,&vector[0]));
	print_test("Encolo otro número", cola_encolar(cola,&vector[1]));
	print_test("El primero concuerda", cola_ver_primero(cola) == &vector[0]);
	print_test("Encolo otro número", cola_encolar(cola,&vector[2]));
	print_test("Desencolo un número", cola_desencolar(cola) == &vector[0]);
	print_test("El primero concuerda", cola_ver_primero(cola) == &vector[1]);
	print_test("Está vacía es Falso", cola_esta_vacia(cola) == false);
	print_test("Desencolo un número", cola_desencolar(cola) == &vector[1]);
	print_test("Desencolo un número", cola_desencolar(cola) == &vector[2]);
	print_test("Está vacía es True", cola_esta_vacia(cola) == true);
	print_test("No hay primero por lo tanto es NULL", cola_ver_primero(cola) == NULL);
	print_test("Intento desapilar el último número pero recibe NULL", cola_desencolar(cola) == NULL);
	print_test("Se apila un NULL", cola_encolar(cola,puntero_null));
	print_test("El primero es NULL", cola_ver_primero(cola) == NULL);
	print_test("Desencolo el NULL", cola_desencolar(cola) == NULL);

	cola_destruir(cola,NULL);
	print_test("Se destruye la cola", true);
	free(vector);
}

void pruebas_volumen() {
	printf("---INICIO PRUEBAS DE VOLUMEN---\n");
	cola_t* cola = cola_crear();
	int tam = 100000;
	bool ok = true;
	int* vector = malloc(tam*sizeof(int));

	for(int i = 0; i < tam; i++) {
		vector[i] = i;
		ok &= cola_encolar(cola,&vector[i]);
		ok &= (cola_ver_primero(cola) == &vector[0]);
	}
	print_test("Se pudieron encolar todos los elementos", ok);

	for(int i = 0; i < tam; i++) {
		ok &= (cola_desencolar(cola) == &vector[i]);
	}

	print_test("Se pudieron desapilar y ver todos los primeros", ok);
	print_test("Está vacía es True", cola_esta_vacia(cola) == true);	

	cola_destruir(cola,NULL);
	print_test("Se destruye la cola", true);
	free(vector);
}

void pruebas_cola_alumno() {

    prueba_crear();
    prueba_individual();
    prueba_encolar_heap();
    prueba_encolar_desencolar();
    pruebas_volumen();
 
}