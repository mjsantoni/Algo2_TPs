#include <stdio.h>

int prim_cero();
int _prim_cero();


int main(void){
	int a[] = {1,1,1,1,0,0,0,0};

	printf("%d\n",prim_cero(a,8));
	return 0;
}


int prim_cero(int a[],size_t n){
	return _prim_cero(a,0,n-1);
}

int _prim_cero(int a[],int inicio, int fin){
	if(inicio>fin){
		return -1;
	}

	int medio = (inicio+fin)/2;

	if(a[medio]==0){
		if(a[medio-1]==0){
			return _prim_cero(a,inicio,medio-1);
		}
	}

	if(a[medio]==1){
		return _prim_cero(a,medio+1,fin);
	}

	return medio;
}

