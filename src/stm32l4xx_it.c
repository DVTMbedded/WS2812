/**
 ******************************************************************************
 * @file    stm32l4xx_it.c
 * @author  Ac6
 * @version V1.0
 * @date    02-Feb-2015
 * @brief   Default Interrupt Service Routines.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "led.h"
#include "ws2812.h"
#include "serial.h"

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

static uint16_t    g_nLedCntr_mS = 0;

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	if (++g_nLedCntr_mS == 500)
	{
		g_nLedCntr_mS = 0;
		LED_Toggle();
	}
}

/**
 * @brief  This function handles DMA1_CHANNEL5 Handler.
 * @param  None
 * @retval None
 */
void DMA1_CH2_IRQHandler(void)
{
	if (DMA1->ISR & DMA_ISR_TCIF2)
	{
		DMA1->IFCR |= DMA_IFCR_CTCIF2;
		TIM2->CCR1  = TIM2->ARR;
	}
}


/**
 * @brief  This function handles USART1 global interrupt.
 * @param  None
 * @retval None
 */
void USART1_IRQHandler(void)
{
	/* Check if the transmission is complete. */
	if((USART1->ISR & USART_ISR_TC))
	{
		USART1->ICR |= USART_ICR_TCCF; 	/* Clear interrupt flag */
		USART1->CR1 &= ~USART_CR1_TCIE; /* Transmition complete interrupt disable. */
		/* -------------------- Tx ISR --------------------- */
		if(flUartTxBusy)
		{
			if(Tx_Fifo_Pull != Tx_Fifo_Push)
			{
				Uart_Transmit_IT(USART1, Tx_Fifo[Tx_Fifo_Pull++]);
				//	Tx_Fifo_Pull++;
				flFifoFull = 0;
				if(Tx_Fifo_Pull == FIFO_SIZE)
					Tx_Fifo_Pull = 0;
			}
			else
			{
				flUartTxBusy = 0;
			}
		}
	}
	/* ----------------------------------------------------------------------*/
	/* Check if new data is received */
	if((USART1->ISR & USART_ISR_RXNE))
	{
		USART1->CR1  &= ~USART_CR1_RXNEIE; /* Transmition complete interrupt disable. */

		RxByte = USART1->RDR;
		/* -------------------- Rx ISR --------------------- */
		switch(RxByte)
		{
		case ASCII_CODE_CR:
			if(Rx_Fifo_Push)
			{
				Rx_Fifo_Push = 0;
				flCommandReceived = 1;
				Serial_Putc('\n');
			}
			else
			{
				Serial_Puts("\r\n:");
			}
			Uart_Receive_IT(USART1);
			break;

		case ASCII_CODE_NL:
			if(Rx_Fifo_Push)
			{
				Rx_Fifo_Push = 0;
				flCommandReceived = 1;
				Serial_Putc('\n');
			}
			else
			{
				Serial_Puts("\r\n:");
			}
			Uart_Receive_IT(USART1);
			break;

		case ASCII_CODE_BS:
			if(Rx_Fifo_Push)
			{
				Uart_Transmit_IT(USART1, RxByte);
				Rx_Fifo_Push --;
			}
			Uart_Receive_IT(USART1);
			break;

		case ASCII_CODE_DEL:
			if(Rx_Fifo_Push)
			{
				Uart_Transmit_IT(USART1, RxByte);
				Rx_Fifo_Push --;
			}
			Uart_Receive_IT(USART1);
			break;

		case ASCII_CODE_ESC:
			if(Rx_Fifo_Push)
			{
				Uart_Transmit_IT(USART1, RxByte);
				Rx_Fifo_Push --;
			}
			Uart_Receive_IT(USART1);
			break;

		default:
			Rx_Fifo[Rx_Fifo_Push++] = RxByte;
			Uart_Transmit_IT(USART1, RxByte);
			Uart_Receive_IT(USART1);
		}

		if(Rx_Fifo_Push == FIFO_SIZE)	Rx_Fifo_Push--;
	}
}
