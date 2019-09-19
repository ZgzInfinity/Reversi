/*********************************************************************************************
* Fichero:	8led.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Funciones de control del display 8-segmentos
* Version:
*********************************************************************************************/

#ifndef _8LED_H_
#define _8LED_H_

/*--- declaración de funciones visibles del módulo 8led.c/8led.h ---*/

// inicialización del estado del display
void D8Led_init(void);

// función para mostrar valores en el display de 8 segmentos
// recibe como parametro el valor a mostrar en el display
void D8Led_symbol(int value);

#endif /* _8LED_H_ */
