#include <stdio.h>
#include <stdlib.h>

typedef struct nodo_lista{
	struct nodo_lista* prox;
	void* dato;
}nodo_lista_t;

typedef struct lista{
	nodo_lista_t* prim;
}lista_t;

void* k_ultimo(lista_t* lista,size_t k){
	nodo_lista_t* actual = lista->prim;
	nodo_lista_t* k_ultimo = lista->prim;

	for(int i=0;i<k;i++){
		actual = actual->prox;
	}

	while(actual->prox){
		actual = actual->prox;
		k_ultimo = k_ultimo->prox;
	}

	return k_ultimo->dato;
}