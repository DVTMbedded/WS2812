/*
 * services.c
 *
 *  Created on: Jan 26, 2020
 *      Author: DVT
 */

#include "services.h"

static void Service_Help(uint8_t *RxBuff);
static void Service_Unknown(uint8_t *RxBuff);

static void Service_SetColor(uint8_t *RxBuff);

const char*  Services[] = {
		"HELP",
		""
};


void (*g_arrServices[])(uint8_t *RxData) =
{
		&Service_Help,
		&Service_SetColor,
		&Service_Unknown
};

void Service_Help(uint8_t *RxBuff)
{
	Serial_Puts("Available commands:\r\n");
	Serial_Puts("HELP - This information\r\n");
	Serial_Puts("SETC - Set new color for WS2812\r\n");
}

void Service_SetColor(uint8_t *RxBuff)
{
	int nNextArg = Serial_ConvertArgDec(Serial_GetNextArgument((char *)RxBuff));
	PIXEL_COLOR eColor = (PIXEL_COLOR)nNextArg;

	if (eColor <= 4)
	{
		WS2812_SetColor(eColor);
		Serial_Puts("New color set!\r\n");
	}
	else
	{
		Serial_Puts("Color not supported!\r\n");
	}

}


void Service_Unknown(uint8_t *RxBuff)
{
	Serial_Puts("Unknown Command!\r\n");
}



