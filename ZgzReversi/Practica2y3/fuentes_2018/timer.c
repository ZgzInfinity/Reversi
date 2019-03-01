/*********************************************************************************************
* Fichero:		timer.c
* Autor:		737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/

#include "option.h"
#include "timer.h"
#include "44b.h"
#include "44blib.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

enum
{
	TIMER0_PRESCALER = 31,
	TIMER0_INTERVALO_SUPERIOR = 20000,
	TIMER0_INTERVALO_INFERIOR = 0,
	TIMER0_DIVISOR = 2
};

#define TIMER0_PERIODO_US 1 / ((MCLK / 1000000) / (TIMER0_PRESCALER + 1) / TIMER0_DIVISOR)

/*--------------------------------------------------------------*/
/*------------------- FUNCIONES DEL MODULO ---------------------*/
/*--------------------------------------------------------------*/

#ifdef SIM_IN_USE
static unsigned int timer0_simulador_us = 0;
#endif
static volatile unsigned int timer0_num_int = 0;

/*--------------------------------------------------------------*/
/*------------------- FUNCIONES DEL MODULO ---------------------*/
/*--------------------------------------------------------------*/

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer0_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer0_ISR(void)
{
	timer0_num_int++;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer0_init(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD &= ~(0x2000);	 // Configura la linea del timer0 como de tipo IRQ
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned)timer0_ISR;

	/* Configura el Timer0 */
	rTCFG0 = (rTCFG0 & ~(0xFF)) | TIMER0_PRESCALER; // ajusta el preescalado
	rTCFG1 &= ~(0xF);								// selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = TIMER0_INTERVALO_SUPERIOR;			// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = TIMER0_INTERVALO_INFERIOR;			// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON |= 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON ^= 0xb;
}

void timer0_empezar(void)
{
#ifdef SIM_IN_USE
	timer0_simulador_us = 0;
#else
	// parar timer0 (bit 0 = 0) y limpiar auto-reload (bit 3)
	rTCON &= ~(0x9);

	timer0_num_int = 0;
	rTCNTO0 = 0x0;

	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON |= 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3) y limpia bit 1*/
	rTCON ^= 0xb;
#endif
}

unsigned int timer0_leer(void)
{
#ifdef SIM_IN_USE
	return timer0_simulador_us;
#else
	unsigned int timer0_num_int_real;
	unsigned int ticks0;
	do
	{
		timer0_num_int_real = timer0_num_int;
		ticks0 = rTCNTO0;
	} while (timer0_num_int_real != timer0_num_int);

	return TIMER0_PERIODO_US * (timer0_num_int_real * (TIMER0_INTERVALO_SUPERIOR - TIMER0_INTERVALO_INFERIOR) + (TIMER0_INTERVALO_SUPERIOR - ticks0));
#endif
}

unsigned int timer0_parar(void)
{
	// parar timer0 (bit 0 = 0) y limpiar auto-reload (bit 3)
	rTCON &= ~(0x9);

	return timer0_leer();
}