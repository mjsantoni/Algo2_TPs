#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
size_t largo_total_join = 0;
/*
 * Devuelve en un arreglo dinámico terminado en NULL con todos los subsegmentos
 * de ‘str’ separados por el carácter ‘sep’. Tanto el arreglo devuelto como las
 * cadenas que contiene son allocadas dinámicamente.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica del
 * arreglo devuelto. La función devuelve NULL si falló alguna llamada a
 * malloc(), o si ‘sep’ es '\0'.
 */

char** malloc_split(const char* str, char sep) {
	int cant_sep = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == sep) cant_sep++;
	}
	char** resultado = malloc(sizeof(char*) * (cant_sep+2));
	if (resultado == NULL) return NULL;
	resultado[cant_sep+1] = NULL;
	return resultado;
}

char** split(const char* str, char sep) {
	char** resultado = malloc_split(str, sep);
	if(!resultado) return NULL;

	int indice = 0;
	int comienzo = 0;
	int cont_resultado = 0;

	for(indice = 0; indice <= strlen(str); indice++) {
		if (str[indice] == sep || indice == strlen(str)) {
			int largo_cadena = indice - comienzo;
			
			if (largo_cadena == 0) { //CASO BORDE
				char cadena[largo_cadena+1];
				cadena[0] = '\0';
				resultado[cont_resultado] = strdup(cadena);
				cont_resultado++;
				comienzo = indice+1;
			}else{ //CASO GENERAL
				char cadena[largo_cadena+1];
				for(int i = comienzo,j = 0; i < indice; i++,j++) {
					cadena[j] = str[i];
				}
				cadena[largo_cadena] = '\0';
				resultado[cont_resultado] = strdup(cadena);
				cont_resultado++;
				comienzo = indice+1;
			}
		}
	}
	return resultado;
}

/*
 * Devuelve una cadena, allocada dinámicamente, resultado de unir todas las
 * cadenas del arreglo terminado en NULL ‘strv’.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica de la
 * cadena devuelta. La función devuelve NULL si no se pudo allocar memoria.
 */
char* malloc_join(char** strv, char sep) {
	size_t cant_cadenas = 0;
	largo_total_join = 0;
	if (strv[0] == NULL) largo_total_join++;
	for (int i = 0; strv[i] != NULL; i++) {
		size_t largo_cadena = strlen(strv[i]);
		largo_total_join += largo_cadena;
		cant_cadenas++;
	}
	largo_total_join += cant_cadenas;
	char* resultado = malloc(sizeof(char) * largo_total_join);
	if (resultado == NULL) return NULL;
	return resultado;
}

char* join(char** strv, char sep) {
	char* resultado = malloc_join(strv, sep);
	if (resultado == NULL) return NULL;

	size_t pos_resul = 0;
	for(int i = 0; strv[i] != NULL; i++) {
		for(int pos_cadena = 0; strv[i][pos_cadena] != '\0'; pos_cadena++,pos_resul++) {
			resultado[pos_resul] = strv[i][pos_cadena];
		}	
		if (pos_resul < largo_total_join-1) { //Ésta es la razón de la variable global.
			resultado[pos_resul] = sep;
			pos_resul++;
		}
	}
	resultado[pos_resul] = '\0';
	return resultado;
}

/*
 * Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
 */
void free_strv(char* strv[]) {
	for(int i = 0; strv[i] != NULL; i++) {
		free(strv[i]);
	}
	free(strv);
}