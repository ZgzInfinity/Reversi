/*********************************************************************************************
 * Fichero:	LCD.H
 * Autor:  Ruben Rodriguez 737215, Adrián Samatán 738455
 * Descrip:	definicion de madro y declaracion de funciones
 * Version:
 *********************************************************************************************/

#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "def.h"

/*--- define macros---*/

/* tamano pantalla */
#define TLCD_160_240 (0)
#define VLCD_240_160 (1)
#define CLCD_240_320 (2)
#define MLCD_320_240 (3)
#define ELCD_640_480 (4)
#define SLCD_160_160 (5)
#define LCD_TYPE MLCD_320_240

#if (LCD_TYPE == TLCD_160_240)
#define SCR_XSIZE (160)
#define SCR_YSIZE (240)
#define LCD_XSIZE (160)
#define LCD_YSIZE (240)
#elif (LCD_TYPE == VLCD_240_160)
#define SCR_XSIZE (240)
#define SCR_YSIZE (160)
#define LCD_XSIZE (240)
#define LCD_YSIZE (160)
#elif (LCD_TYPE == CLCD_240_320)
#define SCR_XSIZE (240)
#define SCR_YSIZE (320)
#define LCD_XSIZE (240)
#define LCD_YSIZE (320)
#elif (LCD_TYPE == MLCD_320_240)
#define SCR_XSIZE (320)
#define SCR_YSIZE (240)
#define LCD_XSIZE (320)
#define LCD_YSIZE (240)
#elif (LCD_TYPE == ELCD_640_480)
#define SCR_XSIZE (640)
#define SCR_YSIZE (480)
#define LCD_XSIZE (640)
#define LCD_YSIZE (480)
#elif (LCD_TYPE == SLCD_160_160)
#define SCR_XSIZE (160)
#define SCR_YSIZE (160)
#define LCD_XSIZE (160)
#define LCD_YSIZE (160)
#endif

/* screen color */
#define MODE_MONO (1)
#define MODE_GREY4 (4)
#define MODE_GREY16 (16)
#define MODE_COLOR (256)

#define Ascii_W 8
#define XWIDTH 6
#define BLACK 0xf
#define WHITE 0x0
#define LIGHTGRAY 0x5
#define DARKGRAY 0xa
#define TRANSPARENCY 0xff

#define HOZVAL (LCD_XSIZE / 4 - 1)
#define HOZVAL_COLOR (LCD_XSIZE * 3 / 8 - 1)
#define LINEVAL (LCD_YSIZE - 1)
#define MVAL (13)
#define M5D(n) ((n)&0x1fffff)
#define MVAL_USED 0

/* tamano array */
#define ARRAY_SIZE_MONO (SCR_XSIZE / 8 * SCR_YSIZE)
#define ARRAY_SIZE_GREY4 (SCR_XSIZE / 4 * SCR_YSIZE)
#define ARRAY_SIZE_GREY16 (SCR_XSIZE / 2 * SCR_YSIZE)
#define ARRAY_SIZE_COLOR (SCR_XSIZE / 1 * SCR_YSIZE)

/* clkval */
#define CLKVAL_MONO (12)
#define CLKVAL_GREY4 (12)
#define CLKVAL_GREY16 (12)
#define CLKVAL_COLOR (10)

#define LCD_BUF_SIZE (SCR_XSIZE * SCR_YSIZE / 2)
#define LCD_ACTIVE_BUFFER (0xc300000)
#define LCD_VIRTUAL_BUFFER (0xc300000 + LCD_BUF_SIZE)

#define LCD_PutPixel(x, y, c)                                                 \
  (*(INT32U *)(LCD_VIRTUAL_BUFFER + (y)*SCR_XSIZE / 2 + ((x)) / 8 * 4)) =     \
      (*(INT32U *)(LCD_VIRTUAL_BUFFER + (y)*SCR_XSIZE / 2 + ((x)) / 8 * 4)) & \
          (~(0xf0000000 >> ((((x)) % 8) * 4))) |                              \
      ((c) << (7 - ((x)) % 8) * 4)
#define LCD_Active_PutPixel(x, y, c)                                           \
  (*(INT32U *)(LCD_ACTIVE_BUFFER + (y)*SCR_XSIZE / 2 + (319 - (x)) / 8 * 4)) = \
      (*(INT32U *)(LCD_ACTIVE_BUFFER + (y)*SCR_XSIZE / 2 +                     \
                   (319 - (x)) / 8 * 4)) &                                     \
          (~(0xf0000000 >> (((319 - (x)) % 8) * 4))) |                         \
      ((c) << (7 - (319 - (x)) % 8) * 4)

#define GUISWAP(a, b) \
  {                   \
    a ^= b;           \
    b ^= a;           \
    a ^= b;           \
  }

  /*--- declaracion de funciones ---*/

  INT8U LCD_GetPixel(INT16U usX, INT16U usY);
  void Lcd_Init(void);
  void Lcd_Active_Clr(void);
  void Lcd_Clr(void);
  void Lcd_Test(void);
  void Lcd_Dma_Trans(void);
  void LcdVirtualToTrue(void);
  void LcdClrRect(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom,
                  INT8U ucColor);
  void Lcd_Draw_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom,
                    INT8U ucColor);
  void Lcd_Draw_Line(INT16 usX0, INT16 usY0, INT16 usX1, INT16 usY1,
                     INT8U ucColor, INT16U usWidth);
  void Lcd_Draw_HLine(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor,
                      INT16U usWidth);
  void Lcd_Draw_VLine(INT16 usY0, INT16 usY1, INT16 usX0, INT8U ucColor,
                      INT16U usWidth);
  void Lcd_Anti_Disp(INT16U usX0, INT16U usY0, INT16U usX1, INT16U usY1);
  void Lcd_DisplayChar(INT16U usX0, INT16U usY0, INT8U ucChar);
  void Lcd_DisplayString(INT16U usX0, INT16U usY0, INT8U *pucStr);
  void Lcd_DisplayShort(INT16 sX, INT16 sY, INT16U usInt);
  void Zdma0Done(void) __attribute__((interrupt("IRQ")));
  void Lcd_DspAscII6x8(INT16U usX0, INT16U usY0, INT8U ForeColor, INT8U *pucChar);
  void Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U *s);
  void Lcd_DspHz16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U *s);
  void ReverseLine(INT32U ulHeight, INT32U ulY);
  INT8U LCD_GetPixel(INT16U usX, INT16U usY);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H__ */
