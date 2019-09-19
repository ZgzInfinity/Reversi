/*********************************************************************************************
* Fichero:	gestion_profiling.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementación del módulo que se encarga de gestionar el profiling en pantalla
*********************************************************************************************/

#include "gestion_profiling.h"
#include "reversi_def.h"
#include "timer2.h"

/*--------------------------------------------------------------*/
/*------------ VARIABLES PRIVADAS DEL MODULO -------------------*/
/*--------------------------------------------------------------*/

static unsigned int tiempo_patron_volteo_us = 0;
static unsigned int tiempo_partida_s = 0;
static unsigned int tiempo_calculos_us = 0;
static unsigned int invocaciones_patron_volteo = 0;
static unsigned int tiempo_referencia_partida_us = 0;

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

// se resetean a cero los tiempos de gestion_profiling
void resetear_tiempos(void)
{
    tiempo_patron_volteo_us = 0;
    tiempo_partida_s = 0;
    tiempo_calculos_us = 0;
    invocaciones_patron_volteo = 0;
    tiempo_referencia_partida_us = 0;
}

// se actualizan los tiempos del profiling
// recibe como parametro el tiempo leido por el timer0
void actualizar_tiempo_partida(unsigned int tiempo_ahora_us)
{
    // se actualiza si ha pasado un segundo
    if (diff_tiempo_pasado(tiempo_ahora_us, tiempo_referencia_partida_us, UN_SEGUNDO_US))
    {
        tiempo_referencia_partida_us = tiempo_ahora_us;
        tiempo_partida_s += 1;
        dibujar_tiempos_virtual(tiempo_partida_s, tiempo_calculos_us, tiempo_patron_volteo_us, invocaciones_patron_volteo);
    }
}

// actualiza el tiempo para realizar los calculos
// recibe como parametro el nuevo tiempo que han tardado en efectuarse los calculos
void actualizar_tiempo_calculos(unsigned int tiempo_calculos_nuevo_us)
{
    tiempo_calculos_us += tiempo_calculos_nuevo_us;
    dibujar_tiempos_virtual(tiempo_partida_s, tiempo_calculos_us, tiempo_patron_volteo_us, invocaciones_patron_volteo);
}

// actualiza el tiempo de invocación a patron_volteo
// recibe como parametro el nuevo tiempo que han tardado en efectuarse la invocación
void actualizar_tiempo_invocaciones_patron_volteo(unsigned int tiempo_patron_nuevo_us)
{
    invocaciones_patron_volteo += 1;
    tiempo_patron_volteo_us += tiempo_patron_nuevo_us;
    dibujar_tiempos_virtual(tiempo_partida_s, tiempo_calculos_us, tiempo_patron_volteo_us, invocaciones_patron_volteo);
}