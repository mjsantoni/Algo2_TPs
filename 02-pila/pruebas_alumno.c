#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_crear() {
	printf("---INICIO PRUEBAS DE CREACIÓN---\n");
	pila_t* pila = pila_crear();
	print_test("Se crea la pila vacía", pila != NULL);
	print_test("Está vacía es True", pila_esta_vacia(pila) == true);
	print_test("El tope concuerda y es NULL", pila_ver_tope(pila) == NULL);
	print_test("Intenta desapilar pero recibe NULL", pila_desapilar(pila) == NULL);

	pila_destruir(pila);
	print_test("Se destruye la pila", true);
}

void prueba_apilar_pocos() {
	printf("---INICIO PRUEBAS DE APILAR POCOS---\n");
	pila_t* pila = pila_crear();
	int* vector = malloc(30*sizeof(int));
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		vector[i] = i;
		ok &= pila_apilar(pila,&vector[i]);
		ok &= (pila_ver_tope(pila) == &vector[i]);
	}

	print_test("Se apilaron y se verificaron los topes correctamente", ok);

	pila_destruir(pila);
	print_test("Se destruye la pila", true);
	free(vector);
}


void prueba_apilar_desapilar() {
	printf("---INICIO PRUEBAS DE APILAR Y DESAPILAR POCOS---\n");
	pila_t* pila = pila_crear();
	int* vector = malloc(30*sizeof(int));
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		vector[i] = i;
		ok &= pila_apilar(pila,&vector[i]);
		ok &= (pila_ver_tope(pila) == &vector[i]);
	}
	print_test("Se apilaron y se verificaron los topes correctamente", ok);

	for(int i = 29; i > 0; i--) {
		vector[i] = i;
		ok &= pila_desapilar(pila) == &vector[i];
		ok &= (pila_ver_tope(pila) == &vector[i-1]);
	}
	print_test("Se desapilaron y se verificaron los topes correctamente", ok);

	pila_destruir(pila);
	print_test("Se destruye la pila", true);
	free(vector);
}

void prueba_individual() {
	printf("---INICIO PRUEBAS INDIVIDUALES---\n");
	void* puntero_null = NULL;
	pila_t* pila = pila_crear();
	print_test("Se crea la pila", true);

	int* vector = malloc(11*sizeof(int));
	for(int i = 0; i < 10; i++) {
		vector[i] = i;
	}

	print_test("Está vacía es True", pila_esta_vacia(pila) == true);
	print_test("Apilo un número", pila_apilar(pila,&vector[0]));
	print_test("Está vacía es Falso", pila_esta_vacia(pila) == false);
	print_test("Desapilo el número", pila_desapilar(pila) == &vector[0]);
	print_test("Está vacía es True", pila_esta_vacia(pila) == true);
	print_test("Apilo un número", pila_apilar(pila,&vector[0]));
	print_test("Apilo otro número", pila_apilar(pila,&vector[1]));
	print_test("El tope concuerda", pila_ver_tope(pila) == &vector[1]);
	print_test("Apilo otro número", pila_apilar(pila,&vector[2]));
	print_test("Desapilo un número", pila_desapilar(pila) == &vector[2]);
	print_test("El tope concuerda", pila_ver_tope(pila) == &vector[1]);
	print_test("Está vacía es Falso", pila_esta_vacia(pila) == false);
	print_test("Desapilo un número", pila_desapilar(pila) == &vector[1]);
	print_test("Desapilo un número", pila_desapilar(pila) == &vector[0]);
	print_test("Está vacía es True", pila_esta_vacia(pila) == true);
	print_test("No hay tope por lo tanto es NULL", pila_ver_tope(pila) == NULL);
	print_test("Intento desapilar el último número pero recibe NULL", pila_desapilar(pila) == NULL);
	print_test("Se apila un NULL", pila_apilar(pila,puntero_null));
	print_test("El tope es NULL", pila_ver_tope(pila) == NULL);

	pila_destruir(pila);
	print_test("Se destruye la pila", true);
	free(vector);
}

void pruebas_volumen() {
	printf("---INICIO PRUEBAS DE VOLUMEN---\n");
	pila_t* pila = pila_crear();
	int tam = 100000;
	bool ok = true;
	int* vector = malloc(tam*sizeof(int));

	for(int i = 0; i < tam; i++) {
		vector[i] = i;
		ok &= pila_apilar(pila,&vector[i]);
		ok &= (pila_ver_tope(pila) == &vector[i]);
	}
	print_test("Se pudieron apilar y ver topes de todos los elementos", ok);

	for(int i = tam-1; i > 0; i--) {
		vector[i] = i;
		ok &= (pila_desapilar(pila) == &vector[i]);
		ok &= (pila_ver_tope(pila) == &vector[i-1]);
	}
	print_test("Se pudieron desapilar y ver topes de todos los elementos", ok);
	print_test("Desapilo el número restante", pila_desapilar(pila) == &vector[0]);
	print_test("Está vacía es True", pila_esta_vacia(pila) == true);	

	pila_destruir(pila);
	print_test("Se destruye la pila", true);
	free(vector);

}

void pruebas_pila_alumno() {

    prueba_crear();
    prueba_individual();
    prueba_apilar_pocos();
    prueba_apilar_desapilar();
    pruebas_volumen();
}