/*********************************************************************************************
* Fichero:	jugada_por_botones.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo jugada_por_botones
*********************************************************************************************/

#include "8led.h"
#include "botones_antirebotes.h"
#include "gestion_tiempo.h"
#include "reversi_def.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

// Constantes para determinar el margen de autoincremento
// y los valores minimo y maximo de fila y columna
enum
{
	TIEMPO_AUTOINCREMENTO_US = 500000,
	VALOR_FILA_COLUMNA_MIN = 0,
	VALOR_FILA_COLUMNA_MAX = DIM_TABLERO,
};

// Constantes para identificar el automata de estados
enum estado_jugada
{
	ESTADO_GESTION_FILA,
	ESTADO_SELECCION_FILA,
	ESTADO_GESTION_COLUMNA,
	ESTADO_SELECCION_COLUMNA,
};

/*--------------------------------------------------------------*/
/*------------ VARIABLES PRIVADAS DEL MODULO -------------------*/
/*--------------------------------------------------------------*/

static enum estado_jugada estado_actual = ESTADO_GESTION_FILA;
static char valor_actual;
static enum estado_button boton_anterior;
static unsigned int tiempo_referencia = 0;

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

// incremento de fila o columna en una unidad en el instante tiempo_ahora
// recibe como parametros el valor a incrementar y el instante temporal devuelto por el timer0
void incrementar_valor_actual(char valor, unsigned int tiempo_ahora);

// actualiza
void estado_gestion(unsigned int tiempo_ahora, enum estado_button boton_leido);

// actualiza el estado del automata
// recibe como parametros el leido por el timer0 y el estado del boton
void estado_seleccion(unsigned int tiempo_ahora, enum estado_button boton_presionado);

// incremento de fila o columna en una unidad en el instante tiempo_ahora
// recibe como parametros el valor a incrementar y el instante temporal devuelto por el timer0
void incrementar_valor_actual(char valor, unsigned int tiempo_ahora)
{
	// Placa realiza division por software, asi es mas rapido

	tiempo_referencia = tiempo_ahora;

	valor_actual++;
	if (valor_actual > VALOR_FILA_COLUMNA_MAX)
	{
		valor_actual = VALOR_FILA_COLUMNA_MIN;
	}
}

// actualiza el estado del automata de la maquina por botones
// recibe como parametros el tiempo leido por el timer0 y el estado del boton
void estado_gestion(unsigned int tiempo_ahora, enum estado_button boton_leido)
{
	int valor_8_led;
	enum estado_jugada nuevo_estado;

	if (estado_actual == ESTADO_GESTION_FILA)
	{
		valor_8_led = 0xF;
		nuevo_estado = ESTADO_SELECCION_FILA;
	}
	else if (estado_actual == ESTADO_GESTION_COLUMNA)
	{
		valor_8_led = 0xC;
		nuevo_estado = ESTADO_SELECCION_COLUMNA;
	}

	D8Led_symbol(valor_8_led);
	boton_anterior = boton_leido;
	if (boton_anterior == BUTTON_IZ)
	{
		valor_actual = VALOR_FILA_COLUMNA_MIN;
		estado_actual = nuevo_estado;
		tiempo_referencia = tiempo_ahora;
	}
}

// actualizar el estado de la maquina de la jugada por botones
void estado_seleccion(unsigned int tiempo_ahora, enum estado_button boton_presionado)
{
	char ha_pasado_tiempo_autoincremento = diff_tiempo_pasado(tiempo_ahora, tiempo_referencia, TIEMPO_AUTOINCREMENTO_US);

	D8Led_symbol(valor_actual);

	if (boton_anterior == BUTTON_IZ && boton_presionado == BUTTON_IZ && ha_pasado_tiempo_autoincremento)
	{
		incrementar_valor_actual(valor_actual, tiempo_ahora);
	}
	else if (boton_anterior == BUTTON_NONE && boton_presionado == BUTTON_IZ)
	{
		incrementar_valor_actual(valor_actual, tiempo_ahora);
	}
	else if (boton_presionado == BUTTON_DR)
	{
		if (estado_actual == ESTADO_SELECCION_FILA)
		{
			estado_actual = ESTADO_GESTION_COLUMNA;
		}
		else if (estado_actual == ESTADO_SELECCION_COLUMNA)
		{
			estado_actual = ESTADO_GESTION_FILA;
		}
	}

	boton_anterior = boton_presionado;
}

// Función para gestionar el procesamiento de una jugada usando los botones
char jugada_lista_botones(unsigned int tiempo_ahora, char *fila, char *columna)
{
	botones_antirebotes_gestion(tiempo_ahora);

	enum estado_button boton_presionado = botones_antirebotes_leer();

	switch (estado_actual)
	{
	case ESTADO_GESTION_FILA:
	{
		estado_gestion(tiempo_ahora, boton_presionado);
		*fila = valor_actual;
		return 0;
	}
	case ESTADO_SELECCION_FILA:
	{
		estado_seleccion(tiempo_ahora, boton_presionado);
		*fila = valor_actual;
		return 0;
	}
	case ESTADO_GESTION_COLUMNA:
	{
		estado_gestion(tiempo_ahora, boton_presionado);
		*columna = valor_actual;
		return 0;
	}
	case ESTADO_SELECCION_COLUMNA:
	{
		estado_seleccion(tiempo_ahora, boton_presionado);
		*columna = valor_actual;
		return estado_actual == ESTADO_GESTION_FILA;
	}
	}
}
