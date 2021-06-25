/*
 * menu_display.c
 *
 *  Created on: Jul 8, 2020
 *      Author: CEPL
 */

#include "menu_display.h"
#include "string.h"
#include "string.h"
#include "macro.h"
#include "x_display.h"
#include "eeprom.h"
#include "xvariables.h"
const char mainmenu[10][22] = {"VEGGIE UPMA","KHATTA METTHA POHA","SUJI HALWA"};
const char Settings[10][25] = {"1. Recipe Settings","2. Reset Count","3. Sleep Settings","4. Deep Sleep Settings","5. Pre-Heat Settings"};
char count_save[20][6];
extern uint32_t pohaCnt, upmaCnt, kesarCnt;
#define MAIN_MENU_AVAILABLE  3
#define SETTINGS_AVAILABLE	 5
extern uint8_t eeprom_buff_writeBuff[2];
extern uint8_t eeprom_buff_writeCnt[4];
extern uint8_t eeprom_buff_read[2];
extern uint8_t eeprom_buff_readCnt[4];
uint8_t cntBuff[4];
extern struct Menu* head;
//extern struct Menu* previousMenu;
extern TIM_HandleTypeDef htim3;

/*
struct Menu *CurrentMenu = NULL;
struct Menu *previous = NULL;
struct Menu* CookingMenuDetails = NULL;
struct Menu * Bold = NULL;*/


uint8_t menuNo,menuSelected,portionNoSelected;
uint8_t dummyEEprom[10];


uint8_t passwordNoOne,passwordNoTwo,passwordNoOneFlag,passwordNoTwoFlag;

uint8_t menuPageNo; //page No
uint8_t previousPageNo;
uint8_t pageStartNo;

uint8_t settingsTemp,settingsMaxCutoff,settingsMinCutoff;
uint16_t settingsTimeT1,settingsHoldingTim,settingsTimeT2;
__RecipieSettingsFlag settingsFlag;

uint8_t noOfSettingsAvailable;


__homeScreenPage machineState = printLogo;
__homeScreenPage previousMachineState = printLogo;
__homeScreenPage currentMachineState = printLogo;


__PrintDisplayState printState = printStateIdle;
//__PrintDisplayState previousPrintState = printHomeScreenInit;


__Sleep_Settings sleepSettings;
__Sleep_Settings getSleepSettings;
__Sleep_Flags  sleepFlag;

__Cooking_Settings cookingSettings;

__OIL_HEAT_SETTINGS oilHeatSettings;
__OIL_HEAT_SETTINGS getoilHeatSet;
__MANUAL_ manualParameters;

__PORTIONS__ portionsAvailable;
__PORTION_MENU__ portionValues[5];
extern __MOTOR_DIRECTION_ previousDirection;


extern __KEY_FLAGS__ keyFlags;
extern __CANISTER_CHECK canister_check;

uint8_t basketFlag;
uint8_t preHeatFlag;


uint32_t totalRecipeCount;
uint32_t totalPortionCount;

volatile uint8_t manualClearFlag;
extern TIM_HandleTypeDef htim1;



uint8_t  basketMoved;


void MachineProcess(void)
{

	switch(machineState){
		case printLogo:{
			machineState = water_heating;
			break;
		}
		case water_heating:{
			strcpy(temp_buff,"WATER BOILING");
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(2);
			if(Temperature_value > 70)
			{
				machineState=water_heated;
			}

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			machineState = water_temp_check;

			break;
		}
		case water_temp_check:{
			if(Temperature_value > 70){
				machineState=water_heated;
			}
//			else if(Temperature_value < 70)
//			{
//				machineState=water_heating;
//			}
			break;
		}
		case water_heated:{
			strcpy(temp_buff,"READY TO COOK...");
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(2);
			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			machineState = homeScreenInit;
			break;
		}

		case homeScreenInit: {
			printState = printHomeScreenInit;
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
			rCount = 1;
			break;
			}

		case upma_sel: {
			if(keyFlags.resetFlag ==0)
			{
				strcpy(temp_buff,"VEGGIE UPMA SELECTED");
				ILI9341_FillScreenOffset(ILI9341_WHITE);
				HAL_Delay(2);
				ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				DISPANSE_UPMA();
				upmaCnt++;
				eeprom_buff_writeCnt[0] = upmaCnt;
				ee24_write((EEPROM_UPMA_COUNT_ADDR),&eeprom_buff_writeCnt[0],1,300);
				HAL_Delay(500);
				rCount = 0;
	//			save_count(UPMA);
				machineState = homeScreenInit;
			}
			break;
		}

		case kesari_sel: {
			if(keyFlags.resetFlag ==0)
			{
				strcpy(temp_buff,"SUJI HALWA SELECTED ");
				ILI9341_FillScreenOffset(ILI9341_WHITE);
				HAL_Delay(2);


				ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				rCount = 0;
				DISPANSE_KESARI();
				kesarCnt++;
				eeprom_buff_writeCnt[0] = kesarCnt;
				ee24_write((EEPROM_KESAR_COUNT_ADDR),&eeprom_buff_writeCnt[0],1,300);
				HAL_Delay(500);
	//			save_count(KESARI);

				machineState = homeScreenInit;
			}
			break;
		}

		case poha_sel: {
			if(keyFlags.resetFlag == 0)
			{
				strcpy(temp_buff,"KHATTA METTHA POHA               SELECTED");
				ILI9341_FillScreenOffset(ILI9341_WHITE);
				HAL_Delay(2);
				rCount = 0;

				ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				DISPANSE_POHA();
				pohaCnt++;
				eeprom_buff_writeCnt[0] = pohaCnt;
				ee24_write((EEPROM_POHA_COUNT_ADDR),&eeprom_buff_writeCnt[0],1,300);
				HAL_Delay(500);
	//			save_count(POHA);

				machineState = homeScreenInit;
			}
			break;
		}


		case displayCnt:
			 if(eeprom_buff_read[0] == 0x14)
			 {
				  ok_detected = 0;
				  ee24_read((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)&cntBuff[0],1,300);
				  HAL_Delay(10);
				  ee24_read((EEPROM_KESAR_COUNT_ADDR),(uint8_t *)&cntBuff[1],1,300);
				  HAL_Delay(10);
				  ee24_read((EEPROM_POHA_COUNT_ADDR),(uint8_t *)&cntBuff[2],1,300);
				  HAL_Delay(10);
				  sprintf(temp_buff,"VEGGIE UPMA : %d               KHATTA METTHA POHA : %d        SUJI HALWA  : %d ",cntBuff[0],cntBuff[2],cntBuff[1]);

				  ILI9341_FillScreenOffset(ILI9341_WHITE);
				  HAL_Delay(2);
				  ILI9341_WriteString1(20, 40, temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			 }
			 HAL_Delay(1000);
			 machineState = waitForOK;
			 break;
		case waitForOK:
			 if(ok_detected == 1)
			 {
				 ok_detected = 0;
				 machineState = homeScreenInit;
			 }
			 break;
		case reset_eeprom:
			if(eeprom_buff_read[0] == 0x14)
			{
				  sprintf(temp_buff,"COUNT RESETTING...! ");
				  ok_detected = 0;
				  ILI9341_FillScreenOffset(ILI9341_WHITE);
				  HAL_Delay(2);
				  ILI9341_WriteString1(80, 80, temp_buff, ILI9341_BLUE, ILI9341_WHITE);
				  HAL_Delay(500);
				  ee24_write((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)&eeprom_buff_writeCnt[2],1,300);
				  HAL_Delay(10);
				  ee24_write((EEPROM_KESAR_COUNT_ADDR),(uint8_t *)&eeprom_buff_writeCnt[2],1,300);
				  HAL_Delay(10);
				  ee24_write((EEPROM_POHA_COUNT_ADDR),(uint8_t *)&eeprom_buff_writeCnt[2],1,300);
				  HAL_Delay(10);
				  pohaCnt = 0;
				  upmaCnt =0;
				  kesarCnt = 0;
			}
			HAL_Delay(1000);
			machineState = displayCnt;
			break;
#if 0
		case menuPageInit:{
			printState = printMenuScreenPage;
			rCount = 1;
			CurrentMenu = head;
			direction = 1;
			previousPageNo = 0;
			menuPageNo = 0;
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
			sleepSettings.sleepCount = 0;
			sleepFlag.countEnabled = 1;
			break;
		}

		case menuPortion: {
			printState = printMenuPortion;
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = menuPageInit;
			sleepSettings.sleepCount = 0;
			break;
		}

		case recipeReadyForCooking: {
			if((Temperature_value >= portionValues[rCount - 1].portionCutTemp))
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_RESET);
			}
			else if((Temperature_value <= portionValues[rCount - 1].portionCutTemp))
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);
			}

			if(turnsDetected == 1)
			{
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				turnsDetected = 0;
			}
			break;
		}

		case preHeatPage: {
				if((Temperature_value >= 180) || (Temperature_value >= oilHeatSettings.preHeatSetTemp))
				{
					machineState = menuPageInit;
				}
				if(turnsDetected == 1)
				{
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					turnsDetected = 0;
				}
			break;
		}

