#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "testing.h"
#include "heap.h"

/////////////////////////////////////////////////////////////////////

int comparador(const void* a,const void* b) {
	return *(int*)a - *(int*)b;
}

/////////////////////////////////////////////////////////////////////

void pruebas_crear() {
	heap_t* heap = heap_crear(comparador);
	void* puntero_null = NULL;
	
	print_test("Se crea el heap vacío", heap);
	print_test("Está vacío es True", heap_esta_vacio(heap));
	print_test("El máximo concuerda y es NULL", heap_ver_max(heap) == NULL);
	print_test("Intenta desencolar pero recibe NULL", heap_desencolar(heap) == NULL);
	print_test("Encolar un NULL es falso", !heap_encolar(heap, puntero_null));
	
	heap_destruir(heap,NULL);
	print_test("Se destruye el heap", true);	
}

/////////////////////////////////////////////////////////////////////

void pruebas_encolar() {
	heap_t* heap = heap_crear(comparador);
	int vector[] = {30,25,13,22,10,5,7,8,28};

	print_test("Se crea el heap vacío", heap);
	print_test("Está vacío es True", heap_esta_vacio(heap));
	print_test("El máximo concuerda y es NULL", !heap_ver_max(heap));

	print_test("Encolo un número", heap_encolar(heap,&vector[0]));
	print_test("El máximo concuerda", heap_ver_max(heap) == &vector[0]);
	print_test("Cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Está vacío es False", !heap_esta_vacio(heap));
	print_test("Encolo otro número", heap_encolar(heap, &vector[1]));
	print_test("Encolo otro número", heap_encolar(heap, &vector[2]));
	print_test("Encolo otro número", heap_encolar(heap, &vector[3]));
	print_test("Encolo otro número", heap_encolar(heap, &vector[4]));
	print_test("Encolo otro número", heap_encolar(heap, &vector[5]));
	print_test("Encolo otro número", heap_encolar(heap, &vector[6]));
	print_test("Encolo otro número", heap_encolar(heap, &vector[7]));
	print_test("El máximo concuerda", heap_ver_max(heap) == &vector[0]);
	print_test("Cantidad es 8", heap_cantidad(heap) == 8);

	print_test("Encolo otro número que genera upheap", heap_encolar(heap, &vector[8]));
	print_test("Desencolo el primer número generando downheap", heap_desencolar(heap) == &vector[0]);
	print_test("Está vacío es False", !heap_esta_vacio(heap));
	print_test("El máximo concuerda", heap_ver_max(heap) == &vector[8]);
	print_test("Cantidad es 8", heap_cantidad(heap) == 8);

	heap_destruir(heap,NULL);
	print_test("Se destruye el heap", true);
}

/////////////////////////////////////////////////////////////////////

void pruebas_encolar_en_heap() {
	heap_t* heap = heap_crear(comparador);	
	bool ok = true;
	int vector[] = {30,25,13,22,10,5,7,8,28};

	for(int i = 0; i < 9; i++) {
		int* dato = malloc(sizeof(int));
		*dato = vector[i];
		ok &= heap_encolar(heap,dato);
	}

	print_test("Se pudieron encolar todos los elementos", ok);
	print_test("Cantidad es 9", heap_cantidad(heap) == 9);
	print_test("Está vacío es False", !heap_esta_vacio(heap));
	heap_destruir(heap,free);
	print_test("Se destruye el heap y sus datos", true);
}

/////////////////////////////////////////////////////////////////////

void prueba_heapify() {
	int* vector = malloc(sizeof(int)*7);
	print_test("Se crea vector desordenado", vector);
	vector[0] = 50;
	vector[1] = 1;
	vector[2] = 2;
	vector[3] = 3;
	vector[4] = 30;
	vector[5] = 10;
	vector[6] = 80;
	void* v0 = &vector[0];
	void* v1 = &vector[1];
	void* v2 = &vector[2];
	void* v3 = &vector[3];
	void* v4 = &vector[4];
	void* v5 = &vector[5];
	void* v6 = &vector[6];
	void** puntero = malloc(sizeof(void*) * 7);
	puntero[0] = v0;
	puntero[1] = v1;
	puntero[2] = v2;
	puntero[3] = v3;
	puntero[4] = v4;
	puntero[5] = v5;
	puntero[6] = v6;

    heap_t* heap = heap_crear_arr(puntero,7,comparador);
    print_test("Llama a Heapify", heap);
    print_test("El maximo concuerda", heap_ver_max(heap) == v6);
    print_test("Cantidad es 7", heap_cantidad(heap) == 7);

    heap_destruir(heap,NULL);
	print_test("Se destruye el heap", true);
	free(vector);
	free(puntero);
}

/////////////////////////////////////////////////////////////////////

void prueba_heapsort() {
	int* vector = malloc(sizeof(int)*7);
	print_test("Se crea vector desordenado", vector);
	vector[0] = 50;
	vector[1] = 1;
	vector[2] = 2;
	vector[3] = 3;
	vector[4] = 30;
	vector[5] = 10;
	vector[6] = 80;
	void* v0 = &vector[0];
	void* v1 = &vector[1];
	void* v2 = &vector[2];
	void* v3 = &vector[3];
	void* v4 = &vector[4];
	void* v5 = &vector[5];
	void* v6 = &vector[6];
	void** puntero = malloc(sizeof(void*) * 7);
	puntero[0] = v0;
	puntero[1] = v1;
	puntero[2] = v2;
	puntero[3] = v3;
	puntero[4] = v4;
	puntero[5] = v5;
	puntero[6] = v6;

	heap_sort(puntero,7,comparador);

	bool ok = true;
	ok &= puntero[0] == v1;
	ok &= puntero[1] == v2;
	ok &= puntero[2] == v3;
	ok &= puntero[3] == v5;
	ok &= puntero[4] == v4;
	ok &= puntero[5] == v0;
	ok &= puntero[6] == v6;
	print_test("El arreglo está ordenado", ok);

	free(vector);
	free(puntero);
}

/////////////////////////////////////////////////////////////////////

void pruebas_heap_alumno() {
	printf("-----CREAR VACIO-----\n");
	pruebas_crear();
	printf("-----ENCOLAR-DESENCOLAR Y VERIFICAR UP/DOWNHEAP-----\n");
	pruebas_encolar();
	printf("-----ENCOLAR MEMORIA DINAMICA-----\n");
	pruebas_encolar_en_heap();
	printf("-----HEAPIFY-----\n");
	prueba_heapify();
	printf("-----HEAPSORT-----\n");
	prueba_heapsort();
}
