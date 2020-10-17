#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"


bool son_anagramas(char* str1, char* str2){
	if (strlen(str1) != strlen(str2)){ // Tienen distinto largo, no son anagramas
		return false;
	}

	hash_t* hash = hash_crear(free); // Creo el hash contador de letras
	for (int i=0; str1[i]!='\0'; ++i){ // Por cada letra en str1
		char c[2] = { str1[i] , '\0' }; // Genero un caracter cadena, para que funcione con nuestra implementacion
		int* cant = (int*) hash_obtener(hash, c);
		if (!cant){ // No existe la letra aun en el hash
			int* valor = malloc(sizeof(int));
			*valor = 1;
			hash_guardar(hash, c, valor); // La guardo, con valor 1
		} else { // Ya habia aparecido la letra
			(*cant)++; // Le incremento la cantidad de apariciones
		}
	}

	for (int i=0; str2[i]!='\0'; ++i){
		char c[2] = { str2[i] , '\0' };
		int* cant = (int*) hash_obtener(hash, c);
		if (!cant){ // No habia aparecido la letra en str1, no son anagramas
			hash_destruir(hash);
			return false;
		} else {
			if (*cant < 1){ // No me quedan de esas letras, no son anagramas
				hash_destruir(hash);
				return false;
			}
			(*cant)--; // Decremento la cantidad de esa letra que me quedan disponibles
		}
	}

	hash_destruir(hash);
	return true;
}


int main(void){
	char* s1 = "argentino";
	char* s2 = "ignorante";
	char* s3 = "aaaaaaaaaaaaaaaaaaaaaafafa";
	char* s4 = "aaaaaaaaaaaaaaaaaaaaaafaaa";

	printf("(1) '%s' y '%s' son anagramas? \n", s1, s2);
	if (son_anagramas(s1,s2)) printf(" --> OK (son anagramas)\n");
	else printf(" --> FALLO\n");

	printf("------------\n");

	printf("(2) '%s' y '%s' son anagramas? \n", s3, s4);
	if (!son_anagramas(s3,s4)) printf(" --> OK (no lo son)\n");
	else printf(" --> FALLO\n");

	return 0;
}
