#define _POSIX_C_SOURCE 200809L
#include <stdio.h> 
#include <string.h> 
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "hash.h"
#include "abb.h"
#include "strutil.h"
#include "pila.h"
#include "cola.h"
#include "heap.h"

////////////////////////  STRUCTS  //////////////////////////////

typedef struct aeropuerto {
	hash_t* v_info;
	abb_t* v_fecha;
	abb_t* v_prior;
} aeropuerto_t;

typedef struct vuelo {
	char* numero;
	char* aerolinea;
	char* aerop_origen;
	char* aerop_destino;
	char* numero_cola;
	char* prioridad;
	char* fecha;
	char* demora;
	char* tiempo_aire;
	char* cancelado;
} vuelo_t;

/////////////////////////////////////////////////////////////////

void destruir_vuelo(void* vuelo_void) {
	if(!vuelo_void) return;
	vuelo_t* vuelo = (vuelo_t*) vuelo_void;
	free(vuelo->numero);
	free(vuelo->aerolinea);
	free(vuelo->aerop_origen);
	free(vuelo->aerop_destino);
	free(vuelo->numero_cola);
	free(vuelo->prioridad);
	free(vuelo->fecha);
	free(vuelo->demora);
	free(vuelo->tiempo_aire);
	free(vuelo->cancelado);
	free(vuelo);
	return;
}

vuelo_t* crear_vuelo(char* numero,char* aerolinea,char* aerop_origen,char* aerop_destino,char* numero_cola,char* prioridad,char* fecha,char* demora,char* tiempo_aire,char* cancelado) {
	vuelo_t* vuelo = malloc(sizeof(vuelo_t));
	if(!vuelo) return NULL;
	vuelo->numero = strdup(datos[0]);
	vuelo->aerolinea = strdup(datos[1]);
	vuelo->aerop_origen = strdup(datos[2]);
	vuelo->aerop_destino = strdup(datos[3]);
	vuelo->numero_cola = strdup(datos[4]);
	vuelo->prioridad = strdup(datos[5]);
	vuelo->fecha = strdup(datos[6]);
	vuelo->demora = strdup(datos[7]);
	vuelo->tiempo_aire = strdup(datos[8]);
	vuelo->cancelado = strdup(datos[9]);

	//Remueve el '\n' del final.
	vuelo->cancelado[strlen(vuelo->cancelado)-1] = '\0';
	return vuelo;
}

/*
	Funciona al revés que el strcmp en cuanto a las prioridades, pero mantiene el orden normal
	de los vuelos. Esto es para ordenar el abb de mayor a menor, y así obtener la mayor prioridad
	al iniciar el iterador, de manera que ver_prioridad se haga en O(k).
*/	
int comparador_fechas_invertido(const char* c1, const char* c2) {
	char** comp1 = split(c1,'?');
	char** comp2 = split(c2,'?');
	int resul;

	if(strcmp(comp1[0],comp2[0]) > 0) resul = -1;
	else if(strcmp(comp1[0],comp2[0]) < 0) resul = 1;
	else {
		if(strcmp(comp1[1],comp2[1]) > 0) resul = 1;
		else if(strcmp(comp1[1],comp2[1]) < 0) resul = -1;
		else resul = 0;
	}
	free_strv(comp1);
	free_strv(comp2);
	return resul;	
}

/*
	Mantiene el orden por fechas, pero lo invierte para los vuelos. De esta manera cumple
	con la manera de imprimir en ver_tablero, que piden por ej. fecha de menor a mayor pero
	vuelos de mayor a menor.
*/
int comparador_fechas(const char* c1, const char* c2) {
	char** comp1 = split(c1,'?');
	char** comp2 = split(c2,'?');
	int resul;
	if(strcmp(comp1[0],comp2[0]) > 0) resul = 1;
	else if(strcmp(comp1[0],comp2[0]) < 0) resul = -1;
	else {
		if(strlen(comp1[1]) > strlen(comp2[1])) resul = -1;
		else if(strlen(comp1[1]) < strlen(comp2[1])) resul = 1;
		else if(strcmp(comp1[1],comp2[1]) > 0) resul = -1;
		else if(strcmp(comp1[1],comp2[1]) < 0) resul = 1;
		else resul = 0;
	}
	free_strv(comp1);
	free_strv(comp2);
	return resul;	
}

