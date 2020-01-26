/*
 * usart.c
 *
 *  Created on: Jan 25, 2020
 *  Author: DVT
 *  Brief: This is a driver for MCU USART modules.
 *  It provides 2 different interfaces for data transmit and reception:
 *   - Non-blocking using interrupt controller
 *   - Non-blocking using DMA controller
 */

#include "usart.h"

/** Private data */

/** Public data */
uint8_t RxByte = 0;

/** Private functions */

/** Public functions definitions */

/**
 * @brief:  This function initializes and enables the USART peripheral. Communication parameters are:
 *          Start bit - 1; Data bits - 8; Stop bit - 1; Parity - none; Baud rate: 115200 bps.
 *          USART global interrupt is enabled. Both Tx and Rx are enabled.
 * @param:  UART - pointer to structure containing USART registers description
 * @return: none
 */
//===================================
void Uart_Init(USART_TypeDef * UART)
//===================================
{
	uint32_t usartClk_Hz       = Clock_GetFrequency();
	uint32_t usartBaudRate_bps = 115200;

	CLEAR_BIT(UART->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	CLEAR_BIT(UART->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  // Enable AHB1 clock for USART1.
	RCC->CCIPR   |= RCC_CCIPR_USART1SEL_0; // Select SYSCLK as USART1 clock source.

	UART->CR1 &= ~USART_CR1_OVER8; // Configure oversampling mode by 16.
	UART->CR1 &= ~USART_CR1_M;	   // Define the word length (1 Start bit, 8 Data bits, n Stop bit).
	UART->CR2 &= ~USART_CR2_STOP;  // Program the number of stop bits.

	UART->BRR = usartClk_Hz / usartBaudRate_bps; // Configure the desired baud rate (115200 bps).

	// Enable UART interrupts at nested vector interrupt controller (NVIC).
	NVIC_SetPriority(USART1_IRQn, 2);
	NVIC_EnableIRQ(USART1_IRQn);

	UART->CR1 |= USART_CR1_UE; // Enable the USART.
	UART->CR1 |= USART_CR1_TE; // Transmitter Enable.
	UART->CR1 |= USART_CR1_RE; // Receiver Enable.
}

/**
 * @brief:  This function sends one byte of data using the USART peripheral in interrupt mode.
 *          After the data is successfully transmitted, an interrupt is generated and the corresponding
 *          ISR is executed (see file stm32l4xx_it.c).
 * @param1:  UART - pointer to structure containing USART registers description
 * @param2:  TxData - 1 Byte of data to be transferred.
 * @return: none
 */
//===================================
void Uart_Transmit_IT(USART_TypeDef * UART, uint8_t TxData)
//===================================
{
	UART->CR1 |= USART_CR1_TCIE; /* Transmition complete interrupt enable. */

	while(!(UART->ISR & USART_ISR_TXE))
	{} /* Wait until data register is empty. */
	UART->TDR = TxData; /* Write the data to be send into Data register. */
}

/**
 * @brief:  This function enables the reception of one byte of data using the USART peripheral in interrupt mode.
 *          After the data is successfully received, an interrupt is generated and the corresponding ISR is
 *          executed (see file stm32l4xx_it.c).
 * @param:  UART - pointer to structure containing USART registers description
 * @return: none
 */
//===================================
void Uart_Receive_IT(USART_TypeDef * UART)
//===================================
{
	UART->CR1 |= USART_CR1_RXNEIE; /* Rx register not empty interrupt enable. */
}
//===================================
