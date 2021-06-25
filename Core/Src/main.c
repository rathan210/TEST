/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "eeprom.h"
#include "menu.h"
#include "rotary.h"
#include "macro.h"
#include "display.h"
#include "variables.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FULL_STEP        1.8
#define HALF_STEP        0.9
#define ONE_BY_FOURTH    0.45
#define period           100
#define ONE_BY_EIGHT     0.225
#define Timer_clock      42000000
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim15;

/* USER CODE BEGIN PV */
struct Menu* head = NULL;
//struct Menu* previousMenu = NULL;
uint8_t Txbuf[3];
uint8_t buf[5];
uint32_t count_sve[50];
/*const char menu_1[] = "MENU 1";
const char menu_2[] = "MENU 2";
const char menu_3[] = "MENU 3";
const char menu_4[] = "MENU 4";
const char menu_5[] = "MENU 5";
const char menu_6[] = "MENU 6";
const char menu_7[] = "MENU 7";
const char menu_8[] = "MENU 8";*/
volatile uint16_t temperature;
volatile uint8_t uart_RX;
volatile uint8_t level_f=0;
uint8_t uart_buff[70];
uint8_t uart_count;
uint8_t uartReceivedFlag;
uint8_t backKeyValue;
uint8_t pin =0;
__esp_queue espQueue;
//IWDG_HandleTypeDef hiwdg;
CIRCQUEUE_DEF(espQueue,10);
//__ESP_UPDATE espBuf[20];
//extern __homeScreenPage machineState;
extern __homeScreenPage machineState;
extern __homeScreenPage currentMachineState;

extern const uint8_t logo[];
extern const uint8_t logo_200_200[];
extern const uint8_t logo_100_100[];
extern const uint8_t menuParameters[][26];

uint8_t eepromEspBuf[128];
uint32_t stopDelay;
uint8_t stopCount;

uint8_t stopValue;

uint8_t espDataReceivedFlag;

TM_RE_t RE1_Data;

//__DISPANSE_STATE dispanse_state;
__KEYS__ switchKeys;
__KEY_FLAGS__ keyFlags;
__CANISTER_CHECK canister_check;

extern __Sleep_Settings sleepSettings;
extern __Sleep_Flags  sleepFlag;
extern __Cooking_Settings cookingSettings;
extern __PrintDisplayState printState;
extern __OIL_HEAT_SETTINGS oilHeatSettings;
extern __MANUAL_ manualParameters;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM15_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void spi_send_Data(void);
void relay_control(void);
void checkRotaryState(void);
void Switch(void);
void processingState(void);
void displayProcess(void);
void user_printf(const char * format, ...);
void SystemInitilization(void);
void CheckStopDetected(void);
void GetRotaryValue(void);

uint8_t basketDown;
uint8_t basketUp;
uint32_t downDelay;
uint32_t upDelay;
void basketUpDown(void);
__MOTOR_DIRECTION_ previousDirection;

uint16_t prevrcount = 0;

uint16_t rcount;


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint16_t Temper;
extern volatile uint16_t Temperature_value;
uint16_t temp_value = 0,temp_val;

unsigned int TIM1_Steps,TIM3_Steps;
volatile float temp1,temp2;
volatile int rpm,prescaler,frequency;
uint16_t RPM_COUNT;
float step_angle=0;
uint8_t Chamber1_flag=0,Chamber2_flag=0,Chamber3_flag=0,Chamber4_flag=0,
		TIM3_flag=0,TIM1_flag=0;
uint8_t uartRecev;
uint8_t m_selet;
uint32_t count_sve[50];
char eeprom_buff_write[20]="evbsysgg";
char eepromReadBuff[20];
//uint8_t A_flag=0,B_flag=0,C_flag=0,D_flag=0,E_flag=0,F_flag=0,G_flag=0,H_flag=0,P_flag=0,PROX_GUIDEWAY1=0,PROX_GUIDEWAY2=0,PROX_CLEANING=0,PROX_MIXING=0;
enum MOTORS
{
	CHAMBER1=1,
	CHAMBER2,
	CHAMBER3,
	CHAMBER4,
//	stop=0xff

};

