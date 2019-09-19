/*********************************************************************************************
* Fichero:	pantalla_tactil.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de interfaz del módulo que se encarga de gestionar la pantalla táctil
*********************************************************************************************/

#ifndef _PANTALLA_TACTIL_H_
#define _PANTALLA_TACTIL_H_

//activa la pantalla tactil
void pantalla_tactil_init(void);

// realiza la calibracon de la pantalla
void pantalla_tactil_calibrar(void);

// comprobar si la pantalla se ha presionado
char comprobar_pantalla_tactil_presionada(void);

// devuelve las coordeanadas x e y de la pantalla
char coordenadas_pantalla_tactil_presionada(unsigned int *x, unsigned int *y);

#endif /* _PANTALLA_TACTIL_H_ */