#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.


int comparador (const void *a, const void *b) {
    if(!a || !b) return 0;
    if(*(int*)a > *(int*)b) return 1;
    if(*(int*)a < *(int*)b) return -1;
    return 0;
}


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/
static void prueba_crear_heap_vacio() {
    int arreglo [] = {30,25,13,22,10,5,7,8};
    void* puntero = &arreglo;
    heap_t* heap = heap_crear_arr(puntero,8,comparador);
    print_test("El maximo es 30", *(int*)heap_ver_max(heap) == arreglo[0]);


}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_heap_alumno() {
    /* Ejecuta todas las pruebas unitarias. */
    printf("Maximo es 30\n");
    prueba_crear_heap_vacio();

}