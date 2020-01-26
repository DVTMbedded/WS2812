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
#include "usart.h"

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

static uint16_t    g_nLedCntr_mS = 0;
static PIXEL_COLOR g_ePixelColor = RED;

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	if (++g_nLedCntr_mS == 500)
	{
		//LED_Toggle();
		g_nLedCntr_mS = 0;
		WS2812_SetColor(g_ePixelColor);
		if (++g_ePixelColor > COLORS_SUPPORTED)
		{
			g_ePixelColor = 0;
		}
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
	if((USART1->ISR & USART_ISR_RXNE))
	{
		USART1->ICR  |= USART_ISR_RXNE; 	/* Clear interrupt flag */
		USART1->CR1  &= ~USART_CR1_RXNEIE; /* Transmition complete interrupt disable. */

		RxByte = USART1->RDR;
		Uart_Transmit_IT(USART1, RxByte);
	}
	if((USART1->ISR & USART_ISR_TC))
	{

	}

	Uart_Receive_IT(USART1);
}
