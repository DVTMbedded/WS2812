/*
 * usart.h
 *
 *  Created on: Jan 25, 2020
 *  Author: DVT
 *  Brief: This is a driver for MCU USART modules.
 *  It provides 2 different interfaces for data transmit and reception:
 *   - Non-blocking using interrupt controller
 *   - Non-blocking using DMA controller
 *  The speed is configured to 115200 bps.
 */

#ifndef DRIVERS_USART_H_
#define DRIVERS_USART_H_

#include "stm32l4xx.h"
#include "clock.h"

/** Public data */
extern uint8_t RxByte;

/** Public function prototypes */

extern void Uart_Init(USART_TypeDef * UART);
extern void Uart_Transmit_IT(USART_TypeDef * UART, uint8_t TxData);
extern void Uart_Receive_IT(USART_TypeDef * UART);
extern void Uart_Receive_DMA(USART_TypeDef * UART, uint16_t Length);
extern void Uart_Transmit_DMA(USART_TypeDef * UART, uint16_t Length);


#endif /* DRIVERS_USART_H_ */
