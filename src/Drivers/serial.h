/*
 * serial.h
 *
 *  Created on: Jan 25, 2020
 *  Author: DVT
 *  Brief: This driver implements command interpreter, based on
 *  	   USART communication. Also, it provides functions based on USART for
 *  	   transmitting char and string. C's standart function printf() is implemented.
 */

#ifndef DRIVERS_SERIAL_H_
#define DRIVERS_SERIAL_H_

#include "stm32l4xx.h"
#include "usart.h"
#include "services.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#define FIFO_SIZE			(100)


/** ASCII CODES DEFINITIONS */
#define ASCII_CODE_CR		(0x0D)
#define ASCII_CODE_PP		(0x3A)
#define ASCII_CODE_NL		(0x0A)
#define ASCII_CODE_SP		(0x20)
#define ASCII_CODE_BS		(0x08)
#define ASCII_CODE_DEL		(0x7F)
#define	ASCII_CODE_ESC		(0x1B)
#define ASCII_CODE_SC		(0x3B)

extern  uint8_t  Rx_Fifo[FIFO_SIZE];
extern  uint8_t  Tx_Fifo[FIFO_SIZE];

extern  uint8_t  Rx_Symbol;
/* -------------------------- */
extern  uint8_t  flCommandReceived;
extern  uint8_t  flCommandFound;

extern  uint8_t  flUartTxBusy;
extern  uint8_t  flFifoFull;

/* -------------------------- */

extern  uint16_t Rx_Fifo_Push;
extern  uint16_t Rx_Fifo_Pull;
extern  uint16_t Tx_Fifo_Push;
extern  uint16_t Tx_Fifo_Pull;

/** Public function prototypes */
extern void  Serial_Init();
extern void  Serial_Process();
extern void  Serial_Putc(uint8_t Symbol);
extern void  Serial_Puts(char *Message);
extern void  Serial_Printf(char *Message, ...);
extern int   Serial_GetMsgLength(char *Message);
extern char* Serial_ConvertDecString(int num);
extern char* Serial_GetNextArgument(char *Message);
extern int 	 Serial_ConvertArgDec(char *Message);
extern void  Serial_ClearRxBuff(void);
extern void  Serial_ClearTxBuff(void);


#endif /* DRIVERS_SERIAL_H_ */
