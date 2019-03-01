/*********************************************************************************************
* Fichero:		led.c
* Autor:        737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:		funciones de control de los LED de la placa
* Version:
*********************************************************************************************/

#ifndef _LED_H_
#define _LED_H_

/*--- declaracion de funciones visibles del módulo led.c/led.h ---*/
/* (*) la función se invoca en el código suministrado */
void leds_on(void);              // todos los leds encendidos
void leds_off(void);             // todos los leds apagados (*)
void led1_on(void);              // led 1 encendido (*)
void led1_off(void);             // led 1 apagado
void led2_on(void);              // led 2 encendido
void led2_off(void);             // led 2 apagado
void leds_switch(void);          // invierte el valor de los leds (*)
void Led_Display(int LedStatus); // control directo del LED

#endif /* _LED_H_ */