enum STEPPER_DIR{

	ANTICLOCKWISE=1,
	CLOCKWISE
};
uint8_t upma_state=0;
uint8_t kesari_state=0;
uint8_t poha_state=0;

/*enum MENU_SEL{

	upma_sel=1,
	kesari_sel,
	poha_sel,
};*/

int size__of_dispaly_par()
{
	return(sizeof(disp_param)/sizeof(struct DISP__));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM15_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

//  user_printf("System Initialised\r\n");



  //SystemInitilization();

  HAL_TIM_Base_Start_IT(&htim3);
//  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim15);
//  HAL_I2C_Master_Transmit(&hi2c2, TEMP_ADDR, i2c_buf, 1, 10);
//  HAL_I2C_Master_Receive(&hi2c2, TEMP_ADDR, i2c_buf, 2, 10);

  init();
  eeprom_init();
//  STEPPER_ROTATE(100, 4000, 1, CHAMBER2);
//  while(Chamber2_flag);
//  HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
//  HAL_Delay(1000);
//  STEPPER_ROTATE(100, 2200, 1, CHAMBER2);
//   while(Chamber2_flag);
//   //HAL_Delay(1000);
//  STEPPER_ROTATE(100, 1200, 1, CHAMBER4);
//  while(Chamber4_flag);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  TM_RE_Get(&RE1_Data);
//	  GetRotaryValue();


	  MachineProcess();
	  switchDetected();
	  DisplayPrint();
	  ReadTemperature(&Temperature_value);
//	  Temper=GetempData(&temperature);

//	  if(espDataReceivedFlag == 1)
//	  {
//		  addMenuUart(&head,eepromEspBuf);
//
//		  espDataReceivedFlag = 0;
//	  }
//	  HAL_IWDG_Refresh(&hiwdg);


//	  basketUpDown();

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 99;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4799;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 999;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 959;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, STRPPER_5_EN_Pin|STEPPER_5_PUL_Pin|EX_FAN_Pin|ILI9341_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SSR_RELAY_Pin|STEPPER_2_PUL_Pin|STEPPER_2_EN_Pin|Temp_CS_Pin
                          |STEPPER_4_PUL_Pin|STEPPER_4_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, ILI9341_CS_Pin|STEPPER_1_EN_Pin|STEPPER_1_PUL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ILI9341_RES_Pin|STEPPER_3_EN_Pin|STEPPER_3_PUL_Pin|INLET_MOTOR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : STRPPER_5_EN_Pin STEPPER_5_PUL_Pin EX_FAN_Pin ILI9341_DC_Pin */
  GPIO_InitStruct.Pin = STRPPER_5_EN_Pin|STEPPER_5_PUL_Pin|EX_FAN_Pin|ILI9341_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SSR_RELAY_Pin STEPPER_2_PUL_Pin STEPPER_2_EN_Pin Temp_CS_Pin
                           STEPPER_4_PUL_Pin STEPPER_4_EN_Pin */
  GPIO_InitStruct.Pin = SSR_RELAY_Pin|STEPPER_2_PUL_Pin|STEPPER_2_EN_Pin|Temp_CS_Pin
                          |STEPPER_4_PUL_Pin|STEPPER_4_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ILI9341_CS_Pin STEPPER_1_EN_Pin STEPPER_1_PUL_Pin */
  GPIO_InitStruct.Pin = ILI9341_CS_Pin|STEPPER_1_EN_Pin|STEPPER_1_PUL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : ILI9341_RES_Pin STEPPER_3_EN_Pin STEPPER_3_PUL_Pin INLET_MOTOR_Pin */
  GPIO_InitStruct.Pin = ILI9341_RES_Pin|STEPPER_3_EN_Pin|STEPPER_3_PUL_Pin|INLET_MOTOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : UP_DIRECTION_Pin DOWN_DIRECTION_Pin */
  GPIO_InitStruct.Pin = UP_DIRECTION_Pin|DOWN_DIRECTION_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : OK_PIN_Pin */
  GPIO_InitStruct.Pin = OK_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OK_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PROX_UPMA_Pin PROX_POHA_Pin PROX_KESARI_Pin */
  GPIO_InitStruct.Pin = PROX_UPMA_Pin|PROX_POHA_Pin|PROX_KESARI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : IR_LEVEL_SNR_Pin */
  GPIO_InitStruct.Pin = IR_LEVEL_SNR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(IR_LEVEL_SNR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void init(void) {
//	HAL_Delay(100);

	HAL_GPIO_WritePin(INLET_MOTOR_GPIO_Port, INLET_MOTOR_Pin, RESET);
    ILI9341_Unselect();

    HAL_Delay(100);
    //ILI9341_TouchUnselect();
    ILI9341_Init();

    HAL_Delay(100);

    //ILI9341_FillScreen(ILI9341_WHITE);

    //HAL_Delay(10);

    //ILI9341_DrawImage((ILI9341_WIDTH - 200) / 2, (ILI9341_HEIGHT - 200) / 2, 100, 100, (const uint8_t*)logo_100_100);

    //HAL_Delay(3000);

    ILI9341_FillScreen(ILI9341_WHITE);

    HAL_Delay(100);
    ILI9341_WriteString1(100, 12*10, "DEHYDRATOR", ILI9341_BLACK, ILI9341_WHITE);

    HAL_Delay(100);
}




/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		uart_buff[uart_count] = (uint8_t)uart_RX;
		uart_count++;
		if(uart_buff[63] == 0x12)
		{
			uart_count = 0;
//			FryerState = updateMenu;
			espDataReceivedFlag = 1;

		}

		 HAL_UART_Receive_IT(&huart2,&uart_RX, 1);
	}
}
*/

