/*********************************************************************************************
* Fichero:	8led.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Funciones de control del display 8-segmentos
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "44b.h"
#include "44blib.h"

/*--- definicion de valores ---*/
/* Mapa de bits de cada segmento
  (valor que se debe escribir en el display para que se ilumine el segmento) */
/* NOTA:
 * Los datos definidos en un tipo enumerado son de tipo int (por defecto)
 */
enum
{
  cero = ~0xED,
  uno = ~0x60,
  dos = ~0xCE,
  tres = ~0xEA,
  cuatro = ~0x63,
  cinco = ~0xAB,
  seis = ~0x2F,
  siete = ~0xE0,
  ocho = ~0xEF,
  nueve = ~0xE3,
  A = ~0xE7,
  B = ~0x2F,
  C = ~0x8D,
  D = ~0x6E,
  E = ~0x8F,
  F = ~0x87,
  blank = ~0x00,
  size_8led = 17
};

/*--- variables globales del módulo ---*/
/* tabla de segmentos */
static int
    Symbol[size_8led] = {cero, uno, dos, tres, cuatro, cinco, seis, siete, ocho, nueve, A, B, C, D, E, F, blank};

#ifdef SIM_IN_USE
static unsigned char estado_8led = (unsigned char)cero;
#endif

/*--- código de las funciones ---*/
// inicialización del estado del display
void D8Led_init(void)
{
  /* Estado inicial del display con todos los segmentos iluminados
	   (buscar en los ficheros de cabecera la direccion implicada) */
#ifdef SIM_IN_USE
  estado_8led = (unsigned char)cero;
#else
  LED8ADDR = (unsigned char)cero;
#endif
}

// función para mostrar valores en el display de 8 segmentos
// recibe como parametro el valor a mostrar en el display
void D8Led_symbol(int value)
{
  /* muestra el Symbol[value] en el display (analogo al caso anterior) */
  if ((value >= 0) && (value < size_8led))
  {
#ifdef SIM_IN_USE
    estado_8led = (unsigned char)Symbol[value];
#else
    LED8ADDR = (unsigned char)Symbol[value];
#endif
  }
}
