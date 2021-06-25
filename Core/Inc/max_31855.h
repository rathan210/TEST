/*
 * max_31855.h
 *
 *  Created on: Jul 1, 2020
 *      Author: CEPL
 */

#ifndef INC_MAX_31855_H_
#define INC_MAX_31855_H_

#include "main.h"
//#include "MAX_31855.h"
//#include "stm32f0xx_hal.h"
//#include "stdint.h"
//#include <string.h>
//#include <stdarg.h>
//#include "ili9341.h"
#include <stdlib.h>


#if 0
typedef enum{
	TEMP_OK = 0,
	TEMP_SCV_FAULT, /*Thermocouple is short circuited to VCC*/
	TEMP_SCG_FAULT, /*Thermocouple is short circuited to GND*/
	TEMP_OC_FAULT, /*Thermocouple is open*/
}Temp_status;

uint16_t Temperature_value;

Temp_status status;

Temp_status ReadTemperature(uint16_t *temp);

void GetempData(void);

void TempError(Temp_status error);

//extern char temp_buff[200];
extern char temp_buff_1[200];

extern SPI_HandleTypeDef hspi1;
#endif


typedef enum{
	TEMP_OK = 0,
	TEMP_SCV_FAULT, /*Thermocouple is short circuited to VCC*/
	TEMP_SCG_FAULT, /*Thermocouple is short circuited to GND*/
	TEMP_OC_FAULT, /*Thermocouple is open*/
}Temp_status;

volatile  uint16_t  Temperature_value;

Temp_status status;

Temp_status ReadTemperature(uint16_t *temp);
//Temp_status ReadTemperature(void);

uint16_t GetempData();

void TempError(Temp_status error);

extern char temp_buff[200];

extern SPI_HandleTypeDef hspi1;


#endif /* INC_MAX_31855_H_ */