/*		case menuRecipieSelected:{
			printState = printMenuSelected;

			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = menuPageInit;
			sleepSettings.sleepCount = 0;
			break;
		}*/

		case menuOilHeating: {
			if((Temperature_value >= portionValues[rCount - 1].portionCutTemp))
			{

				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

				HAL_Delay(50);

				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);

				machineState = recipeReadyForCooking;
				printState = printMenuReadyForCooking;
				basketFlag = 0;
				currentMachineState = machineState;
				previousMachineState = menuPortion;
			}
			if(turnsDetected == 1)
			{
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				turnsDetected = 0;
			}

			break;
		}

		case StartMenuCooking: {
				if(basketFlag == 0)
				{
					basketFlag = 1;
					driveMotor(downDirection);
				}

				else if((HAL_GPIO_ReadPin(m_reverse_switch_GPIO_Port,m_reverse_switch_Pin) == GPIO_PIN_RESET) && (basketFlag == 1))
				{
					cookingSettings.cookingTime = portionValues[rCount - 1].portionT1;
					cookingSettings.checkDoubleDip = portionValues[rCount - 1].portionT2 > 0;
					cookingSettings.cookingEnabled = 1;
					currentMachineState = machineState;
					machineState = CheckMenuCookingTimeOut;
					cookingSettings.cookingCompleted = 0;
					sleepSettings.sleepCount = 0;
					sleepFlag.countEnabled = 0;
					//printState = printMenuStart;
					basketFlag =0;
				}

			break;
		}
		case CheckMenuCookingTimeOut:{

			if(cookingSettings.cookingCompleted == 1)
			{
				cookingSettings.cookingEnabled = 0;
				cookingSettings.cookingT1Flag = 1;
				strcpy(temp_buff,"T1-");
				intToStr(cookingSettings.cookingTime,disp_num_buf,3);
				strcat(temp_buff,disp_num_buf);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				if(cookingSettings.checkDoubleDip == 1)
				{

					if((cookingSettings.checkHolding == 0) && (cookingSettings.secondCooking == 0))
					{
						if(basketFlag == 0)
						{
							basketFlag = 1;
							driveMotor(upDirection);
						}


						if((HAL_GPIO_ReadPin(m_forward_switch_GPIO_Port,m_forward_switch_Pin) == GPIO_PIN_RESET) && (basketFlag == 1))
						{
							cookingSettings.printHolding = 1;
							cookingSettings.printTimeT2 = 0;
							cookingSettings.cookingCompleted = 0;
							cookingSettings.cookingTime = portionValues[rCount - 1].portionHT;
							cookingSettings.cookingEnabled = 1;
							cookingSettings.checkHolding = 1;
							basketFlag = 0;
						}

					}
					else if((cookingSettings.secondCooking == 0) && (cookingSettings.checkHolding == 1))
					{
						if(basketFlag == 0)
						{
							basketFlag = 1;
							driveMotor(downDirection);
						}


						if((HAL_GPIO_ReadPin(m_reverse_switch_GPIO_Port,m_reverse_switch_Pin) == GPIO_PIN_RESET) && (basketFlag == 1))
						{
							cookingSettings.printTimeT2 = 1;
							cookingSettings.printHolding = 0;
							cookingSettings.cookingCompleted = 0;
							cookingSettings.secondCooking = 1;
							cookingSettings.cookingTime = portionValues[rCount - 1].portionT2;
							cookingSettings.cookingEnabled = 1;
							cookingSettings.checkDoubleDip = 0;
							basketFlag = 0;
						}

					}
				}
				else
				{
					cookingSettings.cookingCompleted = 0;
					cookingSettings.cookingEnabled = 0;
					cookingSettings.cookingTime = 0;
					printState = printStateIdle;
					machineState = menuPageInit;
					driveMotor(upDirection);
					rCount = 1;
					cookingSettings.checkHolding = 0;
					cookingSettings.secondCooking = 0;
					cookingSettings.cookingT1Flag = 0;
					cookingSettings.printHolding = 0;
					cookingSettings.printTimeT2 = 0;
					sleepSettings.sleepCount = 0;
					sleepFlag.countEnabled = 1;
					passwordNoOneFlag = 0;
					passwordNoOne = 0;
					passwordNoTwo = 0;
					basketFlag = 0;


					if(portionNoSelected == 1)
					{
						CookingMenuDetails->portion1Count += 1 ;												// Count

						dummyEEprom[0] = (CookingMenuDetails->portion1Count >> 8) & 0xff;
						dummyEEprom[1] = (CookingMenuDetails->portion1Count) & 0xff;
						ee24_write(RECIPIE_BASE_ADDR(menuNo)+MENU_PORTION1_COUNT,dummyEEprom,2,100);
					}
					else if(portionNoSelected == 2)
					{
						CookingMenuDetails->portion2Count += 1 ;
						dummyEEprom[0] = (CookingMenuDetails->portion2Count >> 8) & 0xff;
						dummyEEprom[1] = (CookingMenuDetails->portion2Count) & 0xff;
						ee24_write(RECIPIE_BASE_ADDR(menuNo)+MENU_PORTION2_COUNT,dummyEEprom,2,100);
					}
					else if(portionNoSelected == 3)
					{
						CookingMenuDetails->portion3Count += 1 ;
						dummyEEprom[0] = (CookingMenuDetails->portion3Count >> 8) & 0xff;
						dummyEEprom[1] = (CookingMenuDetails->portion3Count) & 0xff;
						ee24_write(RECIPIE_BASE_ADDR(menuNo)+MENU_PORTION3_COUNT,dummyEEprom,2,100);
					}
					else if(portionNoSelected == 4)
					{
						CookingMenuDetails->portion4Count += 1 ;
						dummyEEprom[0] = (CookingMenuDetails->portion4Count >> 8) & 0xff;
						dummyEEprom[1] = (CookingMenuDetails->portion4Count) & 0xff;
						ee24_write(RECIPIE_BASE_ADDR(menuNo)+MENU_PORTION4_COUNT,dummyEEprom,2,100);
					}
					else if(portionNoSelected == 5)
					{
						CookingMenuDetails->portion5Count += 1 ;
						dummyEEprom[0] = (CookingMenuDetails->portion5Count >> 8) & 0xff;
						dummyEEprom[1] = (CookingMenuDetails->portion5Count) & 0xff;
						ee24_write(RECIPIE_BASE_ADDR(menuNo)+MENU_PORTION5_COUNT,dummyEEprom,2,100);
					}




					ee24_read(COMPLETE_RECIPE_COUNT,dummyEEprom,4,100);



					if((dummyEEprom[0] == 255) || (dummyEEprom[0] == 0))
						dummyEEprom[0] = 0;

					if((dummyEEprom[1] == 255) || (dummyEEprom[1] == 0))
						dummyEEprom[1] = 0;

					if((dummyEEprom[2] == 255) || (dummyEEprom[2] == 0))
						dummyEEprom[2] = 0;

					if((dummyEEprom[3] == 255) || (dummyEEprom[3] == 0))
						dummyEEprom[3] = 0;

					totalRecipeCount = (( dummyEEprom[0] << 24) | (dummyEEprom[1] << 16) | (dummyEEprom[2] << 8) | (dummyEEprom[3] & 0xff));

					totalRecipeCount += 1;


					dummyEEprom[0] = (totalRecipeCount >> 24) & 0xff;
					dummyEEprom[1] = (totalRecipeCount >> 16) & 0xff;
					dummyEEprom[2] = (totalRecipeCount >> 8) & 0xff;
					dummyEEprom[3] = totalRecipeCount & 0xff;

				    ee24_write(COMPLETE_RECIPE_COUNT,dummyEEprom,4,100);		  //Count Value

					HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

					HAL_Delay(50);

					HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
				}
			}
			else
			{
				if((Temperature_value >= portionValues[rCount - 1].portionCutTemp))
				{
					HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_RESET);
				}
				else if((Temperature_value <= portionValues[rCount - 1].portionSetTemp))
				{
					HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);
				}

				printState = printMenuCookingState;
			}

			if(turnsDetected == 1)
			{
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				turnsDetected = 0;
			}

			break;
		}

		case menuRecipieCount: {
			currentMachineState = machineState;
			rCount = 1;
			CurrentMenu = head;
			direction = 1;
			previousPageNo = 0;
			menuPageNo = 0;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
			printState = printMenuScreenPage;
			sleepSettings.sleepCount = 0;

			break;
		}

		case checkPortionCountReset:{
			currentMachineState = machineState;
			printState = printMenuPortionCount;
			previousMachineState = menuRecipieCount;
			machineState = MachineIdle;
			rCount = 1;
			break;
		}

		case WIFIsettings:{
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
			printState = printWiFiSettings;
			sleepSettings.sleepCount = 0;
			break;
		}

		case machineSettings:{
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
			printState = printMachineSettings;
			rCount = 0;
			sleepSettings.sleepCount = 0;
			break;
		}

		case incorrectPasswordPage:{
			currentMachineState = machineState;
			machineState = homeScreenInit;
			previousMachineState = machineSettings;
			printState = printIncorrectPassword;
			break;
		}

		case selectMachineSettings: {
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
			printState = printSelectSettings;
			rCount = 1;
			sleepSettings.sleepCount = 0;
			break;
		}
		case selectRecipeSettings: {
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = selectMachineSettings;
			printState = printSelectRecipeSettings;
//			printState = printMenuScreenPage;
			rCount = 1;
			CurrentMenu = head;
			direction = 1;
			previousPageNo = 0;
			menuPageNo = 0;
			sleepSettings.sleepCount = 0;
			break;
		}
		case selectPortionSettings: {
			printState = printMenuPortion;
			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = selectRecipeSettings;
			sleepSettings.sleepCount = 0;
			break;
		}
		case machineSleepSettings:{
			currentMachineState = machineState;
//			getSleepSettings.sleepHour = sleepSettings.sleepHour;
			getSleepSettings.sleepMinutes = sleepSettings.sleepMinutes;
			getSleepSettings.sleepSetTemp = oilHeatSettings.sleepHeatSet;
			getSleepSettings.sleepCutTemp = oilHeatSettings.sleepHeatCutoff;
			machineState = MachineIdle;
			previousMachineState = selectMachineSettings;
			printState = printSleepSettings;
			sleepSettings.sleepCount = 0;
			break;
		}

		case recipieSettings:{
			//uint8_t i = 0;

			//portionValues[rCount - 1].portionNo;

			settingsTemp = portionValues[rCount - 1].portionSetTemp;;
			settingsMaxCutoff = portionValues[rCount - 1].portionCutTemp;
			settingsTimeT1 = portionValues[rCount - 1].portionT1;
			settingsHoldingTim = portionValues[rCount - 1].portionHT;
			settingsTimeT2 = portionValues[rCount - 1].portionT2;

			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = selectRecipeSettings;
			printState = printRecipieSettings;
			sleepSettings.sleepCount = 0;
			break;
		}

		case resetRecipieCount:{

			currentMachineState = machineState;
			machineState = MachineIdle;
			previousMachineState = selectMachineSettings;
			printState = printFullMenuCount;

			rCount = 1;
			  sleepSettings.sleepCount = 0;

			break;
		}

		case deepSleepSettings:{
			currentMachineState = machineState;
			//getSleepSettings.deepSleepHour = sleepSettings.deepSleepHour;
			getSleepSettings.deepSleepMin = sleepSettings.deepSleepMin;
			machineState = MachineIdle;
			previousMachineState = selectMachineSettings;
			printState = printDeepSleepSettings;
			break;
		}

		case menuStopPressed:{

			if(keyFlags.stopPressed == 1)
			{
				cookingSettings.cookingCompleted = 0;
				cookingSettings.cookingEnabled = 0;
				cookingSettings.cookingTime = 0;
				printState = printStateIdle;
	//			machineState = menuPageInit;
				driveMotor(upDirection);
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_RESET);

				ILI9341_FillScreenOffset(ILI9341_WHITE);

				  strcpy(temp_buff,"Process Stopped");
				  ILI9341_WriteString1(MENU_X_AXIS_CENTRE,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				rCount = 1;
				cookingSettings.checkHolding = 0;
				cookingSettings.secondCooking = 0;
				cookingSettings.cookingT1Flag = 0;
				cookingSettings.printHolding = 0;
				cookingSettings.printTimeT2 = 0;
				sleepSettings.sleepCount = 0;
				keyFlags.stopPressed = 0;
				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);


			}
			else if((keyFlags.stopFlag == 0) && ((keyFlags.stopPressed == 0)))
			{
				machineState = homeScreenInit;
				home_detected = 0;
				back_detected = 0;
				Sw_detected = 0;

				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			}

			break;
		}

		case preHeatSettings: {
			currentMachineState = machineState;
			getoilHeatSet.preHeatSetTemp = oilHeatSettings.preHeatSetTemp;
			getoilHeatSet.preHeatCutTemp = oilHeatSettings.preHeatCutTemp;
			machineState = MachineIdle;
			previousMachineState = selectMachineSettings;
			printState = prPreHeat;
			break;
		}

// 19th august
		case manualCooking: {
			currentMachineState = machineState;
			previousMachineState = homeScreenInit;
			machineState = MachineIdle;
			printState = printManualTemp;
			break;
		}

		case manualTempHeating: {
			if((Temperature_value >= manualParameters.manualCuttOffTemp))
			{
				machineState = startManualCooking;
				printState = printManualCookingStart;
				currentMachineState = machineState;
				manualClearFlag = 0;
				basketMoved = 0;
			}

			if(turnsDetected == 1)
			{
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				turnsDetected = 0;
			}

			break;
		}

		case startManualCooking: {

			if(turnsDetected == 1)
			{
				printState = printGetManualBasket;
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
			}



			if((Temperature_value >=  manualParameters.manualCuttOffTemp))
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_RESET);
			}
			else if((Temperature_value <= manualParameters.manualSetTemp))
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);
			}

			if((manualClearFlag == 1))
			{


				if((previousDirection == upDirection) && (basketMoved == 1))
				{
					manualClearFlag = 0;
					basketMoved = 0;
					ILI9341_FillScreenOffsetManual(ILI9341_WHITE);
				}

				if((previousDirection == downDirection) &&  (basketMoved == 1))
				{
					manualClearFlag = 0;
					basketMoved = 0;
					ILI9341_FillScreenOffsetManual(ILI9341_WHITE);
				}


			}

			break;
		}
