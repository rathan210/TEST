/*
 * menu.h
 *
 *  Created on: 06-Jul-2020
 *      Author: CEPL
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "main.h"

#if 0
struct Menu
{
	char name[15];
	uint8_t menuID;
	uint8_t cookingTemp;
	uint8_t cutOffTemp; // Max Cut off
	uint16_t cookingTimeT1;
	uint16_t cookingTimeT2;
	uint16_t holdingTime;
	uint32_t cookingCount;
	struct Menu * next;
};

#endif

struct Menu
{
	char name[15];
	uint8_t menuID;

	uint8_t portion1SetTemp;
	uint8_t portion1CutOffTemp;
	uint16_t portion1CTime1;
	uint16_t portion1CTime2;
	uint16_t portion1HTime;
	uint16_t portion1Count;

	uint8_t portion2SetTemp;
	uint8_t portion2CutOffTemp;
	uint16_t portion2CTime1;
	uint16_t portion2CTime2;
	uint16_t portion2HTime;
	uint16_t portion2Count;

	uint8_t portion3SetTemp;
	uint8_t portion3CutOffTemp;
	uint16_t portion3CTime1;
	uint16_t portion3CTime2;
	uint16_t portion3HTime;
	uint16_t portion3Count;

	uint8_t portion4SetTemp;
	uint8_t portion4CutOffTemp;
	uint16_t portion4CTime1;
	uint16_t portion4CTime2;
	uint16_t portion4HTime;
	uint16_t portion4Count;

	uint8_t portion5SetTemp;
	uint8_t portion5CutOffTemp;
	uint16_t portion5CTime1;
	uint16_t portion5CTime2;
	uint16_t portion5HTime;
	uint16_t portion5Count;

	//uint32_t cookingCount;
	struct Menu * next;
	struct Menu * prev; // Pointer to previous node
};





#define DEFAULT_COOKING_TEMP		180		//degree
#define DEFAULT_CUTOFF_TEMP			175		//degree
#define DEFAULT_COOKING_T1			5		//mins
#define DEFAULT_COOKING_T2			5		//mins
#define DEFAULT_HOLDING_TIME		1		//min

#define DEFAULT_MENU_ID_1			0x1
#define DEFAULT_MENU_ID_2			0x2
#define DEFAULT_MENU_ID_3			0x3
#define DEFAULT_MENU_ID_4			0x4
#define DEFAULT_MENU_ID_5			0x5


#define ESP_MENU_BYTE_SIZE			64

#define ESP_EEPROM_HEAD				7201
#define ESP_EEPROM_TAIL				7202

#define ESP_EEPROM_BASE_ADDR		7168

#define ESP_MENU_EEPROM_START(x)	ESP_EEPROM_BASE_ADDR+(ESP_MENU_BYTE_SIZE*x)





typedef struct
{
 	uint8_t head;
 	uint8_t tail;
 	uint8_t maxLen;
}__esp_queue;

#define CIRCQUEUE_DEF(x,y)          \
		__esp_queue x = {               \
       .head = 0,                \
       .tail = 0,                \
       .maxLen = y               \
   }

typedef struct{
	uint8_t espUdateQueueBuf[33];
}__ESP_UPDATE;






#define MENU_1_NAME					"momos"
#define MENU_2_NAME					"FrenchFries"
#define MENU_3_NAME					"nuggets"



void addMenu(struct Menu** head_ref,const char *RecipieName,const uint8_t * Parameters);
void printList(struct Menu *node);
void delete(struct Menu** head_ref,uint16_t menuID);
void addMenuUart(struct Menu** head_ref,const uint8_t *RecipieName);

void getMenu(struct Menu** head_ref);

void updateEeprom(struct Menu** head_ref);

uint8_t pushESPData(__esp_queue * qptr,uint8_t *data);
uint8_t updateESPData(__esp_queue * qptr);

void updateMenu(struct Menu *RecipieMenu);

void resetPortionCount(struct Menu *RecipieMenu);


#endif /* INC_MENU_H_ */
