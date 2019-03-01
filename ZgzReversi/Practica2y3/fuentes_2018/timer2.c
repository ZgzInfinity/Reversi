/*********************************************************************************************
* Fichero:	timer2.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo timer2
*********************************************************************************************/

#include "option.h"
#include "timer2.h"
#include "44b.h"
#include "44blib.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

enum
{
    TIMER2_PRESCALER = 1,
    TIMER2_INTERVALO_SUPERIOR = 65535,
    TIMER2_INTERVALO_INFERIOR = 0,
    TIMER2_DIVISOR = 2
};

#define TIMER2_PERIODO_US 1.0 / ((MCLK / 1000000) / (TIMER2_PRESCALER + 1) / TIMER2_DIVISOR)

/*--------------------------------------------------------------*/
/*------------- VARIABLES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

#ifdef SIM_IN_USE
static unsigned int timer2_simulador_us = 0;
#endif
static volatile unsigned int timer2_num_int = 0;

/*--------------------------------------------------------------*/
/*------------------- FUNCIONES  DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

void timer2_ISR(void) __attribute__((interrupt("IRQ")));

void timer2_ISR(void)
{
    timer2_num_int++;

    rI_ISPC |= BIT_TIMER2;
}

void timer2_init(void)
{
    /* Configuraion controlador de interrupciones */
    rINTMOD &= ~(0x800);      // Configura la linea del timer2 como de tipo IRQ
    rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

    /* Establece la rutina de servicio para TIMER2 */
    pISR_TIMER2 = (unsigned)timer2_ISR;

    /* Configura el Timer2 */
    rTCFG0 = (rTCFG0 & ~(0xFF00)) | TIMER2_PRESCALER << 8; // ajusta el preescalado
    rTCFG1 &= ~(0xF00);                                    // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
    rTCNTB2 = TIMER2_INTERVALO_SUPERIOR;                   // valor inicial de cuenta (la cuenta es descendente)
    rTCMPB2 = TIMER2_INTERVALO_INFERIOR;                   // valor de comparación
    // establecer update=manual (bit 13) + inverter=on (¿? será inverter off un cero en el bit 14 pone el inverter en off)
    rTCON |= 0x2000;
    // iniciar timer2 (bit 12) y auto-reload (bit 15) y limpia bit 13
    rTCON ^= 0xb000;
}

void timer2_empezar(void)
{
#ifdef SIM_IN_USE
    timer2_simulador_us = 0;
#else
    // parar timer2 (bit 12 = 0) y limpiar auto-reload (bit 15)
    rTCON &= ~(0x9000);

    timer2_num_int = 0;
    rTCNTO2 = 0x0;

    // establecer update=manual (bit 13) + inverter=on (¿? será inverter off un cero en el bit 14 pone el inverter en off)
    rTCON |= 0x2000;
    // iniciar timer2 (bit 12) y auto-reload (bit 15) y limpia bit 13
    rTCON ^= 0xb000;
#endif
}

unsigned int timer2_leer(void)
{
#ifdef SIM_IN_USE
    return timer2_simulador_us;
#else
    unsigned int timer2_num_int_real;
    unsigned int ticks2;
    do
    {
        timer2_num_int_real = timer2_num_int;
        ticks2 = rTCNTO0;
    } while (timer2_num_int_real != timer2_num_int);

    return TIMER2_PERIODO_US * (timer2_num_int_real * (TIMER2_INTERVALO_SUPERIOR - TIMER2_INTERVALO_INFERIOR) + (TIMER2_INTERVALO_SUPERIOR - ticks2));
#endif
}

unsigned int timer2_parar(void)
{
    // parar timer2 (bit 12 = 0) y limpiar auto-reload (bit 15)
    rTCON &= ~(0x9000);

    return timer2_leer();
}
