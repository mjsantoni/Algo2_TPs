#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "strutil.h"

///////////////////////////////////////////////////////////////////////

char** crear_arreglo_char(size_t cant_separador) {
	char** arreglo = malloc(sizeof(char*)*(cant_separador+2));
	if(!arreglo) return NULL;
	arreglo[0] = NULL;
	arreglo[cant_separador+1] = NULL;
	return arreglo;
}

///////////////////////////////////////////////////////////////////////

void crear_subcadena_char(char** arreglo,size_t posicion_arreglo,size_t caracteres) {
	arreglo[posicion_arreglo] = malloc(sizeof(char)*caracteres+1);
}

///////////////////////////////////////////////////////////////////////

void sin_separador(char** arreglo,const char* str) {
	free(arreglo[0]);
	arreglo[0] = strdup(str);
}

///////////////////////////////////////////////////////////////////////

char** split(const char* str,char sep) {
	size_t largo_cadena = strlen(str);
	size_t cant_separador = 0;

	int* letras_previas = calloc(largo_cadena+1,sizeof(int));
	int pos_letras_previas = 0;

	for(int i = 0; str[i] != '\0';i++) { //cuento los separadores para poder dividir despues y cuantas letras antes de la coma hay.
		if(str[i] == sep) {
			cant_separador++;
			pos_letras_previas++;
		} else letras_previas[pos_letras_previas] += 1;
	}
	char** arreglo = crear_arreglo_char(cant_separador);
	if(cant_separador == 0) {
		sin_separador(arreglo,str);
		free(letras_previas);
		return arreglo;
	}
	
	pos_letras_previas = 0;
	size_t pos_arreglo = 0;
	size_t pos_letra_subcadena = 0;
	size_t inicio = 0;

	while(!(inicio > largo_cadena)) {
		crear_subcadena_char(arreglo,pos_arreglo,letras_previas[pos_letras_previas]);
		for(int i = 0;i<letras_previas[pos_letras_previas];i++) {
			arreglo[pos_arreglo][pos_letra_subcadena] = str[inicio];
			inicio++;
			pos_letra_subcadena++;
		}
		if (str[inicio] == sep || inicio == largo_cadena) arreglo[pos_arreglo][pos_letra_subcadena] = '\0';
		pos_letra_subcadena = 0;
		pos_arreglo++;
		inicio++;
		pos_letras_previas++;
	}
	free(letras_previas);
	return arreglo;
}

////////////////////////////////////////////////////////////

char* join(char** strv,char sep) {
	if(strv == NULL) return strdup("");
	size_t subcadena = 0;
	size_t caracteres = 0;
	for(;strv[subcadena] != NULL;subcadena++) {
		caracteres += (strlen(strv[subcadena]));
	}
	size_t pos_general = 0;
	int j = 0;
	char* resultado = calloc((subcadena+caracteres+1),sizeof(char)); //el 1 es para el \0.
	for(int i = 0;strv[i] != NULL;i++) {
		for(;strv[i][j] != '\0';j++) {
			resultado[pos_general] = strv[i][j];
			pos_general++;
		}
		if(strv[i+1] != NULL) resultado[pos_general] = sep;
		pos_general++;
		j = 0;
	}
	resultado[pos_general] = '\0';
	return resultado;
}

////////////////////////////////////////////////////////////

void free_strv(char* strv[]) {
	size_t i = 0;
	for(;strv[i] != NULL;i++) {
		free(strv[i]);
	}
	free(strv);
}