uint8_t pushESPData(__esp_queue * qptr,uint8_t data[])
{
	uint8_t next = qptr->head + 1;



	memcpy(eepromEspBuf,data,32);

	if(next >= qptr->maxLen)
		next = 0;

	if(next == qptr->tail)
		return -1;

	ee24_write(ESP_MENU_EEPROM_START(qptr->head),eepromEspBuf,8,500);

	ee24_write(ESP_MENU_EEPROM_START(qptr->head)+ 8,&eepromEspBuf[8],8,500);

	ee24_write(ESP_MENU_EEPROM_START(qptr->head)+ 16,&eepromEspBuf[16],8,500);

	ee24_write(ESP_MENU_EEPROM_START(qptr->head)+24,&eepromEspBuf[24],8,500);


	qptr->head = next;

	ee24_write(ESP_EEPROM_HEAD,&next,1,200);

	memset(uart_buff,0,sizeof(uart_buff));

	return 0;
}


uint8_t updateESPData(__esp_queue * qptr)
{
    if((machineState == MachineIdle))
    {
    	if(qptr->head == qptr->tail)
    			return -1;

    	uint8_t next = 0;
    	while(qptr->tail != qptr->head)
    	{

        	ee24_read(ESP_MENU_EEPROM_START(qptr->tail),eepromEspBuf,32,200);

//        	addMenuUart(&head,eepromEspBuf);

        	memset(eepromEspBuf,0,sizeof(eepromEspBuf));

        	next= qptr->tail+1;

        	if(next >= qptr->maxLen)
        		next = 0;

        	qptr->tail = next;
    	}

    	ee24_write(ESP_EEPROM_TAIL,&next,1,200);
//    	getMenu(&head);
    }

    return 0;
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM3)
	{

		if(HAL_GPIO_ReadPin(IR_LEVEL_SNR_GPIO_Port, IR_LEVEL_SNR_Pin) == 0)
		 {
			level_f=0;
		  HAL_GPIO_WritePin(INLET_MOTOR_GPIO_Port, INLET_MOTOR_Pin, RESET);
		 }
		else if(HAL_GPIO_ReadPin(IR_LEVEL_SNR_GPIO_Port, IR_LEVEL_SNR_Pin) == 1)
		 {
			level_f=1;
			 HAL_GPIO_WritePin(INLET_MOTOR_GPIO_Port, INLET_MOTOR_Pin, SET);
		 }

		canister_check.upma_canister = (HAL_GPIO_ReadPin(PROX_UPMA_GPIO_Port, PROX_UPMA_Pin) == RESET)?0:1;
		canister_check.poha_canister = (HAL_GPIO_ReadPin(PROX_POHA_GPIO_Port, PROX_POHA_Pin) == RESET)?0:1;
		canister_check.kesari_canister = (HAL_GPIO_ReadPin(PROX_KESARI_GPIO_Port, PROX_KESARI_Pin) == RESET)?0:1;

	}
