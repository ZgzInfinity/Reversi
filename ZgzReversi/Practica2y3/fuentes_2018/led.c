/*********************************************************************************************
* Fichero:		led.c
* Autor:		737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:		funciones de control de los LED de la placa
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "led.h"
#include "44b.h"
#include "44blib.h"

/*--- variables globales del módulo ---*/
static int led_state = 0; /* estado del LED */

/*--- codigo de las funciones públicas ---*/
void leds_on(void)
{
	Led_Display(0x3);
}

void leds_off(void)
{
	Led_Display(0x0);
}

void led1_on(void)
{
	led_state = led_state | 0x1;
	Led_Display(led_state);
}

void led1_off(void)
{
	led_state = led_state & 0xfe;
	Led_Display(led_state);
}

void led2_on(void)
{
	led_state = led_state | 0x2;
	Led_Display(led_state);
}

void led2_off(void)
{
	led_state = led_state & 0xfd;
	Led_Display(led_state);
}

void leds_switch(void)
{
	led_state ^= 0x03;
	Led_Display(led_state);
}

void Led_Display(int LedStatus)
{
	led_state = LedStatus;

	if ((LedStatus & 0x01) == 0x01)
		rPDATB = rPDATB & 0x5ff; /* poner a 0 el bit 9 del puerto B */
	else
		rPDATB = rPDATB | 0x200; /* poner a 1 el bit 9 del puerto B */

	if ((LedStatus & 0x02) == 0x02)
		rPDATB = rPDATB & 0x3ff; /* poner a 0 el bit 10 del puerto B */
	else
		rPDATB = rPDATB | 0x400; /* poner a 1 el bit 10 del puerto B */
}
