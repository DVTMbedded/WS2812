/*
 * ws2812.h
 *
 *  Created on: Jan 3, 2020
 *      Author: DVT
 */

#ifndef APPLICATION_WS2812_H_
#define APPLICATION_WS2812_H_

#include "stm32l4xx.h"

/** Public data types */
typedef enum {
	RED   = 0,
	GREEN = 1,
	BLUE  = 2,
	WHITE = 3
}PIXEL_COLOR;

/** Public function prototypes */
extern void WS2812_Init(void);
extern void WS2812_SetNumberOfPixels(uint16_t nNumberOfLEDs);
extern void WS2812_WriteColor(PIXEL_COLOR eColor);



#endif /* APPLICATION_WS2812_H_ */
