/*********************************************************************************************
* Fichero:	pantalla.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de implementación del módulo que se encarga de gestionar el dibujado de la pantalla
*********************************************************************************************/

#include "pantalla.h"
#include "lcd.h"
#include "Bmp.h"
#include "reversi_def.h"

// Constantes para la pantalla tactil
enum coordenadas_pantalla
{
	XY_INICIAL_TABLERO = 16,
	XY_FINAL_TABLERO = 200,
	XY_OFFSET_CUADRADO = 23,

	X_INICIAL_N_COORD = 24,
	Y_INICIAL_N_COORD = 21,
	X_INICIAL_N_OFFSET = 12,
	Y_INICIAL_N_OFFSET = 15,

	X_FICHA_OFFSET = 5,
	Y_FICHA_OFFSET = 5,

	X_TIEMPO_TOTAL = 207,
	Y_TIEMPO_TOTAL = 25,
	X_TIEMPO_CALCULOS = 207,
	Y_TIEMPO_CALCULOS = 75,
	X_TIEMPO_PATRON = 207,
	Y_TIEMPO_PATRON = 125,
	X_INVOC_PATRON = 207,
	Y_INVOC_PATRON = 175,

	X_TIEMPO_TOTAL_OFFSET = 0,
	Y_TIEMPO_TOTAL_OFFSET = 10,
	X_TIEMPO_CALCULOS_OFFSET = 0,
	Y_TIEMPO_CALCULOS_OFFSET = 10,
	X_TIEMPO_PATRON_OFFSET = 0,
	Y_TIEMPO_PATRON_OFFSET = 10,
	X_INVOC_PATRON_OFFSET = 0,
	Y_INVOC_PATRON_OFFSET = 10,

	X_NEGRAS_TEXTO = 80,
	Y_NEGRAS_TEXTO = 100,
	X_BLANCAS_TEXTO = 180,
	Y_BLANCAS_TEXTO = 100,

	X_NEGRAS = 100,
	Y_NEGRAS = 120,
	X_BLANCAS = 200,
	Y_BLANCAS = 120,

	X_TITULO_REVERSI = 135,
	Y_TITULO_REVERSI = 20,

	X_TOQUE_PANTALLA = 50,
	Y_TOQUE_PANTALLA = 220,

	X_LEYENDA_PARTIDA = 10,
	Y_LEYENDA_PARTIDA = 220,

	X_CALIBRAR_BOTONES = 0,
	Y_CALIBRAR_BOTONES = 120,

	X_INSTR_CALIBRAR_BOTONES = 0,
	Y_INSTR_CALIBRAR_BOTONES = 140,

	X_CALIBRAR_TACTIL = 70,
	Y_CALIBRAR_TACTIL = 120,

	X_INSTR_CALIBRAR_TACTIL = 0,
	Y_INSTR_CALIBRAR_TACTIL = 140,
};

// devuelve la coordenada en x y en y
char coord_en_centro_tablero(unsigned int x, unsigned int y)
{
	return x >= 0 && y >= 0 &&
		   x <= LCD_XSIZE && y <= LCD_YSIZE;
}

// dibujado de la pantalla informativa de la pantalla tactil
void dibujar_pantalla_calibrar_tactil(void)
{
	Lcd_Clr();
	Lcd_DspAscII8x16(X_CALIBRAR_TACTIL, Y_CALIBRAR_TACTIL, BLACK, "CALIBRAR PANTALLA TACTIL");
	Lcd_Active_Clr();
	Lcd_Dma_Trans();
}

// muestra por pantalla el texto que indica al usuario donde debe pulsar para calibrar
void dibujar_pantalla_calibrar_tactil_texto(char *texto)
{
	limpiar_y_dibujar_texto_virtual(X_INSTR_CALIBRAR_TACTIL, Y_INSTR_CALIBRAR_TACTIL, LCD_XSIZE, LCD_YSIZE, texto);

	Lcd_Dma_Trans();
}

// dibujado de la pantalla de calibración de los botones
void dibujar_pantalla_calibrar_botones(void)
{
	Lcd_Clr();
	Lcd_DspAscII8x16(X_CALIBRAR_BOTONES, Y_CALIBRAR_BOTONES, BLACK, "CALIBRAR TIEMPOS DE BOTONES ANTIREBOTES");
	Lcd_Active_Clr();
	Lcd_Dma_Trans();
}

// muestra por pantalla el mensaje de pulsar o soltar boton
void dibujar_pantalla_calibrar_botones_texto(char *texto)
{
	limpiar_y_dibujar_texto_virtual(X_INSTR_CALIBRAR_BOTONES, Y_INSTR_CALIBRAR_BOTONES, LCD_XSIZE, LCD_YSIZE, texto);

	Lcd_Dma_Trans();
}