int strcmp_heap(void* info_separada1,void* info_separada2) {
	char** informacion1 = info_separada1;
	char** informacion2 = info_separada2;
	int resul;
	if(strcmp(informacion1[0],informacion2[0]) > 0) resul = -1;
	else if(strcmp(informacion1[0],informacion2[0]) < 0) resul = 1;
	else {
		if(strcmp(informacion1[1],informacion2[1]) > 0) resul = -1;
		else if(strcmp(informacion1[1],informacion2[1]) < 0) resul = 1;
		else resul = 0;
	}
	return resul;
}

aeropuerto_t* crear_aeropuerto() {
	aeropuerto_t* aeropuerto = malloc(sizeof(aeropuerto_t));
	if(!aeropuerto) return NULL;
	aeropuerto->v_info = hash_crear(destruir_vuelo);
	if(!aeropuerto->v_info) {
		free(aeropuerto);
		return NULL;
	}
	aeropuerto->v_fecha = abb_crear(comparador_fechas,NULL);
	if(!aeropuerto->v_fecha) {
		hash_destruir(aeropuerto->v_info);
		free(aeropuerto);
		return NULL;
	}
	aeropuerto->v_prior = abb_crear(comparador_fechas_invertido,NULL);
	if(!aeropuerto->v_prior) {
		hash_destruir(aeropuerto->v_info);
		abb_destruir(aeropuerto->v_fecha);
		free(aeropuerto);
		return NULL;
	}
	return aeropuerto;
}

char* cadena_abb(char* fecha,char* vuelo) {
	char** cadena = calloc(3,sizeof(char*));
	cadena[0] = fecha;
	cadena[1] = vuelo;
	cadena[2] = NULL;
	char* cadena_f = join(cadena,'?');
	free(cadena);
	return cadena_f;
}


///////////////////////////////////////////////////////////////////////

void imprimir_mensaje_error(char* imprimir_mensaje_error) {
	fprintf(stderr,"Error en comando %s\n",imprimir_mensaje_error);
}

///////////////////////////////////////////////////////////////////////
void agregar_archivo(aeropuerto_t* algueiza,char* ruta) {
	FILE* file = fopen(ruta,"r");
	if(!file) {
		imprimir_mensaje_error("agregar_archivo");
		return;
	}
	char* linea = NULL;
	size_t capacidad;
	size_t leyo;

	while((leyo = getline(&linea,&capacidad,file)) != -1) {

		char** datos = split(linea,','); //Separo los datos del CSV.
		vuelo_t* vuelo = crear_vuelo(datos); //Creo el vuelo con los datos.
		char* cadena_fecha = cadena_abb(datos[6],datos[0]);
		char* cadena_prior = cadena_abb(datos[5],datos[0]);

		if(hash_pertenece(algueiza->v_info,datos[0])) { //Borrar los viejos del ABB
			vuelo_t* vuelo_viejo = ((vuelo_t*) hash_obtener(algueiza->v_info,datos[0]));
			char* cadena_vieja_fecha = cadena_abb(vuelo_viejo->fecha,vuelo_viejo->numero);
			char* cadena_vieja_prior = cadena_abb(vuelo_viejo->prioridad,vuelo_viejo->numero);
			abb_borrar(algueiza->v_fecha,cadena_vieja_fecha);
			abb_borrar(algueiza->v_prior,cadena_vieja_prior);

			free(cadena_vieja_fecha);
			free(cadena_vieja_prior);
		}
		
		hash_guardar(algueiza->v_info,datos[0],(void*) vuelo);
		abb_guardar(algueiza->v_fecha,cadena_fecha,NULL);
		abb_guardar(algueiza->v_prior,cadena_prior,NULL);

		free(cadena_fecha);
		free(cadena_prior);
		free_strv(datos);
	}
	free(linea);
	fclose(file);
	printf("OK\n");
}

