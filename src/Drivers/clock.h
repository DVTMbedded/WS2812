/*
 * clock.h
 *
 *  Created on: Jan 3, 2020
 *  Author: DVT
 *  Brief: This file contains functions concerning the MCU clock system.
 *  HSI 16 MHz RC oscillator is used as a main clock source. PLL multiplies this
 *  frequency to form SYSCLK of 80 MHz.
 */

#ifndef DRIVERS_CLOCK_H_
#define DRIVERS_CLOCK_H_

#include "stm32l4xx.h"


/** Public function prototypes */
extern void 	Clock_Configure();
extern uint32_t Clock_GetFrequency();


#endif /* DRIVERS_CLOCK_H_ */