// Dibujado de la pantalla inicial
void dibujar_pantalla_inicial(void)
{
	Lcd_Clr();

	Lcd_DspAscII8x16(X_TITULO_REVERSI, Y_TITULO_REVERSI, BLACK, "REVERSI");
	//	"El objetivo es tener mas fichas que el adversario."
	//	"Comienzan las fichas negras (humano)."
	//	"Solo se puede mover ficha para rodear al adversario."
	Lcd_DspAscII8x16(X_TOQUE_PANTALLA, Y_TOQUE_PANTALLA, BLACK, "TOQUE LA PANTALLA PARA JUGAR");

	Lcd_Active_Clr();
	Lcd_Dma_Trans();
}

// dibujar tablero con fichas y tiempos de profiling
void dibujar_pantalla_juego(void)
{
	Lcd_Clr();

	dibujar_tablero_virtual();
	dibujar_ficha_virtual(3, 3, FICHA_BLANCA);
	dibujar_ficha_virtual(3, 4, FICHA_NEGRA);
	dibujar_ficha_virtual(4, 3, FICHA_NEGRA);
	dibujar_ficha_virtual(4, 4, FICHA_BLANCA);

	Lcd_DspAscII6x8(X_TIEMPO_TOTAL, Y_TIEMPO_TOTAL, BLACK, "TPO. TOTAL (s)");
	Lcd_DspAscII6x8(X_TIEMPO_CALCULOS, Y_TIEMPO_CALCULOS, BLACK, "TPO. CALCULOS (us)");
	Lcd_DspAscII6x8(X_TIEMPO_PATRON, Y_TIEMPO_PATRON, BLACK, "TPO. PATRON VOLTEO (us)");
	Lcd_DspAscII6x8(X_INVOC_PATRON, Y_INVOC_PATRON, BLACK, "# PATRON VOLTEO");

	dibujar_leyenda_virtual("PULSE PARA JUGAR");

	Lcd_Active_Clr();
	Lcd_Dma_Trans();
}

// dibujar panatlla final con los resultados
// recibe como parametros las fichas blancas y negras con las que ha acabado cada jugador
void dibujar_pantalla_final(int blancas, int negras)
{
	Lcd_Clr();

	char buffer[10];
	Lcd_DspAscII8x16(X_TITULO_REVERSI, Y_TITULO_REVERSI, BLACK, "RESULTADO");

	Lcd_DspAscII8x16(X_NEGRAS_TEXTO, Y_NEGRAS_TEXTO, BLACK, "Negras");
	Lcd_DspAscII8x16(X_BLANCAS_TEXTO, Y_BLANCAS_TEXTO, BLACK, "Blancas");

	itoa(negras, buffer);
	Lcd_DspAscII8x16(X_NEGRAS, Y_NEGRAS, BLACK, buffer);

	itoa(blancas, buffer);
	Lcd_DspAscII8x16(X_BLANCAS, Y_BLANCAS, BLACK, buffer);

	Lcd_DspAscII8x16(X_TOQUE_PANTALLA, Y_TOQUE_PANTALLA, BLACK, "TOQUE LA PANTALLA PARA JUGAR");

	Lcd_Active_Clr();
	Lcd_Dma_Trans();
}

// refresca la pantalla de juego con las actualzaciones a 60fps
void refrescar_pantalla_juego(unsigned int tiempo_ahora_us)
{
	static unsigned int tiempo_referencia_us = 0;

	if (diff_tiempo_pasado(tiempo_ahora_us, tiempo_referencia_us, 16666))
	{
		Lcd_Dma_Trans();
		tiempo_referencia_us = tiempo_ahora_us;
	}
}

// dibujo de la leyenda
void dibujar_leyenda_virtual(char *leyenda)
{
	limpiar_y_dibujar_texto_virtual(X_LEYENDA_PARTIDA, Y_LEYENDA_PARTIDA, LCD_XSIZE, LCD_YSIZE, leyenda);
}