///////////////////////////////////////////////////////////////////////

bool comando_ver_tablero_no_es_valido(char** parametros,size_t p) {
	if(p != 5) return true;
	if(atoi(parametros[1]) <= 0) return true;
	if((strcmp(parametros[2],"asc") != 0) && (strcmp(parametros[2],"desc") != 0)) return true;
	if((strcmp(parametros[3],parametros[4]) > 0)) return true;
	return false;
}

/*
	Recibe el aeropuerto, parametros y cantidad de parametros.
	Si el modo es ascendente, imprime la cantidad pedida a medida que itera
	desde la fecha pedida. Si el modo es descendente, los apila hasta llegar a la última
	fecha, y luego desapila la cantidad pedida. Esto se hace para invertir el orden de los
	vuelos.
*/
void ver_tablero(aeropuerto_t* algueiza,char* cantidad,char* modo,char* inicio,char* fin) {
	abb_iter_t* iterador = abb_iter_2_punto_0(algueiza->v_fecha, inicio);
	int i = 0;
	int j = 0;
	pila_t* pila = pila_crear(); //Creo la pila por si el modo es DESC
	bool valido = true;
	int cant = atoi(cantidad);

	while(valido && !abb_iter_in_al_final(iterador)) {

		const char* informacion = abb_iter_in_ver_actual(iterador);
		char** info_separada = split(informacion,'?');
		char* fecha = info_separada[0];
		char* numero = info_separada[1];
		
		if(strcmp(fecha,fin) > 0) valido = false; //Si me paso de la fecha max, dejo de buscar.
		if(strcmp(fecha,inicio) >= 0 && strcmp(fecha,fin) <= 0) { //Si el valor esta entre las fechas bien.

			if((strcmp(modo,"asc") == 0)) { //Si es asc lo imprimo directo, porque el iterador es inorder (menor a mayor).
				if(i >= cant-1) valido = false;
				printf("%s - %s\n",fecha,numero); 
				free_strv(info_separada);
			}
			else pila_apilar(pila,(void*) info_separada); //Sino lo apilo, para que quede invertido.
			i++;

		} else free_strv(info_separada);
		
		abb_iter_in_avanzar_2_punto_0(iterador,inicio,fin,algueiza->v_fecha);
	}

	while(!pila_esta_vacia(pila) && j < cant) { //Si la pila estaba vacia ni entra. Sino imrpime los K vuelos pedidos.
		char** vuelo = (char**) pila_desapilar(pila);
		printf("%s - %s\n",vuelo[0],vuelo[1]);
		free_strv(vuelo);
		j++;
	}
	while(!pila_esta_vacia(pila)) free_strv(pila_desapilar(pila)); //Desapila los restantes que no entraron en la cantidad pedida.
	pila_destruir(pila);
	abb_iter_in_destruir(iterador);
	printf("OK\n");
	return;
}

///////////////////////////////////////////////////////////////////////

bool comando_info_vuelo_no_es_valido(hash_t* hash,char** parametros,size_t p) {
	if(p != 2) return true;
	return !hash_pertenece(hash,parametros[1]);
}

void info_vuelo(aeropuerto_t* algueiza,char* numero_vuelo,bool ok) {
	vuelo_t* v = (vuelo_t*) hash_obtener(algueiza->v_info,numero_vuelo);
	printf("%s %s %s %s %s %s %s %s %s %s\n",v->numero,v->aerolinea,v->aerop_origen,v->aerop_destino,v->numero_cola,v->prioridad,v->fecha,v->demora,v->tiempo_aire,v->cancelado);
	if(ok) printf("OK\n");
	return;
}

