#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool _es_magico(int a[], int inicio, int fin) {
	if (inicio > fin) return false;
	int medio = (inicio + fin) / 2;
	if (a[medio] == medio) return true;
	if (a[medio] > medio) return _es_magico(a, inicio, medio-1);
	if (a[medio] < medio) return _es_magico(a, medio+1, fin);
}



bool es_magico(int a[], int largo) {
	return _es_magico(a,0,largo);
}

void main() {
	int a[] = {-3,0,1,3,7,9};
	int b[] = {1,2,4,6,7,9};
	printf("%d\n",_es_magico(b,0,6));
}