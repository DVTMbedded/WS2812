/*
 * ws2812.c
 *
 *  Created on: Jan 3, 2020
 *  Author: DVT
 *  Brief: This file provides functions for controlling LED Strip WS2812. Supported features are:
 *  	   - Configurable number of LEDs
 *  	   - Configurable colors
 *  	   - Configurable effects to be presented on the strip
 *  	   - Configurable fps
 */

#include "ws2812.h"


//===================================
/** Private Data */
//===================================
static uint16_t g_nPixelsNum = 1;
static uint16_t g_cMaximumPixels = 60;
static uint32_t g_arrPixelsData[124];


//===================================
/** Public functions definitions */
//===================================

/**
 * @brief: This function initializes the LED strip. Principle of control is:
 * 		   NRZ communication protocol is used for data transmission between the
 * 		   MCU and the WS2812. For every pixel there are 24 bits sent (1 byte for every color).
 * 		   Depending on what logical level has to be transmitted, the pulse width of the PWM signal
 * 		   varies. After every period of the signal, a DMA event is generated and the duty cycle is
 * 		   updated. The next value is taken from the RAM using the DMA controller.
 * 		   It is doing following tasks:
 * 		   	- Initialize Timer 2 in PWM mode - channel 1
 * 		   	- Initialize DMA controller
 * 		   	- Initialize PA0 in alternate function mode
 * @param:  none
 * @return: none
 */
//===================================
void WS2812_Init()
//===================================
{
	// Enable the clocks for GPIOA, TIM2 and DMA
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	RCC->AHB1ENR  |= RCC_AHB1ENR_DMA1EN;

	// Initialize the TIM2
	TIM2->CR1 &= ~TIM_CR1_DIR; // Counter used as upcounter.
	TIM2->ARR  = 100; // 1 Clock tick = 12.5 nS. Set timer period to 1.25 uS

	// Configure Capture Compare Channel 1 as Output in PWM Mode.
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2); // PWM mode 1 - channel is active as long as TIMx_CNT<TIMx_CCR1 else inactive.
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S; // Configure Capture Compate Channel 1 as output
	TIM2->CCER  |= (TIM_CCER_CC1E); // Enable Capture Compare Channel 1
	TIM2->CCER  &= ~(TIM_CCER_CC1NP); // In output mode this bit has to be keep in 0
	TIM2->CCR1  = 0; // Set Duty Cycle to 0 %.

	// Enable timer interrupts at nested vector interrupt controller (NVIC)
	NVIC_SetPriority(TIM2_IRQn, 1);
	NVIC_EnableIRQ(TIM2_IRQn);

	// Initialize DMA
	// Enable DMA1_Channel2 interrupts at nested vector interrupt controller (NVIC)
	NVIC_SetPriority(DMA1_Channel2_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);

	DMA1_CSELR->CSELR |= 4 << 4; // TIM2_CH1 is mapped to DMA1_Channel2 and the CxS bits are b0100 (4)

	// Be sure that the channel is disabled before configure it.
	if (DMA1_Channel2->CCR & DMA_CCR_EN)
	{
		DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	}

	DMA1_Channel2->CPAR  = (uint32_t)&(TIM2->CCR1);   // Set the peripheral register address
	DMA1_Channel2->CMAR  = (uint32_t)g_arrPixelsData; // Set the memory address

	// Configure  the channel - priority, direction, circular mode, increment mode, data size
	DMA1_Channel2->CCR |= (DMA_CCR_PL | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_PSIZE_1 |
			DMA_CCR_MSIZE_1 | DMA_CCR_TCIE | DMA_CCR_TEIE);

	// Configure PA0 in alternate function mode
	GPIOA->MODER   &= ~GPIO_MODER_MODE0;
	GPIOA->MODER   |= GPIO_MODER_MODE0_1;     // select alternate function mode
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_1; // set the port speed to high
	GPIOA->AFR[0]  |= GPIO_AFRL_AFSEL0_0;     // Select AF1 as alternate function

	// Initialize the pixels array
	for (uint16_t idx = 0; idx < 124; idx ++)
	{
		g_arrPixelsData[idx] = 0;
	}

	// Enable the timer
	TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief:  This function sets the number of pixels which have to be controlled by the MCU.
 * @param:  nNumberOfPixels - Number of pixels
 * @return: none
 */
//===================================
void WS2812_SetNumberOfPixels(uint16_t nNumberOfPixels)
//===================================
{
	if (nNumberOfPixels > g_cMaximumPixels)
	{
		nNumberOfPixels = g_cMaximumPixels;
	}
	g_nPixelsNum = nNumberOfPixels;
}


/**
 * @brief:  This function fills the LED strip with single color. It is a static effect.
 * @param:  eColor - chosen color to fill the pixels. It can be one of the PIXEL_COLOR enum (see ws2812.h)
 * @return: none
 */
//===================================
void WS2812_SetColor(PIXEL_COLOR eColor)
//===================================
{
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;   // Disable DMA channel
	TIM2->DIER         &= ~TIM_DIER_UDE; // Disable DMA request enable

	//DMA1_Channel2->CNDTR = 50 + 24 * g_nPixelsNum;
	DMA1_Channel2->CNDTR = 124; // Number of data to be transferred

	if (eColor == RED)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			g_arrPixelsData[i + 50] = 67; // red
		}

		for (uint8_t j = 0; j < 8; j++)
		{
			g_arrPixelsData[j + 58] = 33; // green
		}

		for (uint8_t k = 0; k < 8; k++)
		{
			g_arrPixelsData[k + 66] = 33; // blue
		}
	}

	else if (eColor == BLUE)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			g_arrPixelsData[i + 50] = 33; // red
		}

		for (uint8_t j = 0; j < 8; j++)
		{
			g_arrPixelsData[j + 58] = 33; // green
		}

		for (uint8_t k = 0; k < 8; k++)
		{
			g_arrPixelsData[k + 66] = 67; // blue
		}
	}

	else if (eColor == GREEN)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			g_arrPixelsData[i + 50] = 33; // red
		}

		for (uint8_t j = 0; j < 8; j++)
		{
			g_arrPixelsData[j + 58] = 67; // green
		}

		for (uint8_t k = 0; k < 8; k++)
		{
			g_arrPixelsData[k + 66] = 33; // blue
		}
	}

	else if (eColor == BLACK)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			g_arrPixelsData[i + 50] = 33; // red
		}

		for (uint8_t j = 0; j < 8; j++)
		{
			g_arrPixelsData[j + 58] = 33; // green
		}

		for (uint8_t k = 0; k < 8; k++)
		{
			g_arrPixelsData[k + 66] = 33; // blue
		}
	}

	else if (eColor == WHITE)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			g_arrPixelsData[i + 50] = 67; // red
		}

		for (uint8_t j = 0; j < 8; j++)
		{
			g_arrPixelsData[j + 58] = 67; // green
		}

		for (uint8_t k = 0; k < 8; k++)
		{
			g_arrPixelsData[k + 66] = 67; // blue
		}
	}

	for (uint8_t i = 74; i < 124; i++)
	{
		g_arrPixelsData[i] = 0;
	}

	DMA1_Channel2->CCR |= DMA_CCR_EN;   // Enable DMA channel
	TIM2->DIER         |= TIM_DIER_UDE; // Update DMA request enable
}
//===================================