// muestreo de los tiempos de profiling
void dibujar_tiempos_virtual(unsigned int tiempo_total_s, unsigned int tiempo_calculos_s, unsigned int tiempo_patron_s, unsigned int n_invocaciones_patron)
{
	char buffer[10];

	/* Limpiar y dibujar tiempo total */
	INT16 usLeft = X_TIEMPO_TOTAL + X_TIEMPO_TOTAL_OFFSET;
	INT16 usTop = Y_TIEMPO_TOTAL + Y_TIEMPO_TOTAL_OFFSET;
	INT16 usRight = LCD_XSIZE;
	INT16 usBottom = Y_TIEMPO_CALCULOS;
	itoa(tiempo_total_s, buffer);
	limpiar_y_dibujar_texto_virtual(usLeft, usTop, usRight, usBottom, buffer);

	/* Limpiar y dibujar tiempo_calculos */
	usLeft = X_TIEMPO_CALCULOS + X_TIEMPO_CALCULOS_OFFSET;
	usTop = Y_TIEMPO_CALCULOS + Y_TIEMPO_CALCULOS_OFFSET;
	usBottom = Y_TIEMPO_PATRON;
	itoa(tiempo_calculos_s, buffer);
	limpiar_y_dibujar_texto_virtual(usLeft, usTop, usRight, usBottom, buffer);

	/* Limpiar y dibujar tiempo_patron */
	usLeft = X_TIEMPO_PATRON + X_TIEMPO_PATRON_OFFSET;
	usTop = Y_TIEMPO_PATRON + Y_TIEMPO_PATRON_OFFSET;
	usBottom = Y_INVOC_PATRON;
	itoa(tiempo_patron_s, buffer);
	limpiar_y_dibujar_texto_virtual(usLeft, usTop, usRight, usBottom, buffer);

	/* Limpiar y dibujar n_invocaciones_patron */
	usLeft = X_INVOC_PATRON + X_INVOC_PATRON_OFFSET;
	usTop = Y_INVOC_PATRON + Y_INVOC_PATRON_OFFSET;
	usBottom = LCD_YSIZE;
	itoa(n_invocaciones_patron, buffer);
	limpiar_y_dibujar_texto_virtual(usLeft, usTop, usRight, usBottom, buffer);
}

// limpieza y dibujado del texto
void limpiar_y_dibujar_texto_virtual(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, char *texto)
{
	LcdClrRect(usLeft, usTop, usRight, usBottom, WHITE);
	Lcd_DspAscII8x16(usLeft, usTop, BLACK, texto);
}

// dibujar el tablero virtual
void dibujar_tablero_virtual(void)
{
	int i;

	char buffer[2];
	for (i = 0; i <= DIM_TABLERO; ++i)
	{
		if (i < DIM_TABLERO)
		{
			itoa(i + 1, buffer);

			// Numeros en horizontal
			Lcd_DspAscII8x16(X_INICIAL_N_COORD + XY_OFFSET_CUADRADO * i, XY_INICIAL_TABLERO - Y_INICIAL_N_OFFSET /* valor */, BLACK, buffer);

			// Numeros en vertical
			Lcd_DspAscII8x16(XY_INICIAL_TABLERO - X_INICIAL_N_OFFSET /* valor */, Y_INICIAL_N_COORD + XY_OFFSET_CUADRADO * i, BLACK, buffer);
		}

		Lcd_Draw_HLine(XY_INICIAL_TABLERO, XY_FINAL_TABLERO, XY_INICIAL_TABLERO + XY_OFFSET_CUADRADO * i, BLACK, 1 /* grosor q sea */);
		Lcd_Draw_VLine(XY_INICIAL_TABLERO, XY_FINAL_TABLERO, XY_INICIAL_TABLERO + XY_OFFSET_CUADRADO * i, BLACK, 1 /* grosor q sea */);
	}
}

/* colocar ficha en tablero con f y c en rango [0..7] */
void dibujar_ficha_virtual(int f, int c, char color)
{
	INT16 usLeft = XY_INICIAL_TABLERO + c * XY_OFFSET_CUADRADO + X_FICHA_OFFSET;
	INT16 usTop = XY_INICIAL_TABLERO + f * XY_OFFSET_CUADRADO + Y_FICHA_OFFSET;
	INT16 usRight = XY_INICIAL_TABLERO + (c + 1) * XY_OFFSET_CUADRADO - X_FICHA_OFFSET;
	INT16 usBottom = XY_INICIAL_TABLERO + (f + 1) * XY_OFFSET_CUADRADO - Y_FICHA_OFFSET;

	LcdClrRect(usLeft, usTop, usRight, usBottom, WHITE);
	switch (color)
	{
	case CASILLA_VACIA:
	{
		LcdClrRect(usLeft, usTop, usRight, usBottom, WHITE);
		break;
	}
	case FICHA_BLANCA:
	{
		Lcd_Draw_Box(usLeft, usTop, usRight, usBottom, BLACK);
		break;
	}
	case FICHA_NEGRA:
	{
		LcdClrRect(usLeft, usTop, usRight, usBottom, BLACK);
		break;
	}
	case FICHA_GRIS:
	{
		LcdClrRect(usLeft, usTop, usRight, usBottom, DARKGRAY);
		break;
	}
	default:
	{
		break;
	}
	}
}