//	machineState=water_temp_check;

if(Temperature_value > 70){
	HAL_GPIO_WritePin(SSR_RELAY_GPIO_Port, SSR_RELAY_Pin, RESET);
//	machineState=water_heated;
}
else if(Temperature_value < 70)
{
//	machineState=water_heating;
	HAL_GPIO_WritePin(SSR_RELAY_GPIO_Port, SSR_RELAY_Pin, SET);
}

	if(htim->Instance == TIM15)
	{
			if((HAL_GPIO_ReadPin(DOWN_DIRECTION_GPIO_Port,DOWN_DIRECTION_Pin) == GPIO_PIN_RESET) && HAL_GPIO_ReadPin(UP_DIRECTION_GPIO_Port,UP_DIRECTION_Pin) == GPIO_PIN_RESET)
			{
				switchKeys.PROCESS_CNT++;
				cntButtonPressed = 1;
				if(switchKeys.PROCESS_CNT >=  50)
				{
					switchKeys.PROCESS_CNT = 0;
					keyFlags.upDownPressed = 1;
					displayProcessCnt = 1;
					machineState = displayCnt;
				}
			}
			else
			{
				switchKeys.PROCESS_CNT = 0;
				keyFlags.upDownPressed = 0;
				displayProcessCnt = 0;
				cntButtonPressed = 0;
			}
			if((HAL_GPIO_ReadPin(UP_DIRECTION_GPIO_Port,UP_DIRECTION_Pin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(OK_PIN_GPIO_Port,OK_PIN_Pin)  == GPIO_PIN_RESET))
			{
				switchKeys.EEPROM_RESET++;
				ResetButtonPressed = 1;
				if(switchKeys.EEPROM_RESET >=  100)
				{
					switchKeys.EEPROM_RESET = 0;
					keyFlags.resetFlag = 1;
//					displayProcessCnt = 1;
					machineState = reset_eeprom;
				}
			}
			else
			{
				switchKeys.EEPROM_RESET = 0;
				keyFlags.resetFlag = 0;
//				displayProcessCnt = 0;
				ResetButtonPressed = 0;
			}
			if(HAL_GPIO_ReadPin(DOWN_DIRECTION_GPIO_Port,DOWN_DIRECTION_Pin) == GPIO_PIN_RESET)
			{
				switchKeys.DOWN++;

				if((switchKeys.DOWN >= 2) && (keyFlags.downFlag == 0) && (cntButtonPressed == 0))
				{
					down_detected = 1;
					switchKeys.DOWN = 0;
					keyFlags.downFlag = 1;
					displayProcessCnt = 0;
				}
			}
			else
			{
				switchKeys.DOWN = 0;
				keyFlags.downFlag = 0;
			}

			if(HAL_GPIO_ReadPin(OK_PIN_GPIO_Port,OK_PIN_Pin)  == GPIO_PIN_RESET)
			{
				switchKeys.OK++;

				if((switchKeys.OK >= 2) && (keyFlags.okFlag == 0)  && (ResetButtonPressed == 0))
				{
					keyFlags.okFlag = 1;
					ok_detected = 1;
					switchKeys.OK = 0;
					displayProcessCnt = 0;
				}
			}
			else
			{
				keyFlags.okFlag = 0;
				switchKeys.OK = 0;
			}


			if(HAL_GPIO_ReadPin(UP_DIRECTION_GPIO_Port,UP_DIRECTION_Pin) == GPIO_PIN_RESET)
			{
				switchKeys.UP++;

				if((switchKeys.UP >= 2) && (keyFlags.upFlag == 0) && (cntButtonPressed == 0) && (ResetButtonPressed == 0))
				{
					keyFlags.upFlag = 1;
					switchKeys.UP = 0;
					up_detected = 1;
					displayProcessCnt = 0;
				}
			}
			else
			{
				keyFlags.upFlag = 0;
				switchKeys.UP = 0;
			}
//		}
	}
}