#endif
////////////////////
		case MachineIdle:{

			if((up_detected == 1) || (down_detected == 1))
			{

				if(currentMachineState == homeScreenInit)
				{
					printState = printHomeScreenBold;
				}
#if 0
				else if((currentMachineState == menuPageInit) || (currentMachineState == selectRecipeSettings) || (currentMachineState == menuRecipieCount))
				{
					if(direction == 1)
					{
						rCount++;

						if(rCount >= recipieCount)
							rCount = recipieCount;

						menuPageNo = ((rCount%DISPLAY_PAGE_SIZE) == 0 ? previousPageNo : (rCount/DISPLAY_PAGE_SIZE));//rCount/DISPLAY_PAGE_SIZE;

						if(previousPageNo != menuPageNo)
						{
							uint8_t i = 1;
							previousPageNo = menuPageNo;

							printState = printMenuScreenPage;

							CurrentMenu = head;

							pageStartNo = 1 + (menuPageNo * DISPLAY_PAGE_SIZE);

							while(CurrentMenu->next != head)
							{
								CurrentMenu = CurrentMenu->next;

								i++;

								if(i == pageStartNo)
									break;

							}
						}
						else
							printState = printMenuScreenBold;


					}
					else if(direction == 2)
					{
						rCount--;

						if((rCount  == 65535) || (rCount <= 0 ))
							rCount=1;

						menuPageNo = ((rCount%DISPLAY_PAGE_SIZE) == 0 ? (previousPageNo -1) : (rCount/DISPLAY_PAGE_SIZE));

						if(previousPageNo != menuPageNo)
						{
							uint8_t i = 1;
							previousPageNo = menuPageNo;

							printState = printMenuScreenPage;

							CurrentMenu = head;

							pageStartNo = 1 + (menuPageNo * DISPLAY_PAGE_SIZE);

							while(CurrentMenu->next != head)
							{
								if(i == pageStartNo)
									break;

								CurrentMenu = CurrentMenu->next;

								i++;
							}
						}
						else
							printState = printMenuScreenBold;
					}

				}
//				else if(currentMachineState == menuRecipieSelected)
//				{
//					printState = printMenuSelected;
//				}
				else if(currentMachineState == machineSettings)
				{
					printState = printPassword;
				}
				else if(currentMachineState == recipieSettings)
				{
					printState = printGetSettings;
				}
				else if(currentMachineState == selectMachineSettings)
				{
					printState = printScrollSettings;
				}
				else if(currentMachineState == machineSleepSettings)
				{
					printState = printGetSleepSettings;
				}
				else if(currentMachineState == deepSleepSettings)
				{
					printState = printGetDeepSleepSettings;
				}
				else if(currentMachineState == preHeatSettings)
				{
					printState = prGetPreHeat;
				}
				else if(currentMachineState == manualCooking)
				{
					printState = printGetManualTemp;
				}
				else if((currentMachineState == menuPortion) || (currentMachineState == selectPortionSettings))
				{
					printState = printGetMenuPortion;
				}
				else if((currentMachineState == resetRecipieCount) || (currentMachineState == checkPortionCountReset))
				{
					printState = printGetMenuCountReset;
				}
				else
				{
					turnsDetected = 0;
					//HAL_TIM_Base_Start_IT(&htim1);
				}
#endif
			}
			break;
		}
#if 0
		case machineSleepMode: {

			if((Temperature_value >= oilHeatSettings.sleepHeatCutoff))
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_RESET);
			}
			else if((Temperature_value <= oilHeatSettings.sleepHeatSet))
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);
			}


			break;
		}
		case machineDeepSleep: {

			break;
		}
#endif
	}
}





void DisplayPrint(void)
{
	uint8_t i = 0,j =0;

	switch(printState){

		case printHomeScreenInit:{
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);
			i = 1;
			noOfSettingsAvailable = 0;
			for(i = 1;i <=MAIN_MENU_AVAILABLE ;i++)
			{

				strcpy(temp_buff,mainmenu[i-1]);
			  ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * i)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			  noOfSettingsAvailable++;
			 HAL_Delay(5);
			}

		       strcpy(temp_buff,mainmenu[0]);
			ILI9341_WriteString1(MENU_X_AXIS_START,( 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			rCount = 1;
			printState = printStateIdle;
			break;
		}
		case printHomeScreenBold: {

//			if(turnsDetected == 1)
//			{
//				turnsDetected = 0;

				if(down_detected == 1)
				{
					rCount++;

					if(rCount >= MAIN_MENU_AVAILABLE)
						rCount = MAIN_MENU_AVAILABLE;

//					if(rCount != 4){
						strcpy(temp_buff,mainmenu[rCount - 1]);
						ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						HAL_Delay(2);
						if(rCount != 1){
							strcpy(temp_buff,mainmenu[rCount - 2]);
							ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount-1))*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
						}

//					}
						down_detected = 0;
				}
				else if(up_detected == 1)
				{
					rCount--;

					if((rCount  == 65535) || (rCount <= 0 ))
						rCount=1;

					strcpy(temp_buff,mainmenu[rCount - 1]);
					ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
					HAL_Delay(2);

					strcpy(temp_buff,mainmenu[rCount]);
					ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount+1))*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				}
				up_detected = 0;
//			}

			printState = printStateIdle;

			break;
		}
