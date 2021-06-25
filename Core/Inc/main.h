/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MAX_31855.h"
#include "stdint.h"
#include <string.h>
#include <stdarg.h>
#include "ili9341.h"
#include "fonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "eeprom.h"
#include "menu_display.h"
//#include "menu.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
uint32_t pohaCnt, upmaCnt, kesarCnt;
uint8_t eeprom_buff_writeBuff[2];
uint8_t eeprom_buff_writeCnt[4];
uint8_t eeprom_buff_read[2];
uint8_t eeprom_buff_readCnt[4];
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void loop(void);
void init(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STRPPER_5_EN_Pin GPIO_PIN_13
#define STRPPER_5_EN_GPIO_Port GPIOC
#define STEPPER_5_PUL_Pin GPIO_PIN_0
#define STEPPER_5_PUL_GPIO_Port GPIOC
#define EX_FAN_Pin GPIO_PIN_3
#define EX_FAN_GPIO_Port GPIOC
#define SSR_RELAY_Pin GPIO_PIN_0
#define SSR_RELAY_GPIO_Port GPIOA
#define STEPPER_2_PUL_Pin GPIO_PIN_2
#define STEPPER_2_PUL_GPIO_Port GPIOA
#define STEPPER_2_EN_Pin GPIO_PIN_3
#define STEPPER_2_EN_GPIO_Port GPIOA
#define ILI9341_CS_Pin GPIO_PIN_5
#define ILI9341_CS_GPIO_Port GPIOF
#define Temp_CS_Pin GPIO_PIN_4
#define Temp_CS_GPIO_Port GPIOA
#define ILI9341_DC_Pin GPIO_PIN_5
#define ILI9341_DC_GPIO_Port GPIOC
#define ILI9341_RES_Pin GPIO_PIN_0
#define ILI9341_RES_GPIO_Port GPIOB
#define UP_DIRECTION_Pin GPIO_PIN_13
#define UP_DIRECTION_GPIO_Port GPIOB
#define DOWN_DIRECTION_Pin GPIO_PIN_14
#define DOWN_DIRECTION_GPIO_Port GPIOB
#define OK_PIN_Pin GPIO_PIN_6
#define OK_PIN_GPIO_Port GPIOC
#define PROX_UPMA_Pin GPIO_PIN_7
#define PROX_UPMA_GPIO_Port GPIOC
#define PROX_POHA_Pin GPIO_PIN_8
#define PROX_POHA_GPIO_Port GPIOC
#define PROX_KESARI_Pin GPIO_PIN_9
#define PROX_KESARI_GPIO_Port GPIOC
#define STEPPER_4_PUL_Pin GPIO_PIN_9
#define STEPPER_4_PUL_GPIO_Port GPIOA
#define STEPPER_4_EN_Pin GPIO_PIN_10
#define STEPPER_4_EN_GPIO_Port GPIOA
#define STEPPER_1_EN_Pin GPIO_PIN_6
#define STEPPER_1_EN_GPIO_Port GPIOF
#define STEPPER_1_PUL_Pin GPIO_PIN_7
#define STEPPER_1_PUL_GPIO_Port GPIOF
#define STEPPER_3_EN_Pin GPIO_PIN_4
#define STEPPER_3_EN_GPIO_Port GPIOB
#define STEPPER_3_PUL_Pin GPIO_PIN_5
#define STEPPER_3_PUL_GPIO_Port GPIOB
#define INLET_MOTOR_Pin GPIO_PIN_6
#define INLET_MOTOR_GPIO_Port GPIOB
#define IR_LEVEL_SNR_Pin GPIO_PIN_7
#define IR_LEVEL_SNR_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
volatile uint8_t rLastState;
volatile uint8_t rStateClk;
volatile uint8_t rStateDt;
volatile uint16_t rCount;
//static uint16_t	rPreviousCount;
volatile uint16_t forward_callback;
volatile uint16_t reverse_callback;
volatile uint8_t turnsDetected;
volatile uint8_t Sw_detected;
volatile uint8_t down_detected;
///
volatile uint8_t cntButtonPressed;
//volatile uint8_t cntButtonPressed;
volatile uint8_t displayProcessCnt;
volatile uint8_t ResetButtonPressed;
///
volatile uint8_t ok_detected;
volatile uint8_t up_detected;
char temp_buff[200];
char disp_num_buf[10];
char temp_buff1[50];
char temp_num_buf[10];
volatile uint16_t lastCount;
volatile uint16_t direction;

/*volatile uint32_t CountSecondsT1;
volatile uint32_t CountSecondsT2;
volatile uint32_t CountSecondsT3;*/
uint16_t timeout;
uint32_t CookingTimeCompleted;
uint8_t CheckDoubleDip;
uint8_t CheckHoldingTime;
uint8_t SecondCookingTime;

uint32_t BasketTime;

uint8_t recipieCount;
//typedef struct
//{
//	volatile uint8_t upma_state;
//	volatile uint8_t kesari_state;
//	volatile uint8_t poha_state;
//}__DISPANSE_STATE;

typedef struct
{
//	volatile uint16_t rotaryOk;
	volatile uint16_t OK;
	volatile uint16_t UP;
//	uint8_t rotaryOk:1;
	volatile uint16_t DOWN;
	volatile uint16_t PROCESS_CNT;
	volatile uint16_t EEPROM_RESET;
}__KEYS__;


typedef struct
{
	volatile uint8_t upFlag:1;
	volatile uint8_t okFlag:1;
	volatile uint8_t downFlag:1;
	volatile uint8_t rotaryFlag:1;
	volatile uint8_t stopPressed:1;
	volatile uint8_t upDownPressed:1;
	volatile uint8_t resetFlag:1;
	volatile uint8_t UpPressed:1;
}__KEY_FLAGS__;


typedef enum
{
	upDirection = 1,
	downDirection
}__MOTOR_DIRECTION_;

typedef struct
{
	volatile uint8_t upma_canister;
	volatile uint8_t poha_canister;
	volatile uint8_t kesari_canister;
}__CANISTER_CHECK;

typedef union COUNT__
{
	unsigned char char_[4];
	uint32_t _4bytes;

}COUN_T;
void driveMotor(__MOTOR_DIRECTION_ direction);

int intToStr(int x,char str[],int d);
void reverse2(char *str,int len);

int intToStrIndex(int x,char str[],int d);
void STEPPER_ROTATE(unsigned int RPM, unsigned int Step, uint8_t Direction, uint8_t MotorSel);
void StopAllMotors();
void DISPANSE_UPMA();
void DISPANSE_KESARI();
void DISPANSE_POHA();

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
