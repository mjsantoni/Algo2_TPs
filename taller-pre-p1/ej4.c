#include "pila.h"

/*bool es_piramidal(pila_t* pila){

	bool resultado = true;
	_es_piramidal(pila,&resultado);
	return resultado;
}

void _es_piramidal(pila_t* pila,bool *resultado){
	if(pila_esta_vacia(pila)){
		return;
	}
	
	int* desapilado = pila_desapilar();

	if(*desapilado<*pila_ver_tope(pila)){
		*resultado = false;
	}
	
}*/

bool _es_piramidal(pila_t* pila, bool r) {
    int* desapilado = pila_desapilar(pila);
    if(pila_esta_vacia(pila)) {
        pila_apilar(pila, desapilado);
        return r;
    }
    if(*pila_ver_tope(pila) > *desapilado) r= false;
    r = _es_piramidal(pila,r);
    pila_apilar(pila, desapilado);
    return r;
}
 
bool es_piramidal(pila_t* pila){
    return _es_piramidal(pila, true);
}