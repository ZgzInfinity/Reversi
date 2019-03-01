/*********************************************************************************************
* Fichero:	reversi8_2018.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero principal del juego reversi8
*********************************************************************************************/

#include "fuentes_2018/botones_antirebotes.h"
#include "fuentes_2018/8led.h"
#include "fuentes_2018/gestion_latido.h"
#include "fuentes_2018/jugada_por_botones.h"
#include "fuentes_2018/timer.h"
#include "fuentes_2018/timer2.h"
#include "fuentes_2018/exceptions.h"
#include "fuentes_2018/pantalla.h"
#include "fuentes_2018/lcd.h"
#include "fuentes_2018/gestion_profiling.h"
#include "fuentes_2018/reversi_def.h"

// Valores que puede devolver la función patron_volteo())
enum
{
    NO_HAY_PATRON = 0,
    PATRON_ENCONTRADO = 1
};

// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
enum
{
    NO = 0,
    SI = 1,
    CASILLA_OCUPADA = 2,
};

/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean más fáciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const char __attribute__((aligned(8))) tabla_valor[DIM_TABLERO][DIM_TABLERO] =
    {
        {8, 2, 7, 3, 3, 7, 2, 8},
        {2, 1, 4, 4, 4, 4, 1, 2},
        {7, 4, 6, 5, 5, 6, 4, 7},
        {3, 4, 5, 0, 0, 5, 4, 3},
        {3, 4, 5, 0, 0, 5, 4, 3},
        {7, 4, 6, 5, 5, 6, 4, 7},
        {2, 1, 4, 4, 4, 4, 1, 2},
        {8, 2, 7, 3, 3, 7, 2, 8}};

// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
static const char vSF[DIM_TABLERO] = {-1, -1, 0, 1, 1, 1, 0, -1};
static const char vSC[DIM_TABLERO] = {0, 1, 1, 1, 0, -1, -1, -1};

//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deberían serlo
// tablero, fila, columna y ready son varibles que se deberían definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea más fácil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
static char __attribute__((aligned(8))) tablero[DIM_TABLERO][DIM_TABLERO] = {
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
    {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}};