#if 0
		case printSelectRecipeSettings:
		case printMenuScreenPage: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);
			  i = 1 + (menuPageNo * DISPLAY_PAGE_SIZE);
			  j = 1;
			  if(recipieCount >= 1){

				  Bold = CurrentMenu;
				  while (CurrentMenu->next != head)
				  {
					    memset(temp_buff,0,strlen(temp_buff));
					    intToStrIndex(i,temp_buff,1);
					    strcat(temp_buff,CurrentMenu->name);
					  ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * j)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

					  if(j == DISPLAY_PAGE_SIZE)
					  {
						  j++;

						  break;
					  }

					  i++;
					  j++;
					  CurrentMenu = CurrentMenu->next;
				     HAL_Delay(5);
				  }

				  if(CurrentMenu->next == head)
				  {
					    memset(temp_buff,0,strlen(temp_buff));
					    intToStrIndex(i,temp_buff,1);
					    strcat(temp_buff,CurrentMenu->name);
					  ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * j)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				  }

				  if(direction == 1)
				  {
					    memset(temp_buff,0,strlen(temp_buff));
					    intToStrIndex(rCount,temp_buff,1);
					    strcat(temp_buff,Bold->name);
					  ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * 1)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
					  CookingMenuDetails = Bold;
				  }
				  else if(direction == 2)
				  {
					    memset(temp_buff,0,strlen(temp_buff));
					    intToStrIndex(rCount,temp_buff,1);
					    strcat(temp_buff,CurrentMenu->name);
					  ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * (j - 1))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
					  CookingMenuDetails = CurrentMenu;
				  }
				  turnsDetected = 0;
				  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				 // HAL_TIM_Base_Start_IT(&htim1);
			  }
			  printState = printStateIdle;
			  break;
		}

		case printMenuScreenBold: {
			if(turnsDetected == 1)
			{
				i = 0;
					turnsDetected = 0;

					if(direction == 1)
					{
						CurrentMenu = head;
						previous = NULL;

						if(rCount >= recipieCount)
							rCount = recipieCount;

						for(i = 0;i < rCount-1;i++){
							previous = CurrentMenu;
							CurrentMenu = CurrentMenu->next;
						}
						i = ((rCount%DISPLAY_PAGE_SIZE) == 0 ? DISPLAY_PAGE_SIZE : rCount%DISPLAY_PAGE_SIZE);
						if(rCount != (1 + (menuPageNo * DISPLAY_PAGE_SIZE)))
						{
							    memset(temp_buff,0,strlen(temp_buff));
							    intToStrIndex(rCount,temp_buff,1);
							    strcat(temp_buff,CurrentMenu->name);
								ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * (i))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
								HAL_Delay(2);

							    memset(temp_buff,0,strlen(temp_buff));
							    intToStrIndex(rCount-1,temp_buff,1);
							    strcat(temp_buff,previous->name);
								ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * (i-1))*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
						}
						CookingMenuDetails = CurrentMenu;

					}
					else if(direction == 2)
					{

						CurrentMenu = head;
						previous = NULL;

						if((rCount  == 65535) || (rCount <= 0 ))
							rCount=1;

						for(i = 0;i < rCount;i++){
							previous = CurrentMenu;
							CurrentMenu = CurrentMenu->next;
						}
						i = ((rCount%DISPLAY_PAGE_SIZE) == 0 ? DISPLAY_PAGE_SIZE : rCount%DISPLAY_PAGE_SIZE);

					    memset(temp_buff,0,strlen(temp_buff));
					    intToStrIndex(rCount,temp_buff,1);
					    strcat(temp_buff,previous->name);
						ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * (i))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						HAL_Delay(2);


					    memset(temp_buff,0,strlen(temp_buff));
					    intToStrIndex(rCount+1,temp_buff,1);
					    strcat(temp_buff,CurrentMenu->name);
						ILI9341_WriteString1(MENU_X_AXIS_START,( Y_AXIS_SPACING * (i+1))*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

						CookingMenuDetails = previous;
					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
			HAL_Delay(20);
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
				printState = printStateIdle;

			break;
		}

		case printMenuPortion: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);

			  ILI9341_WriteString1(120,( 4)*10,"Portions", ILI9341_BLACK, ILI9341_WHITE);
			  portionsAvailable.noOfPortions = 0;

			  if((currentMachineState == selectPortionSettings) || ((CookingMenuDetails->portion1CTime1 > 0) && (CookingMenuDetails->portion1SetTemp > 0) && (CookingMenuDetails->portion1CutOffTemp > 0)))
			  {
				  portionsAvailable.noOfPortions++;
				  portionValues[portionsAvailable.noOfPortions - 1].portionNo = 1;
				  portionValues[portionsAvailable.noOfPortions - 1].portionSetTemp = CookingMenuDetails->portion1SetTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionCutTemp = CookingMenuDetails->portion1CutOffTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT1 = CookingMenuDetails->portion1CTime1;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT2 = CookingMenuDetails->portion1CTime2;
				  portionValues[portionsAvailable.noOfPortions - 1].portionHT = CookingMenuDetails->portion1HTime;
				  ILI9341_WriteString1(153,(( 3 * (portionsAvailable.noOfPortions) )*10 )+ 40,"1", ILI9341_RED, ILI9341_WHITE);
			  }

			  if((currentMachineState == selectPortionSettings) || ((CookingMenuDetails->portion2CTime1 > 0) && (CookingMenuDetails->portion2SetTemp > 0) && (CookingMenuDetails->portion2CutOffTemp > 0)))
			  {
				  portionsAvailable.noOfPortions++;
				  portionValues[portionsAvailable.noOfPortions - 1].portionNo = 2;
				  portionValues[portionsAvailable.noOfPortions - 1].portionSetTemp = CookingMenuDetails->portion2SetTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionCutTemp = CookingMenuDetails->portion2CutOffTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT1 = CookingMenuDetails->portion2CTime1;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT2 = CookingMenuDetails->portion2CTime2;
				  portionValues[portionsAvailable.noOfPortions - 1].portionHT = CookingMenuDetails->portion2HTime;
				  ILI9341_WriteString1(153,(( 3 * (portionsAvailable.noOfPortions) )*10 )+ 40,"2", ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((currentMachineState == selectPortionSettings) || ((CookingMenuDetails->portion3CTime1 > 0) && (CookingMenuDetails->portion3SetTemp > 0) && (CookingMenuDetails->portion3CutOffTemp > 0)))
			  {
				  portionsAvailable.noOfPortions++;
				  portionValues[portionsAvailable.noOfPortions - 1].portionNo = 3;
				  portionValues[portionsAvailable.noOfPortions - 1].portionSetTemp = CookingMenuDetails->portion3SetTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionCutTemp = CookingMenuDetails->portion3CutOffTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT1 = CookingMenuDetails->portion3CTime1;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT2 = CookingMenuDetails->portion3CTime2;
				  portionValues[portionsAvailable.noOfPortions - 1].portionHT = CookingMenuDetails->portion3HTime;
				  ILI9341_WriteString1(153,(( 3 * (portionsAvailable.noOfPortions) )*10 )+ 40,"3", ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((currentMachineState == selectPortionSettings) || ((CookingMenuDetails->portion4CTime1 > 0) && (CookingMenuDetails->portion4SetTemp > 0) && (CookingMenuDetails->portion4CutOffTemp > 0)))
			  {
				  portionsAvailable.noOfPortions++;
				  portionValues[portionsAvailable.noOfPortions - 1].portionNo = 4;
				  portionValues[portionsAvailable.noOfPortions - 1].portionSetTemp = CookingMenuDetails->portion4SetTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionCutTemp = CookingMenuDetails->portion4CutOffTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT1 = CookingMenuDetails->portion4CTime1;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT2 = CookingMenuDetails->portion4CTime2;
				  portionValues[portionsAvailable.noOfPortions - 1].portionHT = CookingMenuDetails->portion4HTime;
				  ILI9341_WriteString1(153,(( 3 * (portionsAvailable.noOfPortions) )*10 )+ 40,"4", ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((currentMachineState == selectPortionSettings) || ((CookingMenuDetails->portion5CTime1 > 0) && (CookingMenuDetails->portion5SetTemp > 0) && (CookingMenuDetails->portion5CutOffTemp > 0)))
			  {
				  portionsAvailable.noOfPortions++;
				  portionValues[portionsAvailable.noOfPortions - 1].portionNo = 5;
				  portionValues[portionsAvailable.noOfPortions - 1].portionSetTemp = CookingMenuDetails->portion5SetTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionCutTemp = CookingMenuDetails->portion5CutOffTemp;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT1 = CookingMenuDetails->portion5CTime1;
				  portionValues[portionsAvailable.noOfPortions - 1].portionT2 = CookingMenuDetails->portion5CTime2;
				  portionValues[portionsAvailable.noOfPortions - 1].portionHT = CookingMenuDetails->portion5HTime;
				  ILI9341_WriteString1(153,(( 3 * (portionsAvailable.noOfPortions) )*10 )+ 40,"5", ILI9341_BLACK, ILI9341_WHITE);
			  }

			  printState = printStateIdle;

			break;
		}

		case printGetMenuPortion: {
			if(turnsDetected == 1)
			{
				i = 0;
					turnsDetected = 0;

					if(direction == 1)
					{
						rCount++;

						if(rCount >= portionsAvailable.noOfPortions)
							rCount = portionsAvailable.noOfPortions;

							    memset(temp_buff,0,strlen(temp_buff));
							    intToStr(portionValues[rCount - 2].portionNo,temp_buff,1);
								ILI9341_WriteString1(153,(( 3 * (rCount -1) )*10 )+ 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
								HAL_Delay(2);


							    memset(temp_buff,0,strlen(temp_buff));
							    intToStr(portionValues[rCount - 1].portionNo,temp_buff,1);
								ILI9341_WriteString1(153,(( 3 * (rCount) )*10 )+ 40,temp_buff, ILI9341_RED, ILI9341_WHITE);



					}
					else if(direction == 2)
					{
						rCount--;


						if((rCount  == 65535) || (rCount <= 0 ))
							rCount=1;

					    memset(temp_buff,0,strlen(temp_buff));
					    intToStr(portionValues[rCount - 1].portionNo,temp_buff,1);
						ILI9341_WriteString1(153,(( 3 * (rCount) )*10 )+ 40,temp_buff, ILI9341_RED, ILI9341_WHITE);
						HAL_Delay(2);


					    memset(temp_buff,0,strlen(temp_buff));
					    intToStr(portionValues[rCount ].portionNo,temp_buff,1);
						ILI9341_WriteString1(153,(( 3 * (rCount +1) )*10 )+ 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}

				printState = printStateIdle;

			break;
		}

		case printMenuReadyForCooking: {

			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);
			  ILI9341_WriteString1(70,( 2)*10,"Ready For Cooking", ILI9341_BLACK, ILI9341_WHITE);

			  ILI9341_WriteString1(80,( 8)*10,"Place Ingredients", ILI9341_BLACK, ILI9341_WHITE);

			  ILI9341_WriteString1(80,( 10)*10,"and press ok", ILI9341_BLACK, ILI9341_WHITE);



			  ILI9341_WriteString1(150,( 4 * 5)*10,"OK", ILI9341_RED, ILI9341_WHITE);


				printState = printStateIdle;
			break;
		}

		case printMenuStart: {

			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);


			strcpy(temp_buff,CookingMenuDetails->name);
			ILI9341_WriteString1(MENU_START_BUTTON,( 4 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"T1-");
			intToStr(portionValues[rCount - 1].portionT1,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TEMP,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"HT-");
			intToStr(portionValues[rCount - 1].portionHT,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_START_BUTTON,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"T2-");
			intToStr(portionValues[rCount - 1].portionT2,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TIME,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);



			printState = printStateIdle;
			break;
		}
		case printMenuCookingState:{
			if(cookingSettings.cookingT1Flag == 0)
			{
				strcpy(temp_buff,"T1-");
				intToStr(cookingSettings.cookingTime,disp_num_buf,3);
				strcat(temp_buff,disp_num_buf);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}
			if(cookingSettings.printHolding == 1)
			{
				strcpy(temp_buff,"HT-");
				intToStr(cookingSettings.cookingTime,disp_num_buf,3);
				strcat(temp_buff,disp_num_buf);
				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}
			else if((cookingSettings.printTimeT2 == 1) && (cookingSettings.printHolding == 0))
			{
				strcpy(temp_buff,"T2-");
				intToStr(cookingSettings.cookingTime,disp_num_buf,3);
				strcat(temp_buff,disp_num_buf);
				ILI9341_WriteString1(MENU_X_AXIS_TIME,( 4 * MENU_Y_AXIS_TIME)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}


			break;
		}


		case printMenuPortionCount: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);
			  ILI9341_WriteString1(100,( 2)*10,"Portions Count", ILI9341_BLACK, ILI9341_WHITE);

			portionsAvailable.noOfPortions = 0;
			totalPortionCount = 0;

			if((CookingMenuDetails->portion1CTime1 > 0) && (CookingMenuDetails->portion1SetTemp > 0) && (CookingMenuDetails->portion1CutOffTemp > 0))
			  {
				  portionsAvailable.noOfPortions++;
				  totalPortionCount += CookingMenuDetails->portion1Count;

			    memset(temp_buff,0,strlen(temp_buff));
			    intToStrIndex(1,temp_buff,1);
			    strcat(temp_buff,"Count - ");
				intToStr(CookingMenuDetails->portion1Count,disp_num_buf,1);
				strcat(temp_buff,disp_num_buf);

				ILI9341_WriteString1(MENU_X_AXIS_START,( 2 * (portionsAvailable.noOfPortions-1))*10 + 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((CookingMenuDetails->portion2CTime1 > 0) && (CookingMenuDetails->portion2SetTemp > 0) && (CookingMenuDetails->portion2CutOffTemp > 0))
			  {
				  portionsAvailable.noOfPortions++;
				  totalPortionCount += CookingMenuDetails->portion2Count;

			    memset(temp_buff,0,strlen(temp_buff));
			    intToStrIndex(2,temp_buff,1);
			    strcat(temp_buff,"Count - ");
				intToStr(CookingMenuDetails->portion2Count,disp_num_buf,1);
				strcat(temp_buff,disp_num_buf);

				ILI9341_WriteString1(MENU_X_AXIS_START,( 2 * (portionsAvailable.noOfPortions-1))*10 + 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((CookingMenuDetails->portion3CTime1 > 0) && (CookingMenuDetails->portion3SetTemp > 0) && (CookingMenuDetails->portion3CutOffTemp > 0))
			  {
				  portionsAvailable.noOfPortions++;
				  totalPortionCount += CookingMenuDetails->portion3Count;

			    memset(temp_buff,0,strlen(temp_buff));
			    intToStrIndex(3,temp_buff,1);
			    strcat(temp_buff,"Count - ");
				intToStr(CookingMenuDetails->portion3Count,disp_num_buf,1);
				strcat(temp_buff,disp_num_buf);

				ILI9341_WriteString1(MENU_X_AXIS_START,( 2 * (portionsAvailable.noOfPortions-1))*10 + 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((CookingMenuDetails->portion4CTime1 > 0) && (CookingMenuDetails->portion4SetTemp > 0) && (CookingMenuDetails->portion4CutOffTemp > 0))
			  {
				  portionsAvailable.noOfPortions++;
				  totalPortionCount += CookingMenuDetails->portion4Count;

				    memset(temp_buff,0,strlen(temp_buff));
				    intToStrIndex(4,temp_buff,1);
				    strcat(temp_buff,"Count - ");
					intToStr(CookingMenuDetails->portion4Count,disp_num_buf,1);
					strcat(temp_buff,disp_num_buf);

				ILI9341_WriteString1(MENU_X_AXIS_START,( 2 * (portionsAvailable.noOfPortions-1))*10 + 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			  }

			  if((CookingMenuDetails->portion5CTime1 > 0) && (CookingMenuDetails->portion5SetTemp > 0) && (CookingMenuDetails->portion5CutOffTemp > 0))
			  {
				  portionsAvailable.noOfPortions++;
				  totalPortionCount += CookingMenuDetails->portion5Count;

				    memset(temp_buff,0,strlen(temp_buff));
				    intToStrIndex(5,temp_buff,1);
				    strcat(temp_buff,"Count - ");
					intToStr(CookingMenuDetails->portion5Count,disp_num_buf,1);
					strcat(temp_buff,disp_num_buf);

				ILI9341_WriteString1(MENU_X_AXIS_START,( 2 * (portionsAvailable.noOfPortions-1))*10 + 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			  }

			    strcpy(temp_buff,"Total - ");
				intToStr(totalPortionCount,disp_num_buf,1);
				strcat(temp_buff,disp_num_buf);
			  ILI9341_WriteString1(73,( 2 * (portionsAvailable.noOfPortions + 1))*10 + 40,temp_buff, ILI9341_BLACK, ILI9341_WHITE);


				ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 5 )*10,"Back", ILI9341_RED, ILI9341_WHITE);

				ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 5 )*10,"Reset", ILI9341_BLACK, ILI9341_WHITE);

			  printState = printStateIdle;
			break;
		}

		case printWiFiSettings:{
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);

			ILI9341_WriteString1(MENU_X_AXIS_CENTRE,( 4 * 2)*10,"WiFI Settings", ILI9341_BLACK, ILI9341_WHITE);

			printState = printStateIdle;
			break;
		}

		case printMachineSettings:{
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(2);

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 1)*10,"Machine Settings", ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,"Enter Password:", ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(PASSWORD_ONE_OFFSET,( 4 * 3)*10,"0 0", ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(PASSWORD_ONE_OFFSET,( 4 * 3)*10,"0", ILI9341_RED, ILI9341_WHITE);

			printState = printStateIdle;
			break;
		}

		case printPassword:{
			if(turnsDetected == 1)
			{
					turnsDetected = 0;

					if(direction == 1)
					{
						rCount++;

						if(rCount >=9)
							rCount = 9;

						if(passwordNoOneFlag == 0)
						{
						    memset(temp_buff,0,strlen(temp_buff));
							intToStr(rCount,temp_buff,1);
							ILI9341_WriteString1(PASSWORD_ONE_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
							passwordNoOne = rCount;
						}
						else if(passwordNoOneFlag == 1)
						{
						    memset(temp_buff,0,strlen(temp_buff));
							intToStr(rCount,temp_buff,1);
							ILI9341_WriteString1(PASSWORD_TWO_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
							passwordNoTwo = rCount;
						}


					}
					else if(direction == 2)
					{

						rCount--;

						if((rCount  == 65535) || (rCount <= 0 ))
							rCount=0;

						if(passwordNoOneFlag == 0)
						{
						    memset(temp_buff,0,strlen(temp_buff));
							intToStr(rCount,temp_buff,1);
							ILI9341_WriteString1(PASSWORD_ONE_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
							passwordNoOne = rCount;
						}
						else if(passwordNoOneFlag == 1)
						{
						    memset(temp_buff,0,strlen(temp_buff));
							intToStr(rCount,temp_buff,1);
							ILI9341_WriteString1(PASSWORD_TWO_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
							passwordNoTwo = rCount;
						}
					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}

				printState = printStateIdle;
				break;
		}

		case printIncorrectPassword:{
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);

				ILI9341_WriteString1(MENU_X_AXIS_CENTRE,( 4 * 1)*10,"Incorrect Password", ILI9341_BLACK, ILI9341_WHITE);

				HAL_Delay(500);
				printState = printStateIdle;
			break;
		}

		case printRecipieSettings: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			  HAL_Delay(1);

			strcpy(temp_buff,CookingMenuDetails->name);
			ILI9341_WriteString1(MENU_START_BUTTON,( 4 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"MaxTemp-");
			intToStr(settingsMaxCutoff,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(67,( 4 * 2 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"MinTemp-");
			intToStr(settingsTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TEMP,( 4 * 3 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"TimeT1-");
			intToStr(settingsTimeT1,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TIME,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"HT-");
			intToStr(settingsHoldingTim,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TEMP,( 4 * 4 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"TimeT2-");
			intToStr(settingsTimeT2,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TIME,( 4 * 4)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			memset(temp_buff,0,strlen(temp_buff));
			intToStr(settingsMaxCutoff,temp_buff,3);
			ILI9341_WriteString1(TEMP_X_AXIS+80,( 4 * 2 )*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

			ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5 )*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);

			printState = printStateIdle;


			break;
		}

		case printGetSettings: {
			if(turnsDetected == 1)
			{
					turnsDetected = 0;

					if(direction == 1)
					{

						if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0)  && (settingsFlag.CutOffFlag == 0)
								&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
						{

							settingsMaxCutoff++;
							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsMaxCutoff,temp_buff,3);
							ILI9341_WriteString1(TEMP_X_AXIS+80,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						}
						else if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
						{
							settingsTemp++;

							if(settingsTemp >= settingsMaxCutoff)
								settingsTemp = settingsMaxCutoff;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsTemp,temp_buff,3);
							ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
						{
							settingsTimeT1++;
							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsTimeT1,temp_buff,3);
							ILI9341_WriteString1(Time_X_AXIS,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0))
						{
							settingsHoldingTim++;
							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsHoldingTim,temp_buff,3);
							ILI9341_WriteString1(HOLDING_TIME_OFFSET,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 1)  && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0))
						{
							settingsTimeT2++;
							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsTimeT2,temp_buff,3);
							ILI9341_WriteString1(Time_X_AXIS,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						}
					}
					else if(direction == 2)
					{

						if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 0 )
								&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
						{
							settingsMaxCutoff--;

							if((settingsMaxCutoff  == 65535) || (settingsMaxCutoff <= 0 ))
								settingsMaxCutoff=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsMaxCutoff,temp_buff,3);
							ILI9341_WriteString1(TEMP_X_AXIS+80,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
						{
							settingsTemp--;

							if((settingsTemp  == 65535) || (settingsTemp <= 0 ))
								settingsTemp=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsTemp,temp_buff,3);
							ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0)  && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
						{
							settingsTimeT1--;

							if((settingsTimeT1  == 65535) || (settingsTimeT1 <= 0 ))
								settingsTimeT1=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsTimeT1,temp_buff,3);
							ILI9341_WriteString1(Time_X_AXIS,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0))
						{
							settingsHoldingTim--;

							if((settingsHoldingTim  == 65535) || (settingsHoldingTim <= 0 ))
								settingsHoldingTim=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsHoldingTim,temp_buff,3);
							ILI9341_WriteString1(HOLDING_TIME_OFFSET,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 1) && (settingsFlag.CutOffFlag == 1)
								&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0))
						{
							settingsTimeT2--;

							if((settingsTimeT2  == 65535) || (settingsTimeT2 <= 0 ))
								settingsTimeT2=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(settingsTimeT2,temp_buff,3);
							ILI9341_WriteString1(Time_X_AXIS,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}

					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}

				printState = printStateIdle;


			break;
		}

		case printSelectSettings: {

				ILI9341_FillScreenOffset(ILI9341_WHITE);
				HAL_Delay(1);
				i = 1;
				noOfSettingsAvailable = 0;
				for(i = 1;i <=SETTINGS_AVAILABLE ;i++)
				{
					strcpy(temp_buff,Settings[i-1]);
				  ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * i)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				  noOfSettingsAvailable++;
				 HAL_Delay(50);
				}

				strcpy(temp_buff,Settings[0]);
				ILI9341_WriteString1(MENU_X_AXIS_START,( 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				rCount = 1;
				printState = printStateIdle;


			break;
		}

		case printScrollSettings: {
			if(turnsDetected == 1)
			{
				turnsDetected = 0;

				if(direction == 1)
				{
					rCount++;

					if(rCount >= SETTINGS_AVAILABLE)
						rCount = SETTINGS_AVAILABLE;

//					if(rCount != 4){
						strcpy(temp_buff,Settings[rCount - 1]);
						ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						HAL_Delay(2);
						if(rCount != 1){
							strcpy(temp_buff,Settings[rCount - 2]);
							ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount-1))*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
						}

//					}

				}
				else if(direction == 2)
				{
					rCount--;

					if((rCount  == 65535) || (rCount <= 0 ))
						rCount=1;

					strcpy(temp_buff,Settings[rCount - 1]);
					ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount))*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
					HAL_Delay(2);

					strcpy(temp_buff,Settings[rCount]);
					ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * (rCount+1))*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				}
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				//HAL_TIM_Base_Start_IT(&htim1);
			}

			printState = printStateIdle;
			break;
		}

		case printSleepSettings: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 )*10,"Sleep Settings", ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"MaxTemp-");
			intToStr(getSleepSettings.sleepCutTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_TEMP,( 4 * 2 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"MinTemp-");
			intToStr(getSleepSettings.sleepSetTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(189,( 4 * 2 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"Sleep Min-");
			intToStr(getSleepSettings.sleepMinutes,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);

			memset(temp_buff,0,strlen(temp_buff));
			intToStr(getSleepSettings.sleepCutTemp,temp_buff,3);
			ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

			printState = printStateIdle;
			break;
		}

		case printGetSleepSettings:{
//			sleepFlag
			if(turnsDetected == 1)
			{
					turnsDetected = 0;

					if(direction == 1)
					{

						if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 0) && (sleepFlag.minSaved == 0))
						{

							getSleepSettings.sleepCutTemp++;

							if(getSleepSettings.sleepCutTemp >= 200)
								getSleepSettings.sleepCutTemp = 200;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.sleepCutTemp,temp_buff,3);
							ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 0))
						{
							getSleepSettings.sleepSetTemp++;

							if(getSleepSettings.sleepSetTemp >= getSleepSettings.sleepCutTemp)
								getSleepSettings.sleepSetTemp = getSleepSettings.sleepCutTemp;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.sleepSetTemp,temp_buff,3);
							ILI9341_WriteString1(Time_X_AXIS,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						}
						else if((sleepFlag.sleepSetTempFlag == 1) && (sleepFlag.sleepCutTempFlag == 1) &&  (sleepFlag.minSaved == 0))
						{
							getSleepSettings.sleepMinutes++;

							if(getSleepSettings.sleepMinutes >= 255)
								getSleepSettings.sleepMinutes = 254;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.sleepMinutes,temp_buff,3);
							ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}

					}
					else if(direction == 2)
					{

						if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 0) && (sleepFlag.minSaved == 0))
						{

							getSleepSettings.sleepCutTemp--;

							if((getSleepSettings.sleepCutTemp  == 255) || (getSleepSettings.sleepCutTemp <= 0 ))
									getSleepSettings.sleepCutTemp=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.sleepCutTemp,temp_buff,3);
							ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
						else if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 0))
						{
							getSleepSettings.sleepSetTemp--;

							if((getSleepSettings.sleepSetTemp  == 255) || (getSleepSettings.sleepSetTemp <= 0 ))
									getSleepSettings.sleepSetTemp=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.sleepSetTemp,temp_buff,3);
							ILI9341_WriteString1(Time_X_AXIS,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						}
						if((sleepFlag.sleepSetTempFlag == 1) && (sleepFlag.sleepCutTempFlag == 1) &&(sleepFlag.minSaved == 0))
						{
							getSleepSettings.sleepMinutes--;

							if((getSleepSettings.sleepMinutes  == 65535) || (getSleepSettings.sleepMinutes <= 1 ))
									getSleepSettings.sleepMinutes=1;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.sleepMinutes,temp_buff,3);
							ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}

					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}

				printState = printStateIdle;
			break;
		}
		case printDeepSleepSettings: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 )*10,"Deep Sleep Settings", ILI9341_BLACK, ILI9341_WHITE);


			strcpy(temp_buff,"Sleep Min-");
			intToStr(getSleepSettings.deepSleepMin,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);

			memset(temp_buff,0,strlen(temp_buff));
			intToStr(getSleepSettings.deepSleepMin,temp_buff,3);
			ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			printState = printStateIdle;

			break;
		}
		case printGetDeepSleepSettings:{
//			sleepFlag
			if(turnsDetected == 1)
			{
					turnsDetected = 0;

					if(direction == 1)
					{

						if((sleepFlag.deepMinSaved == 0))
						{
							getSleepSettings.deepSleepMin++;

							if(getSleepSettings.deepSleepMin >= 255)
								getSleepSettings.deepSleepMin = 254;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.deepSleepMin,temp_buff,3);
							ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
					}
					else if(direction == 2)
					{

						if((sleepFlag.deepMinSaved == 0))
						{
							getSleepSettings.deepSleepMin--;

							if((getSleepSettings.deepSleepMin  == 65535) || (getSleepSettings.deepSleepMin <= 1 ))
									getSleepSettings.deepSleepMin=1;


							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getSleepSettings.deepSleepMin,temp_buff,3);
							ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}

				printState = printStateIdle;
			break;
		}
		case printSleepMode:{
			ILI9341_FillScreenOffset(ILI9341_BLACK);
			HAL_Delay(1);

			ILI9341_WriteString1(MENU_X_AXIS_CENTRE,( 4 * 3)*10,"Sleep Mode", ILI9341_WHITE, ILI9341_BLACK);

			printState = printStateIdle;
			break;
		}
		case printDeepSleep:{

			ILI9341_FillScreen(ILI9341_BLACK);
			HAL_Delay(1);

			HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_RESET);

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 3)*10,"Deep Sleep Mode", ILI9341_WHITE, ILI9341_BLACK);

			printState = printStateIdle;
			break;
		}

		case prPreHeat: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);

			ILI9341_WriteString1(70,( 4 )*10,"Pre-Heat Settings", ILI9341_BLACK, ILI9341_WHITE);


			strcpy(temp_buff,"Max Temp-");
			intToStr(getoilHeatSet.preHeatCutTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(78,( 4 * 2 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"Min Temp-");
			intToStr(getoilHeatSet.preHeatSetTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(78,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(130,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);

			memset(temp_buff,0,strlen(temp_buff));
			intToStr(getoilHeatSet.preHeatCutTemp,temp_buff,3);
			ILI9341_WriteString1(PRE_HEAT,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			printState = printStateIdle;
			break;
		}

		case prGetPreHeat: {
			if(turnsDetected == 1)
			{
					turnsDetected = 0;

					if(direction == 1)
					{

						if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 0))
						{

							getoilHeatSet.preHeatCutTemp++;

							if(getoilHeatSet.preHeatCutTemp >= 200)
								getoilHeatSet.preHeatCutTemp = 200;


							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getoilHeatSet.preHeatCutTemp,temp_buff,3);
							ILI9341_WriteString1(PRE_HEAT,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						}
						else if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 1))
						{
							getoilHeatSet.preHeatSetTemp++;

							if(getoilHeatSet.preHeatSetTemp >= getoilHeatSet.preHeatCutTemp)
								getoilHeatSet.preHeatSetTemp = getoilHeatSet.preHeatCutTemp;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getoilHeatSet.preHeatSetTemp,temp_buff,3);
							ILI9341_WriteString1(PRE_HEAT,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}

					}
					else if(direction == 2)
					{

						if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 0))
						{

							getoilHeatSet.preHeatCutTemp--;

							if((getoilHeatSet.preHeatCutTemp  == 255) || (getoilHeatSet.preHeatCutTemp <= 0 ))
									getoilHeatSet.preHeatCutTemp=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getoilHeatSet.preHeatCutTemp,temp_buff,3);
							ILI9341_WriteString1(PRE_HEAT,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
						}
						else if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 1))
						{
							getoilHeatSet.preHeatSetTemp--;

							if((getoilHeatSet.preHeatSetTemp  == 255) || (getoilHeatSet.preHeatSetTemp <= 0 ))
									getoilHeatSet.preHeatSetTemp=0;

							memset(temp_buff,0,strlen(temp_buff));
							intToStr(getoilHeatSet.preHeatSetTemp,temp_buff,3);
							ILI9341_WriteString1(PRE_HEAT,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

						}
					}
					HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
					//HAL_TIM_Base_Start_IT(&htim1);
				}

				printState = printStateIdle;
			break;
		}

