/*********************************************************************************************
* Fichero:	jugada_por_pantalla.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementación del módulo que se encarga de gestionar la jugada por pantalla
*********************************************************************************************/

#include "jugada_por_pantalla.h"
#include "botones_antirebotes.h"
#include "gestion_tiempo.h"

enum
{
    TIEMPO_PARPADEO_US = 500000,
    TIEMPO_CONFIRMACION_US = 2000000,
};

enum estado_jugada
{
    ESTADO_SELECCION,
    ESTADO_CONFIRMACION,
};

static unsigned int tiempo_referencia_parpadeo = 0;
static unsigned int tiempo_comienzo_confirmacion = 0;
static enum estado_jugada estado_actual = ESTADO_SELECCION;
static enum estado_button boton_presionado_anterior = BUTTON_NONE;
static char color_actual = FICHA_GRIS;

// incremento de la coordenada de la fila
// recibe como parametros el tablero, la fila y la columna
void incrementar_fila(char tablero[][DIM_TABLERO], char *fila, char columna)
{
    do
    {
        (*fila)++;
        if (*fila >= DIM_TABLERO)
        {
            *fila = 0;
        }
    } while (tablero[*fila][columna] != CASILLA_VACIA);
}

// incremento de la coordenada de la columna
// recibe como parametros el tablero, la fila y la columna
void incrementar_columna(char tablero[][DIM_TABLERO], char fila, char *columna)
{
    do
    {
        (*columna)++;
        if (*columna >= DIM_TABLERO)
        {
            *columna = 0;
        }
    } while (tablero[fila][*columna] != CASILLA_VACIA);
}

// gestion del parpadeo de la ficha
// recibe como parametros la fila y la columna donde esta la ficha
void parpadeo_ficha(char fila, char columna)
{
    if (color_actual == FICHA_GRIS)
    {
        color_actual = CASILLA_VACIA;
    }
    else
    {
        color_actual = FICHA_GRIS;
    }

    dibujar_ficha_virtual(fila, columna, color_actual);
}

// retorna si se pulsa el centro de la pantalla
char centro_tablero_pulsado()
{
    unsigned int x, y;
    if (coordenadas_pantalla_tactil_presionada(&x, &y))
    {
        return coord_en_centro_tablero(x, y);
    }
    else
    {
        return 0;
    }
}

// procesamiento de la jugada por medio de los pulsadores
// recibe como parametros el tablero, el tiempo de la actualización, la fila y la columna
char jugada_lista_pantalla(char tablero[][DIM_TABLERO], unsigned int tiempo_ahora, char *fila, char *columna)
{
    botones_antirebotes_gestion(tiempo_ahora);

    switch (estado_actual)
    {
    case ESTADO_SELECCION:
    {
        if (centro_tablero_pulsado())
        {
            dibujar_ficha_virtual(*fila, *columna, FICHA_GRIS);
            dibujar_leyenda_virtual("PULSA PARA CANCELAR");
            estado_actual = ESTADO_CONFIRMACION;
            tiempo_comienzo_confirmacion = tiempo_ahora;
        }
        else
        {
            enum estado_button boton_presionado = botones_antirebotes_leer();

            if (boton_presionado_anterior != boton_presionado)
            {
                boton_presionado_anterior = boton_presionado;

                switch (boton_presionado)
                {
                case BUTTON_IZ:
                {
                    dibujar_ficha_virtual(*fila, *columna, CASILLA_VACIA);
                    incrementar_columna(tablero, *fila, columna);
                    break;
                }
                case BUTTON_DR:
                {
                    dibujar_ficha_virtual(*fila, *columna, CASILLA_VACIA);
                    incrementar_fila(tablero, fila, *columna);
                    break;
                }
                default:
                {
                    break;
                }
                }
            }

            if (diff_tiempo_pasado(tiempo_ahora, tiempo_referencia_parpadeo, TIEMPO_PARPADEO_US))
            {
                parpadeo_ficha(*fila, *columna);
                tiempo_referencia_parpadeo = tiempo_ahora;
            }
        }

        break;
    }
    case ESTADO_CONFIRMACION:
    {
        if (diff_tiempo_pasado(tiempo_ahora, tiempo_comienzo_confirmacion, TIEMPO_CONFIRMACION_US))
        {
            dibujar_leyenda_virtual("PULSA PARA JUGAR");
            dibujar_ficha_virtual(*fila, *columna, CASILLA_VACIA);
            estado_actual = ESTADO_SELECCION;

            return 1;
        }

        enum estado_button boton_presionado = botones_antirebotes_leer();

        if (comprobar_pantalla_tactil_presionada() || boton_presionado != BUTTON_NONE)
        {
            dibujar_leyenda_virtual("PULSA PARA JUGAR");
            dibujar_ficha_virtual(*fila, *columna, CASILLA_VACIA);
            estado_actual = ESTADO_SELECCION;
            *fila = 0;
            *columna = 0;
        }

        if (diff_tiempo_pasado(tiempo_ahora, tiempo_referencia_parpadeo, TIEMPO_PARPADEO_US))
        {
            parpadeo_ficha(*fila, *columna);
            tiempo_referencia_parpadeo = tiempo_ahora;
        }

        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}
