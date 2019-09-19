/*********************************************************************************************
* Fichero:	pantalla_tactil.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementación del módulo que se encarga de gestionar la pantalla táctil
*********************************************************************************************/

#include "pantalla_tactil.h"
#include "lcd.h"
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "reversi_def.h"

static unsigned int x_max;
static unsigned int y_max;
static unsigned int x_min;
static unsigned int y_min;

static volatile unsigned int x_leida, y_leida;
static float x_ratio_tactil_lcd, y_ratio_tactil_lcd;
static volatile char se_ha_presionado = 0;

static char esta_calibrada = 0;

enum
{
    N_LECTURAS = 8,
    N_CALIBRACIONES = 4,
};

void pantalla_tactil_ISR(void) __attribute__((interrupt("IRQ")));

void pantalla_tactil_ISR(void)
{
    int i;
    ULONG x_tmp = 0;
    ULONG y_tmp = 0;
    ULONG Pt[N_LECTURAS];

    // <X-Position Read>
    // TSPX(GPE4_Q4(+)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(+)) TSMX(GPE7_Q1(-))
    //       0               1                 1                0
    // TODO: cambiar?
    rPDATE = 0x68;
    rADCCON = 0x1 << 2; // AIN1

    // TODO: cambiar?
    Delay(100); // delay to set up the next channel
    for (i = 0; i < N_LECTURAS; i++)
    {
        rADCCON |= 0x1; // Start X-position A/D conversion
        while (rADCCON & 0x1)
            ; // Check if Enable_start is low
        while (!(rADCCON & 0x40))
            ; // Check ECFLG
        Pt[i] = (0x3ff & rADCDAT);
    }

    // Ordenamos las lecturas y obtenemos la media de todos los valores menos el
    // maximo y el minimo
    ordenar(Pt, N_LECTURAS);
    for (i = 1; i < N_LECTURAS - 1; ++i)
    {
        x_tmp += Pt[i];
    }
    x_tmp = x_tmp / (N_LECTURAS - 2);

    // <Y-Position Read>
    // TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(+)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+))
    //       1               0                 0                1
    rPDATE = 0x98;
    rADCCON = 0x0 << 2; // AIN0

    // TODO: cambiar?
    Delay(100); // delay to set up the next channel
    for (i = 0; i < N_LECTURAS; i++)
    {
        rADCCON |= 0x1; // Start Y-position conversion
        while (rADCCON & 0x1)
            ; // Check if Enable_start is low
        while (!(rADCCON & 0x40))
            ; // Check ECFLG
        Pt[i] = (0x3ff & rADCDAT);
    }

    // Ordenamos las lecturas y obtenemos la media de todos los valores menos el
    // maximo y el minimo
    ordenar(Pt, N_LECTURAS);
    for (i = 1; i < N_LECTURAS - 1; ++i)
    {
        y_tmp += Pt[i];
    }
    y_tmp = y_tmp / (N_LECTURAS - 2);

    if (esta_calibrada)
    {
        if (x_tmp < x_min)
        {
            x_tmp = x_min;
        }
        else if (x_tmp > x_max)
        {
            x_tmp = x_max;
        }

        if (y_tmp < y_min)
        {
            y_tmp = y_min;
        }
        else if (y_tmp > y_max)
        {
            y_tmp = y_max;
        }
    }

    se_ha_presionado = 1;

    x_leida = x_tmp;
    y_leida = y_tmp;

    rPDATE = 0xb8; // should be enabled
                   // TODO: cambiar?
    Delay(3000);   // delay to set up the next channel

    rI_ISPC |= BIT_EINT2; // clear pending_bit
}

//activa la pantalla tactil
void pantalla_tactil_init(void)
{
    se_ha_presionado = 0;
    esta_calibrada = 0;

    /* enable interrupt */
    rINTMOD &= ~(0x800000);
    rI_ISPC |= BIT_EINT2; // clear pending_bit

    // TODO: cambiar?
    rPUPE = 0x0; // Pull up
    // PDATE se deberia sobreescribir (?)
    // TODO: cambiar?
    rPDATE = 0xb8; // should be enabled

    rEXTINT |= 0x200;                             // falling edge trigger
    pISR_EINT2 = (unsigned *)pantalla_tactil_ISR; // set interrupt handler

    // TODO: cambiar?
    rCLKCON = 0x7ff8; // enable clock
                      // TODO: cambiar?
    rADCPSR = 0x1;    //0x4;             // A/D prescaler
    rINTMSK &= ~(BIT_EINT2);
}

// comprobar si la pantalla se ha presionado
char comprobar_pantalla_tactil_presionada(void)
{
    if (se_ha_presionado)
    {
        se_ha_presionado = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

// devuelve las coordeanadas x e y de la pantalla
char coordenadas_pantalla_tactil_presionada(unsigned int *x, unsigned int *y)
{
    if (comprobar_pantalla_tactil_presionada())
    {
        *x = (x_leida - x_min) * x_ratio_tactil_lcd;
        *y = (y_leida - y_min) * y_ratio_tactil_lcd;

        return 1;
    }
    else
    {
        return 0;
    }
}

void realizar_calibracion(void)
{
    if (x_leida > x_max)
    {
        x_max = x_leida;
    }
    else if (x_leida < x_min)
    {
        x_min = x_leida;
    }

    if (y_leida < y_min)
    {
        y_min = y_leida;
    }
    else if (y_leida > y_max)
    {
        y_max = y_leida;
    }
}

void pantalla_tactil_calibrar(void)
{
    se_ha_presionado = 0;
    dibujar_pantalla_calibrar_tactil();
    char num_calibraciones = 0;

    dibujar_pantalla_calibrar_tactil_texto("PRESIONAR ESQUINA SUPERIOR IZQUIERDA");
    while (num_calibraciones < N_CALIBRACIONES)
    {
        while (!se_ha_presionado)
            ;
        num_calibraciones++;

        switch (num_calibraciones)
        {
        case 1:
        {
            dibujar_pantalla_calibrar_tactil_texto("PRESIONAR ESQUINA INFERIOR IZQUIERDA");
            x_max = x_leida;
            x_min = x_leida;
            y_min = y_leida;
            y_max = y_leida;
            delay(10000);
            break;
        }
        case 2:
        {
            dibujar_pantalla_calibrar_tactil_texto("PRESIONAR ESQUINA INFERIOR DERECHA");
            realizar_calibracion();
            delay(10000);
            break;
        }
        case 3:
        {
            dibujar_pantalla_calibrar_tactil_texto("PRESIONAR ESQUINA SUPERIOR DERECHA");
            realizar_calibracion();
            delay(10000);
            break;
        }
        case 4:
        {
            realizar_calibracion();
            break;
        }
        }

        se_ha_presionado = 0;
    }

    x_ratio_tactil_lcd = (x_max - x_min) / (float)LCD_XSIZE;
    y_ratio_tactil_lcd = (y_max - y_min) / (float)LCD_YSIZE;

    esta_calibrada = 1;
    delay(500000);
}
