/*********************************************************************************************
* Fichero:	exceptions.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo exceptions
*********************************************************************************************/

#include <stdint.h>
#include "exceptions.h"
#include "44b.h"
#include "8led.h"
#include "timer.h"
#include "gestion_tiempo.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

// Constantes para manejar los tipos de excepciones para identificarlas en el 8-led
enum tipo_8led
{
    SWI_8LED = 0x1,
    DABT_8LED = 0x2,
    UDEF_8LED = 0x3,
    ERROR_8LED = 0xF,
};

// Constantes para determinar el tipo de excepción
enum tipo_interrupcion
{
    NINGUNA_EXCP = 0x0,
    SWI = 0x13,
    DABT = 0x17,
    UDEF = 0x1B,
};

// Constante para determinar el retardo del led
enum
{
    DELAY_8LED_US = 500000,
    BLANK_8LED = 16,
};

/*--------------------------------------------------------------*/
/*------------ VARIABLES PRIVADAS DEL MODULO -------------------*/
/*--------------------------------------------------------------*/

static volatile enum tipo_interrupcion excepcion_tratada;
static volatile uint32_t dir_instr_excepcion;

/*--------------------------------------------------------------*/
/*------------------ FUNCIONES DEL MODULO ----------------------*/
/*--------------------------------------------------------------*/

// tratamiento de los posibles excepciones
void exception_tratamiento(void);

// Activación del tratamiento de excepciones
void exception_init(void)
{
    excepcion_tratada = NINGUNA_EXCP;
    dir_instr_excepcion = 0;
    pISR_UNDEF = (unsigned)exception_tratamiento;
    pISR_SWI = (unsigned)exception_tratamiento;
    pISR_DABORT = (unsigned)exception_tratamiento;
}

// gestion de las excepciones
void exception_tratamiento(void)
{
    uint32_t interrupcion;
    __asm__ volatile("mrs %0, cpsr\n\t"
                     "and %0, %0, #0x1F"
                     : "+r"(interrupcion));

    // captura de la excepción producida
    excepcion_tratada = (enum tipo_interrupcion)interrupcion;

    // se obtiene la dirección de la instrucción que ha producido la excepción
    __asm__ volatile("mov %0, lr"
                     : "=r"(dir_instr_excepcion));

    switch (interrupcion)
    {
    // la excepción es SWI
    case SWI:
        dir_instr_excepcion -= 4;
        exception_bucle();
        break;
    // la excepción es Data Abort
    case DABT:
        dir_instr_excepcion -= 8;
        exception_bucle();
        break;
    // la excepción es Undefined
    case UDEF:
        dir_instr_excepcion -= 4;
        exception_bucle();
        break;
    default:
        break;
    }
}

// muestreo del tipo de excepción en el display
void exception_bucle(void)
{
    enum tipo_8led numero_8led;

    // se procesa la excepción
    switch (excepcion_tratada)
    {
    // SWI
    case SWI:
    {
        numero_8led = SWI_8LED;
        break;
    }
    // Data Abort
    case DABT:
    {
        numero_8led = DABT_8LED;
        break;
    }
    // Undefined
    case UDEF:
    {
        numero_8led = UDEF_8LED;
        break;
    }
    default:
    {
        break;
    }
    }

    // bucle infinito que detiene la ejecución del programa
    // mantiene el led parpadeando el identificador de la excepción
    while (1)
    {
        D8Led_symbol(numero_8led);
        Delay(DELAY_8LED_US);
        D8Led_symbol(BLANK_8LED);
        Delay(DELAY_8LED_US);
    }
}