// 19th August

		case printManualTemp: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);


			ILI9341_WriteString1(MANUAL_TEMP,( 4 )*10,"Manual Mode", ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"Max Temp-");
			intToStr(manualParameters.manualCuttOffTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MANUAL_TEMP,( 4 * 2 )*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			strcpy(temp_buff,"Max Temp-");
			intToStr(manualParameters.manualSetTemp,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MANUAL_TEMP,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(OK_OFFSET,( 4 * 5)*10,"OK", ILI9341_BLACK, ILI9341_WHITE);

			memset(temp_buff,0,strlen(temp_buff));
			intToStr(manualParameters.manualCuttOffTemp,temp_buff,3);
			ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

			printState = printStateIdle;
			break;
		}

		case printGetManualTemp: {
			if(turnsDetected == 1)
			{
						turnsDetected = 0;

						if(direction == 1)
						{

							if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 0))
							{
								manualParameters.manualCuttOffTemp++;

								if(manualParameters.manualCuttOffTemp >= 200)
									manualParameters.manualCuttOffTemp = 200;

								memset(temp_buff,0,strlen(temp_buff));
								intToStr(manualParameters.manualCuttOffTemp,temp_buff,3);
								ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

							}
							else if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 1))
							{
								manualParameters.manualSetTemp++;

								if(manualParameters.manualSetTemp >= manualParameters.manualCuttOffTemp)
									manualParameters.manualSetTemp = manualParameters.manualCuttOffTemp;

								memset(temp_buff,0,strlen(temp_buff));
								intToStr(manualParameters.manualSetTemp,temp_buff,3);
								ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
							}

						}
						else if(direction == 2)
						{


							if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 0))
							{
								manualParameters.manualCuttOffTemp--;

								if((manualParameters.manualCuttOffTemp  == 255) || (manualParameters.manualCuttOffTemp <= 0 ))
										manualParameters.manualCuttOffTemp=0;

								memset(temp_buff,0,strlen(temp_buff));
								intToStr(manualParameters.manualCuttOffTemp,temp_buff,3);
								ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

							}
							else if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 1))
							{
								manualParameters.manualSetTemp--;

								if((manualParameters.manualSetTemp  == 255) || (manualParameters.manualSetTemp <= 0 ))
										manualParameters.manualSetTemp=0;

								memset(temp_buff,0,strlen(temp_buff));
								intToStr(manualParameters.manualSetTemp,temp_buff,3);
								ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
							}

						}
						HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
						//HAL_TIM_Base_Start_IT(&htim1);

				}

			printState = printStateIdle;
			break;

		}

		case printManualCookingStart: {

			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);

			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 )*10,"Manual Cooking", ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(120,( 4 * 2)*10,"Basket", ILI9341_BLACK, ILI9341_WHITE);


			ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 4 )*10,"UP", ILI9341_BLACK, ILI9341_WHITE);

			ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 4 )*10,"Down", ILI9341_RED, ILI9341_WHITE);

			printState = printStateIdle;
			break;
		}

		case printGetManualBasket: {
			if(turnsDetected == 1)
			{
				turnsDetected = 0;

				if(direction == 1)
				{
					rCount++;

					if(rCount >= 2)
						rCount = 2;

					ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 4 )*10,"UP", ILI9341_BLACK, ILI9341_WHITE);

					ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 4 )*10,"Down", ILI9341_RED, ILI9341_WHITE);

				}
				else if(direction == 2)
				{
					rCount--;

					if(rCount >= 2)
						rCount = 2;

					if((rCount  == 65535) || (rCount <= 0 ))
						rCount = 1;

					ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 4 )*10,"UP", ILI9341_RED, ILI9341_WHITE);

					ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 4 )*10,"Down", ILI9341_BLACK, ILI9341_WHITE);

				}
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				//HAL_TIM_Base_Start_IT(&htim1);

			}

			printState = printStateIdle;
			break;

		}

		case printFullMenuCount: {
			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(1);
			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 )*10,"Full Recipe Count", ILI9341_BLACK, ILI9341_WHITE);

			ee24_read(COMPLETE_RECIPE_COUNT,dummyEEprom,4,100);

			totalRecipeCount = (( dummyEEprom[0] << 24) | (dummyEEprom[1] << 16) | (dummyEEprom[2] << 8) | (dummyEEprom[3] & 0xff));

			strcpy(temp_buff,"Count - ");
			intToStr(totalRecipeCount,disp_num_buf,3);
			strcat(temp_buff,disp_num_buf);
			ILI9341_WriteString1(MENU_X_AXIS_START,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);


			ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 5 )*10,"Back", ILI9341_RED, ILI9341_WHITE);

			ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 5 )*10,"Reset", ILI9341_BLACK, ILI9341_WHITE);
			printState = printStateIdle;

			break;
		}

		case printGetMenuCountReset: {
			if(turnsDetected == 1)
			{
				turnsDetected = 0;

				if(direction == 1)
				{
					rCount++;

					if(rCount >= 2)
						rCount = 2;

					ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 5 )*10,"Back", ILI9341_BLACK, ILI9341_WHITE);

					ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 5 )*10,"Reset", ILI9341_RED, ILI9341_WHITE);

				}
				else if(direction == 2)
				{
					rCount--;

					if(rCount >= 2)
						rCount = 2;

					if((rCount  == 65535) || (rCount <= 0 ))
						rCount = 1;

					ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 5 )*10,"Back", ILI9341_RED, ILI9341_WHITE);

					ILI9341_WriteString1(MANUAL_BASKET_DOWN,( 4 * 5 )*10,"Reset", ILI9341_BLACK, ILI9341_WHITE);

				}
				HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
				//HAL_TIM_Base_Start_IT(&htim1);

			}

			printState = printStateIdle;
			break;
		}