///////////////////////////////////////////////////////////////////////

bool comando_prioridad_no_es_valido(char** parametros,size_t p) {
	if(p != 2) return true;
	if(strcmp(parametros[1],"") == 0) return true;
	int i = 0;
	while(i < strlen(parametros[1])) {
		if(parametros[1][i] > '9' || parametros[1][i] < '0') return true;
		i++;
	}
	return false;
}

/*
	Al ordenar el abb de prioridades de manera invertida, el iterador comienza desde la mayor
	prioridad. Creo que esto reduce aún más el orden, que pasaría a ser O(k).
*/
void prioridad_vuelos(aeropuerto_t* algueiza,char* cantidad) {
	int i = 0;
	abb_iter_t* iterador = abb_iter_in_crear(algueiza->v_prior);
	cola_t* cola = cola_crear();

	while(!abb_iter_in_al_final(iterador) && i < atoi(cantidad)) {
		const char* informacion = abb_iter_in_ver_actual(iterador);
		char** info_separada = split(informacion,'?');
		cola_encolar(cola,(void*) info_separada);
		abb_iter_in_avanzar(iterador);
		i++;
	}
	abb_iter_in_destruir(iterador);

	while(!cola_esta_vacia(cola)) {
		char** info = (char**) cola_desencolar(cola);
		printf("%s - %s\n",info[0],info[1]);
		free_strv(info);
	}
	cola_destruir(cola,NULL);
	printf("OK\n");
}

///////////////////////////////////////////////////////////////////////

bool comando_borrar_no_es_valido(char** parametros,size_t p) {
	if(p != 3) return true;
	if((strcmp(parametros[1],parametros[2]) < 0)) return false;
	return true;
}

void borrar(aeropuerto_t* algueiza,char* inicio,char* fin) {
	bool valido = true;
	abb_iter_t* iterador = abb_iter_2_punto_0(algueiza->v_fecha, inicio);
	heap_t* heap = heap_crear(strcmp_heap);

	while(!abb_iter_in_al_final(iterador) && valido) {

		const char* informacion = abb_iter_in_ver_actual(iterador);
		char** info_separada = split(informacion,'?');
		char* fecha = info_separada[0];

		if(strcmp(fecha,fin) > 0) valido = false; //Si me paso de fecha, dejo de buscar.
		if(strcmp(fecha,inicio) >= 0 && strcmp(fecha,fin) < 0) heap_encolar(heap,(void*) info_separada);
		else free_strv(info_separada);
		abb_iter_in_avanzar_2_punto_0(iterador,inicio,fin,algueiza->v_fecha);
	}

	while(!heap_esta_vacio(heap)) {

		char** informacion = heap_desencolar(heap); //FECHA?VUELO.
		vuelo_t* vuelo = (vuelo_t*) hash_obtener(algueiza->v_info,informacion[1]); //Necesito la prior para borrar del abb_prior
		char* cadena_fecha = cadena_abb(informacion[0],informacion[1]);
		char* cadena_prior = cadena_abb(vuelo->prioridad,informacion[1]);

		info_vuelo(algueiza,vuelo->numero,false);

		destruir_vuelo(hash_borrar(algueiza->v_info,informacion[1])); //Borro del hash.
		abb_borrar(algueiza->v_fecha,cadena_fecha); //Borro del abb_fecha.
		abb_borrar(algueiza->v_prior,cadena_prior); //Borro del abb_prioridad.

		free_strv(informacion);
		free(cadena_prior);
		free(cadena_fecha);

	}
	heap_destruir(heap,NULL);
	abb_iter_in_destruir(iterador);
	printf("OK\n");
	return;
}

///////////////////////////////////////////////////////

