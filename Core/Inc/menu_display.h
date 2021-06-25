/*
 * menu_display.h
 *
 *  Created on: Jul 8, 2020
 *      Author: CEPL
 */

#ifndef INC_MENU_DISPLAY_H_
#define INC_MENU_DISPLAY_H_
#include "main.h"
#include "menu.h"



#define PRE_HEAT_CUTOFF_MIN		190
#define PRE_HEAT_CUTOFF_MAX		195

#define K_TYPE_TEMP_VALUE		230
#define K_TYPE_Y_AXIS			0


#define MENU_X_AXIS_START		40
#define MENU_X_AXIS_CENTRE		100
#define MENU_X_AXIS_TEMP		20		//temp
#define MENU_X_AXIS_TIME		200		//time
#define MENU_Y_AXIS_TIME		3		// time y axis

#define MENU_START_BUTTON		120

#define PASSWORD_ONE_OFFSET		135
#define PASSWORD_TWO_OFFSET		157

#define DEFAULT_PASS_ONE		1
#define DEFAULT_PASS_TWO		1







//Settings Offset
#define MACHINE_SETTINGS		80




#define TEMP_X_AXIS				75
#define Time_X_AXIS				277
#define HOLDING_TIME_OFFSET		MENU_X_AXIS_TEMP+(11*3)



#define ERROR_OFFSET			210



#define SLEEP_HOUR_OFFSET		MENU_X_AXIS_CENTRE+(11*11)
#define SLEEP_MIN_OFFSET		MACHINE_SETTINGS+(11*10)



#define	COOKING_TEMP_VALUE		50


#define DISPLAY_PAGE_SIZE		6
#define Y_AXIS_SPACING			3


#define HOUR_TO_MIN(x)			x*60

#define MIN_TO_SEC(x)			x*60


#define SLEEP_OIL_TEMP_MAX		155
#define SLEEP_OIL_TEMP_MIN		150

#define PRE_HEAT				100+(11*7)


// 19th august
#define MANUAL_TEMP				90
#define MANUAL_TEMP_OFFSET		189
#define OK_OFFSET				142
#define MANUAL_BASKET_UP		40
#define MANUAL_BASKET_DOWN		220


extern volatile uint16_t Temperature_value;
typedef enum
{
	printLogo,
	water_heating,
	water_temp_check,
	water_heated,
	homeScreenInit,
	preHeatPage,

	waitForOK,

	upma_sel,
	kesari_sel,
	poha_sel,

	displayCnt,
	reset_eeprom,
#if 0
	/*Cooking Recipe*/
	menuPageInit,
	menuPortion,
//	menuRecipieSelected,
	menuOilHeating,
	StartMenuCooking,
	CheckMenuCookingTimeOut,
	recipeReadyForCooking,

	/*Manual Cooking*/
	manualCooking,
	manualTempHeating,
	startManualCooking,

	/*Recipe Count*/
	menuRecipieCount,
	checkPortionCountReset,

	/*WiFi Settings*/
	WIFIsettings,

	/*Machine Settings*/
	machineSettings,
//	passwordPage,
	selectMachineSettings,
	selectRecipeSettings,
	selectPortionSettings,
	recipieSettings,
	incorrectPasswordPage,
	machineSleepSettings,
	resetRecipieCount,
	deepSleepSettings,
	preHeatSettings,

	/*Sleep Modes*/
	machineSleepMode,
	machineDeepSleep,

	/*Stop Cooking*/
	menuStopPressed,
#endif
	/*Idle*/
	MachineIdle,
}__homeScreenPage;


