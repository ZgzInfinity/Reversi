/*********************************************************************************************
* Fichero:	main.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "timer2.h"
#include "44blib.h"
#include "44b.h"
#include "tests.h"
#include "exceptions.h"
#include "pila_depuracion.h"
#include "botones_antirebotes.h"
#include "tp.h"
#include "lcd.h"
#include "pantalla.h"
#include "option.h"

//extern void reversi8_test();
extern void reversi_main();

void cambiar_a_modo_usuario()
{
	char *userStack = (char *)_ISR_STARTADDRESS - 0xf00;
	__asm__ volatile("mrs r0, cpsr");
	__asm__ volatile("msr spsr, r0");
	__asm__ volatile("msr cpsr_c, 0x10");
	__asm__ volatile("mov sp, %0"
					 : "+r"(userStack));
}

/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init(); // Inicializacion de la placa, interrupciones y puertos

	leds_off();

	pila_debug_init();
	timer0_init(); // Inicializacion del temporizador
	timer0_empezar();
	timer2_init(); // Inicializacion del temporizador
	timer2_empezar();
	button_init(); // inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();  // inicializamos el 8led
	exception_init();
	Lcd_Init();
	pantalla_tactil_init();

	pantalla_tactil_calibrar();
	botones_antirebotes_calibrar();

	cambiar_a_modo_usuario();
	reversi_main();
}
