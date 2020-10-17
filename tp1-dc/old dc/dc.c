#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pila.h"
#include "strutil.h"

///////////////////////////////////////////////////////////////////////
//Funciones auxiliares

int suma(int num1, int num2) {return num1 + num2;}
int resta(int num1, int num2) {return num1 - num2;}
int multip(int num1, int num2) {return num1 * num2;}
int dividir(int num1, int num2) {return (int)(num1 / num2);}
int raiz(int n, int inicio, int fin) {
	if(fin<inicio) return -1;
	if(n == 1) return 1;
	int medio = (inicio+fin) / 2;
	if(medio * medio == n) return medio;
	if(medio * medio > n) return raiz(n, inicio, medio + 1);
	if((medio+1) * (medio+1) > n) return medio;
	return raiz(n, medio, fin);
}
int raiz_c(int num1) {
	return raiz(num1,0,num1);
}
int potencia(int num1, int num2) {
	int res = 1;
	int base = num1;
	int exponente = num2;
	if(exponente == 0) return res;
	for(int i = 0; i < exponente; i++) {
		res *= base;
	}
	return res;
}
int logaritmo(int operando, int base) {
	int resul;
	if(base == operando) return 1;
	if(base > operando) return 0;
	for(resul = 1; resul < operando; resul++) {
		if(potencia(base,resul) < operando && potencia(base,resul+1) > operando) break;
		if(potencia(base,resul) >= operando) break;
	}
	return resul;
}

///////////////////////////////////////////////////////////////////////

void dc(char* arch_entrada) {
	FILE* archivo;
	if (arch_entrada) {
		archivo = fopen(arch_entrada, "r");
		if(archivo == NULL) return;
	}else{
		archivo = stdin;
	}
	char* operandos = "+-*/^logsqrt?";
	char* suma_str = "+";
	char* resta_str = "-";
	char* producto_str = "*";
	char* division_str = "/";
	char* potencia_str = "^";
	char* logaritmo_str = "log";
	char* raiz_str = "sqrt";
	char* ternario = "?";
	char* espacio_str = "";
	char* linea = NULL;
	size_t capacidad = 0;
	ssize_t cant_caracteres = 0;
	pila_t* pila = pila_crear();
	bool error = false;
	void* num1;
	void* num2;
	void* num3;
	while((cant_caracteres = getline(&linea, &capacidad, archivo)) != -1) {
		char** leyenda = split(linea, ' ');
		int largo_arrays = 0;
		for (int j = 0; leyenda[j] != NULL; j++) {
			largo_arrays++;
		}
		//int numero[largo_arrays+1];
		int resul[largo_arrays+1];
		for(int i = 0; leyenda[i] != NULL; i++) {
			if(strcmp(leyenda[i], espacio_str) == 0) continue;
			if (leyenda[i][strlen(leyenda[i])-1] == '\n') {
				leyenda[i][strlen(leyenda[i])-1] = '\0';
			}
			//Casos borde de errores
			if(strcmp(leyenda[i], espacio_str) == 0) continue;
			if(largo_arrays == 2 && strcmp(leyenda[0], espacio_str) == 0) {
				printf("ERROR\n");
				error = true;
				break;
			}
			//Fin casos borde de errores
			if(!strstr(operandos, leyenda[i])) {
				resul[i] = (atoi(leyenda[i]));
				pila_apilar(pila, &resul[i]);
				continue;
			}
			//Hago todo lo demas porque es un operando
			num1 = pila_desapilar(pila);

			if(strcmp(leyenda[i], raiz_str) == 0) {
				if(!num1 || (*(int*)num1) < 0) {
					printf("ERROR\n");
					error = true;
					break;
				}
				resul[i] = raiz_c(*(int*)num1);
				pila_apilar(pila,&resul[i]);
				continue;
			}

			num2 = pila_desapilar(pila);

			if(!num1 || !num2) {
				printf("ERROR\n");
				error = true;
				break;
			}
			if(strstr(leyenda[i], ternario) != NULL) {
				num3 = pila_desapilar(pila);
				if(!num3) {
					printf("ERROR\n");
					error = true;
					break;
				}
				if((*(int*)num1) == 0) {
					resul[i] = *(int*)num3;
					pila_apilar(pila,&resul[i]);
				}else{
					resul[i] = *(int*)num2;
					pila_apilar(pila,&resul[i]);
				} 
				continue;
			}
			if(strcmp(leyenda[i], suma_str) == 0) {
				resul[i] = suma(*(int*)num1,*(int*)num2);
				pila_apilar(pila,&resul[i]);
			}
			if(strcmp(leyenda[i], resta_str) == 0) {
				resul[i] = resta(*(int*)num1,*(int*)num2);
				pila_apilar(pila,&resul[i]);
			}
			if(strcmp(leyenda[i], producto_str) == 0) {
				resul[i] = multip(*(int*)num1,*(int*)num2);
				pila_apilar(pila,&resul[i]);
			}
			if(strcmp(leyenda[i], division_str) == 0) {
				if((*(int*)num2) == 0) {
					printf("ERROR\n");
					error = true;
					break;	
				}
				resul[i] = dividir(*(int*)num1,*(int*)num2);
				pila_apilar(pila,&resul[i]);
			}
			if(strcmp(leyenda[i], potencia_str) == 0) {
				if((*(int*)num2) < 0) {
					printf("ERROR\n");
					error = true;
					break;	
				}
				resul[i] = potencia(*(int*)num1,*(int*)num2);
				pila_apilar(pila,&resul[i]);
			}
			if(strcmp(leyenda[i], logaritmo_str) == 0) {
				if(*(int*)num1 < 0 || (*(int*)num2) <= 1) {
					printf("ERROR\n");
					error = true;
					break;	
				}
				resul[i] = logaritmo(*(int*)num1,*(int*)num2);
				pila_apilar(pila,&resul[i]);
			}

		}
		//Fuera del for
		if (error) {
			error = false;
			free_strv(leyenda);
			while(!pila_esta_vacia(pila)) pila_desapilar(pila);
			continue;
		}
		void* resultado = pila_desapilar(pila);
		if(largo_arrays == 1 || !pila_esta_vacia(pila)) {
			printf("ERROR\n");
			free_strv(leyenda);
			while(!pila_esta_vacia(pila)) pila_desapilar(pila);
			continue;
		}
		if (!resultado) {
			free_strv(leyenda);
			continue;
		}
		int devolver = *(int*)resultado;
		printf("%d\n", devolver);
		free_strv(leyenda);
	}
	free(linea);
	fclose(archivo);
	pila_destruir(pila);
}


int main(int argc, char* argv[]) {
	if(argc > 2) {
		fprintf(stderr, "%s\n", "Cantidad de parametros erronea");
		return 0;
	}
	if(argv[1]) {
		FILE* archivo = fopen(argv[1], "r");
		if(archivo == NULL) {
			fprintf(stderr, "%s\n", "No se pudo leer el archivo indicado");
			return 0;
		}else{
			fclose(archivo);
		}
	}
	dc(argv[1]);
	return 0;
}