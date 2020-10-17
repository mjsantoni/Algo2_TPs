#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cola.h"

bool karp_rabin_comp(char* cadena, char* subcadena) {
	int la = (int) strlen(cadena);
	int lb = (int) strlen(subcadena);
	int d = 100,p=0,t=0,h=1,i,j,mod = 101;
	for(i=0;i<lb-1;i++) h=(d*h)%mod;
	for(i=0;i<lb;i++) {
		p = (d*p + subcadena[i])%mod;
		t = (d*t + cadena[i])%mod;
	}
	for(i=0;i<=la-lb;i++) {
		if(p==t) {
			for(j=0;j<lb;j++) {
				if(cadena[i+j]!=subcadena[j]) break;
			}
			if(j==lb) {
				return true;
			}
		}
		if(i<la-lb) {
			t = (d*(t - cadena[i]*h) + cadena[i+lb])%mod;
			if(t < 0) t = (t + mod);
		}
	}
	return false;
}

void grep(char* cadena, char* l_previas, char* arch_entrada) {
	FILE* archivo;
	if (arch_entrada) {
		archivo = fopen(arch_entrada, "r");
		if(archivo == NULL) return;
	}else{
		archivo = stdin;
	}

	char* linea = NULL;
	size_t capacidad = 0;
	char* fin;
	int lineas_previa = (int) strtol(l_previas, &fin, 0);
	ssize_t cant_caracteres = 0;
	int cont = 0;
	cola_t* cola = cola_crear();

	while((cant_caracteres = getline(&linea, &capacidad, archivo)) != -1) {
		if(cont == lineas_previa+1) {
			free(cola_desencolar(cola));
			cont--;
		}
		if(karp_rabin_comp(linea,cadena)) {
			while(!cola_esta_vacia(cola)) {
				void* desencolado = cola_desencolar(cola);
				printf("%s", (char*)desencolado);
				free(desencolado);
			}
			printf("%s", linea);
			cont = 0;
			continue;
		}
		cola_encolar(cola,strdup(linea));
		cont++;
	}
	free(linea);
	cola_destruir(cola,free);
	fclose(archivo);
}


int main(int argc, char* argv[]) {
	if(argc < 3 || argc > 4) {
		fprintf(stderr, "%s\n", "Cantidad de parametros erronea");
		return 0;
	}
	char* fin;
	strtol(argv[2], &fin, 0);
	if (argv[2] == fin) {
		fprintf(stderr, "%s\n", "Tipo de parámetro incorrecto");
		return 0;
	}
	if(argv[3]) {
		FILE* archivo = fopen(argv[3], "r");
		if(archivo == NULL) {
			fprintf(stderr, "%s\n", "No se pudo leer el archivo indicado");
			return 0;
		}else{
			fclose(archivo);
		}
	}
	grep(argv[1],argv[2],argv[3]);
	return 0;
}