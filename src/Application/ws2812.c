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
/** Public functions definitions */
//===================================

/**
 * @brief: This function initializes the LED strip. Principle of control is:
 * 		   NRZ communication protocol is used for data transmission between the
 * 		   MCU and the WS2812. For every pixel there are 24 bits sent (1 byte for every color).
 * 		   Depending on what logical level has to be transmitted, the pulse width of the PWM signal
 * 		   varies. After every period of the signal, a DMA event is generated and the duty cycle is
 * 		   updated. The next value is taken from the RAM using the DMA controller.
 * @param:  none
 * @return: none
 */
//===================================
void WS2812_Init()
//===================================
{
	// Enable the clock for GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Initialize PA0
	GPIOA->MODER &= ~GPIO_MODER_MODE0;
	GPIOA->MODER |= GPIO_MODER_MODER0_0; // Configure the pin as output.
	GPIOA->ODR	 |= 1;       			 // Set the GPIO to 0.
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void WS2812_SetNumberOfPixels(uint16_t nNumberOfLEDs)
//===================================
{

}


/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void WS2812_WriteColor(PIXEL_COLOR eColor)
//===================================
{

}

//===================================



