/*********************************************************************************************
* Fichero:	botones_antirebotes.h
* Autores: 737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de interfaz del modulo botones_antirebotes
*********************************************************************************************/

#ifndef _BOTONES_ANTIREBOTES_H_
#define _BOTONES_ANTIREBOTES_H_

#include "button.h"

/*--------------------------------------------------------------*/
/*----------------- FUNCIONES DEL MODULO -----------------------*/
/*--------------------------------------------------------------*/

void botones_antirebotes_init(void);

enum estado_button botones_antirebotes_leer(void);

void botones_antirebotes_gestion(unsigned int tiempo_actual);

void calcular_tiempo_antirebotes(void);

#endif /* _BOTONES_ANTIREBOTES_H_ */
