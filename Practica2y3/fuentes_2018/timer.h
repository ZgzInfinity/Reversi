/*********************************************************************************************
* Fichero:		timer.h
* Autor:        737215 Ruben Rodríguez Esteban, 738455 Adrián Samatán Alastuey 
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

/*--- declaracion de funciones visibles del módulo timer.c/timer.h ---*/
void timer0_init(void);

// Comienza la cuenta del timer0
void timer0_empezar(void);

// Devuelve el numero de microsegundos desde que se llamo a timer0_empezar
unsigned int timer0_leer(void);

// Para timer0 y devuelve el numero de microsegundos desde que se llamo a timer0_empezar
unsigned int timer0_parar(void);

#endif /* _TIMER_H_ */
