/*
 * clock.c
 *
 *  Created on: Jan 3, 2020
 *  Author: DVT
 *  Brief: This file contains functions concerning the MCU clock system.
 *  HSI 16 MHz RC oscillator is used as a main clock source. PLL multiplies this
 *  frequency to form SYSCLK of 80 MHz.
 */

#include "clock.h"

/** Private functions */

/** Private data */
uint32_t g_cSystemClock_Hz = 80000000;

/** Public functions definitions */

/**
 * @brief:  This functions configures the clocks used in the system:
 * 		    HSI 16MHz RC oscillator is used. PLL is used.
 * 		    SYSCLK = 80 MHz; AHB1 = 80 MHz; APB1 = 80 MHz; APB2 = 80 MHz
 * @param:  none
 * @return: none
 */
//===================================
void Clock_Configure()
//===================================
{
	/** PLL configuration:
	 * 	HSI used as clock source
	 * 	M = 1; N = 20; R = 4
	 * 	SYSCLOCK = (HSI * N) / (R * M) = 80 MHz
	 */
	uint32_t nPllRegMask = (uint32_t)(RCC_PLLCFGR_PLLPEN | RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLR_0 | (20 << 8) | RCC_PLLCFGR_PLLSRC_HSI);

	// Enable HSI clock source generated from 16MHz RC oscillator
	RCC->CR |= RCC_CR_HSION;
	// Stay here until HSI clock gets ready
	while(!(RCC->CR & RCC_CR_HSIRDY))
	{}
	// Write PLL configuration register
	RCC->PLLCFGR = nPllRegMask;
	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	// Wait till the main PLL is ready
	while(!(RCC->CR & RCC_CR_PLLRDY))
	{}
	// Select PLL as a system clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait till the main PLL is used as system clock source
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
	{}
	// Configure SysTick interrupt - 1 mS
	SysTick_Config(g_cSystemClock_Hz / 1000);
	NVIC_SetPriority(SysTick_IRQn, 1);
}

/**
 * @brief:  This function returns the SYSCLK measured in Hz
 * @param:  none
 * @return: g_cSystemClock_Hz = 80 MHz (80000000 Hz)
 */
//===================================
uint32_t Clock_GetFrequency()
//===================================
{
	return g_cSystemClock_Hz;
}
//===================================
