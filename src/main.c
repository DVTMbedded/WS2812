/**
 ******************************************************************************
 * @file    main.c
 * @author  Denislav Trifonov
 * @version V0.1
 * @date    14-December-2019
 * @brief   Default main function.
 ******************************************************************************
 */


#include "stm32l4xx.h"
#include "system_stm32l4xx.h"
#include "clock.h"
#include "led.h"
#include "ws2812.h"
#include "serial.h"


int main(void)
{
	// Initialize the system
	SystemInit();
	Clock_Configure();

	// Initialize peripherals
	LED_Init();
	WS2812_Init();
	Serial_Init();


	while (1)
	{
		Serial_Process();
	}
	return 0;
}
