/*********************************************************************************************
* Fichero:	gestion_latido.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo gestion_latido
*********************************************************************************************/

#include "gestion_latido.h"
#include "gestion_tiempo.h"
#include "led.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

// Constante para gestionar el intervalo de latido
enum
{
	TIEMPO_ESPERA_LATIDO_US = 250000
};

/*--------------------------------------------------------------*/
/*------------ VARIABLES PRIVADAS DEL MODULO -------------------*/
/*--------------------------------------------------------------*/

static unsigned int tiempo_referencia = 0;

/*-----------------------------------------------------*/
/*------------ FUNCIONES DEL MODULO -------------------*/
/*-----------------------------------------------------*/

// tratamiento del latido
// recibe como parámetro el tiempo leido por el timer0
void gestion_latido(unsigned int tiempo_ahora)
{
	// se detecta si ha pasado un cuarto de segundo
	if (diff_tiempo_pasado(tiempo_ahora, tiempo_referencia, TIEMPO_ESPERA_LATIDO_US))
	{
		// se cambia el estado del led de ON a OFF y al revés
		leds_switch();
		// se actualiza el tiempo
		tiempo_referencia = tiempo_ahora;
	}
}
