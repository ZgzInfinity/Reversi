/*********************************************************************************************
* Fichero:	timer2.c
* Autores: 737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo timer2
*********************************************************************************************/

#include "timer2.h"
#include "44b.h"
#include "44blib.h"

void timer2_ISR(void) __attribute__((interrupt("IRQ")));

void timer2_ISR(void)
{
    timer2_num_int++;

    rI_ISPC |= BIT_TIMER2;
}

inline void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD &= ~(0x800); // Configura la linea del timer2 como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

    /* Configura el Timer2 */
	rTCFG0 = (rTCFG0 & ~(0xFF00)) | TIMER2_PRESCALER << 8; // ajusta el preescalado
	rTCFG1 &= ~(0xF00); // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = TIMER2_INTERVALO_SUPERIOR;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = TIMER2_INTERVALO_INFERIOR;// valor de comparación
    // establecer update=manual (bit 13) + inverter=on (¿? será inverter off un cero en el bit 14 pone el inverter en off)
    rTCON |= 0x2000;
    // iniciar timer2 (bit 12) y auto-reload (bit 15) y limpia bit 13
	rTCON ^= 0xb000;
}

inline void timer2_empezar(void)
{
    // parar timer2 (bit 12 = 0) y limpiar auto-reload (bit 15)
    rTCON &= ~(0x9000);

    timer2_num_int = 0;
    rTCNTO2 = 0x0;

    // establecer update=manual (bit 13) + inverter=on (¿? será inverter off un cero en el bit 14 pone el inverter en off)
    rTCON |= 0x2000;
    // iniciar timer2 (bit 12) y auto-reload (bit 15) y limpia bit 13
	rTCON ^= 0xb000;
}

inline unsigned int timer2_leer(void)
{
    return TIMER2_PERIODO_US * (timer2_num_int * (TIMER2_INTERVALO_SUPERIOR - TIMER2_INTERVALO_INFERIOR) + (TIMER2_INTERVALO_SUPERIOR - rTCNTO2));
}

inline unsigned int timer2_parar(void)
{
    // parar timer2 (bit 12 = 0) y limpiar auto-reload (bit 15)
    rTCON &= ~(0x9000);

    return timer2_leer();
}
