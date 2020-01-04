/*
 * led.h
 *
 *  Created on: Jan 3, 2020
 *  Author:     DVT
 *  Brief: This file contains functions concerning the LED mounted on the board.
 *  It provides interface for initialize and set the LED On or Off.
 */

#ifndef DRIVERS_LED_H_
#define DRIVERS_LED_H_

#include "stm32l4xx.h"


/** Public data types */
typedef enum {
	LED_OFF = 0,
	LED_ON  = 1
}LED_STATE;

/** Public function prototypes */
extern void LED_Init();
extern void LED_SetState(LED_STATE eState);
extern void LED_Toggle();


#endif /* DRIVERS_LED_H_ */