#if 0
void user_printf(const char * format, ...)
{
	va_list arg;
	uint16_t n;
   char buf[100];

   va_start (arg, format);
   n = vsprintf (buf, format,arg);
   HAL_UART_Transmit(&huart2,(uint8_t*)buf,n,100);
   va_end (arg);

}


void SystemInitilization(void)
{
//	ILI9341_WriteString1(100, 12*10, "11", ILI9341_BLACK, ILI9341_WHITE);
	  rCount = 0;
	  uint8_t dummy[15] = "\0";
	  uint8_t zero = 0;


	  ee24_read(MENU_COUNT,dummy,9,200);


	  recipieCount = dummy[0];



	  if((dummy[1] == 255) || (dummy[1] == 0))
		  oilHeatSettings.preHeatSetTemp = PRE_HEAT_CUTOFF_MIN;
	  else
		  oilHeatSettings.preHeatSetTemp = dummy[1];

	  if((dummy[2] == 255) || (dummy[2] == 0))
		  oilHeatSettings.preHeatCutTemp = PRE_HEAT_CUTOFF_MAX;
	  else
		  oilHeatSettings.preHeatCutTemp = dummy[2];


	  if((dummy[3] == 255) || (dummy[3] == 0))
		  oilHeatSettings.sleepHeatSet = SLEEP_OIL_TEMP_MIN;
	  else
		  oilHeatSettings.sleepHeatSet = dummy[3];

	  if((dummy[4] == 255) || (dummy[4] == 0))
		  oilHeatSettings.sleepHeatCutoff = SLEEP_OIL_TEMP_MAX;
	  else
		  oilHeatSettings.sleepHeatCutoff = dummy[4];

/*	  if((dummy[5] == 255) || (dummy[5] == 0))
		  sleepSettings.sleepHour = 1;
	  else
		  sleepSettings.sleepHour = dummy[5];*/

	  if((dummy[6] == 255) || (dummy[6] == 0))
		  sleepSettings.sleepMinutes = 20;
	  else
		  sleepSettings.sleepMinutes = dummy[6];

/*	  if((dummy[7] == 255) || (dummy[7] == 0))
		  sleepSettings.deepSleepHour = 1;
	  else
		  sleepSettings.deepSleepHour = dummy[7];*/

	  if((dummy[8] == 255) || (dummy[8] == 0))
		  sleepSettings.deepSleepMin = 20;
	  else
		  sleepSettings.deepSleepMin = dummy[8];


	  sleepSettings.sleepTimeout = MIN_TO_SEC((sleepSettings.sleepMinutes));

	  sleepSettings.deepSleepTimeout = MIN_TO_SEC((sleepSettings.deepSleepMin));


	  if((recipieCount == 255) || (recipieCount == 0))
	  {
		  recipieCount = 0;

		  addMenu(&head,menu_1,menuParameters[0]);
		  addMenu(&head,menu_2,menuParameters[1]);
		  addMenu(&head,menu_3,menuParameters[2]);
		  addMenu(&head,menu_4,menuParameters[3]);
		  addMenu(&head,menu_5,menuParameters[4]);
		  addMenu(&head,menu_6,menuParameters[5]);
		  addMenu(&head,menu_7,menuParameters[6]);
		  addMenu(&head,menu_8,menuParameters[7]);

		    ee24_write(COMPLETE_RECIPE_COUNT,&zero,1,100);
		    ee24_write(COMPLETE_RECIPE_COUNT+1,&zero,1,100);
		    ee24_write(COMPLETE_RECIPE_COUNT+2,&zero,1,100);
		    ee24_write(COMPLETE_RECIPE_COUNT+3,&zero,1,100);

		    ee24_write(ESP_EEPROM_HEAD,&zero,1,200);
		    ee24_write(ESP_EEPROM_TAIL,&zero,1,200);
	  }
	  else
	  {

		  getMenu(&head);

//		  ILI9341_FillScreenOffset(ILI9341_WHITE);
	  }
//	previousMenu = head;
//	HAL_GPIO_WritePin(relay_4_GPIO_Port, relay_4_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);


	ee24_read(ESP_EEPROM_HEAD,&espQueue.head,1,200);
	if((espQueue.head == 255) || (espQueue.head == 0))
	{
		espQueue.head = 0;
	}

	ee24_read(ESP_EEPROM_TAIL,&espQueue.tail,1,200);

	if((espQueue.tail == 255) || (espQueue.tail == 0))
	{
		espQueue.tail = 0;
	}

	  //TM_RE_Init(&RE1_Data, GPIOA, rotary_clk_Pin, GPIOA, rotary_dt_Pin);

	  /* Set rotation mode for rotary 1 */
	  //TM_RE_SetMode(&RE1_Data, TM_RE_Mode_One);


	  ee24_read(MANUAL_MIN_TEMP,dummy,2,200);

		if((dummy[0] == 255) || (dummy[0] == 0))
		{
			manualParameters.manualSetTemp = 170;
		}
		else
			manualParameters.manualSetTemp = dummy[0];

		if((dummy[1] == 255) || (dummy[1] == 0))
		{
			manualParameters.manualCuttOffTemp = 175;
		}
		else
			manualParameters.manualCuttOffTemp = dummy[1];

//		ILI9341_WriteString1(100, 12*10, "xxxxxxxxxxxxxx", ILI9341_BLACK, ILI9341_WHITE);
	sleepFlag.countEnabled = 1;



}
#endif

