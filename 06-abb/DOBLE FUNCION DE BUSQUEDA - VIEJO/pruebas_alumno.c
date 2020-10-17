#include "abb.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/
static void prueba_crear_abb_vacio() {

    abb_t* abb = abb_crear(strcmp,NULL);
    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));
    abb_destruir(abb);
}

static void prueba_abb_insertar() {

    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_insertar_desorden() {

    abb_t* abb = abb_crear(strcmp,NULL);
    int numero [] = {1,2,3,4,5,6,7,8,9,10,11,12};
    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "M",&numero[0]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "H",&numero[1]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "P",&numero[2]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "B",&numero[3]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "Y",&numero[4]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "I",&numero[5]));
    print_test("Prueba abb editar elemento", abb_guardar(abb, "Y",&numero[6]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "Q",&numero[7]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "L",&numero[8]));
    print_test("Prueba abb insertar elemento", abb_guardar(abb, "D",&numero[9]));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "M"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "H"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "P"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "B"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "Y"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "I"));
    print_test("Prueba abb pertenece elemento", !abb_pertenece(abb, "N"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "Q"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "L"));
    print_test("Prueba abb pertenece elemento", abb_pertenece(abb, "D"));
    print_test("Prueba abb pertenece elemento", !abb_pertenece(abb, "F"));
    print_test("Cantidad de elementos",abb_cantidad(abb) == 9);

    int num1 = *(int*) abb_obtener(abb,"M");
    int num2 = *(int*) abb_obtener(abb,"H");
    int num3 = *(int*) abb_obtener(abb,"P");
    int num4 = *(int*) abb_obtener(abb,"B");
    int num5 = *(int*) abb_obtener(abb,"Y");


    print_test("Prueba abb obtener elemento", (num1 == numero[0]));
    print_test("Prueba abb obtener elemento", (num2 == numero[1]));
    print_test("Prueba abb obtener elemento", (num3 == numero[2]));
    print_test("Prueba abb obtener elemento", (num4 == numero[3]));
    print_test("Prueba abb obtener elemento", (num5 == numero[6]));
    print_test("Prueba abb obtener elemento", (!abb_obtener(abb,"A")));

    int num01 = *(int*) abb_borrar(abb,"M");
    int num02 = *(int*) abb_borrar(abb,"I");
    int num03 = *(int*) abb_borrar(abb,"P");
    print_test("Prueba abb borrar elemento 2 hijo", (num01 == numero[0]));
    print_test("Prueba abb borrar elemento 1 hijo", (num02 == numero[5]));
    print_test("Prueba abb borrar elemento 2 hijo", (num03 == numero[2]));
    print_test("Prueba abb obtener elemento", (!abb_obtener(abb,"M")));

    abb_destruir(abb);
}


static void prueba_abb_borrar() {

    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia() {

    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null() {
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

void prueba_reemplazar_con_free() {

    abb_t* abb = abb_crear(strcmp,free);
    char* clave1 = "perro";
    char* valor1 = malloc(sizeof(char));
    char* valor2 = malloc(sizeof(char));
    valor1[0] = 'a';
    valor2[0] = 'b';
    print_test("Se crea el abb vacio", abb != NULL);
    print_test("Insertó clave1", abb_guardar(abb, clave1, valor1));
    print_test("Insertó clave1 cambiando valor", abb_guardar(abb, clave1, valor2));
    print_test("Obtener es nuevo valor",abb_obtener(abb, clave1) == valor2);
    abb_destruir(abb);

}


static void prueba_abb_volumen(size_t largo, bool debug) {

    abb_t* abb = abb_crear(strcmp,NULL);
    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);
    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb borrar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Destruye el abb y crea uno nuevo que sí libera */
    abb_destruir(abb);
    abb = abb_crear(strcmp,free);

    /* Inserta 'largo' parejas en el abb */
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);
    /* Destruye el abb - debería liberar los enteros */
    abb_destruir(abb);
}
/* ******************************************************************
 *                        PRUEBAS ITERADOR
 * *****************************************************************/

static ssize_t buscar(const char* clave, char* claves[], size_t largo) {
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_iterar_abb_vacio() {

    abb_t* abb = abb_crear(NULL,NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


static void prueba_abb_iterar() {

    abb_t* abb = abb_crear(strcmp,NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));


    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo) {

    abb_t* abb = abb_crear(strcmp,NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        valores[i] = i;
        ok = abb_guardar(abb, claves[i], &valores[i]);
        if (!ok) break;
    }

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

////////////////////Iterador interno/////////////////////////

bool iterar_pruebas(const char *clave, void *dato, void *extra) {
    int* contador = extra;
    if (*contador >= 2) return false;
    ++(*contador);
    return true;
}

void ejecutar_iterador_interno(abb_t* abb) {
    int contador = 0;
    abb_in_order(abb, iterar_pruebas, &contador);
    print_test("El iterador interno iteró correctamente", contador == 2);
}

void prueba_abb_iterador_interno() {

    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    char *clave4 = "delfin", *valor4 = "ikik";
    char *clave5 = "pato", *valor5 = "cuak";
    char *clave6 = "sifon", *valor6 = "soda";
    char *clave7 = "xilofon", *valor7 = "music";

    print_test("Se crea el abb vacio", abb != NULL);
    print_test("Insertó clave1", abb_guardar(abb, clave1, valor1));
    print_test("Insertó clave2", abb_guardar(abb, clave2, valor2));
    print_test("Insertó clave3", abb_guardar(abb, clave3, valor3));
    print_test("Insertó clave4", abb_guardar(abb, clave4, valor4));
    print_test("Insertó clave5", abb_guardar(abb, clave5, valor5));
    print_test("Insertó clave6", abb_guardar(abb, clave6, valor6));
    print_test("Insertó clave7", abb_guardar(abb, clave7, valor7));

    ejecutar_iterador_interno(abb);
    abb_destruir(abb);
}



/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_alumno() {
    /* Ejecuta todas las pruebas unitarias. */
    printf("CREAR VACIO\n");
    prueba_crear_abb_vacio();
    printf("DESORDEN\n");
    prueba_insertar_desorden();
    printf("INSERTAR\n");
    prueba_abb_insertar();
    printf("BORRAR\n");
    prueba_abb_borrar();
    printf("CLAVE VACIA\n");
    prueba_abb_clave_vacia();
    printf("VALOR NULL\n");
    prueba_abb_valor_null();
    printf("REEMPLAZAR CON FREE\n");
    prueba_reemplazar_con_free();
    printf("VOLUMEN\n");
    prueba_abb_volumen(50, true);
    printf("ITERAR VACIO\n");
    prueba_iterar_abb_vacio();
    printf("ITERAR\n");
    prueba_abb_iterar();
    printf("ITERAR VOLUMEN\n");
    prueba_abb_iterar_volumen(50);
    printf("ITERADOR INTERNO\n");
    prueba_abb_iterador_interno();
    
}