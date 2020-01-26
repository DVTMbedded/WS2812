/*
 * services.h
 *
 *  Created on: Jan 26, 2020
 *      Author: DVT
 */

#ifndef APPLICATION_SERVICES_H_
#define APPLICATION_SERVICES_H_

#include "stm32l4xx.h"
#include "serial.h"
#include "ws2812.h"

extern const char*  Services[];
extern void (*g_arrServices[])(uint8_t *RxData);


#endif /* APPLICATION_SERVICES_H_ */
