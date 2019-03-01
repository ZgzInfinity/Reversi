/*********************************************************************************************
* Fichero:	gestion_profiling.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de interfaz del módulo que se encarga de gestionar el profiling en pantalla
*********************************************************************************************/

#ifndef __GESTION_PROFILING_H_
#define __GESTION_PROFILING_H_

// se resetean a cero los tiempos de gestion_profiling
void resetear_tiempos(void);

// se actualizan los tiempos del profiling
// recibe como parametro el tiempo leido por el timer0
void actualizar_tiempo_partida(unsigned int tiempo_ahora_us);

// actualiza el tiempo para realizar los calculos
// recibe como parametro el nuevo tiempo que han tardado en efectuarse los calculos
void actualizar_tiempo_calculos(unsigned int tiempo_calculos_nuevo_us);

// actualiza el tiempo de invocación a patron_volteo
// recibe como parametro el nuevo tiempo que han tardado en efectuarse la invocación
void actualizar_tiempo_invocaciones_patron_volteo(unsigned int tiempo_patron_nuevo_us);

#endif /* __GESTION_PROFILING_H_ */