////////////////
		case printBuf:{

			ILI9341_FillScreenOffset(ILI9341_WHITE);
			HAL_Delay(2);


			ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			printState = printStateIdle;
			break;
		}

#endif

		case print_canister_error:{

		}
		case printStateIdle: {


			break;
		}


	}

}


void canister_detect(void)
{
	if(canister_check.upma_canister == 0)
	{

	}
}



void switchDetected(void)
{
	if(ok_detected == 1)
	{

		if(currentMachineState == homeScreenInit)
		{
			if(rCount == 1)
			{
				machineState = upma_sel;
			}
			else if(rCount == 2)
			{
				machineState = poha_sel;
			}
			else if(rCount == 3)
			{
				machineState = kesari_sel;
			}
			//rCount = 1;
		}

#if 0
		else if(currentMachineState == menuPageInit)
		{
			menuNo = rCount;
			menuSelected = 1;
			rCount = 1;
//			machineState = menuRecipieSelected;

			machineState = menuPortion;
		}
		else if(currentMachineState == menuPortion)
		{
			if((Temperature_value >= portionValues[rCount - 1].portionCutTemp))
			{
/*				machineState = StartMenuCooking;
				printState = printMenuStart;
				BasketTime = HAL_GetTick();*/
				portionNoSelected = portionValues[rCount - 1].portionNo;
				machineState = recipeReadyForCooking;
				printState = printMenuReadyForCooking;

				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

				HAL_Delay(50);

				HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);
	//			printState = printMenuStart;
				portionNoSelected = portionValues[rCount - 1].portionNo;
				machineState = menuOilHeating;
				strcpy(temp_buff,"Oil Heating");

				printState = printBuf;
			}

			sleepSettings.sleepCount = 0;
			sleepFlag.countEnabled = 0;

//			portionNoSelected = portionValues[rCount - 1].portionNo;
//			machineState = recipeReadyForCooking;
//			printState = printMenuReadyForCooking;


			basketFlag = 0;
			currentMachineState = machineState;
			previousMachineState = menuPortion;
		}
		else if(currentMachineState == recipeReadyForCooking)
		{
			basketFlag = 0;
			machineState = StartMenuCooking;
			printState = printMenuStart;
			currentMachineState = machineState;
			previousMachineState = StartMenuCooking;
			BasketTime = HAL_GetTick();
		}
		else if(currentMachineState == machineSettings)
		{
			if(passwordNoOneFlag == 0)
			{
				passwordNoOneFlag = 1;
				passwordNoOne = rCount;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(passwordNoOne,temp_buff,1);
				ILI9341_WriteString1(PASSWORD_ONE_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				rCount = passwordNoTwo;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(passwordNoTwo,temp_buff,1);
				ILI9341_WriteString1(PASSWORD_TWO_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if(passwordNoOneFlag == 1)
			{
				if((passwordNoOne == DEFAULT_PASS_ONE) && (passwordNoTwo == DEFAULT_PASS_TWO))
				{
					machineState = selectMachineSettings;
					passwordNoOneFlag = 0;
				}
				else
				{
					passwordNoOneFlag = 0;
					machineState = machineSettings;
					passwordNoOne = 0;
					passwordNoTwo = 0;
					strcpy(temp_buff,"Incorrect Password");
					printState = printBuf;
				}
			}
		}
		else if(currentMachineState == recipieSettings)
		{

			if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 0)
					&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.CutOffFlag = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsMaxCutoff,temp_buff,3);
				ILI9341_WriteString1(TEMP_X_AXIS+80,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				if(settingsMaxCutoff < portionValues[portionNoSelected - 1].portionCutTemp)
					settingsTemp = settingsMaxCutoff;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTemp,temp_buff,3);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.TempFlag = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTemp,temp_buff,3);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);


				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT1,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.TimeT1 = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT1,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsHoldingTim,temp_buff,3);
				ILI9341_WriteString1(HOLDING_TIME_OFFSET,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.HoldingTime = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsHoldingTim,temp_buff,3);
				ILI9341_WriteString1(HOLDING_TIME_OFFSET,( 4 * 4)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT2,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 1)  && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0) && (settingsFlag.SaveOk == 0))
			{
				settingsFlag.TimeT2 = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT2,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 4)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_RED, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 1)  && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 1)&& (settingsFlag.SaveOk == 0))
			{
				passwordNoOneFlag = 0;
				settingsFlag.TempFlag = 0;
				settingsFlag.TimeT1 = 0;
				settingsFlag.HoldingTime = 0;
				settingsFlag.CutOffFlag = 0;
				settingsFlag.SaveOk = 0;
				settingsFlag.TimeT2 = 0;


				if((settingsTemp > 0) && (settingsMaxCutoff > 0) && (settingsTimeT1 > 0))
				{
					if(portionNoSelected == 1)
					{
						CookingMenuDetails->portion1SetTemp = settingsTemp;
						CookingMenuDetails->portion1CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion1CTime1 = settingsTimeT1;
						CookingMenuDetails->portion1HTime = settingsHoldingTim;
						CookingMenuDetails->portion1CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 2)
					{
						CookingMenuDetails->portion2SetTemp = settingsTemp;
						CookingMenuDetails->portion2CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion2CTime1 = settingsTimeT1;
						CookingMenuDetails->portion2HTime = settingsHoldingTim;
						CookingMenuDetails->portion2CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 3)
					{
						CookingMenuDetails->portion3SetTemp = settingsTemp;
						CookingMenuDetails->portion3CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion3CTime1 = settingsTimeT1;
						CookingMenuDetails->portion3HTime = settingsHoldingTim;
						CookingMenuDetails->portion3CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 4)
					{
						CookingMenuDetails->portion4SetTemp = settingsTemp;
						CookingMenuDetails->portion4CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion4CTime1 = settingsTimeT1;
						CookingMenuDetails->portion4HTime = settingsHoldingTim;
						CookingMenuDetails->portion4CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 5)
					{
						CookingMenuDetails->portion5SetTemp = settingsTemp;
						CookingMenuDetails->portion5CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion5CTime1 = settingsTimeT1;
						CookingMenuDetails->portion5HTime = settingsHoldingTim;
						CookingMenuDetails->portion5CTime2 = settingsTimeT2;
					}


					updateMenu(CookingMenuDetails);

					strcpy(temp_buff,"Settings Updated");

					printState = printBuf;

					machineState = selectMachineSettings;
				}
				else if((settingsTemp == 0) && (settingsMaxCutoff == 0) && (settingsTimeT1 == 0) && (settingsHoldingTim == 0) && (settingsTimeT2 == 0))
				{
					if(portionNoSelected == 1)
					{
						CookingMenuDetails->portion1SetTemp = settingsTemp;
						CookingMenuDetails->portion1CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion1CTime1 = settingsTimeT1;
						CookingMenuDetails->portion1HTime = settingsHoldingTim;
						CookingMenuDetails->portion1CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 2)
					{
						CookingMenuDetails->portion2SetTemp = settingsTemp;
						CookingMenuDetails->portion2CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion2CTime1 = settingsTimeT1;
						CookingMenuDetails->portion2HTime = settingsHoldingTim;
						CookingMenuDetails->portion2CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 3)
					{
						CookingMenuDetails->portion3SetTemp = settingsTemp;
						CookingMenuDetails->portion3CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion3CTime1 = settingsTimeT1;
						CookingMenuDetails->portion3HTime = settingsHoldingTim;
						CookingMenuDetails->portion3CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 4)
					{
						CookingMenuDetails->portion4SetTemp = settingsTemp;
						CookingMenuDetails->portion4CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion4CTime1 = settingsTimeT1;
						CookingMenuDetails->portion4HTime = settingsHoldingTim;
						CookingMenuDetails->portion4CTime2 = settingsTimeT2;
					}
					else if(portionNoSelected == 5)
					{
						CookingMenuDetails->portion5SetTemp = settingsTemp;
						CookingMenuDetails->portion5CutOffTemp = settingsMaxCutoff;
						CookingMenuDetails->portion5CTime1 = settingsTimeT1;
						CookingMenuDetails->portion5HTime = settingsHoldingTim;
						CookingMenuDetails->portion5CTime2 = settingsTimeT2;
					}


					updateMenu(CookingMenuDetails);

					strcpy(temp_buff,"Settings Updated");

					printState = printBuf;

					machineState = selectMachineSettings;
				}
				else
				{
					strcpy(temp_buff,"Settings Not Updated");
					ILI9341_FillScreenOffset(ILI9341_WHITE);
					HAL_Delay(2);


					ILI9341_WriteString1(MACHINE_SETTINGS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

					printState = printRecipieSettings;
				}


			}
		}
		else if(currentMachineState == selectMachineSettings )
		{
			if(rCount == 1)
			{
				machineState = selectRecipeSettings;

			}
			else if(rCount == 2)
			{
				machineState = resetRecipieCount;
			}
			else if(rCount == 3)
			{
				machineState = machineSleepSettings;

			}
			else if(rCount == 4)
			{
				machineState = deepSleepSettings;
			}
			else if(rCount == 5)
			{
				machineState = preHeatSettings;
			}
		}
		else if(currentMachineState == selectRecipeSettings)
		{
			menuNo = rCount;
			menuSelected = 1;
			rCount = 1;
			machineState = selectPortionSettings;
			//machineState = recipieSettings;
		}
		else if(currentMachineState == selectPortionSettings)
		{
			portionNoSelected = portionValues[rCount - 1].portionNo;
			machineState = recipieSettings;
		}
		else if(currentMachineState == machineSleepSettings)
		{
			if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 0) && (sleepFlag.minSaved == 0))
			{
				sleepFlag.sleepCutTempFlag = 1;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepCutTemp,temp_buff,3);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				if(getSleepSettings.sleepCutTemp < oilHeatSettings.sleepHeatCutoff)
					getSleepSettings.sleepSetTemp = getSleepSettings.sleepCutTemp;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepSetTemp,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 0))
			{
				sleepFlag.sleepSetTempFlag = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepSetTemp,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepMinutes,temp_buff,3);
				ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

			}
			else if((sleepFlag.sleepSetTempFlag == 1) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 0))
			{
				sleepFlag.minSaved = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepMinutes,temp_buff,3);
				ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_RED, ILI9341_WHITE);
			}
			else if((sleepFlag.sleepSetTempFlag == 1) && (sleepFlag.sleepCutTempFlag == 1) &&(sleepFlag.minSaved == 1))
			{

				sleepFlag.minSaved = 0;
				sleepFlag.sleepCutTempFlag = 0;
				sleepFlag.sleepSetTempFlag = 0;

				sleepSettings.sleepTimeout = MIN_TO_SEC((getSleepSettings.sleepMinutes));
				oilHeatSettings.sleepHeatSet = getSleepSettings.sleepSetTemp;
				oilHeatSettings.sleepHeatCutoff = getSleepSettings.sleepCutTemp;
				sleepSettings.sleepMinutes = getSleepSettings.sleepMinutes;


				dummyEEprom[0] = getSleepSettings.sleepMinutes;

				ee24_write(SLEEP_MIN,dummyEEprom,1,100);		  //sleep Settings

				dummyEEprom[0] = getSleepSettings.sleepSetTemp;

				ee24_write(SLEEP_SET_TMP,dummyEEprom,1,100);		  //sleep setTemp Settings

				dummyEEprom[0] = getSleepSettings.sleepCutTemp;

				ee24_write(SLEEP_CUT_TMP,dummyEEprom,1,100);		  //sleep Cut temp Settings

				strcpy(temp_buff,"Settings Updated");

				printState = printBuf;

				machineState = selectMachineSettings;
			}

		}

		else if(currentMachineState == deepSleepSettings)
		{
			if((sleepFlag.deepMinSaved == 0))
			{
				sleepFlag.deepMinSaved = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.deepSleepMin,temp_buff,3);
				ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_RED, ILI9341_WHITE);
			}
			else if((sleepFlag.deepMinSaved == 1))
			{
//				sleepFlag.deepHourSaved = 0;
				sleepFlag.deepMinSaved = 0;

				sleepSettings.deepSleepTimeout = MIN_TO_SEC((getSleepSettings.deepSleepMin));
				sleepSettings.deepSleepMin = getSleepSettings.deepSleepMin;

//				dummyEEprom[0] = getSleepSettings.deepSleepHour;
				dummyEEprom[0] = getSleepSettings.deepSleepMin;

				ee24_write(DEEP_SLEEP_MIN,dummyEEprom,1,100);		  //Deep sleep Settings

				strcpy(temp_buff,"Settings Updated");

				printState = printBuf;

				machineState = selectMachineSettings;
			}

		}

		else if(currentMachineState == preHeatSettings)
		{
			if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 0))
			{
				getoilHeatSet.preHeatCut = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getoilHeatSet.preHeatCutTemp,temp_buff,3);
				ILI9341_WriteString1(PRE_HEAT,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				if(getoilHeatSet.preHeatCutTemp < oilHeatSettings.preHeatCutTemp)
					getoilHeatSet.preHeatSetTemp = getoilHeatSet.preHeatCutTemp;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getoilHeatSet.preHeatSetTemp,temp_buff,3);
				ILI9341_WriteString1(PRE_HEAT,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 1))
			{
				getoilHeatSet.preHeatSet = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getoilHeatSet.preHeatSetTemp,temp_buff,3);
				ILI9341_WriteString1(PRE_HEAT,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				ILI9341_WriteString1(130,( 4 * 5)*10,"SAVE", ILI9341_RED, ILI9341_WHITE);
			}
			else if((getoilHeatSet.preHeatSet == 1) && (getoilHeatSet.preHeatCut == 1))
			{
				getoilHeatSet.preHeatSet = 0;
				getoilHeatSet.preHeatCut = 0;

				oilHeatSettings.preHeatSetTemp = getoilHeatSet.preHeatSetTemp;
				oilHeatSettings.preHeatCutTemp = getoilHeatSet.preHeatCutTemp;

				dummyEEprom[0] = getoilHeatSet.preHeatSetTemp;
				dummyEEprom[1] = getoilHeatSet.preHeatCutTemp;

				ee24_write(PRE_HEAT_SET_TMP,dummyEEprom,2,100);		  //Pre Heat

				strcpy(temp_buff,"Settings Updated");

				printState = printBuf;


				machineState = selectMachineSettings;
			}

		}
		else if(currentMachineState == manualCooking)			// 19th august
		{
			if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 0))
			{
				manualParameters.manualCutOffTempflag = 1;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(manualParameters.manualCuttOffTemp,temp_buff,3);
				ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);


				manualParameters.manualSetTemp = manualParameters.manualCuttOffTemp;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(manualParameters.manualSetTemp,temp_buff,3);
				ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 1 ))
			{
				manualParameters.manualSetTempflag = 1;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(manualParameters.manualSetTemp,temp_buff,3);
				ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
				ILI9341_WriteString1(OK_OFFSET,( 4 * 5)*10,"OK", ILI9341_RED, ILI9341_WHITE);
			}
			else if((manualParameters.manualSetTempflag == 1) && (manualParameters.manualCutOffTempflag == 1))
			{
				manualParameters.manualCutOffTempflag = 0;
				manualParameters.manualSetTempflag = 0;
				manualParameters.basketUpFlag = 0;
				manualParameters.basketDownFlag = 0;

				rCount = 2;

				ee24_write(MANUAL_MIN_TEMP,&manualParameters.manualSetTemp,1,200);
				ee24_write(MANUAL_MAX_TEMP,&manualParameters.manualCuttOffTemp,1,200);

				sleepSettings.sleepCount = 0;
				sleepFlag.countEnabled = 0;


				if((Temperature_value >= manualParameters.manualCuttOffTemp))
				{
					machineState = startManualCooking;
					printState = printManualCookingStart;
					currentMachineState = machineState;
					previousMachineState = manualCooking;
				}
				else
				{


					machineState = manualTempHeating;

//					machineState = startManualCooking;//
					currentMachineState = machineState;
					strcpy(temp_buff,"Oil Heating");

					printState = printBuf;
//					printState = printManualCookingStart;//

					previousMachineState = manualCooking;

					HAL_GPIO_WritePin(op_5_GPIO_Port, op_5_Pin, GPIO_PIN_SET);
					driveMotor(upDirection);

				}

			}
		}
		else if(currentMachineState == startManualCooking)
		{
			if((rCount == 1) && (manualParameters.basketUpFlag == 0))//UP
			{

				manualParameters.basketUpFlag = 1;
				manualParameters.basketDownFlag = 0;

				  strcpy(temp_buff,"Moving Up PLease Wait");

				ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				driveMotor(upDirection);

				  manualClearFlag = 1;
				  basketMoved = 0;

			}
			else if((rCount == 2) && (manualParameters.basketDownFlag == 0))
			{

				manualParameters.basketDownFlag = 1;
				manualParameters.basketUpFlag = 0;

				strcpy(temp_buff,"Moving Down PLease Wait");

				ILI9341_WriteString1(MANUAL_BASKET_UP,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);


				driveMotor(downDirection);


				manualClearFlag = 1;
				basketMoved = 0;

			}
		}
		else if(currentMachineState == resetRecipieCount)
		{
			if((rCount == 1))//Back
			{
				machineState = previousMachineState;
			}
			else if((rCount == 2))
			{
				dummyEEprom[0] =  0;
			    dummyEEprom[1] =  0;
			    dummyEEprom[2] =  0;
			    dummyEEprom[3] =  0;
			    ee24_write(COMPLETE_RECIPE_COUNT,dummyEEprom,4,100);		  //Count Value

			    strcpy(temp_buff,"Count Reset");
			    printState = printBuf;

				machineState = selectMachineSettings;
			}
		}
		else if(currentMachineState == menuRecipieCount)
		{
			menuNo = rCount;
			menuSelected = 1;
			rCount = 1;
			machineState = checkPortionCountReset;
		}
		else if(currentMachineState == checkPortionCountReset)
		{
			if((rCount == 1))//Back
			{
				machineState = previousMachineState;
			}
			else if((rCount == 2))
			{

				resetPortionCount(CookingMenuDetails);

			    strcpy(temp_buff,"Count Reset");
			    printState = printBuf;

				machineState = menuRecipieCount;
			}
		}

