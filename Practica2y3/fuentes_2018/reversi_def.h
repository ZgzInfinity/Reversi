/*********************************************************************************************
* Fichero:	reversi_def.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de definiciones comunes de constantes y funciones utiles para reversi
*********************************************************************************************/

#ifndef __REVERSI_DEF_H_
#define __REVERSI_DEF_H_

#include "def.h"

// Tamaño del tablero
enum
{
    DIM_TABLERO = 8
};

// Estados de las casillas del tablero
enum
{
    CASILLA_VACIA = 0,
    FICHA_BLANCA = 1,
    FICHA_NEGRA = 2,
    FICHA_GRIS = 3,
};

enum
{
    UN_SEGUNDO_US = 1000000,
};

// itoa de K&R, The C Programming Language pagina 60
/* itoa:  convert n to characters in s */
void itoa(int n, char s[]);

/* reverse:  reverse string s in place */
void reverse(char s[]);

void ordenar(ULONG valores[], int n_elementos);

#endif /* __REVERSI_DEF_H_ */
