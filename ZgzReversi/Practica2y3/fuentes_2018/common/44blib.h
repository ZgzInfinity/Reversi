/*********************************************************************************************
* File:		44BLIB.H
* Author:	embest
* Desc:		Samsung 44B0X CPU function declare and common define
* History:
*********************************************************************************************/

#ifndef __44BLIB_H__
#define __44BLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DebugOut Uart_Printf

#define min(x1,x2) 	((x1<x2)? x1:x2)
#define max(x1,x2) 	((x1>x2)? x1:x2)

#define ONESEC0 	(62500)					// 16us resolution, max 1.04 sec
#define ONESEC1 	(31250)					// 32us resolution, max 2.09 sec
#define ONESEC2 	(15625)					// 64us resolution, max 4.19 sec
#define ONESEC3 	(7812)					// 128us resolution, max 8.38 sec
#define ONESEC4 	(MCLK/128/(0xff+1))  	// @60Mhz, 128*4us resolution, max 32.53 sec

//#define NULL 0

#define EnterPWDN(clkcon) ((void (*)(int))0xe0)(clkcon)
#define DOWNLOAD_ADDRESS _RAM_STARTADDRESS
/* 8led control register address */
#define	LED8ADDR	(*(volatile unsigned char *)(0x2140000))

/* 44blib.c */
void Delay(int time); 				// Watchdog Timer is used.
void DelayMs(int ms_time);
void *malloc(unsigned nbyte);
void free(void *pt);
void Port_Init(void);
void Beep(int BeepStatus);
void sys_init();// Interrupt,Port and UART

#ifdef __cplusplus
}
#endif

#endif /* __44BLIB_H__ */
