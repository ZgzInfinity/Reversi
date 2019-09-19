/*********************************************************************************************
* Fichero:	timer2.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero cabecera del modulo timer2
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

// Inicializa los registros del timer2, configurando las lineas, habilitando el timer, estableciendo la precision...
void timer2_init(void);

// Comienza la cuenta del timer2
void timer2_empezar(void);

// Devuelve el numero de microsegundos desde que se llamo a timer2_empezar
unsigned int timer2_leer(void);

// Para timer2 y devuelve el numero de microsegundos desde que se llamo a timer2_empezar
unsigned int timer2_parar(void);

#endif /* _TIMER2_H_ */
