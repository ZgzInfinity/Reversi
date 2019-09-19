/*********************************************************************************************
* Fichero:	jugada_por_pantalla.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de interfaz del módulo que se encarga de gestionar la jugada por pantalla
*********************************************************************************************/

#ifndef JUGADA_POR_PANTALLA_H_
#define JUGADA_POR_PANTALLA_H_

#include "reversi_def.h"

char jugada_lista_pantalla(char tablero[][DIM_TABLERO], unsigned int tiempo_ahora, char *fila, char *columna);

#endif /* JUGADA_POR_PANTALLA_H_ */