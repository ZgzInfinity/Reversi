/*********************************************************************************************
* Fichero:	botones_antirebotes.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementacion del modulo botones_antirebotes
*********************************************************************************************/

#include "botones_antirebotes.h"
#include "gestion_tiempo.h"
#include "timer.h"
#include "reversi_def.h"
#include "pila_depuracion.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

enum estado_botones_antirebotes
{
    ESTADO_INICIAL,
    ESTADO_ESPERA_INICIAL,
    ESTADO_ESPERA_ACTIVA,
    ESTADO_FINAL,
};

enum
{
    N_CALIBRACIONES = 5,
    TIEMPO_ESPERA_ACTIVA_US = 20000,
    TIEMPO_ESPERA_BASE_US = 200000,
    TIEMPO_MARGEN_ANTIREBOTE_US = 500000,
};

/*--------------------------------------------------------------*/
/*------------- VARIABLES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

static volatile enum estado_botones_antirebotes estado_actual = ESTADO_INICIAL;
static volatile enum estado_button boton_actual = BUTTON_NONE;
static volatile unsigned int tiempo_referencia;
static unsigned int TIEMPO_ESPERA_INICIAL_US = TIEMPO_ESPERA_BASE_US;
static unsigned int TIEMPO_ESPERA_FINAL_US = TIEMPO_ESPERA_BASE_US;

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

void botones_antirebotes_callback(enum estado_button boton_presionado);

void botones_antirebotes_init(void)
{
    estado_actual = ESTADO_INICIAL;
    boton_actual = BUTTON_NONE;
    button_empezar(&botones_antirebotes_callback);
}

void botones_antirebotes_callback(enum estado_button boton_presionado)
{
    if (boton_presionado != BUTTON_NONE)
    {
        button_deshabilitar();
        estado_actual = ESTADO_ESPERA_INICIAL;
        boton_actual = boton_presionado;
        tiempo_referencia = timer0_leer();
    }
}

enum estado_button botones_antirebotes_leer(void)
{
    return boton_actual;
}

void botones_antirebotes_gestion(unsigned int tiempo_actual)
{
    switch (estado_actual)
    {
    case ESTADO_INICIAL:
    {
#ifdef SIM_IN_USE
        if (button_estado() != BUTTON_NONE)
        {
            botones_antirebotes_callback(button_estado());
        }
#endif
        break;
    }
    case ESTADO_ESPERA_INICIAL:
    {
        if (diff_tiempo_pasado(tiempo_actual, tiempo_referencia, TIEMPO_ESPERA_INICIAL_US))
        {
            push_debug(ESTADO_ESPERA_INICIAL, 0);
            estado_actual = ESTADO_ESPERA_ACTIVA;
            tiempo_referencia = tiempo_actual;
        }
        break;
    }
    case ESTADO_ESPERA_ACTIVA:
    {
        if (diff_tiempo_pasado(tiempo_actual, tiempo_referencia, TIEMPO_ESPERA_ACTIVA_US))
        {
            enum estado_button boton_nuevo = button_estado();
            push_debug(ESTADO_ESPERA_ACTIVA, boton_nuevo);
            tiempo_referencia = tiempo_actual;
            if (boton_nuevo != boton_actual)
            {
                boton_actual = boton_nuevo;
                estado_actual = ESTADO_FINAL;
            }
        }
        break;
    }
    case ESTADO_FINAL:
    {
        if (diff_tiempo_pasado(tiempo_actual, tiempo_referencia, TIEMPO_ESPERA_FINAL_US))
        {
            push_debug(ESTADO_FINAL, 0);
            estado_actual = ESTADO_INICIAL;
            button_empezar(&botones_antirebotes_callback);
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

enum estados_calibracion
{
    SIN_PULSAR,
    PULSADO_INICIO,
    PULSADO_FIN
};

void calibrar_callback(enum estado_button boton_presionado)
{
    push_debug(0, 0);
}

void botones_antirebotes_calibrar()
{
    dibujar_pantalla_calibrar_botones();

    button_empezar(&calibrar_callback);
    int num_calibraciones;
    unsigned int tiempo_espera_inicial_calibrado_us = 0;
    unsigned int tiempo_espera_final_calibrado_us = 0;

    dibujar_pantalla_calibrar_botones_texto("PREPARARSE PARA PRESIONAR BOTON");
    delay(UN_SEGUNDO_US * 2);

    for (num_calibraciones = 0; num_calibraciones < N_CALIBRACIONES; ++num_calibraciones)
    {
        dibujar_pantalla_calibrar_botones_texto("PRESIONAR BOTON IZQUIERDO O DERECHO");
        delay(TIEMPO_MARGEN_ANTIREBOTE_US);
        tiempo_espera_inicial_calibrado_us += pila_debug_ultimo_evento() - pila_debug_evento(1);
        pila_debug_limpiar();

        dibujar_pantalla_calibrar_botones_texto("SOLTAR BOTON");
        delay(TIEMPO_MARGEN_ANTIREBOTE_US);
        tiempo_espera_final_calibrado_us += pila_debug_ultimo_evento() - pila_debug_evento(1);
        pila_debug_limpiar();
    }

    TIEMPO_ESPERA_INICIAL_US = tiempo_espera_inicial_calibrado_us / N_CALIBRACIONES;
    TIEMPO_ESPERA_FINAL_US = tiempo_espera_final_calibrado_us / N_CALIBRACIONES;

    if (TIEMPO_ESPERA_INICIAL_US > TIEMPO_ESPERA_BASE_US)
    {
        TIEMPO_ESPERA_INICIAL_US = TIEMPO_ESPERA_BASE_US;
    }

    if (TIEMPO_ESPERA_FINAL_US > TIEMPO_ESPERA_BASE_US)
    {
        TIEMPO_ESPERA_FINAL_US = TIEMPO_ESPERA_BASE_US;
    }
}
