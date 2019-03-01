/*********************************************************************************************
* Fichero:	tests.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo tests
*********************************************************************************************/

#include "tests.h"
#include "timer.h"
#include "button.h"
#include "botones_antirebotes.h"
#include "pila_depuracion.h"

/*--------------------------------------------------------------*/
/*------------------- FUNCIONES DEL MODULO ---------------------*/
/*--------------------------------------------------------------*/

int exceptions_test(void)
{
    // Test SWI
    __asm__ volatile("swi 1");
    //
    //    // Test UNDEFINED
    //    //    volatile para que el compilador no reordene el ensamblador
    __asm__ volatile(".word 0xe7f000f0\n");

    // Test DABT
    intptr_t invalid_ptr = 0xFFFFFFFF;
    int invalid_var = *(int *)invalid_ptr;

    return invalid_var;
}

void pila_depuracion_test(void)
{
    push_debug(1, 4);
    push_debug(2, 5);
    push_debug(3, 6);
}
