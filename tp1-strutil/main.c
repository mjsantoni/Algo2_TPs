#include "testing.h"
#include <stdio.h>

void pruebas_tp1_alumno(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(void) {
	/* Ejecuta todas las pruebas unitarias. */
	printf("~~~ PRUEBAS DEL ALUMNO ~~~\n");
	pruebas_tp1_alumno();

	return failure_count() > 0;
}
