#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pila.h"
#include "strutil.h"

///////////////////////////////////////////////////////////////////////
//Funciones de operaciones.

int raiz(int n, int inicio, int fin) {
	if(fin<inicio) return -1;
	if(n == 1) return 1;
	int medio = (inicio+fin) / 2;
	if(medio * medio == n) return medio;
	if(medio * medio > n) return raiz(n, inicio, medio + 1);
	if((medio+1) * (medio+1) > n) return medio;
	return raiz(n, medio, fin);
}
bool raiz_c(void* num1, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	if(!num1) {
		printf("ERROR\n");
		return false;
	}
	int num = *(int*)num1;
	if(num < 0) {
		printf("ERROR\n");
		return false;
	}
	arr_auxiliar[pos_arr] = raiz(num,0,num);
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
bool suma(void* num1, void* num2, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	arr_auxiliar[pos_arr] = n1 + n2;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
bool resta(void* num1, void* num2, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	arr_auxiliar[pos_arr] = n1 - n2;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
bool multip(void* num1, void* num2, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	arr_auxiliar[pos_arr] = n1 * n2;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
bool dividir(void* num1, void* num2, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	if(n2 == 0) {
		printf("ERROR\n");
		return false;
	}
	arr_auxiliar[pos_arr] = n1 / n2;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
bool potencia(void* num1, void* num2, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int res = 1;
	int base = *(int*)num1;
	int exponente = *(int*)num2;
	if(exponente < 0) {
		printf("ERROR\n");
		return false;
	}
	if(exponente == 0) {
		arr_auxiliar[pos_arr] = res;
		pila_apilar(pila,&arr_auxiliar[pos_arr]);
		return true;
	}
	for(int i = 0; i < exponente; i++) {
		res *= base;
	}
	arr_auxiliar[pos_arr] = res;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
int potencia_auxiliar(int num1, int num2) {
	int res = 1;
	int base = num1;
	int exponente = num2;
	if(exponente == 0) return res;
	for(int i = 0; i < exponente; i++) {
		res *= base;
	}
	return res;
}
bool logaritmo(void* num1, void* num2, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int operando = *(int*)num1;
	int base = *(int*)num2;
	int resul;
	int uno = 1;
	int cero = 0;
	if(operando < 0 || base <= 1) {
		printf("ERROR\n");
		return false;
	}
	if(base == operando) {
		arr_auxiliar[pos_arr] = uno;
		pila_apilar(pila,&arr_auxiliar[pos_arr]);
		return true;
	}
	if(base > operando) {
		arr_auxiliar[pos_arr] = cero;
		pila_apilar(pila,&arr_auxiliar[pos_arr]);
		return true;
	}
	for(resul = 1; resul < operando; resul++) {
		if(potencia_auxiliar(base,resul) < operando && potencia_auxiliar(base,resul+1) > operando) break;
		if(potencia_auxiliar(base,resul) >= operando) break;
	}
	arr_auxiliar[pos_arr] = resul;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}
bool ternario(void* num1, void* num2, void* num3, int* arr_auxiliar, pila_t* pila, int pos_arr) {
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	if(!num3) {
		printf("ERROR\n");
		return false;
	}
	int n3 = *(int*)num3;
	if(n1 == 0) arr_auxiliar[pos_arr] = n3;
	else arr_auxiliar[pos_arr] = n2;
	pila_apilar(pila,&arr_auxiliar[pos_arr]);
	return true;
}

///////////////////////////////////////////////////////////////////////
//Funciones auxiliares.

void error_de_operacion(char** linea, pila_t* pila) {
	free_strv(linea);
	while(!pila_esta_vacia(pila)) pila_desapilar(pila);
}

void imprimir_resultado(char** linea, pila_t* pila, int* arr_auxiliar) {
	void* resultado = pila_desapilar(pila);
	if(linea[1] == NULL || !pila_esta_vacia(pila) || !resultado) {
			printf("ERROR\n");
			error_de_operacion(linea, pila);
			return;
		}
	int devolver = *(int*)resultado;
	printf("%d\n", devolver);
	free_strv(linea);
}

///////////////////////////////////////////////////////////////////////
//Funciones principales.

bool encargado(char** linea, int* arr_auxiliar, pila_t* pila, int largo_array) {
	char* operandos = "+-*/^logsqrt?";
	void* num1;
	void* num2;
	void* num3;
	for(int i = 0; linea[i] != NULL; i++) { //Itera por los elementos de la linea.
		//Errores o espacios.
		if(strcmp(linea[i], "") == 0) continue;
		if (linea[i][strlen(linea[i])-1] == '\n') {
			linea[i][strlen(linea[i])-1] = '\0';
		}
		if(strcmp(linea[i], "") == 0) continue;
		if(largo_array == 2 && strcmp(linea[0], "") == 0) {
			printf("ERROR\n");
			return false;
		}
		//Apilo números.
		if(!strstr(operandos, linea[i])) {
			arr_auxiliar[i] = (atoi(linea[i]));
			pila_apilar(pila, &arr_auxiliar[i]);
			continue;
		}
		//Hago todo lo demás porque es un operando.
		num1 = pila_desapilar(pila);

		if(strcmp(linea[i], "sqrt") == 0) {
			if(!raiz_c(num1, arr_auxiliar, pila, i)) return false;
			continue;
		}

		num2 = pila_desapilar(pila);

		if(!num1 || !num2) {
			printf("ERROR\n");
			return false;
		}

		if(strcmp(linea[i], "+") == 0) {
			if(!suma(num1, num2, arr_auxiliar, pila, i)) return false;
		}
		if(strcmp(linea[i], "-") == 0) {
			if(!resta(num1, num2, arr_auxiliar, pila, i)) return false;
		}
		if(strcmp(linea[i], "*") == 0) {
			if(!multip(num1, num2, arr_auxiliar, pila, i)) return false;
		}
		if(strcmp(linea[i], "/") == 0) {
			if(!dividir(num1, num2, arr_auxiliar, pila, i)) return false;
		}
		if(strcmp(linea[i], "^") == 0) {
			if(!potencia(num1, num2, arr_auxiliar, pila, i)) return false;
		}
		if(strcmp(linea[i], "log") == 0) {
			if(!logaritmo(num1, num2, arr_auxiliar, pila, i)) return false;
		}
		if(strcmp(linea[i], "?") == 0) {
			num3 = pila_desapilar(pila);
			if(!ternario(num1, num2, num3, arr_auxiliar, pila, i)) return false;
		}
	}
	return true;
}

void dc(char* arch_entrada) {
	FILE* archivo;
	if (arch_entrada) {
		archivo = fopen(arch_entrada, "r");
		if(archivo == NULL) return;
	}else{
		archivo = stdin;
	}

	char* line = NULL;
	size_t capacidad = 0;
	ssize_t cant_caracteres = 0;
	pila_t* pila = pila_crear();

	while((cant_caracteres = getline(&line, &capacidad, archivo)) != -1) {
		char** linea = split(line, ' ');
		//Creo el array auxiliar acá porque no lo puedo hacer en una función auxiliar.
		int largo_array = 0;
		for (int j = 0; linea[j] != NULL; j++) {
			largo_array++;
		}
		int arr_auxiliar[largo_array+1];
		//
		if(!encargado(linea,arr_auxiliar,pila,largo_array)) {
			error_de_operacion(linea, pila);
			continue;
		}
		imprimir_resultado(linea, pila, arr_auxiliar);
	}
	free(line);
	fclose(archivo);
	pila_destruir(pila);
}

///////////////////////////////////////////////////////////////////////

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