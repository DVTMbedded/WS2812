/*
 * led.c
 *
 *  Created on: Jan 3, 2020
 *  Author:     DVT
 *  Brief: This file contains functions concerning the LED mounted on the board.
 *  It provides interface for initialize and set the LED On or Off.
 */

#include "led.h"

/** Private functions */

/** Private data */
GPIO_TypeDef* const g_pLedPort = GPIOB;
const uint16_t 	    g_cLedPin  = (1 << 2);

//===================================
/** Public functions definitions */
//===================================
/**
 * @brief:  This function initializes the LED mounted on the PCB. It is connected
 * 			to MCU pin PB2. It is doing following tasks:
 * 			1. Initialize the GPIO - configure PB2 as an output
 * 			2. Set its state to 0.
 * @param:  none
 * @return: none
 */
//===================================
void LED_Init()
//===================================
{
	// Enable the clock for GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// Initialize PB2
	g_pLedPort->MODER &= ~GPIO_MODER_MODE2;
	g_pLedPort->MODER |= GPIO_MODER_MODER2_0; // Configure the pin as output.
	g_pLedPort->ODR	 &= ~g_cLedPin;       // Set the GPIO to 0.
}

/**
 * @brief:  This function sets the state of the LED mounted on the PCB.
 * @param:  eState - enum. It can be one of the following values:
 * 		    LED_OFF - LED will be Off
 * 		    LED_ON	- LED will be on
 * @return: none
 */
//===================================
void LED_SetState(LED_STATE eState)
//===================================
{
	if (eState == LED_ON)
	{
		g_pLedPort->ODR	 |= g_cLedPin;
	}
	else
	{
		g_pLedPort->ODR	 &= ~g_cLedPin;
	}
}

/**
 * @brief:  This function toggles the state of the LED mounter on the PCB.
 * @param:  none
 * @return: none
 */
//===================================
void LED_Toggle()
//===================================
{
	g_pLedPort->ODR ^= g_cLedPin;
}
//===================================