enum
{
    MOVIMIENTOS_TEST = 3
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM_TABLERO], char candidatas[][DIM_TABLERO])
{
    botones_antirebotes_init();
    resetear_tiempos();

    int i, j;

    for (i = 0; i < DIM_TABLERO; i++)
    {
        for (j = 0; j < DIM_TABLERO; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
#if 0
    for (i = 3; i < 5; ++i) {
	for(j = 3; j < 5; ++j) {
	    tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
	}
    }

    for (i = 2; i < 6; ++i) {
	for (j = 2; j < 6; ++j) {
	    if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
		candidatas[i][j] = CASILLA_OCUPADA;
	    } else {
		candidatas[i][j] = SI; //CASILLA_LIBRE;
	    }
	}
    }
#endif
    // arriba hay versión alternativa
    tablero[3][3] = FICHA_BLANCA;
    tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = FICHA_NEGRA;
    tablero[4][3] = FICHA_NEGRA;

    candidatas[3][3] = CASILLA_OCUPADA;
    candidatas[4][4] = CASILLA_OCUPADA;
    candidatas[3][4] = CASILLA_OCUPADA;
    candidatas[4][3] = CASILLA_OCUPADA;

    // casillas a explorar:
    candidatas[2][2] = SI;
    candidatas[2][3] = SI;
    candidatas[2][4] = SI;
    candidatas[2][5] = SI;
    candidatas[3][2] = SI;
    candidatas[3][5] = SI;
    candidatas[4][2] = SI;
    candidatas[4][5] = SI;
    candidatas[5][2] = SI;
    candidatas[5][3] = SI;
    candidatas[5][4] = SI;
    candidatas[5][5] = SI;

    dibujar_pantalla_juego();
}

void actualizar_ficha(char tablero[][DIM_TABLERO], char f, char c, char color)
{
    tablero[f][c] = color;
    dibujar_ficha_virtual(f, c, color);
}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
char ficha_valida(char tablero[][DIM_TABLERO], char f, char c, int *posicion_valida)
{
    char ficha;

    if ((f < DIM_TABLERO) && (f >= 0) && (c < DIM_TABLERO) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
    {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }
    else
    {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
int patron_volteo(char tablero[][DIM_TABLERO], int *longitud, char FA, char CA, char SF, char SC, char color)
{
    unsigned int tiempo_referencia_us = timer2_leer();

    int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
    int patron;          //indica si se ha encontrado un patrón o no
    char casilla;        // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
    // mientras la casilla está en el tablero, no está vacía,
    // y es del color rival seguimos buscando el patron de volteo
    if ((posicion_valida == 1) && (casilla != color))
    {
        *longitud = *longitud + 1;
        patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
        //printf("longitud: %d \n", *longitud);
        //printf("fila: %d; columna: %d \n", FA, CA);
    }
    // si la ultima posición era válida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrón
    else if ((posicion_valida == 1) && (casilla == color))
    {
        if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
        {
            patron = PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
            //printf("PATRON_ENCONTRADO \n");
        }
        else
        {
            patron = NO_HAY_PATRON; // si no hay que voltear no hay patrón
            //printf("NO_HAY_PATRON \n");
        }
    }
    // en caso contrario es que no hay patrón
    else
    {
        patron = NO_HAY_PATRON;
        //printf("NO_HAY_PATRON \n");
    }

    actualizar_tiempo_invocaciones_patron_volteo(timer2_leer() - tiempo_referencia_us);

    return patron;
}

// extern int patron_volteo_arm_c(char tablero[][DIM_TABLERO], int *longitud, char FA, char CA, char SF, char SC, char color);

// extern int patron_volteo_arm_arm(char tablero[][DIM_TABLERO], int *longitud, char FA, char CA, char SF, char SC, char color);

// static unsigned int patron_volteo_tiempo;
// static unsigned int patron_volteo_arm_c_tiempo;
// static unsigned int patron_volteo_arm_arm_tiempo;

// int patron_volteo_test(char tablero[][DIM_TABLERO], int *longitud, char FA, char CA, char SF, char SC, char color)
// {
//     int longitud_arm_c = *longitud;
//     int longitud_arm_arm = *longitud;

//     timer2_empezar();
//     int patron_correcto = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
//     patron_volteo_tiempo = timer2_parar();

//     timer2_empezar();
//     int patron_arm_c = patron_volteo_arm_c(tablero, &longitud_arm_c, FA, CA, SF, SC, color);
//     patron_volteo_arm_c_tiempo = timer2_parar();
//     if (patron_correcto != patron_arm_c)
//     {
//         // indicar error de alguna manera
//         while (1)
//         {
//             // patron_volteo_arm_c no funciona correctamente
//         }
//     }

//     timer2_empezar();
//     int patron_arm_arm = patron_volteo_arm_arm(tablero, &longitud_arm_arm, FA, CA, SF, SC, color);
//     patron_volteo_arm_arm_tiempo = timer2_parar();
//     if (patron_correcto != patron_arm_arm)
//     {
//         // indicar error de alguna manera
//         while (1)
//         {
//             // patron_volteo_arm_arm no funciona correctamente
//         }
//     }

//     unsigned int total = patron_volteo_tiempo + patron_volteo_arm_c_tiempo + patron_volteo_arm_arm_tiempo;

//     return patron_correcto;
// }

////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM_TABLERO], char FA, char CA, char SF, char SC, int n, char color)
{
    int i;

    for (i = 0; i < n; i++)
    {
        FA = FA + SF;
        CA = CA + SC;
        actualizar_ficha(tablero, FA, CA, color);
    }
}

////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// char vSF[DIM_TABLERO] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM_TABLERO] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(char tablero[][DIM_TABLERO], char f, char c, char color)
{
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;

    for (i = 0; i < DIM_TABLERO; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO)
        {
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posición si se puede mover
// En caso afirmativo, consulta la puntuación de la posición y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(char candidatas[][DIM_TABLERO], char tablero[][DIM_TABLERO], char *f, char *c)
{
    int i, j, k, found;
    int mf = -1;    // almacena la fila del mejor movimiento encontrado
    int mc;         // almacena la columna del mejor movimiento encontrado
    char mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque

    // Recorremos todo el tablero comprobando dónde podemos mover
    // Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
    for (i = 0; i < DIM_TABLERO; i++)
    {
        for (j = 0; j < DIM_TABLERO; j++)
        { // indica en qué casillas quizá se pueda mover
            if (candidatas[i][j] == SI)
            {
                if (tablero[i][j] == CASILLA_VACIA)
                {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento válido
                    // (es decir si implica voltear en alguna dirección)
                    while ((found == 0) && (k < DIM_TABLERO))
                    {
                        SF = vSF[k]; // k representa la dirección que miramos
                        SC = vSC[k]; // 1 es norte, 2 NE, 3 E ...

                        // nos dice qué hay que voltear en cada dirección
                        longitud = 0;
                        patron = patron_volteo(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO)
                        {
                            found = 1;
                            if (tabla_valor[i][j] > mejor)
                            {
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente dirección
                    }
                }
            }
        }
    }
    *f = (char)mf;
    *c = (char)mc;
    // si no se ha encontrado una posición válida devuelve -1
    return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el número de fichas de cada color.
// Los guarda en la dirección b (blancas) y n (negras)
void contar(char tablero[][DIM_TABLERO], int *b, int *n)
{
    int i, j;

    *b = 0;
    *n = 0;

    // recorremos todo el tablero contando las fichas de cada color
    for (i = 0; i < DIM_TABLERO; i++)
    {
        for (j = 0; j < DIM_TABLERO; j++)
        {
            if (tablero[i][j] == FICHA_BLANCA)
            {
                (*b)++;
            }
            else if (tablero[i][j] == FICHA_NEGRA)
            {
                (*n)++;
            }
        }
    }
}

void actualizar_candidatas(char candidatas[][DIM_TABLERO], char f, char c)
{
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor sí
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0)
    {
        if (candidatas[f - 1][c] != CASILLA_OCUPADA)
            candidatas[f - 1][c] = SI;

        if ((c > 0) && (candidatas[f - 1][c - 1] != CASILLA_OCUPADA))
            candidatas[f - 1][c - 1] = SI;

        if ((c < 7) && (candidatas[f - 1][c + 1] != CASILLA_OCUPADA))
            candidatas[f - 1][c + 1] = SI;
    }
    if (f < 7)
    {
        if (candidatas[f + 1][c] != CASILLA_OCUPADA)
            candidatas[f + 1][c] = SI;

        if ((c > 0) && (candidatas[f + 1][c - 1] != CASILLA_OCUPADA))
            candidatas[f + 1][c - 1] = SI;

        if ((c < 7) && (candidatas[f + 1][c + 1] != CASILLA_OCUPADA))
            candidatas[f + 1][c + 1] = SI;
    }
    if ((c > 0) && (candidatas[f][c - 1] != CASILLA_OCUPADA))
        candidatas[f][c - 1] = SI;

    if ((c < 7) && (candidatas[f][c + 1] != CASILLA_OCUPADA))
        candidatas[f][c + 1] = SI;
}

char movimiento_valido(char candidatas[][DIM_TABLERO], char fila, char columna)
{
    if (fila == DIM_TABLERO || columna == DIM_TABLERO)
    {
        return 0;
    }
    else
    {
        return candidatas[fila][columna] == SI;
    }
}

char reversi_procesar(char tablero[][DIM_TABLERO], char fila, char columna, char candidatas[][DIM_TABLERO])
{
    int move = 0;
    int fin = 0;
    int done;
    char f, c;

    if (movimiento_valido(candidatas, fila, columna))
    {
        actualizar_ficha(tablero, fila, columna, FICHA_NEGRA);
        actualizar_tablero(tablero, fila, columna, FICHA_NEGRA);
        actualizar_candidatas(candidatas, fila, columna);
        move = 1;
    }

    done = elegir_mov(candidatas, tablero, &f, &c);
    if (done == -1)
    {
        if (move == 0)
            fin = 1;
    }
    else
    {
        actualizar_ficha(tablero, f, c, FICHA_BLANCA);
        actualizar_tablero(tablero, f, c, FICHA_BLANCA);
        actualizar_candidatas(candidatas, f, c);
    }

    return fin;
}

static volatile char boton_pulsado = 0;

void se_pulso_boton(enum estado_button boton_presionado)
{
    button_deshabilitar();
    boton_pulsado = 1;
}

void mostrar_pantalla_inicial()
{
    button_empezar(&se_pulso_boton);

    dibujar_pantalla_inicial();

    char pantalla_pulsada;

    do
    {
        pantalla_pulsada = comprobar_pantalla_tactil_presionada();
    } while (!boton_pulsado && !pantalla_pulsada);
}

void mostrar_pantalla_final(char tablero[][DIM_TABLERO])
{
    int negras, blancas;

    contar(tablero, &blancas, &negras);

    dibujar_pantalla_final(blancas, negras);

    char pantalla_pulsada;

    do
    {
        pantalla_pulsada = comprobar_pantalla_tactil_presionada();
    } while (!pantalla_pulsada);
}

void reversi_main()
{
    char __attribute__((aligned(8))) candidatas[DIM_TABLERO][DIM_TABLERO] =
        {
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO}};

    mostrar_pantalla_inicial();

    unsigned int tiempo_ahora_us;
    char fila = 0;
    char columna = 0;

    init_table(tablero, candidatas);

    delay(UN_SEGUNDO_US * 2);

    while (1)
    {
        tiempo_ahora_us = timer0_leer();
        gestion_latido(tiempo_ahora_us);
        actualizar_tiempo_partida(tiempo_ahora_us);
        refrescar_pantalla_juego(tiempo_ahora_us);

        if (jugada_lista_pantalla(tablero, tiempo_ahora_us, &fila, &columna))
        {
            unsigned int tiempo_referencia_calculos_us = timer2_leer();
            char partida_terminada = reversi_procesar(tablero, fila, columna, candidatas);
            actualizar_tiempo_calculos(timer2_leer() - tiempo_referencia_calculos_us);

            if (partida_terminada)
            {
                mostrar_pantalla_final(tablero);
                init_table(tablero, candidatas);
            }
            fila = 0;
            columna = 0;
        }
    }
}

char son_tableros_iguales(char tablero[][DIM_TABLERO], char tablero_a_probar[][DIM_TABLERO])
{
    int i, j;

    for (i = 0; i < DIM_TABLERO; i++)
    {
        for (j = 0; j < DIM_TABLERO; j++)
        {
            if (tablero[i][j] != tablero_a_probar[i][j])
            {
                return 0;
            }
        }
    }

    return 1;
}

void reversi8_test()
{

    ////////////////////////////////////////////////////////////////////
    // Tablero candidatas: se usa para no explorar todas las posiciones del tablero
    // sólo se exploran las que están alrededor de las fichas colocadas
    ////////////////////////////////////////////////////////////////////
    char __attribute__((aligned(8))) candidatas[DIM_TABLERO][DIM_TABLERO] =
        {
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO},
            {NO, NO, NO, NO, NO, NO, NO, NO}};

    char __attribute__((aligned(8))) tablero_negras[MOVIMIENTOS_TEST][DIM_TABLERO][DIM_TABLERO] = {
        {{CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_NEGRA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}},
        {{CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}},
        {{CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}},
    };

    char __attribute__((aligned(8))) tablero_blancas[MOVIMIENTOS_TEST][DIM_TABLERO][DIM_TABLERO] = {
        {{CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}},
        {{CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, CASILLA_VACIA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}},
        {{CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_NEGRA, FICHA_NEGRA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, FICHA_BLANCA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, FICHA_BLANCA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
         {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}},
    };

    char filas_negras[MOVIMIENTOS_TEST] = {3, 1, 2};
    char columnas_negras[MOVIMIENTOS_TEST] = {2, 2, 3};

    char filas_blancas[MOVIMIENTOS_TEST] = {2, 2, 5};
    char columnas_blancas[MOVIMIENTOS_TEST] = {2, 4, 2};
    int i = 0;

    init_table(tablero, candidatas);
    for (i = 0; i < MOVIMIENTOS_TEST; i++)
    {
        actualizar_ficha(tablero, filas_negras[i], columnas_negras[i], FICHA_NEGRA);
        actualizar_tablero(tablero, filas_negras[i], columnas_negras[i], FICHA_NEGRA);
        actualizar_candidatas(candidatas, filas_negras[i], columnas_negras[i]);

        if (!son_tableros_iguales(tablero_negras[i], tablero))
        {
            while (1)
            {
                // problema
            }
        }

        actualizar_ficha(tablero, filas_blancas[i], columnas_blancas[i], FICHA_BLANCA);
        actualizar_tablero(tablero, filas_blancas[i], columnas_blancas[i], FICHA_BLANCA);
        actualizar_candidatas(candidatas, filas_blancas[i], columnas_blancas[i]);

        if (!son_tableros_iguales(tablero_blancas[i], tablero))
        {
            while (1)
            {
                // problema
            }
        }
    }
}
