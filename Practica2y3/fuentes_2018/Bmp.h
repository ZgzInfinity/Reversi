/*********************************************************************************************
* Fichero:	Bmp.h
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	definicion mapas de bits del LCD
* Version:
*********************************************************************************************/

#ifndef __BMP_H_
#define __BMP_H_

#include "def.h"

/* estructura mapa de bits */
typedef struct BITMAP
{
    INT8U 		ucFlags;          	// combination of flags above
    INT8U 		ucBitsPix;        	// 1, 2, 4, or 8
    INT16U 		usWidth;        	// in pixels
    INT16U  	usHeight;        	// in pixels
    INT32U 		ulTransColor;     	// transparent color for > 8bpp bitmaps
    INT8U 		*pucStart;  		// bitmap data pointer
}STRU_BITMAP, *pSTRU_BITMAP;

/*--- definicion macros ---*/

#define BLACK 		0xf
#define WHITE 		0x0
#define LIGHTGRAY   0x5
#define DARKGRAY    0xa
#define TRANSPARENCY 0xff

void CursorInit(void);
void CursorPush(INT16U x, INT16U y);
void CursorPop(void);

void BitmapPop(INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap);
void BitmapView(INT16U x, INT16U y, STRU_BITMAP Stru_Bitmap);
void CursorView(INT16U x, INT16U y);

#endif /* __BMP_H_ */
