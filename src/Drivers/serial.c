/*
 * serial.c
 *
 *  Created on: Jan 25, 2020
 *      Author: DVT
 */

#include "serial.h"

static void Serial_InitGPIOs();

void Serial_Init()
{
	Serial_InitGPIOs();
	Uart_Init(USART1);
	Uart_Receive_IT(USART1);
}


/**
 * @brief: Initialize USART1 Tx and Rx GPIO pins - PA9 and PA10
 * @param:  none
 * @return: none
 */
void Serial_InitGPIOs()
{
	RCC->AHB2ENR   |= RCC_AHB2ENR_GPIOAEN; /* Enable GPIOA clock. */

	GPIOA->MODER   &= 0xffc3ffff;
	GPIOA->MODER   |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1); /* Configure the pins as alternate function mode. */
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10); /* Configure output speed - very high. */
	GPIOA->AFR[1]  |= 0x00000770; /* Configure the pins for UART mode. (Tx & Rx) */
}