void remover_caracteres(char* linea) {
	if(linea[strlen(linea) -1] == '\n') linea[strlen(linea)-1] = '\0';
	if(strlen(linea) == 0) return;
	if(linea[strlen(linea) -1] == '\r') linea[strlen(linea)-1] = '\0';
}

size_t contador_parametros(char** parametros) {
	size_t i = 1;
	while(parametros[i] != NULL) i++;
	return i;
}

void control_de_aeropuerto(aeropuerto_t* algueiza,char* linea) {
	char** parametros = split(linea,' ');
	char* comando = parametros[0];
	size_t p = contador_parametros(parametros);

	if(!strcmp(comando,"agregar_archivo")) {
		if(p != 2) imprimir_mensaje_error(parametros[0]);
		else agregar_archivo(algueiza,parametros[1]);
	}

	else if(!strcmp(comando,"ver_tablero")) {
		if(comando_ver_tablero_no_es_valido(parametros,p)) imprimir_mensaje_error(parametros[0]);
		else ver_tablero(algueiza,parametros[1],parametros[2],parametros[3],parametros[4]);
	}

	else if(!strcmp(comando,"info_vuelo")) {
		if(comando_info_vuelo_no_es_valido(algueiza->v_info,parametros,p)) imprimir_mensaje_error(parametros[0]);
		else info_vuelo(algueiza,parametros[1],true);
	}

	else if(!strcmp(comando,"prioridad_vuelos")) {
		if(comando_prioridad_no_es_valido(parametros,p)) imprimir_mensaje_error(parametros[0]);
		else prioridad_vuelos(algueiza,parametros[1]);
	}

	else if(!strcmp(comando,"borrar")) {
		if(comando_borrar_no_es_valido(parametros,p)) imprimir_mensaje_error(parametros[0]);
		else borrar(algueiza,parametros[1],parametros[2]);
	}

	else imprimir_mensaje_error(comando);
	free_strv(parametros);
}

void destruir_aeropuerto(aeropuerto_t* algueiza) {
	hash_destruir(algueiza->v_info);
	abb_destruir(algueiza->v_fecha);
	abb_destruir(algueiza->v_prior);
	free(algueiza);
}

int main(int argc, char const *argv[]) {

	aeropuerto_t* algueiza = crear_aeropuerto();
	if(!algueiza) return 1;
	///////////////////////////////////////////
	char* archivo1 = "agregar_archivo vuelos-algueiza-parte-01.csv";
	char* archivo2 = "agregar_archivo vuelos-algueiza-parte-02.csv";
	char* archivo3 = "agregar_archivo vuelos-algueiza-parte-03.csv";
	char* archivo4 = "agregar_archivo vuelos-algueiza-parte-04.csv";
	char* archivo5 = "agregar_archivo vuelos-algueiza-parte-05.csv";
	char* ver_tablero = "ver_tablero 3 asc 2018-04-08T10:00:00 2018-04-21T00:12:00";
	char* borrar = "borrar 2018-10-04T02:01:01 2018-11-01T03:00:00";
	char* prioridad = "prioridad_vuelos 3";
	char* info = "info_vuelo 4608";

	control_de_aeropuerto(algueiza,archivo1);
	control_de_aeropuerto(algueiza,archivo2);
	control_de_aeropuerto(algueiza,archivo3);
	control_de_aeropuerto(algueiza,archivo4);
	control_de_aeropuerto(algueiza,archivo5);
	
	control_de_aeropuerto(algueiza,info);
	control_de_aeropuerto(algueiza,ver_tablero);
	control_de_aeropuerto(algueiza,prioridad);
	control_de_aeropuerto(algueiza,borrar);

	destruir_aeropuerto(algueiza);
/*
	char* linea = NULL;
	size_t capacidad;
	size_t input;

	while((input = getline(&linea,&capacidad,stdin) != -1)) {
		remover_caracteres(linea);
		aeropuerto(algueiza,linea);
	}
	destruir_aeropuerto(algueiza);
	return 0;
*/
}