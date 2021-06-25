#include "MAX_31855.h"

/************************************************************
 * Function: void PrintTempData(void)
 *
 * @brief:	 This Function is to be called in main loop for printing
 * 		   	 the temperature value
 *
 * @Note: 	 This function calls the ReadTemperature(uint16 *)
 * 			 function which reads the temperature
 *
 * @Note: 	 user_printf() function template is given below
 *************************************************************/
#if 0
uint32_t TempDelay;

uint8_t errorScreenFlag,tempScreenFlag;

uint16_t pTemp;
uint16_t pTemp_1;



void GetempData(void)
{

	if (HAL_GetTick()-TempDelay >= 100)
	{
		TempDelay=HAL_GetTick();
		status = ReadTemperature(&Temperature_value);

/*		if(status != TEMP_OK)
		{
			if(errorScreenFlag == 0)
			{
				errorScreenFlag = 1;
				tempScreenFlag = 0;
				ILI9341_FillScreenTemp(ILI9341_WHITE);
			}

			TempError(status);

		}
		else
		{*/
			if(tempScreenFlag == 0)
			{
				tempScreenFlag = 1;
				errorScreenFlag = 0;
				//ILI9341_FillScreenTemp(ILI9341_WHITE);
			}
			//sprintf(temp_buff1,"T: %03d",Temperature_value);
//			ILI9341_WriteString(K_TYPE_TEMP_VALUE,K_TYPE_Y_AXIS,temp_buff1, Font_11x18, ILI9341_WHITE, ILI9341_WHITE);
			//ILI9341_WriteString1(K_TYPE_TEMP_VALUE,K_TYPE_Y_AXIS,temp_buff1, ILI9341_BLACK, ILI9341_WHITE);
//		}
	}
/*	else
	{
		sprintf(temp_buff,"Temp - %d",Temperature_value);
		ILI9341_WriteString(0, 3*10,temp_buff, Font_11x18, ILI9341_GREEN, ILI9341_WHITE);
	}*/
}

/************************************************************
 * Function: void TempError(Temp_status error)
 *
 * @brief:	 Function is called by PrintTempData() to print
 * 		   	 the thermocouple Fault condition
 *************************************************************/
