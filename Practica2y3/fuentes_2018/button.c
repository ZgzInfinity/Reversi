/*********************************************************************************************
* Fichero:	button.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include <stddef.h>
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PRIVADAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

// Constantes para denotar los botones de la placa
enum bit_boton_pdatg
{
	BIT_BOTON_IZ = 6,
	BIT_BOTON_DR = 7,
};

/*--- variables globales del módulo ---*/

/*--------------------------------------------------------------*/
/*------------ VARIABLES PRIVADAS DEL MODULO -------------------*/
/*--------------------------------------------------------------*/

static funcion_callback_button *button_callback = NULL;

#ifdef SIM_IN_USE
static volatile enum estado_button boton_actual = BUTTON_NONE;
#endif

/*--------------------------------------------------------------*/
/*-------------------- FUNCIONES DEL MODULO --------------------*/
/*--------------------------------------------------------------*/

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void button_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void button_ISR(void)
{
	if (button_callback != NULL)
	{
		button_callback(button_estado());
	}

	/* Finalizar ISR */
	rEXTINTPND = 0xf;		 // borra los bits en EXTINTPND
	rI_ISPC |= BIT_EINT4567; // borra el bit pendiente en INTPND
}

/* Inicializar el dispositivo dejandolo listo para ser usado */
void button_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	// Se quitan PND y EXTINTPND ya que se eliminan las peticiones pendientes en la llamada a sys_init()

	rEXTINTPND = 0xf;		// Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD &= ~(0x200000); // Configura las linas como de tipo IRQ
	//rINTMSK &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int)button_ISR;

	/* Configuracion del puerto G */
	rPCONG = 0xffff;	   // Establece la funcion de los pines (EINT0-7)
	rPUPG = 0x0;		   // Habilita el "pull up" del puerto
	rEXTINT |= 0x22222222; // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se borran los bits de INTPND y EXTINTPND */
	rI_ISPC |= BIT_EINT4567;
	rEXTINTPND = 0xf;
}

/*  re-activa interrupciones y reprograma la función de callback a llamar desde la RSI.  */
void button_empezar(funcion_callback_button *funcion_callback)
{
	button_callback = funcion_callback;
	rEXTINTPND = 0xf;
	rINTMSK &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras
}

/* deshabilita las interrupciones del boton */
void button_deshabilitar(void)
{
	rINTMSK |= BIT_EINT4567;
}

/* lee el estado actual del botón */
enum estado_button button_estado(void)
{
#ifdef SIM_IN_USE
	// Devolución del estado actual del botón
	return boton_actual;
#else
	// Determinación de que botón se ha pulsado
	char boton_derecho_pulsado = ~rPDATG & (1 << BIT_BOTON_DR);
	char boton_izquierdo_pulsado = ~rPDATG & (1 << BIT_BOTON_IZ);

	// se pulsan los dos
	if (boton_derecho_pulsado && boton_izquierdo_pulsado)
	{
		return BUTTON_IZ_DR;
	}
	// se pulsa solo el izquierdo
	else if (boton_derecho_pulsado)
	{
		return BUTTON_DR;
	}
	// se pulsa solo el derecho
	else if (boton_izquierdo_pulsado)
	{
		return BUTTON_IZ;
	}
	// no se ha presionado el boton
	return BUTTON_NONE;
#endif
}