/*
void driveMotor(__MOTOR_DIRECTION_ direction)
{

//	if(previousDirection != direction)
//	{
		if((direction == upDirection) && (HAL_GPIO_ReadPin(m_forward_switch_GPIO_Port,m_forward_switch_Pin) != GPIO_PIN_RESET))
		{
			HAL_GPIO_WritePin(motor_n1_GPIO_Port, motor_n1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(motor_n2_GPIO_Port, motor_n2_Pin, GPIO_PIN_RESET);
		}

		if((direction == downDirection) && (HAL_GPIO_ReadPin(m_reverse_switch_GPIO_Port,m_reverse_switch_Pin) != GPIO_PIN_RESET))
		{
			HAL_GPIO_WritePin(motor_n2_GPIO_Port, motor_n2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(motor_n1_GPIO_Port, motor_n1_Pin, GPIO_PIN_RESET);
		}
//	}
	previousDirection = direction;
}
*/



void basketUpDown(void)
{
	if((HAL_GetTick()-downDelay >= 5000) && (basketDown == 0))
	{
		upDelay=HAL_GetTick();
//		downDelay=HAL_GetTick();

		basketDown = 1;
		basketUp = 0;

		ILI9341_WriteString1(100, 12*10, "DOWN", ILI9341_BLACK, ILI9341_WHITE);

//		HAL_GPIO_WritePin(relay_1_GPIO_Port, relay_1_Pin, GPIO_PIN_SET);

		driveMotor(downDirection);

	}

	if((HAL_GetTick()-upDelay >= 5000) && (basketUp == 0))
	{
		downDelay=HAL_GetTick();
//		upDelay=HAL_GetTick();

		basketUp = 1;
		basketDown = 0;

		ILI9341_WriteString1(100, 12*10, "UP    ", ILI9341_BLACK, ILI9341_WHITE);

//		HAL_GPIO_WritePin(relay_1_GPIO_Port, relay_1_Pin, GPIO_PIN_RESET);
		driveMotor(upDirection);
	}
}

void reverse2(char *str,int len)
{
    int i = 0,j = len-1,temp;

    while(i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;j--;
    }
}