void TempError(Temp_status error)
{
	if(error == TEMP_SCV_FAULT) /*thermocouple is short circuited to VCC*/
	{
//		ILI9341_WriteString(ERROR_OFFSET,K_TYPE_Y_AXIS,"T:SC-VCC", Font_11x18, ILI9341_WHITE, ILI9341_WHITE);
		//ILI9341_WriteString1(ERROR_OFFSET,K_TYPE_Y_AXIS,"T:SC-VCC", ILI9341_BLACK, ILI9341_WHITE);
	}
	else if(error == TEMP_SCG_FAULT) /*thermocouple is short circuited to GND*/
	{
		//ILI9341_WriteString(ERROR_OFFSET, K_TYPE_Y_AXIS,"T:SC-GND", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	}
	else if(error == TEMP_OC_FAULT) /*thermocouple is open*/
	{
		//ILI9341_WriteString(ERROR_OFFSET,K_TYPE_Y_AXIS,"T:Open  ", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	}
}


/**************************************************************
 * Function: Temp_status ReadTemperature(uint16_t *temp)
 *
 * @brief:	 Function is called by PrintTempData() to capture
 * 		   	 the thermocouple temperature and Fault condition
 *
 * @Note: 	 Update the HAL_GPIO_WritePin() function with
 * 			 the port value and pin value to which the SPI
 * 			 chip select  is connected
 *
 * @Note: 	 SPI pheripheral to be configured with
 * 			 Mode      - Receive Only Master
 * 			 Data Size - 8Bits
 * 			 CPOL 	   - LOW
 * 			 CPHA 	   - 1 Edge
 * 			 and pass the appropriate SPI handle structure
 *
 * @Retval:  Temp_status value is returned
 *************************************************************/
Temp_status ReadTemperature(uint16_t *temp)
{
	uint8_t Temp_buff_1[5] = {"\0"};

	Temp_status status = TEMP_OK;

	/*Update this function with the port value and pin value
		to which the SPI chip select is connected*/
//	HAL_GPIO_WritePin(K_SEL_GPIO_Port, K_SEL_Pin, RESET);
	HAL_GPIO_WritePin(Temp_CS_GPIO_Port, Temp_CS_Pin, SET);
 	HAL_GPIO_WritePin(Temp_CS_GPIO_Port, Temp_CS_Pin, RESET);

	HAL_Delay(50);

	/*Configure with the appropriate SPI handle structure*/

	HAL_SPI_Receive(&hspi1,Temp_buff_1,sizeof(Temp_buff_1),HAL_MAX_DELAY);

	*temp = Temp_buff_1[0] << 8;

	*temp |= Temp_buff_1[1];

   HAL_GPIO_WritePin(Temp_CS_GPIO_Port, Temp_CS_Pin, SET);

	if(*temp & 0x1)
	{
		if(Temp_buff_1[3] & 0x1) /*Check to see if thermocouple is open*/
		{
			status = TEMP_OC_FAULT;
		}
		else if(Temp_buff_1[3] & 0x2) /*Check to see if thermocouple is short circuited to GND*/
		{
			status = TEMP_SCG_FAULT;
		}
		else if(Temp_buff_1[3] & 0x4)/*Check to see if thermocouple is short circuited to VCC*/
		{
			status = TEMP_SCV_FAULT;
		}
		*temp = pTemp;

	}
	else
	{
		*temp >>= 2;
		*temp *= 0.25; /*Temperature Resolution Calculation*/

		pTemp = *temp;
	}
	return status; /*R
	eturn the Status of the thermocouple*/

}

#endif



uint32_t TempDelay;

uint8_t errorScreenFlag,tempScreenFlag;

float pTemp = 0;

extern __homeScreenPage currentMachineState;


uint16_t GetempData()
{

//	if(currentMachineState != machineDeepSleep)
//	{
		if (HAL_GetTick()-TempDelay >= 500)
		{
			TempDelay = HAL_GetTick();
	//		status = ReadTemperature(&Temperature_value);
//			ReadTemperature();

			/*if(status != TEMP_OK)
			{
				if(errorScreenFlag == 0)
				{
					errorScreenFlag = 1;
					tempScreenFlag = 0;
					ILI9341_FillScreenTemp(ILI9341_WHITE);
				}

				TempError(status);

			}
			else
			{
				if(tempScreenFlag == 0)
				{
					tempScreenFlag = 1;
					errorScreenFlag = 0;
					ILI9341_FillScreenTemp(ILI9341_WHITE);
				}
	//			sprintf(temp_buff1,"T: %03d",Temperature_value);
				strcpy(temp_buff1,"T: ");
				intToStr(Temperature_value,temp_num_buf,3);
				strcat(temp_buff1,temp_num_buf);
	//			ILI9341_WriteString(K_TYPE_TEMP_VALUE,K_TYPE_Y_AXIS,temp_buff1, Font_11x18, ILI9341_WHITE, ILI9341_WHITE);
				ILI9341_WriteString1(K_TYPE_TEMP_VALUE,K_TYPE_Y_AXIS,temp_buff1, ILI9341_BLACK, ILI9341_WHITE);
			}*/
		}
	/*	else
		{
			sprintf(temp_buff,"Temp - %d",Temperature_value);
			ILI9341_WriteString(0, 3*10,temp_buff, Font_11x18, ILI9341_GREEN, ILI9341_WHITE);
		}*/
//	}

}

/************************************************************
 * Function: void TempError(Temp_status error)
 *
 * @brief:	 Function is called by PrintTempData() to print
 * 		   	 the thermocouple Fault condition
 *************************************************************/
void TempError(Temp_status error)
{
	if(error == TEMP_SCV_FAULT) /*thermocouple is short circuited to VCC*/
	{
		ILI9341_WriteString1(ERROR_OFFSET,K_TYPE_Y_AXIS,"T:SC-VCC", ILI9341_BLACK, ILI9341_WHITE);
	}
	else if(error == TEMP_SCG_FAULT) /*thermocouple is short circuited to GND*/
	{
		ILI9341_WriteString1(ERROR_OFFSET, K_TYPE_Y_AXIS,"T:SC-GND", ILI9341_BLACK, ILI9341_WHITE);
	}
	else if(error == TEMP_OC_FAULT) /*thermocouple is open*/
	{
		ILI9341_WriteString1(ERROR_OFFSET,K_TYPE_Y_AXIS,"T:Open  ", ILI9341_BLACK, ILI9341_WHITE);
	}
}


/**************************************************************
 * Function: Temp_status ReadTemperature(uint16_t *temp)
 *
 * @brief:	 Function is called by PrintTempData() to capture
 * 		   	 the thermocouple temperature and Fault condition
 *
 * @Note: 	 Update the HAL_GPIO_WritePin() function with
 * 			 the port value and pin value to which the SPI
 * 			 chip select is connected
 *
 * @Note: 	 SPI pheripheral to be configured with
 * 			 Mode      - Receive Only Master
 * 			 Data Size - 16Bits
 * 			 CPOL 	   - LOW
 * 			 CPHA 	   - 1 Edge
 * 			 and pass the appropriate SPI handle structure
 *
 * @Retval:  Temp_status value is returned
 *************************************************************/
Temp_status ReadTemperature(uint16_t *temp)
//Temp_status ReadTemperature(void)
{
	uint8_t Temp_buf[5] = {"\0"};

	Temp_status status = TEMP_OK;

	/*Update this function with the port value and pin value
		to which the SPI chip select is connected*/
	HAL_GPIO_WritePin(Temp_CS_GPIO_Port, Temp_CS_Pin, GPIO_PIN_RESET);

	HAL_Delay(5);

	/*Configure with the appropriate SPI handle structure*/
	HAL_SPI_Receive(&hspi1,Temp_buf,4,100);

	Temperature_value = Temp_buf[0] << 8;

	Temperature_value |= Temp_buf[1];

	HAL_GPIO_WritePin(Temp_CS_GPIO_Port, Temp_CS_Pin, GPIO_PIN_SET);

//	if(Temperature_value & 0x1)
//	{
//		if(Temp_buf[3] & 0x1) /*Check to see if thermocouple is open*/
//		{
//			status = TEMP_OC_FAULT;
//		}
//		else if(Temp_buf[3] & 0x2) /*Check to see if thermocouple is short circuited to GND*/
//		{
//			status = TEMP_SCG_FAULT;
//		}
//		else if(Temp_buf[3] & 0x4)/*Check to see if thermocouple is short circuited to VCC*/
//		{
//			status = TEMP_SCV_FAULT;
//		}
//		Temperature_value = pTemp;
//
//	}
//	else
//	{
		Temperature_value >>= 2;
		pTemp = Temperature_value;

		pTemp = pTemp * 0.25; /*Temperature Resolution Calculation*/

		Temperature_value = pTemp;
//	}


//	if(status != TEMP_OK)
//	{
//		if(errorScreenFlag == 0)
//		{
//			errorScreenFlag = 1;
//			tempScreenFlag = 0;
//			ILI9341_FillScreenTemp(ILI9341_WHITE);
//		}
//
//		TempError(status);

//	}
//	else
//	{
		if(tempScreenFlag == 0)
		{
			tempScreenFlag = 1;
			errorScreenFlag = 0;
			ILI9341_FillScreenTemp(ILI9341_WHITE);
		}
//			sprintf(temp_buff1,"T: %03d",Temperature_value);
		strcpy(temp_buff1,"T: ");
		intToStr(Temperature_value,temp_num_buf,3);
		strcat(temp_buff1,temp_num_buf);
//			ILI9341_WriteString(K_TYPE_TEMP_VALUE,K_TYPE_Y_AXIS,temp_buff1, Font_11x18, ILI9341_WHITE, ILI9341_WHITE);
		ILI9341_WriteString1(K_TYPE_TEMP_VALUE,K_TYPE_Y_AXIS,temp_buff1, ILI9341_BLACK, ILI9341_WHITE);
//	}



	return status; /*Return the Status of the thermocouple*/

}