#endif

		ok_detected = 0;
	}
#if 0
	else if(back_detected == 1)
	{

		if(/*(currentMachineState == menuRecipieSelected) || */(currentMachineState == preHeatPage) || (currentMachineState == menuPageInit)
				|| (currentMachineState == menuPortion) /*|| (currentMachineState == menuOilHeating) || (currentMachineState == recipeReadyForCooking)*/
				|| (currentMachineState == menuRecipieCount) || (currentMachineState == WIFIsettings))
		{
			machineState = previousMachineState;
		}
		else if((currentMachineState == menuOilHeating) || (currentMachineState == recipeReadyForCooking) || (currentMachineState == selectRecipeSettings) || (currentMachineState == selectPortionSettings))
		{
			machineState = previousMachineState;
			rCount = 1;
		}
		else if(currentMachineState == machineSettings)
		{
			if(passwordNoOneFlag == 0)
			{
				machineState = previousMachineState;
				passwordNoOne = 0;
				passwordNoTwo = 0;
			}
			else if(passwordNoOneFlag == 1)
			{
				passwordNoOneFlag = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(passwordNoOne,temp_buff,1);
				ILI9341_WriteString1(PASSWORD_ONE_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				rCount = passwordNoOne;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(passwordNoTwo,temp_buff,1);
				ILI9341_WriteString1(PASSWORD_TWO_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}
		}
		else if(currentMachineState ==selectRecipeSettings)
		{
			machineState = previousMachineState;
		}
		else if(currentMachineState == recipieSettings)
		{

			if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 0)
					&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
			{
				machineState = previousMachineState;

			}
			else if((settingsFlag.TempFlag == 0) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.CutOffFlag = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsMaxCutoff,temp_buff,3);
				ILI9341_WriteString1(TEMP_X_AXIS+80,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTemp,temp_buff,3);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 0) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.TempFlag = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTemp,temp_buff,3);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT1,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 0) && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0))
			{
				settingsFlag.TimeT1 = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT1,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsHoldingTim,temp_buff,3);
				ILI9341_WriteString1(HOLDING_TIME_OFFSET,( 4 * 4)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 1)  && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 0) && (settingsFlag.SaveOk == 0))
			{
				settingsFlag.HoldingTime = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT2,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 4)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsHoldingTim,temp_buff,3);
				ILI9341_WriteString1(HOLDING_TIME_OFFSET,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
			}
			else if((settingsFlag.TempFlag == 1) && (settingsFlag.HoldingTime == 1)  && (settingsFlag.CutOffFlag == 1)
					&& (settingsFlag.TimeT1 == 1) && (settingsFlag.TimeT2 == 1)&& (settingsFlag.SaveOk == 0))
			{

				settingsFlag.TimeT2 = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(settingsTimeT2,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 4)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);
			}
		}

		else if(currentMachineState == selectMachineSettings)
		{
			machineState = previousMachineState;
			passwordNoOneFlag = 0;
			passwordNoOne = 0;
			passwordNoTwo = 0;
		}
		else if(currentMachineState == preHeatSettings)
		{
			if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 0))
			{
				machineState = previousMachineState;
			}
			else if((getoilHeatSet.preHeatSet == 0) && (getoilHeatSet.preHeatCut == 1))
			{
				getoilHeatSet.preHeatCut = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getoilHeatSet.preHeatCutTemp,temp_buff,3);
				ILI9341_WriteString1(PRE_HEAT,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getoilHeatSet.preHeatSetTemp,temp_buff,3);
				ILI9341_WriteString1(PRE_HEAT,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}
			else if((getoilHeatSet.preHeatSet == 1) && (getoilHeatSet.preHeatCut == 1))
			{
				getoilHeatSet.preHeatSet = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getoilHeatSet.preHeatSetTemp,temp_buff,3);
				ILI9341_WriteString1(PRE_HEAT,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
				ILI9341_WriteString1(130,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);
			}

		}
		else if(currentMachineState == machineSleepSettings)
		{
			if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 0) && (sleepFlag.minSaved == 0))
			{
				machineState = previousMachineState;
			}
			else if((sleepFlag.sleepSetTempFlag == 0) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 0))
			{
				sleepFlag.sleepCutTempFlag = 0;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepCutTemp,temp_buff,3);
				ILI9341_WriteString1(MENU_X_AXIS_TEMP+88,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepSetTemp,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 2)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);

			}
			else if((sleepFlag.sleepSetTempFlag == 1) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 0))
			{
				sleepFlag.sleepSetTempFlag = 0;

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepSetTemp,temp_buff,3);
				ILI9341_WriteString1(Time_X_AXIS,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepMinutes,temp_buff,3);
				ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}

			else if((sleepFlag.sleepSetTempFlag == 1) && (sleepFlag.sleepCutTempFlag == 1) && (sleepFlag.minSaved == 1))
			{
				sleepFlag.minSaved = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.sleepMinutes,temp_buff,3);
				ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);
			}
		}
		else if(currentMachineState == deepSleepSettings)
		{

			if((sleepFlag.deepMinSaved == 0))
			{
				machineState = previousMachineState;
			}

			else if((sleepFlag.deepMinSaved == 1))
			{
				sleepFlag.deepMinSaved = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(getSleepSettings.deepSleepMin,temp_buff,3);
				ILI9341_WriteString1(SLEEP_MIN_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
				ILI9341_WriteString1(MENU_START_BUTTON,( 4 * 5)*10,"SAVE", ILI9341_BLACK, ILI9341_WHITE);
			}

		}
		else if(currentMachineState == manualCooking)
		{
			if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 0))
			{
				machineState = previousMachineState;
				manualParameters.basketUpFlag = 0;
				manualParameters.basketDownFlag = 0;
//				machineState = homeScreenInit;
				driveMotor(upDirection);
			}
			else if((manualParameters.manualSetTempflag == 0) && (manualParameters.manualCutOffTempflag == 1))
			{
				manualParameters.manualCutOffTempflag = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(manualParameters.manualCuttOffTemp,temp_buff,3);
				ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 2)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

				memset(temp_buff,0,strlen(temp_buff));
				intToStr(manualParameters.manualSetTemp,temp_buff,3);
				ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_BLACK, ILI9341_WHITE);
			}
			else if((manualParameters.manualSetTempflag == 1) && (manualParameters.manualCutOffTempflag == 1))
			{
				manualParameters.manualSetTempflag = 0;
				memset(temp_buff,0,strlen(temp_buff));
				intToStr(manualParameters.manualSetTemp,temp_buff,3);
				ILI9341_WriteString1(MANUAL_TEMP_OFFSET,( 4 * 3)*10,temp_buff, ILI9341_RED, ILI9341_WHITE);
				ILI9341_WriteString1(OK_OFFSET,( 4 * 5)*10,"OK", ILI9341_BLACK, ILI9341_WHITE);
			}
		}
		else if(currentMachineState == startManualCooking)
		{

			machineState = previousMachineState;
			manualClearFlag = 0;
			basketMoved = 0;
		}
		else if(currentMachineState == manualTempHeating)
		{
			machineState = manualCooking;
		}
		else if((currentMachineState == checkPortionCountReset))
		{
			machineState = previousMachineState;
		}

		back_detected = 0;
	}
	else if(home_detected == 1)
	{
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
		//HAL_TIM_Base_Start_IT(&htim1);
		if(currentMachineState == homeScreenInit)
		{
			preHeatFlag = 1;
			currentMachineState = homeScreenInit;
			machineState = MachineIdle;
			previousMachineState = homeScreenInit;
//			rCount = 1;
			passwordNoOneFlag = 0;
			passwordNoOne = 0;
			passwordNoTwo = 0;
			sleepSettings.sleepCount = 0;
			sleepFlag.countEnabled = 1;
			sleepFlag.deepSleepEnabled = 0;
			sleepSettings.deepSleepCount = 0;
			basketFlag = 0;
			home_detected = 0;

			sleepSettings.sleepTimeout = MIN_TO_SEC((sleepSettings.sleepMinutes));

			sleepSettings.deepSleepTimeout = MIN_TO_SEC((sleepSettings.deepSleepMin));
		}
		else if((machineState == machineDeepSleep))
		{
			ILI9341_FillScreenTemp1(ILI9341_WHITE);
			machineState = homeScreenInit;
			rCount = 1;

			passwordNoOneFlag = 0;
			passwordNoOne = 0;
			passwordNoTwo = 0;
		}
		else if((machineState == MachineIdle) || (machineState == machineSleepMode)
				/*|| (machineState == machineDeepSleep)*/ || (currentMachineState == selectMachineSettings)
				|| (machineState == preHeatPage) || (currentMachineState == menuOilHeating) || (currentMachineState == manualTempHeating))
		{
			machineState = homeScreenInit;
			rCount = 1;

			passwordNoOneFlag = 0;
			passwordNoOne = 0;
			passwordNoTwo = 0;
		}
		else if(currentMachineState == startManualCooking)
		{
			manualParameters.basketUpFlag = 0;
			manualParameters.basketDownFlag = 0;
			machineState = homeScreenInit;
			driveMotor(upDirection);
			manualClearFlag = 0;
		}
		home_detected = 0;
	}
#endif
}



