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

#define COLORS_SUPPORTED 4

typedef enum {
	RED   = 0,
	GREEN = 1,
	BLUE  = 2,
	BLACK = 3,
	WHITE = 4
}PIXEL_COLOR;

/** Public function prototypes */
extern void WS2812_Init(void);
extern void WS2812_SetNumberOfPixels(uint16_t nNumberOfLEDs);
extern void WS2812_SetColor(PIXEL_COLOR eColor);



#endif /* APPLICATION_WS2812_H_ */
