/*
 * serial.h
 *
 *  Created on: Jan 25, 2020
 *  Author: DVT
 *  Brief: This is a driver for USART serial communication
 *
 */

#ifndef DRIVERS_SERIAL_H_
#define DRIVERS_SERIAL_H_

#include "stm32l4xx.h"
#include "usart.h"

/** Public function prototypes */
extern void Serial_Init();
extern void Serial_Process();


#endif /* DRIVERS_SERIAL_H_ */
