#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <string.h>
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_split() {
	char* test = "abc,def,ghi";
	// Ejemplo de arreglo dinámico de cadenas
	char **strv = malloc(sizeof(char*) * 4);
	strv[0] = strdup("abc");
	strv[1] = strdup("def");
	strv[2] = strdup("ghi");
	strv[3] = NULL;
	char** res = split(test, ',');
	printf("ESPERADO %s\n", strv[0]);
	printf("ESPERADO %s\n", strv[1]);
	printf("ESPERADO %s\n", strv[2]);
	printf("ESPERADO %s\n", strv[3]);

	printf("RESULTADO %s\n", res[0]);
	printf("RESULTADO %s\n", res[1]);
	printf("RESULTADO %s\n", res[2]);
	printf("RESULTADO %s\n", res[3]);
	
	print_test("Me devuelve lo correcto?", strcmp(strv[0],res[0]) == 0);
	print_test("Me devuelve lo correcto?", strcmp(strv[1],res[1]) == 0);
	print_test("Me devuelve lo correcto?", strcmp(strv[2],res[2]) == 0);
	print_test("Me devuelve lo correcto?", strv[3] == res[3]);
	
	free_strv(strv);
	free_strv(res);

	
	char **strv2 = malloc(sizeof(char*) * 3);
	char* test2 = ",";
	strv2[0] = strdup("");
	strv2[1] = strdup("");
	strv2[2] = NULL;
	
	//strv2[0] = test2;
	char** res2 = split(test2, ',');

	printf("ESPERADO %s\n", strv2[0]);
	printf("ESPERADO %s\n", strv2[1]);

	printf("RESULTADO %s\n", res2[0]);
	printf("RESULTADO %s\n", res2[1]);

	print_test("Me devuelve lo correcto? Prueba 2", strcmp(strv2[0],res2[0]) == 0);
	print_test("Me devuelve lo correcto? Prueba 2", strcmp(strv2[1],res2[1]) == 0);
	print_test("Me devuelve lo correcto?", strv2[2] == res2[2]);

	free_strv(strv2);
	free_strv(res2);
	
	char **strv3 = malloc(sizeof(char*) * 4);
	strv3[0] = strdup("abc");
	strv3[1] = strdup("");
	strv3[2] = strdup("def");
	strv3[3] = NULL;
	char* test3 = "abc,,def";
	char** res3 = split(test3, ',');

	printf("ESPERADO %s\n", strv3[0]);
	printf("ESPERADO %s\n", strv3[1]);
	printf("ESPERADO %s\n", strv3[2]);
	printf("ESPERADO %s\n", strv3[3]);

	printf("RESULTADO %s\n", res3[0]);
	printf("RESULTADO %s\n", res3[1]);
	printf("RESULTADO %s\n", res3[2]);
	printf("RESULTADO %s\n", res3[3]);

	print_test("Me devuelve lo correcto? Prueba 3", strcmp(strv3[0],res3[0]) == 0);
	print_test("Me devuelve lo correcto? Prueba 3", strcmp(strv3[1],res3[1]) == 0);
	print_test("Me devuelve lo correcto? Prueba 3", strcmp(strv3[2],res3[2]) == 0);
	print_test("Me devuelve lo correcto? Prueba 3", strv3[3] == res3[3]);

	free_strv(strv3);
	free_strv(res3);
}
void prueba_join() {
	char* test = "abc,def,ghi";
	// Ejemplo de arreglo dinámico de cadenas
	char **strv = malloc(sizeof(char*) * 4);
	strv[0] = strdup("abc");
	strv[1] = strdup("def");
	strv[2] = strdup("ghi");
	strv[3] = NULL;
	char** res = split(test, ',');
	char* output = join(strv, ';');
	printf("QUE SALE: %s\n", output);

	free_strv(strv);
	free_strv(res);
	free(output);


	char **strv2 = malloc(sizeof(char*) * 2);
	char* test2 = "abc";
	strv2[0] = strdup("abc");
	strv2[1] = NULL;
	char** res2 = split(test2, '\0');
	char* output2 = join(strv2, ',');
	printf("QUE SALE PRUEBA 2: %s\n", output2);

	free_strv(strv2);
	free_strv(res2);
	free(output2);


	char **strv3 = malloc(sizeof(char*) * 1);
	char* test3 = malloc(sizeof(char));
	test3[0] = '\0';
	//strv3[0] = strdup(",");
	strv3[0] = NULL;
	char** res3 = split(test3, ',');
	char* output3 = join(strv3, ',');
	printf("QUE SALE PRUEBA 3: %s\n", output3);
	printf("QUE SALE PRUEBA 3: %s\n", test3);
	//print_test("prueba 3", strcmp(output3,",") == 0);
	print_test("prueba 3", output3[0] == test3[0]);

	free(test3);
	free_strv(strv3);
	free_strv(res3);
	free(output3);
}

 void pruebas_tp1_alumno() {
 	printf("-----INICIO PRUEBAS DE STRUTIL-----\n");
  
    prueba_split();
    prueba_join();
    //prueba_grep();
    //prueba_dc();

}