typedef enum
{
	printHomeScreenInit,
	printHomeScreenBold,

	printMenuScreenPage,
	printMenuScreenBold,

#if 0
//	printMenuSelected,
	printMenuReadyForCooking, 		//26 August
	printMenuStart,
	printMenuCookingState,
	printMenuPortion,
	printGetMenuPortion,

	printMenuPortionCount,

	printWiFiSettings,

	printMachineSettings,
	printPassword,
	printIncorrectPassword,
	printSelectSettings,
	printScrollSettings,
	printSelectRecipeSettings,
	printRecipieSettings,
	printGetSettings,

	printFullMenuCount,
	printGetMenuCountReset,

	printSleepSettings,
	printGetSleepSettings,

	printDeepSleepSettings,
	printGetDeepSleepSettings,

	prPreHeat,
	prGetPreHeat,

	printSleepMode,
	printDeepSleep,

	printBuf,

	/*Manual*/
	printManualTemp,
	printGetManualTemp,
	printManualCookingStart,
	printGetManualBasket,
#endif

	print_canister_error,
	printStateIdle,
}__PrintDisplayState;


typedef struct
{
	uint8_t TempFlag:1;
	uint8_t CutOffFlag:1;
	uint8_t TimeT1:1;
	uint8_t HoldingTime:1;
	uint8_t TimeT2:1;
	uint8_t SaveOk:1;
}__RecipieSettingsFlag;



typedef struct
{
	uint8_t page1Flag:1;
	uint8_t page2Flag:1;
//	uint8_t
}__PAGE_FLAGS;


typedef struct
{
	uint32_t sleepTimeout;
	uint16_t sleepMinutes;
	volatile uint32_t sleepCount;
	uint32_t deepSleepTimeout;
	uint16_t deepSleepMin;
	volatile uint32_t deepSleepCount;
	uint8_t  sleepSetTemp;
	uint8_t  sleepCutTemp;
}__Sleep_Settings;

typedef struct
{
	uint8_t hourSaved:1;
	uint8_t minSaved:1;
	uint8_t sleepSetTempFlag:1;
	uint8_t sleepCutTempFlag:1;
	uint8_t countEnabled:1;
	uint8_t deepHourSaved:1;
	uint8_t deepMinSaved:1;
	uint8_t deepSleepEnabled:1;
}__Sleep_Flags;

typedef struct
{
	volatile uint16_t cookingTime;
	volatile uint8_t  cookingEnabled:1;
	volatile uint8_t  checkDoubleDip:1;
	volatile uint8_t  cookingCompleted:1;
	uint8_t  cookingT1Flag:1;
	uint8_t  checkHolding:1;
	uint8_t  secondCooking:1;
	uint8_t  printHolding:1;
	uint8_t  printTimeT2:1;
}__Cooking_Settings;




typedef struct
{
	uint8_t preHeatSetTemp;
	uint8_t preHeatCutTemp;
	uint8_t sleepHeatSet;
	uint8_t sleepHeatCutoff;
	uint8_t preHeatSet:1;
	uint8_t preHeatCut:1;
	uint8_t slpreHeatSet:1;
	uint8_t slpreHeatCut:1;
}__OIL_HEAT_SETTINGS;



typedef struct
{
	uint8_t manualSetTemp;
	uint8_t manualCuttOffTemp;
	uint8_t manualSetTempflag:1;
	uint8_t manualCutOffTempflag:1;
	uint8_t basketUpFlag:1;
	uint8_t basketDownFlag:1;
}__MANUAL_;


typedef struct
{
	uint8_t noOfPortions;
	uint8_t Portion1Flag:1;
	uint8_t Portion2Flag:1;
	uint8_t Portion3Flag:1;
	uint8_t Portion4Flag:1;
	uint8_t Portion5Flag:1;
}__PORTIONS__;


typedef struct
{
	uint8_t portionNo;
	uint8_t portionSetTemp;
	uint8_t portionCutTemp;
	uint16_t portionT1;
	uint16_t portionT2;
	uint16_t portionHT;
}__PORTION_MENU__;


void displayProcess(void);
void DisplayPrint(void);
void MachineProcess(void);



void switchDetected(void);

#endif /* INC_MENU_DISPLAY_H_ */
