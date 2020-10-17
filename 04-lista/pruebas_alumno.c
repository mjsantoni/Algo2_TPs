#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_crear_lista() {
	printf("---INICIO PRUEBAS DE CREACIÓN---\n");
	lista_t* lista = lista_crear();
	print_test("Se crea la lista vacía", lista != NULL);
	print_test("Está vacía es True", lista_esta_vacia(lista) == true);
	print_test("El primero concuerda y es NULL", lista_ver_primero(lista) == NULL);
	print_test("El último concuerda y es NULL",lista_ver_ultimo(lista) == NULL);
	print_test("Intenta borrar primero pero recibe NULL", lista_borrar_primero(lista) == NULL);

	lista_destruir(lista,NULL);
	print_test("Se destruye la lista", true);
}

void prueba_individual_lista() {
	printf("---INICIO PRUEBAS INDIVIDUALES---\n");
	void* puntero_null = NULL;
	lista_t* lista = lista_crear();
	print_test("Se crea la lista", lista != NULL);
	int* vector = malloc(11*sizeof(int));
	for(int i = 0; i < 10; i++) {
		vector[i] = i;
	}

	print_test("Está vacía es True", lista_esta_vacia(lista) == true);
	print_test("Agrego un número", lista_insertar_primero(lista,&vector[0]));
	print_test("Veo si es el primero", lista_ver_primero(lista) == &vector[0]);
	print_test("Veo si es el último", lista_ver_ultimo(lista) == &vector[0]);
	print_test("Está vacía es Falso", lista_esta_vacia(lista) == false);
	print_test("Borro el primero", lista_borrar_primero(lista) == &vector[0]);
	print_test("Está vacía es True", lista_esta_vacia(lista) == true);
	print_test("Agrego un número al inicio", lista_insertar_primero(lista,&vector[0]));
	print_test("Agrego otro número al final", lista_insertar_ultimo(lista,&vector[1]));
	print_test("El primero concuerda", lista_ver_primero(lista) == &vector[0]);
	print_test("El último concuerda", lista_ver_ultimo(lista) == &vector[1]);
	print_test("El largo es el correcto (2)",lista_largo(lista) == 2);
	print_test("Agrego otro número al inicio", lista_insertar_primero(lista,&vector[2]));
	print_test("El primero concuerda", lista_ver_primero(lista) == &vector[2]);
	print_test("Borro el primero", lista_borrar_primero(lista) == &vector[2]);
	print_test("El primero concuerda", lista_ver_primero(lista) == &vector[0]);
	print_test("Está vacía es Falso", lista_esta_vacia(lista) == false);
	print_test("Borro un número", lista_borrar_primero(lista) == &vector[0]);
	print_test("Borro un número", lista_borrar_primero(lista) == &vector[1]);
	print_test("Está vacía es True", lista_esta_vacia(lista) == true);
	print_test("No hay primero por lo tanto es NULL", lista_ver_primero(lista) == NULL);
	print_test("No hay último por lo tanto es NULL", lista_ver_ultimo(lista) == NULL);
	print_test("Intento borrar el primer número pero recibe NULL", lista_borrar_primero(lista) == NULL);
	print_test("Se agrega un NULL", lista_insertar_primero(lista,puntero_null));
	print_test("El primero es NULL", lista_ver_primero(lista) == NULL);
	print_test("Borro el NULL", lista_borrar_primero(lista) == NULL);

	lista_destruir(lista,NULL);
	print_test("Se destruye la lista", true);
	free(vector);
}

void prueba_agregar_heap() {
	lista_t* lista = lista_crear();
	printf("---INICIO PRUEBAS ENCOLAR POCOS EN HEAP---\n");
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		int* dato = malloc(sizeof(int));
		*dato = i;
		ok &= lista_insertar_ultimo(lista,dato);
		ok &= (dato == lista_ver_ultimo(lista));
	}
	print_test("Se pudieron agregar todos los elementos", ok);
	lista_destruir(lista,free);
	print_test("Se destruye la lista y sus datos", true);
}

void prueba_agregar_borrar_lista() {
	printf("---INICIO PRUEBAS DE ENCOLAR Y DESENCOLAR POCOS---\n");
	lista_t* lista = lista_crear();
	int* vector = malloc(30*sizeof(int));
	int* vector2 = malloc(30*sizeof(int));
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		vector[i] = i;
		ok &= lista_insertar_ultimo(lista,&vector[i]);
		ok &= (lista_ver_ultimo(lista) == &vector[i]);
	}
	print_test("Se agregaron y verificaron a lo último correctamente", ok);
	for(int i = 0; i < 30; i++) {
		ok &= (lista_borrar_primero(lista) == &vector[i]);
	}
	print_test("Se borraron y verificaron los elementos del agregado último correctamente", ok);
	
	for(int i = 0; i < 30; i++) {
		vector2[i] = i;
		ok &= lista_insertar_primero(lista,&vector2[i]);
		ok &= (lista_ver_primero(lista) == &vector2[i]);
	}
	print_test("Se agregaron y verificaron al inicio correctamente", ok);
	for(int i = 29; i > 0; i--) {
		ok &= (lista_borrar_primero(lista) == &vector2[i]);
	}
	print_test("Se borraron y verificaron los elementos del agregado primero correctamente", ok);

	lista_destruir(lista,NULL);
	print_test("Se destruye la lista", true);
	free(vector);
	free(vector2);
}