int intToStr(int x,char str[],int d)
{
    int i = 0;

    while(x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    while(i < d)
        str[i++] = '0';

    reverse2(str,i);
    str[i] = '\0';
    return i;
}

int intToStrIndex(int x,char str[],int d)
{
    int i = 0;

    while(x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    while(i < d)
        str[i++] = '0';

    reverse2(str,i);
    str[i] = '.';
    str[i+1] = ' ';
    i++;
    return i;
}


void GetRotaryValue(void)
{
	  if ((TIM1->CNT > 60000) || (TIM1->CNT <= 0))
	  {
		  TIM1->CNT = 65536/2;
	  }

	  if (prevrcount > rcount)
	  {
		  turnsDetected = 1;
		  direction = 2;
		  prevrcount = rcount;
	  }
	  else if (prevrcount < rcount)
	  {
		  turnsDetected = 1;
		  direction = 1;
		  prevrcount = rcount;
	  }
	  else
	  {

	  }

	  rcount = TIM1->CNT;
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
/*void MX_IWDG_Init(void)
{

   USER CODE BEGIN IWDG_Init 0

   USER CODE END IWDG_Init 0

   USER CODE BEGIN IWDG_Init 1

   USER CODE END IWDG_Init 1
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
   USER CODE BEGIN IWDG_Init 2

   USER CODE END IWDG_Init 2

}*/

void DISPANSE_UPMA()
{
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
	STEPPER_ROTATE(300, 11800, 1, CHAMBER4);//160ml
	while(Chamber4_flag);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , RESET);
	STEPPER_ROTATE(100, 10500, 1, CHAMBER1);
	while(Chamber1_flag);
	   //HAL_Delay(1000);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
	STEPPER_ROTATE(300, 11800, 1, CHAMBER4);
	while(Chamber4_flag);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , RESET);


}
void DISPANSE_KESARI()
{
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
	STEPPER_ROTATE(300, 4950, 1, CHAMBER4);
	while(Chamber4_flag);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , RESET);
	STEPPER_ROTATE(100, 9250, 1, CHAMBER2);
	while(Chamber2_flag);
	   //HAL_Delay(1000);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
	STEPPER_ROTATE(300, 4950, 1, CHAMBER4);
	while(Chamber4_flag);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , RESET);


}
void DISPANSE_POHA()
{
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
	STEPPER_ROTATE(300, 7130, 1, CHAMBER4);
	while(Chamber4_flag);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , RESET);
	STEPPER_ROTATE(100, 11400, 1, CHAMBER3);
    while(Chamber3_flag);
     //HAL_Delay(1000);
    HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , SET);
	STEPPER_ROTATE(300, 7130, 1, CHAMBER4);
	while(Chamber4_flag);
	HAL_GPIO_WritePin(EX_FAN_GPIO_Port, EX_FAN_Pin , RESET);


}
void STEPPER_ROTATE(unsigned int RPM, unsigned int Step, uint8_t Direction, uint8_t MotorSel)
{

	switch(MotorSel){

	case CHAMBER1   : HAL_GPIO_WritePin(STEPPER_1_EN_GPIO_Port, STEPPER_1_EN_Pin, 1);
//	if(Direction == CLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER1_MOTOR_DIR_GPIO_Port, CHAMBER1_MOTOR_DIR_Pin, 1);
//	else if(Direction == ANTICLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER1_MOTOR_DIR_GPIO_Port, CHAMBER1_MOTOR_DIR_Pin, 0);
	Chamber1_flag=1;

	break;
	case CHAMBER2   : HAL_GPIO_WritePin(STEPPER_2_EN_GPIO_Port, STEPPER_2_EN_Pin, 1);
//	if(Direction == CLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER2_MOTOR_DIR_GPIO_Port, CHAMBER2_MOTOR_DIR_Pin, 1);
//	else if(Direction == ANTICLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER2_MOTOR_DIR_GPIO_Port, CHAMBER2_MOTOR_DIR_Pin, 0);
	Chamber2_flag=1;

	break;

	case CHAMBER3 : HAL_GPIO_WritePin(STEPPER_3_EN_GPIO_Port, STEPPER_3_EN_Pin, 1);
//	if(Direction == CLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER3_MOTOR_DIR_GPIO_Port, CHAMBER3_MOTOR_DIR_Pin, 1);
//	else if(Direction == ANTICLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER3_MOTOR_DIR_GPIO_Port, CHAMBER3_MOTOR_DIR_Pin, 0);
	Chamber3_flag=1;

	break;

	case CHAMBER4 : HAL_GPIO_WritePin(STEPPER_4_EN_GPIO_Port, STEPPER_4_EN_Pin, 1);
//	if(Direction == CLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER4_MOTOR_DIR_GPIO_Port, CHAMBER4_MOTOR_DIR_Pin, 1);
//	else if(Direction == ANTICLOCKWISE)
//		HAL_GPIO_WritePin(CHAMBER4_MOTOR_DIR_GPIO_Port, CHAMBER4_MOTOR_DIR_Pin, 0);
	Chamber4_flag=1;

	break;
	default : break;
	}

	step_angle=HALF_STEP;

		TIM1_Steps=Step*2;
		TIM1_flag=0;


		HAL_TIM_Base_Start_IT(&htim1);
		if(RPM>=300){
			for(RPM_COUNT=1;RPM_COUNT<RPM;RPM_COUNT++){

				temp1=(RPM_COUNT*360)/(step_angle*60);
				temp1=temp1*2;
				frequency=(int)temp1;
				prescaler=(int)Timer_clock/(period*frequency);
				TIM1->CCR1=20;
				TIM1->ARR=(period-1);
				TIM1->PSC=(prescaler-1);
				HAL_Delay(1);

			}
		}
		temp1=(RPM*360)/(step_angle*60);
		temp1=temp1*2;
		frequency=(int)temp1;
		prescaler=(int)Timer_clock/(period*frequency);
		TIM1->CCR1=20;
		TIM1->ARR=(period-1);
		TIM1->PSC=(prescaler-1);

}

