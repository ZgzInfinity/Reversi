/*********************************************************************************************
* Fichero:	button.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- declaracion de funciones visibles del módulo button.c/button.h ---*/

/*--------------------------------------------------------------*/
/*------------ CONSTANTES PUBLICAS DEL MODULO ------------------*/
/*--------------------------------------------------------------*/

// Constantes para identificar los tipos de boton a manejar
enum estado_button
{
    BUTTON_NONE,
    BUTTON_IZ,
    BUTTON_DR,
    BUTTON_IZ_DR,
};

/*--------------------------------------------------------------*/
/*------------------ FUNCIONES DEL MODULO ----------------------*/
/*--------------------------------------------------------------*/

typedef void(funcion_callback_button)(enum estado_button);

// Inicializar el dispositivo dejandolo listo para ser usado
void button_init(void);

// re-activa interrupciones y reprograma la función de callback a llamar desde la RSI.
void button_empezar(funcion_callback_button *funcion_callback);

// deshabilitar las interrupciones del boton
void button_deshabilitar(void);

// lee el estado del botón
enum estado_button button_estado(void);

#endif /* _BUTTON_H_ */
