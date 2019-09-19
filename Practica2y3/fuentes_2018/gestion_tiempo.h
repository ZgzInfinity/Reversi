/*********************************************************************************************
* Fichero:		gestion_tiempo.h
* Autor:        737215 Ruben Rodríguez Esteban, 738455 Adrián Samatán Alastuey 
* Descrip:		fichero de interfaz del módulo gestion_tiempo
* Version:
*********************************************************************************************/

#ifndef _GESTION_TIEMPO_H_
#define _GESTION_TIEMPO_H_

// se retarda la ejecucion durante tiempo_espera_us microsegundos
void delay(unsigned int tiempo_espera_us);

// retorna si entre tiempo_actual_us y tiempo_referencia_us es mayor que tiempo_pasado_us
char diff_tiempo_pasado(unsigned int tiempo_actual, unsigned int tiempo_referencia, unsigned int tiempo_pasado);

#endif /* _GESTION_TIEMPO_H_ */