void StopAllMotors(){

	/*
	CC1_flag=0;CC2_flag=0;CAKE1_flag=0;CAKE2_flag=0;BP_flag=0,CRUMS_flag=0;TM_flag=0,CHEESE_flag=0;GUIDEWAY_flag=0;
	CLEANING_flag=0;CLEANING2_flag=0;MIXING_flag=0;MIXING2_flag=0;SAUCE_A_flag=0;SAUCE_B_flag=0;WATER_BOILER_flag=0;
	*/

	if(Chamber1_flag==1){
		Chamber1_flag=0;
		HAL_GPIO_WritePin(STEPPER_1_EN_GPIO_Port, STEPPER_1_EN_Pin, 0);
		HAL_GPIO_WritePin(STEPPER_1_PUL_GPIO_Port, STEPPER_1_PUL_Pin, 0);   // Pulse pin since EN is always high
	}
	if(Chamber2_flag==1){
		Chamber2_flag=0;
		HAL_GPIO_WritePin(STEPPER_2_EN_GPIO_Port, STEPPER_2_EN_Pin, 0);
		HAL_GPIO_WritePin(STEPPER_2_PUL_GPIO_Port, STEPPER_2_PUL_Pin, 0);   // Pulse pin since EN is always high
	}
	if(Chamber3_flag==1){
		Chamber3_flag=0;
		HAL_GPIO_WritePin(STEPPER_3_EN_GPIO_Port, STEPPER_3_EN_Pin, 0);
		HAL_GPIO_WritePin(STEPPER_3_PUL_GPIO_Port, STEPPER_3_PUL_Pin, 0);   // Pulse pin since EN is always high
	}
	if(Chamber4_flag==1){
		Chamber4_flag=0;
		HAL_GPIO_WritePin(STEPPER_4_EN_GPIO_Port, STEPPER_4_EN_Pin, 0);
		HAL_GPIO_WritePin(STEPPER_4_PUL_GPIO_Port, STEPPER_4_PUL_Pin, 0);   // Pulse pin since EN is always high
		}

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
