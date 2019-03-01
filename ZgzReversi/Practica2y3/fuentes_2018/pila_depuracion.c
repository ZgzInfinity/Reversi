/*********************************************************************************************
* Fichero:	pila_depuracion.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo pila_depuracion
*********************************************************************************************/

#include "option.h"
#include "pila_depuracion.h"
#include "timer2.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

enum
{
    TAMANYO_PILA = 1024,
    STACK_START = _ISR_STARTADDRESS - 0xff0 - TAMANYO_PILA * 4,
};

/*--------------------------------------------------------------*/
/*------------- VARIABLES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

static int stack_pointer = 0;
static uint32_t *pila;

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

void pila_debug_limpiar(void)
{
    stack_pointer = 0;
}

// vaciado de la pila
void pila_debug_init(void)
{
    pila_debug_limpiar();
    pila = (uint32_t *)STACK_START;
    int i;
    for (i = 0; i < TAMANYO_PILA; ++i)
    {
        pila[i] = 0;
    } 
}

// apilación de los eventos en la pila
void push_debug(uint8_t id_evento, uint32_t aux_data)
{
    uint32_t dato_a_apilar;
    // __asm__ volatile("bic %1, #0xFF000000\n\t"
    //                  "orr %0, %1, %2, ror #8"
    //                  : "=r"(dato_a_apilar), "+r"(aux_data)
    //                  : "r"(id_evento));
    dato_a_apilar = id_evento << 24 | aux_data;
    pila[stack_pointer] = dato_a_apilar;
    stack_pointer = (stack_pointer + 1) % TAMANYO_PILA;
    pila[stack_pointer] = timer2_leer();
    stack_pointer = (stack_pointer + 1) % TAMANYO_PILA;
}

uint32_t pila_debug_evento(int i)
{
	return pila[i];
}

uint32_t pila_debug_primer_evento(void)
{
    return pila[0];
}

// Obtención del último tiempo de evento apilado en la pila
uint32_t pila_debug_ultimo_evento(void)
{
    if (stack_pointer > 0)
    {
        return pila[stack_pointer - 1];
    }
    else
    {
        return 0;
    }
}
