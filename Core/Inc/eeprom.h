/*
 * eeprom.h
 *
 *  Created on: Jul 4, 2020
 *      Author: CEPL
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_


#include "main.h"
#include "stm32f0xx_hal_def.h"

#define M_24C_WRITE_ADDRESS 	0xA0
#define M_24C_READ_ADDRESS 	    0xA1

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef M_24C_WriteData(uint16_t address,uint8_t * dataBuf,uint16_t size);
HAL_StatusTypeDef M_24C_ReadData(uint16_t address,uint8_t * dataBuf,uint16_t size);

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
//#include "ee24Config.h"
void eeprom_init();
bool    ee24_isConnected(void);
bool    ee24_write(uint16_t address, uint8_t *data, size_t lenInBytes, uint32_t timeout);
bool    ee24_read(uint16_t address, uint8_t *data, size_t lenInBytes, uint32_t timeout);
char test_eeprom_flg;
#define		_EEPROM_SIZE_KBIT							64
#define		_EEPROM_I2C   								hi2c1
#define		_EEPROM_USE_FREERTOS          0
#define		_EEPROM_ADDRESS               0xA0
#define		_EEPROM_USE_WP_PIN            0

#define EEPROM_START_ADDR				100

#define EEPROM_DEFAULT					1000

#define EEPROM_UPMA_COUNT_ADDR			(EEPROM_DEFAULT + 2 * sizeof(int))
#define EEPROM_KESAR_COUNT_ADDR			(EEPROM_UPMA_COUNT_ADDR + 2 * sizeof(int))
#define EEPROM_POHA_COUNT_ADDR			(EEPROM_KESAR_COUNT_ADDR + 2 * sizeof(int))
//#define PRE_HEAT_ADDR					1
#define MENU_COUNT						2		//No of menus in the system
#define PRE_HEAT_SET_TMP				3
#define PRE_HEAT_CUT_TMP				4
#define SLEEP_SET_TMP					5
#define SLEEP_CUT_TMP					6
#define SLEEP_HOUR						7
#define SLEEP_MIN						8
#define DEEP_SLEEP_HOUR					9
#define DEEP_SLEEP_MIN					10
#define COMPLETE_RECIPE_COUNT			16  // 4 bytes
#define MANUAL_MIN_TEMP					21
#define MANUAL_MAX_TEMP					22

#define RECIPIES_START_ADDR				80		//Starting EEPROM address of Menu

#define RECIPIE_BASE_ADDR(x)			x*RECIPIES_START_ADDR

#define MENU_ID_ADDR					0
#define MENU_NAME_START_ADDR			1
#define MENU_PORTION1_SET_TEMP			17		//Portion 1
#define MENU_PORTION1_CUTOFF_TEMP		18
#define MENU_PORTION1_C_TIME_T1			19
#define MENU_PORTION1_C_TIME_T2			21
#define MENU_PORTION1_H_TIME			23
#define MENU_PORTION1_COUNT				25 		//Count of each portion cooked

#define MENU_PORTION2_SET_TEMP			28		//Portion 2
#define MENU_PORTION2_CUTOFF_TEMP		29
#define MENU_PORTION2_C_TIME_T1			30
#define MENU_PORTION2_C_TIME_T2			32
#define MENU_PORTION2_H_TIME			34
#define MENU_PORTION2_COUNT				36 		//Count of each portion cooked

#define MENU_PORTION3_SET_TEMP			39		//Portion 3
#define MENU_PORTION3_CUTOFF_TEMP		40
#define MENU_PORTION3_C_TIME_T1			41
#define MENU_PORTION3_C_TIME_T2			43
#define MENU_PORTION3_H_TIME			45
#define MENU_PORTION3_COUNT				48 		//Count of each portion cooked

#define MENU_PORTION4_SET_TEMP			51		//Portion 4
#define MENU_PORTION4_CUTOFF_TEMP		52
#define MENU_PORTION4_C_TIME_T1			53
#define MENU_PORTION4_C_TIME_T2			55
#define MENU_PORTION4_H_TIME			57
#define MENU_PORTION4_COUNT				59 		//Count of each portion cooked

#define MENU_PORTION5_SET_TEMP			64		//Portion 5
#define MENU_PORTION5_CUTOFF_TEMP		65
#define MENU_PORTION5_C_TIME_T1			66
#define MENU_PORTION5_C_TIME_T2			68
#define MENU_PORTION5_H_TIME			70
#define MENU_PORTION5_COUNT				72 		//Count of each portion cooked





#if (_EEPROM_USE_WP_PIN==1)
#define		_EEPROM_WP_GPIO								EE_WP_GPIO_Port
#define		_EEPROM_WP_PIN								EE_WP_Pin
#endif

#endif /* INC_EEPROM_H_ */