void pruebas_volumen_lista() {
	printf("---INICIO PRUEBAS DE VOLUMEN---\n");
	lista_t* lista = lista_crear();
	int tam = 100000;
	bool ok = true;
	int* vector = malloc(tam*sizeof(int));

	for(int i = 0; i < tam; i++) {
		vector[i] = i;
		ok &= lista_insertar_ultimo(lista,&vector[i]);
		ok &= (lista_ver_primero(lista) == &vector[0]);
	}
	print_test("Se pudieron agregar todos los elementos", ok);

	for(int i = 0; i < tam; i++) {
		ok &= (lista_borrar_primero(lista) == &vector[i]);
	}

	print_test("Se pudieron borrar y ver todos los primeros", ok);
	print_test("Está vacía es True", lista_esta_vacia(lista) == true);	

	lista_destruir(lista,NULL);
	print_test("Se destruye la lista", true);
	free(vector);
}

void prueba_iterador_externo() {
	printf("\n-----INICIO PRUEBAS ITERADOR EXTERNO-----\n");
	lista_t* lista = lista_crear();
	print_test("Se crea la lista vacía", lista != NULL);
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("Se crea el iterador externo", iter != NULL);

	int* vector = malloc(11*sizeof(int));
	for(int i = 0; i < 10; i++) {
		vector[i] = i;
	}

	print_test("Intento avanzar en el iterador == Falso", lista_iter_avanzar(iter) == false);
	print_test("Ver actual es NULL", lista_iter_ver_actual(iter) == NULL);
	print_test("Iterar al final es True inmediatamente", lista_iter_al_final(iter) == true);
	print_test("Agrego un número", lista_iter_insertar(iter,&vector[0]) == true);
	print_test("Verifico si es el primero de la lista", lista_ver_primero(lista) == &vector[0]);
	print_test("Verifico número == actual", lista_iter_ver_actual(iter) == &vector[0]);
	print_test("Largo de la lista igual a 1", lista_largo(lista) == 1);
	print_test("Borro el actual", lista_iter_borrar(iter) == &vector[0]);
	print_test("Verifico el primero de la lista == NULL", lista_ver_primero(lista) == NULL);
	print_test("Agrego un número", lista_iter_insertar(iter,&vector[0]) == true);
	print_test("Verifico número == actual", lista_iter_ver_actual(iter) == &vector[0]);
	print_test("Agrego otro número", lista_iter_insertar(iter,&vector[1]) == true);
	print_test("Verifico número == actual", lista_iter_ver_actual(iter) == &vector[1]);
	print_test("Agrego otro número", lista_iter_insertar(iter,&vector[2]) == true);
	print_test("Verifico número == actual y luego avanzo", lista_iter_ver_actual(iter) == &vector[2]);
	lista_iter_avanzar(iter);
	print_test("Actual == segundo número en la lista", lista_iter_ver_actual(iter) == &vector[1]);
	print_test("Inserto un número en la posición actual", lista_iter_insertar(iter,&vector[3]) == true);
	print_test("Largo de la lista igual a 4", lista_largo(lista) == 4);
	print_test("El actual ahora es el nuevo número", lista_iter_ver_actual(iter) == &vector[3]);
	lista_iter_avanzar(iter);
	print_test("Avanzo y borro el actual", lista_iter_borrar(iter) == &vector[1]);
	print_test("Largo de la lista igual a 3", lista_largo(lista) == 3);
	print_test("El actual es el próximo número", lista_iter_ver_actual(iter) == &vector[0]);
	print_test("Iterar al final es Falso", lista_iter_al_final(iter) == false);
	print_test("Avanzar es True", lista_iter_avanzar(iter) == true);
	print_test("Iterar al final es True", lista_iter_al_final(iter) == true);
	print_test("Agrego otro número", lista_iter_insertar(iter,&vector[4]) == true);
	print_test("Verifico si es el último de la lista", lista_ver_ultimo(lista) == &vector[4]);
	print_test("Borro el actual", lista_iter_borrar(iter) == &vector[4]);
	print_test("Verifico cuál es el último de la lista", lista_ver_ultimo(lista) == &vector[0]);
	print_test("Intento avanzar nuevamente == Falso", lista_iter_avanzar(iter) == false);
	print_test("Intento borrar pero devuelve NULL", lista_iter_borrar(iter) == NULL);
	print_test("Ver actual es NULL",lista_iter_ver_actual(iter) == NULL);

	lista_iter_destruir(iter);
	print_test("Se destruye el iterador", true);
	lista_destruir(lista,NULL);
	print_test("Se destruye la lista", true);

	free(vector);
}

bool iterar_pruebas(void *elemento, void *extra) {
	int* contador = extra;
    if (*contador >= 20) return false;
    ++(*contador);
    return true;
}

void ejecutar_iterador_interno(lista_t *lista) {
    int contador = 0;
    lista_iterar(lista, iterar_pruebas, &contador);
    print_test("El iterador interno iteró correctamente", contador == 20);
}

void prueba_iterador_interno() {
	printf("\n-----INICIO PRUEBAS ITERADOR INTERNO-----\n");
	lista_t* lista = lista_crear();
	print_test("Se crea la lista vacía", lista != NULL);
	int* vector = malloc(30*sizeof(int));
	bool ok = true;

	for(int i = 0; i < 30; i++) {
		vector[i] = i;
		ok &= lista_insertar_ultimo(lista,&vector[i]);
		ok &= (lista_ver_ultimo(lista) == &vector[i]);
	}
	print_test("Se agregaron todos los elementos", ok);
	ejecutar_iterador_interno(lista);
	free(vector);
	lista_destruir(lista,NULL);
}

 void pruebas_lista_alumno() {
 	printf("-----INICIO PRUEBAS DE LISTA-----\n");
    prueba_crear_lista();
    prueba_individual_lista();
    prueba_agregar_heap();
    prueba_agregar_borrar_lista();
    pruebas_volumen_lista();
    prueba_iterador_externo();
    prueba_iterador_interno();
}