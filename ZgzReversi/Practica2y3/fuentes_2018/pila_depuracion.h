/*********************************************************************************************
* Fichero:	pila_depuracion.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de interfaz del modulo pila_depuracion
*********************************************************************************************/

#ifndef _PILA_DEPURACION_H_
#define _PILA_DEPURACION_H_

#include <stdint.h>

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

// vaciado de la pila
void pila_debug_init(void);

void pila_debug_limpiar(void);

// apilación de los eventos en la pila
void push_debug(uint8_t id_evento, uint32_t aux_data);

uint32_t pila_debug_evento(int i);

uint32_t pila_debug_primer_evento(void);

uint32_t obtencion_ultimo_evento(void);

#endif /* _PILA_DEPURACION_H_ */
