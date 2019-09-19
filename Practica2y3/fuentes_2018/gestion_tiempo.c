/*********************************************************************************************
* Fichero:		gestion_tiempo.h
* Autor:        737215 Ruben Rodríguez Esteban, 738455 Adrián Samatán Alastuey 
* Descrip:		fichero de implementación del módulo gestion_tiempo
* Version:
*********************************************************************************************/

#include <limits.h>
#include "gestion_tiempo.h"
#include "timer.h"

// Función para gestionar el tiempo de espera
void delay(unsigned int tiempo_espera_us)
{
	// Lectura del tiempo actual del timer0
	unsigned int tiempo_referencia_us = timer0_leer();

	char ha_pasado_tiempo = 0;
	while (!ha_pasado_tiempo)
	{

		ha_pasado_tiempo = diff_tiempo_pasado(timer0_leer(), tiempo_referencia_us, tiempo_espera_us);
	}
}

// retorna si entre tiempo_actual_us y tiempo_referencia_us es mayor que tiempo_pasado_us
char diff_tiempo_pasado(unsigned int tiempo_actual_us, unsigned int tiempo_referencia_us, unsigned int tiempo_pasado_us)
{
	if (tiempo_actual_us > tiempo_referencia_us)
	{
		return tiempo_actual_us - tiempo_referencia_us > tiempo_pasado_us;
	}

	// se evita desbordamiento
	return UINT_MAX - tiempo_referencia_us + tiempo_actual_us > tiempo_pasado_us;
}
