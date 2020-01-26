/*
 * serial.c
 *
 *  Created on: Jan 25, 2020
 *  Author: DVT
 *  Brief:  This driver implements command interpreter, based on
 *  	    USART communication. Also, it provides functions based on USART for
 *  	    transmitting char and string. C's standard function printf() is implemented.
 */

#include "serial.h"

static void Serial_InitGPIOs();

/** Public data */
uint8_t Rx_Fifo[FIFO_SIZE];
uint8_t Tx_Fifo[FIFO_SIZE];

uint8_t Rx_Fifo[FIFO_SIZE];
uint8_t Tx_Fifo[FIFO_SIZE];

uint8_t flCommandReceived = 0;
uint8_t flCommandFound 	  = 0;

uint8_t flUartTxBusy = 0;
uint8_t flFifoFull	 = 0;

uint16_t Rx_Fifo_Push = 0;
uint16_t Rx_Fifo_Pull = 0;
uint16_t Tx_Fifo_Push = 0;
uint16_t Tx_Fifo_Pull = 0;

uint8_t ascii_douple_point 		= ASCII_CODE_PP;
uint8_t ascii_new_line	 		= ASCII_CODE_NL;
uint8_t ascii_carriage_return	= ASCII_CODE_CR;
uint8_t	ascii_space				= ASCII_CODE_SP;

/** Public functions definitions */

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_Init()
//===================================
{
	Serial_InitGPIOs();
	Uart_Init(USART1);
	Uart_Receive_IT(USART1);
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_Process(void)
//===================================
{
  int idx = 0;
  if(flCommandReceived)
  {
    Tx_Fifo_Push = 0;
    Tx_Fifo_Pull = 0;
    flCommandFound = 0;
    flCommandReceived = 0;

    while(*(Services[idx]) != '\0')
    {
      if(!(strncmp(Services[idx],(char *)Rx_Fifo, 4)))
      {
        flCommandFound = 1;
        Serial_Puts("\r\n");
        (*g_arrServices[idx])(Rx_Fifo);
        break;
      }
      idx ++;
    }
    if (!flCommandFound)
    {
      Serial_Puts("\r\n");
      (*g_arrServices[idx])(Rx_Fifo);
    }
    idx = 0;

    Serial_ClearRxBuff();
    Serial_Puts("\r\n:");
  }
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_Putc(uint8_t Symbol)
//===================================
{
  // If fifo get full has to wait here ...
  while(flFifoFull)
  {}
  if(flUartTxBusy && !flFifoFull)
  {
    Tx_Fifo[Tx_Fifo_Push++] = Symbol;
    if(((Tx_Fifo_Push) % FIFO_SIZE) == Tx_Fifo_Pull)
    {
      flFifoFull = 1;
    }
    if(Tx_Fifo_Push == FIFO_SIZE)
      Tx_Fifo_Push = 0;
  }
  else
  {
    Uart_Transmit_IT(USART1, Symbol);
    flUartTxBusy = 1;
  }
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_Puts(char *Message)
//===================================
{
  int symbol = 0;
  while(Message[symbol] != '\0')
  {
    Serial_Putc(Message[symbol++]);
  }
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_Printf(char *Message, ...)
//===================================
{
  int symbol = 0;
  va_list arguments;
  va_start(arguments, Message);
  int iArg = 0;
  char iStr[10];
  char chArg = 0;
  char *strArg = 0;
  float f32Arg = 0.0;


  while(Message[symbol] != '\0')
  {
    if(Message[symbol] == '%')
    {
      symbol++;
      switch(Message[symbol])
      {
        case 'c':
        chArg = va_arg(arguments, int);
        Serial_Putc(chArg);
        break;

        case 'd':
        iArg = va_arg(arguments, int);
        sprintf(iStr, "%d", iArg);
        Serial_Puts(iStr);
        break;

        case 'x':
        iArg = va_arg(arguments, int);
        sprintf(iStr, "%x", iArg);
        Serial_Puts(iStr);
        break;

        case 's':
        strArg = va_arg(arguments,char *);
        Serial_Puts(strArg);
        break;

        case 'f':
        f32Arg = va_arg(arguments, double);
        sprintf(iStr, "%.2f", f32Arg);
        Serial_Puts(iStr);
        break;

        default:
        Serial_Putc(Message[--symbol]);
      }
      symbol++;
    }

    else
      Serial_Putc(Message[symbol++]);
  }
  va_end(arguments);
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
int Serial_GetMsgLength(char *Message)
//===================================
{
  int length = 0;
  int i = 0;
  while(Message[i++] != '\0')
  {
    length ++;
  }
  return length;
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
char* Serial_GetNextArgument(char *Message)
//===================================
{
  char *StrPtr = NULL;
  int i = 0;

  while(Message[i] != '\0')
  {
    if(Message[i] == ASCII_CODE_SP)
    {
      i++;
      StrPtr = &Message[i];
      break;
    }
    i++;
  }

  return StrPtr;
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
int Serial_ConvertArgDec(char *Message)
//===================================
{
  char StrPtr[10];
  int i = 0;
  int arg_num = 0;

  /* Check if the next symbol is a digit */
  while((Message[i] >= 0x30) && (Message[i] <= 0x39))
  {
    StrPtr[i] = Message[i];
    i++;
  }
  StrPtr[i] = '\0';

  /* Convert string to int using standard function from <stdlib.h> */
  arg_num = atoi(StrPtr);

  return arg_num;
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_ClearRxBuff(void)
//===================================
{
  for(int i = 0; i < FIFO_SIZE; i++)
  {
    Rx_Fifo[i] = 0;
  }
}

/**
 * @brief:
 * @param:  none
 * @return: none
 */
//===================================
void Serial_ClearTxBuff(void)
//===================================
{
  Tx_Fifo_Pull = 0;
  Tx_Fifo_Push = 0;
  for(int i = 0; i < FIFO_SIZE; i++)
  {
    Tx_Fifo[i] = 0;
  }
}

/** Private functions definitions */

/**
 * @brief: Initialize USART1 Tx and Rx GPIO pins - PA9 and PA10
 * @param:  none
 * @return: none
 */
//===================================
void Serial_InitGPIOs()
//===================================
{
	RCC->AHB2ENR   |= RCC_AHB2ENR_GPIOAEN; /* Enable GPIOA clock. */

	GPIOA->MODER   &= 0xffc3ffff;
	GPIOA->MODER   |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1); /* Configure the pins as alternate function mode. */
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10); /* Configure output speed - very high. */
	GPIOA->AFR[1]  |= 0x00000770; /* Configure the pins for UART mode. (Tx & Rx) */
}

//===================================
