/*********************************************************************************************
* Fichero:	pantalla.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de interfaz del módulo que se encarga de gestionar el dibujado de la pantalla
*********************************************************************************************/

#ifndef _PANTALLA_H_
#define _PANTALLA_H_

#include "def.h"

// Dibujado de la pantalla inicial
void dibujar_pantalla_inicial(void);

// dibujar el tablero virtual
void dibujar_tablero_virtual(void);

// limpieza y dibujado del texto
void limpiar_y_dibujar_texto_virtual(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, char *texto);

// muestreo de los tiempos de profiling
void dibujar_tiempos_virtual(unsigned int tiempo_total_s, unsigned int tiempo_calculos_s, unsigned int tiempo_patron_s, unsigned int n_invocaciones_patron);

// dibujo de la leyenda
void dibujar_leyenda_virtual(char *leyenda);

// refresca la pantalla de juego con las actualzaciones a 60fps
void refrescar_pantalla_juego(unsigned int tiempo_ahora_us, unsigned int tiempo_referencia_us);

// dibujar panatlla final con los resultados
// recibe como parametros las fichas blancas y negras con las que ha acabado cada jugador
void dibujar_pantalla_final(int blancas, int negras);

// dibujar tablero con fichas y tiempos de profiling
void dibujar_pantalla_juego(void);

// Dibujado de la pantalla inicial
void dibujar_pantalla_inicial(void);

// muestra por pantalla el mensaje de pulsar o soltar boton
void dibujar_pantalla_calibrar_botones_texto(char *texto);

// dibujado de la pantalla de calibración de los botones
void dibujar_pantalla_calibrar_botones(void);

// muestra por pantalla el texto que indica al usuario donde debe pulsar para calibrar
void dibujar_pantalla_calibrar_tactil_texto(char *texto);

// dibujado de la pantalla informativa de la pantalla tactil
void dibujar_pantalla_calibrar_tactil(void);

// devuelve la coordenada en x y en y
char coord_en_centro_tablero(unsigned int x, unsigned int y);

/* colocar ficha en tablero con f y c en rango [0..7] */
void dibujar_ficha_virtual(int f, int c, char color);

#endif /* _PANTALLA_H_ */
