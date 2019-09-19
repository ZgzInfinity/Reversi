/*********************************************************************************************
* Fichero:	Bmp.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Funciones de control y visualizacion del LCD
* Version:
*********************************************************************************************/

/*--- Archivos cabecera ---*/
#include "bmp.h"
#include "def.h"
#include "lcd.h"

/*--- variables globales ---*/
/* mapa de bits del cursor del raton */
const INT8U ucMouseMap[] = {
BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY,
BLACK,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        WHITE,        BLACK,        BLACK,        BLACK,        BLACK,        BLACK,
BLACK,        WHITE,        WHITE,        BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        WHITE,        BLACK,        TRANSPARENCY, BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        BLACK,        TRANSPARENCY, TRANSPARENCY, BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY,
BLACK,        TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY,
TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY, TRANSPARENCY,
TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY,
TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, BLACK,        WHITE,        WHITE,        WHITE,        BLACK,        TRANSPARENCY,
TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, BLACK,        BLACK,        BLACK,        TRANSPARENCY, TRANSPARENCY
};

STRU_BITMAP Stru_Bitmap_gbMouse = {0x10, 4, 12, 20, TRANSPARENCY, (INT8U *)ucMouseMap};

INT16U ulMouseX;
INT16U ulMouseY;
INT8U ucCursorBackUp[20][12/2];

/*--- codigo de funcion ---*/
/*********************************************************************************************
* name:		BitmapView()
* func:		display bitmap
* para:		x,y -- pot's X-Y coordinate 
*			Stru_Bitmap -- bitmap struct
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void BitmapView (INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap)
{
	INT32U i, j;
	INT8U ucColor;
	
	for (i =  0; i < Stru_Bitmap.usHeight; i++)
	{
		for (j = 0; j <Stru_Bitmap.usWidth; j++)
		{
			if ((ucColor = *(INT8U*)(Stru_Bitmap.pucStart + i * Stru_Bitmap.usWidth + j)) != TRANSPARENCY)
			{
				LCD_PutPixel(x + j, y + i, ucColor); 
			}
		}
	}
}

/*********************************************************************************************
* name:		BitmapPush()
* func:		push bitmap data into LCD active buffer
* para:		x,y -- pot's X-Y coordinate 
*			Stru_Bitmap -- bitmap struct
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void BitmapPush (INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap)
{
	INT32U i, j;

	ulMouseX = x; 
	ulMouseY = y;
	for (i =  0; i < Stru_Bitmap.usHeight; i++)
	{
		for (j = 0; j < Stru_Bitmap.usWidth; j+=2)
		{
			if ((x + j)%2)
			{
				ucCursorBackUp[i][j/2] =
				(((*(INT8U*)(LCD_ACTIVE_BUFFER + (y + i) * SCR_XSIZE / 2 + (x + j) / 8 * 4  + 3 - ((x + j)%8) / 2)) << 4) & 0xf0) + 
				(((*(INT8U*)(LCD_ACTIVE_BUFFER + (y + i) * SCR_XSIZE / 2 + (x + j+1) / 8 * 4  + 3 - ((x + j+1)%8) / 2)) >> 4) & 0x0f);
			}
			else
			{
				ucCursorBackUp[i][j/2] = (*(INT8U*)(LCD_ACTIVE_BUFFER + (y + i) * SCR_XSIZE / 2 + (x + j) / 8 * 4  + 3 - ((x + j)%8) / 2));
			}
		}
	}
}

/*********************************************************************************************
* name:		BitmapPop()
* func:		pop bitmap data into LCD active buffer
* para:		x,y -- pot's X-Y coordinate 
*			Stru_Bitmap -- bitmap struct
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void BitmapPop(INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap)
{
	INT32U i, j;
	INT32U ulAddr, ulAddr1;

	for (i =  0; i < Stru_Bitmap.usHeight; i++)
	{
		for (j = 0; j <Stru_Bitmap.usWidth; j+=2)
		{
			ulAddr = LCD_ACTIVE_BUFFER + (y + i) * SCR_XSIZE / 2 + (x + j) / 8 * 4 + 3 - ((x + j)%8) / 2;
			ulAddr1 =LCD_ACTIVE_BUFFER + (y + i) * SCR_XSIZE / 2 + (x + j + 1) / 8 * 4 + 3 - ((x + j + 1)%8) / 2;
			if ((x + j)%2)
			{
				
				(*(INT8U*)ulAddr) &= 0xf0;
				(*(INT8U*)ulAddr) |= ((ucCursorBackUp[i][j/2] >> 4) & 0x0f);
				(*(INT8U*)ulAddr1) &= 0x0f;
				(*(INT8U*)ulAddr1) |= ((ucCursorBackUp[i][j/2] << 4) & 0xf0);
			}
			else
			{
				(*(INT8U*)ulAddr) = ucCursorBackUp[i][j/2];
			}
			
		}
	}
}

/*********************************************************************************************
* name:		CursorInit()
* func:		cursor init
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void CursorInit(void)
{
	ulMouseX = 0;
	ulMouseY = 0;
	CursorView(ulMouseX, ulMouseY);
}

/*********************************************************************************************
* name:		CursorPush()
* func:		cursor push
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void CursorPush(INT16U x, INT16U y)
{
	BitmapPush(x, y, Stru_Bitmap_gbMouse);
}

/*********************************************************************************************
* name:		CursorPop()
* func:		cursor pop
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void CursorPop()
{
	BitmapPop(ulMouseX, ulMouseY, Stru_Bitmap_gbMouse);
}

/*********************************************************************************************
* name:		CursorView()
* func:		cursor display
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void CursorView(INT16U x, INT16U y)
{
	CursorPush(x, y);
	BitmapView(x, y, Stru_Bitmap_gbMouse);
}

