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

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

static uint16_t g_nLedCntr_mS = 0;

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if (++g_nLedCntr_mS == 1000)
	{
		LED_Toggle();
		g_nLedCntr_mS = 0;
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
		LED_SetState(LED_ON);
		DMA1->IFCR |= DMA_IFCR_CTCIF2;
	}
}
