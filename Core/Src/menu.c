/*
 * menu.c
 *
 *  Created on: 06-Jul-2020
 *      Author: CEPL
 */

#include "menu.h"
#include "stdlib.h"

extern uint8_t menuNo,menuSelected,portionNoSelected;

const uint8_t menuParameters[][26] = {
		{1,180,185,150,0,0,180,185,180,0,0,180,185,165,0,0,180,185,180,0,0,180,185,60,0,0},
		{2,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0},
		{3,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0},
		{4,180,185,30,0,0,180,185,0,0,0,180,185,0,0,0,180,185,50,0,0,180,185,60,0,0},
		{5,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0},
		{6,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0},
		{7,180,185,30,0,0,180,185,0,0,0,180,185,0,0,0,180,185,50,0,0,180,185,60,0,0},
		{8,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0},
		{9,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0,180,185,30,0,0}
};


#if 0
void addMenu(struct Menu** head_ref,const char *RecipieName,uint16_t menuID,uint16_t cookingTemp,uint16_t cutOffTemp,uint16_t cookingTimeT1,
		uint16_t cookingTimeT2,uint16_t holdingTime)
{
	int i =0;
	recipieCount++;
	uint16_t Menu_base_addr = 0;
	uint8_t nameSize = 0;
	uint8_t dummyBuf[6];
    /* 1. allocate node */
    struct Menu* newMenu = (struct Menu*) malloc(sizeof(struct Menu));

    struct Menu *lastMenu = *head_ref;  /* used in step 5*/

    memset(newMenu->name,0,sizeof(newMenu->name));

    Menu_base_addr = RECIPIE_BASE_ADDR(recipieCount);

    /* 2. put in the data  */
    while(*RecipieName != '\0')
    {
    	newMenu->name[i]  = *(RecipieName);
    	i++;
    	RecipieName++;
    }

    newMenu->menuID = menuID;
    newMenu->cookingTemp = cookingTemp;
    newMenu->cutOffTemp = cutOffTemp;
    newMenu->cookingTimeT1 = cookingTimeT1;
    newMenu->cookingTimeT2 = cookingTimeT2;
    newMenu->holdingTime = holdingTime;


    ee24_write(Menu_base_addr+MENU_ID_ADDR,&newMenu->menuID,1,100); //id

    nameSize = sizeof(newMenu->name);
    ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)newMenu->name,nameSize,100); //name
    ee24_write(Menu_base_addr+MENU_NAME_SIZE,&nameSize,1,100);

    ee24_write(Menu_base_addr+MENU_COOKING_TEMP_ADDR,&newMenu->cookingTemp,1,100);   //cooking temp

    ee24_write(Menu_base_addr+MENU_CUTOFF_TEMP_ADDR,&newMenu->cutOffTemp,1,100);     // Cutoff Temp

    dummyBuf[0] = newMenu->cookingTimeT1 >> 8;
    dummyBuf[1] = newMenu->cookingTimeT1 & 0xff;
    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1,dummyBuf,2,100);    //Cooking time t1
//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1+1,(uint8_t*)(newMenu->cookingTimeT1 &0xff),1,100);

    dummyBuf[0] = newMenu->cookingTimeT2 >> 8;
    dummyBuf[1] = newMenu->cookingTimeT2 & 0xff;
    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2,dummyBuf,2,100);    //Cooking time t2
//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2+1,(uint8_t*)(newMenu->cookingTimeT2 &0xff),1,100);

    dummyBuf[0] = newMenu->holdingTime >> 8;
    dummyBuf[1] = newMenu->holdingTime & 0xff;
    ee24_write(Menu_base_addr+MENU_HOLDING_TIME,dummyBuf,2,100);		  //Holding time
//    ee24_write(Menu_base_addr+MENU_HOLDING_TIME+1,(uint8_t*)(newMenu->holdingTime &0xff),1,100);

	dummyBuf[0] = 0;
	dummyBuf[1] = 0;
	dummyBuf[2] = 0;
	dummyBuf[3] = 0;
	ee24_write(Menu_base_addr+MENU_COUNT_ADDR,dummyBuf,4,100);


    ee24_write(MENU_COUNT,&recipieCount,1,100);
    /* 3. This new node is going to be the last node, so make next of
          it as NULL*/
    newMenu->next = NULL;

    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
       *head_ref = newMenu;
       return;
    }

    /* 5. Else traverse till the last node */
    while (lastMenu->next != NULL)
    	lastMenu = lastMenu->next;

    /* 6. Change the next of last node */
    lastMenu->next = newMenu;
    return;
}

#endif

void addMenu(struct Menu** head_ref,const char *RecipieName,const uint8_t * menuParameters)
{
	int i =0;
	recipieCount++;
	uint16_t Menu_base_addr = 0;
	uint8_t dummyBuf[6];
    /* 1. allocate node */
    struct Menu* newMenu = (struct Menu*) malloc(sizeof(struct Menu));



    memset(newMenu->name,0,sizeof(newMenu->name));

    Menu_base_addr = RECIPIE_BASE_ADDR(recipieCount);

    /* 2. put in the data  */
    while(*RecipieName != '\0')
    {
    	newMenu->name[i]  = *(RecipieName);
    	i++;
    	RecipieName++;
    }

    newMenu->menuID = menuParameters[0];

    ee24_write(Menu_base_addr+MENU_ID_ADDR,&newMenu->menuID,1,100); //id

    ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)newMenu->name,15,100); //name


    /***********************************Portion 1************************************************************/

	newMenu->portion1SetTemp = menuParameters[1];
	newMenu->portion1CutOffTemp = menuParameters[2];

    ee24_write(Menu_base_addr+MENU_PORTION1_SET_TEMP,&newMenu->portion1SetTemp,1,100);   //cooking temp

    ee24_write(Menu_base_addr+MENU_PORTION1_CUTOFF_TEMP,&newMenu->portion1CutOffTemp,1,100);     // Cutoff Temp

    newMenu->portion1CTime1 = menuParameters[3];

    dummyBuf[0] = newMenu->portion1CTime1 >> 8;
    dummyBuf[1] = newMenu->portion1CTime1 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

    newMenu->portion1CTime2 = menuParameters[4];

    dummyBuf[0] = newMenu->portion1CTime2 >> 8;
    dummyBuf[1] = newMenu->portion1CTime2 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

    newMenu->portion1HTime = menuParameters[5];

    dummyBuf[0] = newMenu->portion1HTime >> 8;
    dummyBuf[1] = newMenu->portion1HTime & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION1_H_TIME,dummyBuf,2,100);		  //Holding time

    newMenu->portion1Count = 0;												// Count

	dummyBuf[0] = 0;
	dummyBuf[1] = 0;
	ee24_write(Menu_base_addr+MENU_PORTION1_COUNT,dummyBuf,2,100);

    /***********************************Portion 2************************************************************/

	newMenu->portion2SetTemp = menuParameters[6];
	newMenu->portion2CutOffTemp = menuParameters[7];

    ee24_write(Menu_base_addr+MENU_PORTION2_SET_TEMP,&newMenu->portion2SetTemp,1,100);   //cooking temp

    ee24_write(Menu_base_addr+MENU_PORTION2_CUTOFF_TEMP,&newMenu->portion2CutOffTemp,1,100);     // Cutoff Temp

    newMenu->portion2CTime1 = menuParameters[8];

    dummyBuf[0] = newMenu->portion2CTime1 >> 8;
    dummyBuf[1] = newMenu->portion2CTime1 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

    newMenu->portion2CTime2 = menuParameters[9];

    dummyBuf[0] = newMenu->portion2CTime2 >> 8;
    dummyBuf[1] = newMenu->portion2CTime2 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

    newMenu->portion2HTime = menuParameters[10];

    dummyBuf[0] = newMenu->portion2HTime >> 8;
    dummyBuf[1] = newMenu->portion2HTime & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION2_H_TIME,dummyBuf,2,100);		  //Holding time

    newMenu->portion2Count = 0;												// Count

	dummyBuf[0] = 0;
	dummyBuf[1] = 0;
	ee24_write(Menu_base_addr+MENU_PORTION2_COUNT,dummyBuf,2,100);


    /***********************************Portion 3************************************************************/

	newMenu->portion3SetTemp = menuParameters[11];
	newMenu->portion3CutOffTemp = menuParameters[12];

    ee24_write(Menu_base_addr+MENU_PORTION3_SET_TEMP,&newMenu->portion3SetTemp,1,100);   //cooking temp

    ee24_write(Menu_base_addr+MENU_PORTION3_CUTOFF_TEMP,&newMenu->portion3CutOffTemp,1,100);     // Cutoff Temp

    newMenu->portion3CTime1 = menuParameters[13];

    dummyBuf[0] = newMenu->portion3CTime1 >> 8;
    dummyBuf[1] = newMenu->portion3CTime1 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

    newMenu->portion3CTime2 = menuParameters[14];

    dummyBuf[0] = newMenu->portion3CTime2 >> 8;
    dummyBuf[1] = newMenu->portion3CTime2 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

    newMenu->portion3HTime = menuParameters[15];

    dummyBuf[0] = newMenu->portion3HTime >> 8;
    dummyBuf[1] = newMenu->portion3HTime & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION3_H_TIME,dummyBuf,2,100);		  //Holding time

    newMenu->portion3Count = 0;												// Count

	dummyBuf[0] = 0;
	dummyBuf[1] = 0;
	ee24_write(Menu_base_addr+MENU_PORTION3_COUNT,dummyBuf,2,100);


    /***********************************Portion 4************************************************************/

	newMenu->portion4SetTemp = menuParameters[16];
	newMenu->portion4CutOffTemp = menuParameters[17];

    ee24_write(Menu_base_addr+MENU_PORTION4_SET_TEMP,&newMenu->portion4SetTemp,1,100);   //cooking temp

    ee24_write(Menu_base_addr+MENU_PORTION4_CUTOFF_TEMP,&newMenu->portion4CutOffTemp,1,100);     // Cutoff Temp

    newMenu->portion4CTime1 = menuParameters[18];

    dummyBuf[0] = newMenu->portion4CTime1 >> 8;
    dummyBuf[1] = newMenu->portion4CTime1 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

    newMenu->portion4CTime2 = menuParameters[19];

    dummyBuf[0] = newMenu->portion4CTime2 >> 8;
    dummyBuf[1] = newMenu->portion4CTime2 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

    newMenu->portion4HTime = menuParameters[20];

    dummyBuf[0] = newMenu->portion4HTime >> 8;
    dummyBuf[1] = newMenu->portion4HTime & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION4_H_TIME,dummyBuf,2,100);		  //Holding time

    newMenu->portion4Count = 0;												// Count

	dummyBuf[0] = 0;
	dummyBuf[1] = 0;
	ee24_write(Menu_base_addr+MENU_PORTION4_COUNT,dummyBuf,2,100);


    /***********************************Portion 5************************************************************/

	newMenu->portion5SetTemp = menuParameters[21];
	newMenu->portion5CutOffTemp = menuParameters[22];

    ee24_write(Menu_base_addr+MENU_PORTION5_SET_TEMP,&newMenu->portion5SetTemp,1,100);   //cooking temp

    ee24_write(Menu_base_addr+MENU_PORTION5_CUTOFF_TEMP,&newMenu->portion5CutOffTemp,1,100);     // Cutoff Temp

    newMenu->portion5CTime1 = menuParameters[23];

    dummyBuf[0] = newMenu->portion5CTime1 >> 8;
    dummyBuf[1] = newMenu->portion5CTime1 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

    newMenu->portion5CTime2 = menuParameters[24];

    dummyBuf[0] = newMenu->portion5CTime2 >> 8;
    dummyBuf[1] = newMenu->portion5CTime2 & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

    newMenu->portion5HTime = menuParameters[25];

    dummyBuf[0] = newMenu->portion5HTime >> 8;
    dummyBuf[1] = newMenu->portion5HTime & 0xff;
    ee24_write(Menu_base_addr+MENU_PORTION5_H_TIME,dummyBuf,2,100);		  //Holding time

    newMenu->portion5Count = 0;												// Count

	dummyBuf[0] = 0;
	dummyBuf[1] = 0;
	ee24_write(Menu_base_addr+MENU_PORTION5_COUNT,dummyBuf,2,100);


    ee24_write(MENU_COUNT,&recipieCount,1,100);
    /* 3. This new node is going to be the last node, so make next of
          it as NULL*/
//    newMenu->next = NULL;

    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
    	newMenu->next = newMenu->prev = newMenu;
       *head_ref = newMenu;
       return;
    }

    struct Menu *lastMenu = (*head_ref)->prev;  /* used in step 5*/

    newMenu->next = *head_ref;

    (*head_ref)->prev = newMenu;

    newMenu->prev = lastMenu;

    lastMenu->next = newMenu;

/*     5. Else traverse till the last node
    while (lastMenu->next != NULL)
    	lastMenu = lastMenu->next;

     6. Change the next of last node
    lastMenu->next = newMenu;*/
    return;
}



void getMenu(struct Menu** head_ref)
{
	int i =0;
	ee24_read(MENU_COUNT,&recipieCount,1,100);
	if(recipieCount == 255)
	{
		recipieCount = 0;
	}
	uint16_t Menu_base_addr = 0;
//	uint8_t nameSize = 0;
	uint8_t dummybuff[5];

//	*head_ref = NULL;
    /* 1. allocate node */

	for(i = 1; i <= recipieCount ; i++)
	{
//		  sprintf(temp_buff,"%d",i);
//		  ILI9341_WriteString1(120, 14*10,temp_buff, ILI9341_RED, ILI9341_WHITE);

		Menu_base_addr = RECIPIE_BASE_ADDR(i);
	    struct Menu* newMenu = (struct Menu*) malloc(sizeof(struct Menu));



	    memset(newMenu->name,0,sizeof(newMenu->name));

	    ee24_read(Menu_base_addr+MENU_ID_ADDR,&newMenu->menuID,1,100); //id

	    ee24_read(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)newMenu->name,15,100); //name


	    /***********************************Portion 1************************************************************/


	    ee24_read(Menu_base_addr+MENU_PORTION1_SET_TEMP,&newMenu->portion1SetTemp,1,100);   //cooking temp

	    ee24_read(Menu_base_addr+MENU_PORTION1_CUTOFF_TEMP,&newMenu->portion1CutOffTemp,1,100);     // Cutoff Temp


	    ee24_read(Menu_base_addr+MENU_PORTION1_C_TIME_T1,dummybuff,2,100);    //Cooking time t1
	    newMenu->portion1CTime1 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION1_C_TIME_T2,dummybuff,2,100);    //Cooking time t2
	    newMenu->portion1CTime2 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION1_H_TIME,dummybuff,2,100);		  //Holding time
	    newMenu->portion1HTime = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION1_COUNT,dummybuff,2,100);		  //Count value
	    newMenu->portion1Count = (dummybuff[0] << 8) | (dummybuff[1]);


	    /***********************************Portion 2************************************************************/


	    ee24_read(Menu_base_addr+MENU_PORTION2_SET_TEMP,&newMenu->portion2SetTemp,1,100);   //cooking temp

	    ee24_read(Menu_base_addr+MENU_PORTION2_CUTOFF_TEMP,&newMenu->portion2CutOffTemp,1,100);     // Cutoff Temp


	    ee24_read(Menu_base_addr+MENU_PORTION2_C_TIME_T1,dummybuff,2,100);    //Cooking time t1
	    newMenu->portion2CTime1 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION2_C_TIME_T2,dummybuff,2,100);    //Cooking time t2
	    newMenu->portion2CTime2 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION2_H_TIME,dummybuff,2,100);		  //Holding time
	    newMenu->portion2HTime = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION2_COUNT,dummybuff,2,100);		  //Count value
	    newMenu->portion2Count = (dummybuff[0] << 8) | (dummybuff[1]);


	    /***********************************Portion 3************************************************************/


	    ee24_read(Menu_base_addr+MENU_PORTION3_SET_TEMP,&newMenu->portion3SetTemp,1,100);   //cooking temp

	    ee24_read(Menu_base_addr+MENU_PORTION3_CUTOFF_TEMP,&newMenu->portion3CutOffTemp,1,100);     // Cutoff Temp


	    ee24_read(Menu_base_addr+MENU_PORTION3_C_TIME_T1,dummybuff,2,100);    //Cooking time t1
	    newMenu->portion3CTime1 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION3_C_TIME_T2,dummybuff,2,100);    //Cooking time t2
	    newMenu->portion3CTime2 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION3_H_TIME,dummybuff,2,100);		  //Holding time
	    newMenu->portion3HTime = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION3_COUNT,dummybuff,2,100);		  //Count value
	    newMenu->portion3Count = (dummybuff[0] << 8) | (dummybuff[1]);


	    /***********************************Portion 4************************************************************/

	    ee24_read(Menu_base_addr+MENU_PORTION4_SET_TEMP,&newMenu->portion4SetTemp,1,100);   //cooking temp

	    ee24_read(Menu_base_addr+MENU_PORTION4_CUTOFF_TEMP,&newMenu->portion4CutOffTemp,1,100);     // Cutoff Temp


	    ee24_read(Menu_base_addr+MENU_PORTION4_C_TIME_T1,dummybuff,2,100);    //Cooking time t1
	    newMenu->portion4CTime1 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION4_C_TIME_T2,dummybuff,2,100);    //Cooking time t2
	    newMenu->portion4CTime2 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION4_H_TIME,dummybuff,2,100);		  //Holding time
	    newMenu->portion4HTime = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION4_COUNT,dummybuff,2,100);		  //Count value
	    newMenu->portion4Count = (dummybuff[0] << 8) | (dummybuff[1]);


	    /***********************************Portion 5************************************************************/

	    ee24_read(Menu_base_addr+MENU_PORTION5_SET_TEMP,&newMenu->portion5SetTemp,1,100);   //cooking temp

	    ee24_read(Menu_base_addr+MENU_PORTION5_CUTOFF_TEMP,&newMenu->portion5CutOffTemp,1,100);     // Cutoff Temp


	    ee24_read(Menu_base_addr+MENU_PORTION5_C_TIME_T1,dummybuff,2,100);    //Cooking time t1
	    newMenu->portion5CTime1 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION5_C_TIME_T2,dummybuff,2,100);    //Cooking time t2
	    newMenu->portion5CTime2 = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION5_H_TIME,dummybuff,2,100);		  //Holding time
	    newMenu->portion5HTime = (dummybuff[0] << 8) | dummybuff[1];

	    ee24_read(Menu_base_addr+MENU_PORTION5_COUNT,dummybuff,2,100);		  //Count value
	    newMenu->portion5Count = (dummybuff[0] << 8) | (dummybuff[1]);


	    /* 3. This new node is going to be the last node, so make next of
	          it as NULL*/
//	    newMenu->next = NULL;

	    /* 4. If the Linked List is empty, then make the new node as head */
//	    if (*head_ref == NULL)
//	    {
//	       *head_ref = newMenu;
////	       return;
//	    }
//	    else
//	    {
//		    /* 5. Else traverse till the last node */
//		    while (lastMenu->next != NULL)
//		    	lastMenu = lastMenu->next;
//
//		    /* 6. Change the next of last node */
//		    lastMenu->next = newMenu;
//	    }

	    if (*head_ref == NULL)
	    {
	    	newMenu->next = newMenu->prev = newMenu;
	       *head_ref = newMenu;
//	       return;
	    }
	    else
	    {
	    	struct Menu *lastMenu = (*head_ref)->prev;  /* used in step 5*/
		    newMenu->next = *head_ref;

		    (*head_ref)->prev = newMenu;

		    newMenu->prev = lastMenu;

		    lastMenu->next = newMenu;
	    }
	}

    return;
}




void addMenuUart(struct Menu** head_ref,const uint8_t *RecipieMenu)
{


	if(RecipieMenu[0] == 0x13)
	{
		int i =0;

		uint16_t Menu_base_addr = 0;
		uint8_t MenuNo = 1;
//		uint8_t nameSize = 0;
		uint8_t dummyBuf[5];

	    /* 1. allocate node */


	    struct Menu *Current = *head_ref;
	    struct Menu* previous = NULL;


	    while(Current->menuID != RecipieMenu[1]) {

	       //if it is last node
	       if(Current->next == NULL) {

	    	   recipieCount++;

		   struct Menu* newMenu = (struct Menu*) malloc(sizeof(struct Menu));

		   memset(newMenu->name,0,sizeof(newMenu->name));

		   Menu_base_addr = RECIPIE_BASE_ADDR(recipieCount);

	   	    /* 2. put in the data  */
	   	    for(i = 0; i < 15;i++)
	   	    {
	   	    	newMenu->name[i]  = (char )RecipieMenu[i+2];
	   	    }

	   	    newMenu->menuID = RecipieMenu[1];

#if 0
	   	    newMenu->cookingTemp = RecipieMenu[17];
	   	    newMenu->cutOffTemp = RecipieMenu[18];
	   	    newMenu->cookingTimeT1 = ((RecipieMenu[19] << 8) | RecipieMenu[20]);
	   	    newMenu->cookingTimeT2 = ((RecipieMenu[21] << 8) | (RecipieMenu[22]));
	   	    newMenu->holdingTime = ((RecipieMenu[23] << 8) | (RecipieMenu[24]));

	   	    ee24_write(Menu_base_addr+MENU_ID_ADDR,&newMenu->menuID,1,100); //id

	   	    nameSize = sizeof(newMenu->name);
	   	    ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)newMenu->name,nameSize,100); //name
	   	    ee24_write(Menu_base_addr+MENU_NAME_SIZE,&nameSize,1,100);

	   	    ee24_write(Menu_base_addr+MENU_COOKING_TEMP_ADDR,&newMenu->cookingTemp,1,100);   //cooking temp

	   	    ee24_write(Menu_base_addr+MENU_CUTOFF_TEMP_ADDR,&newMenu->cutOffTemp,1,100);     // Cutoff Temp


	   	    dummyBuf[0] = newMenu->cookingTimeT1 >> 8;
	   	    dummyBuf[1] = newMenu->cookingTimeT1 & 0xff;
	   	    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1,dummyBuf,2,100);    //Cooking time t1
	   	//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1+1,(uint8_t*)(newMenu->cookingTimeT1 &0xff),1,100);

	   	    dummyBuf[0] = newMenu->cookingTimeT2 >> 8;
	   	    dummyBuf[1] = newMenu->cookingTimeT2 & 0xff;
	   	    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2,dummyBuf,2,100);    //Cooking time t2
	   	//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2+1,(uint8_t*)(newMenu->cookingTimeT2 &0xff),1,100);

	   	    dummyBuf[0] = newMenu->holdingTime >> 8;
	   	    dummyBuf[1] = newMenu->holdingTime & 0xff;
	   	    ee24_write(Menu_base_addr+MENU_HOLDING_TIME,dummyBuf,2,100);		  //Holding time

	   	    newMenu->cookingCount = 0;
	   	    dummyBuf[0] = 0;
	   	    dummyBuf[1] = 0;
	   	    dummyBuf[2] = 0;
	   	    dummyBuf[3] = 0;
	   	    ee24_write(Menu_base_addr+MENU_COUNT_ADDR,dummyBuf,4,100);
	   	//    ee24_write(Menu_base_addr+MENU_HOLDING_TIME+1,(uint8_t*)(newMenu->holdingTime &0xff),1,100);

#endif
	   	    //////////////////////////////////////////////////////////////////////////////////////////////////

//	   	    newMenu->menuID = menuParameters[0];

	   	     ee24_write(Menu_base_addr+MENU_ID_ADDR,&newMenu->menuID,1,100); //id

	   	     ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)newMenu->name,15,100); //name


	   	     /***********************************Portion 1************************************************************/

	   	 	newMenu->portion1SetTemp = RecipieMenu[17];
	   	 	newMenu->portion1CutOffTemp = RecipieMenu[18];

	   	     ee24_write(Menu_base_addr+MENU_PORTION1_SET_TEMP,&newMenu->portion1SetTemp,1,100);   //cooking temp

	   	     ee24_write(Menu_base_addr+MENU_PORTION1_CUTOFF_TEMP,&newMenu->portion1CutOffTemp,1,100);     // Cutoff Temp

	   	     newMenu->portion1CTime1 = ((RecipieMenu[19] << 8) | RecipieMenu[20]);

	   	     dummyBuf[0] = newMenu->portion1CTime1 >> 8;
	   	     dummyBuf[1] = newMenu->portion1CTime1 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

	   	     newMenu->portion1CTime2 = ((RecipieMenu[21] << 8) | RecipieMenu[22]);

	   	     dummyBuf[0] = newMenu->portion1CTime2 >> 8;
	   	     dummyBuf[1] = newMenu->portion1CTime2 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

	   	     newMenu->portion1HTime = ((RecipieMenu[23] << 8) | RecipieMenu[24]);

	   	     dummyBuf[0] = newMenu->portion1HTime >> 8;
	   	     dummyBuf[1] = newMenu->portion1HTime & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION1_H_TIME,dummyBuf,2,100);		  //Holding time

	   	     newMenu->portion1Count = 0;												// Count

	   	 	dummyBuf[0] = 0;
	   	 	dummyBuf[1] = 0;
	   	 	ee24_write(Menu_base_addr+MENU_PORTION1_COUNT,dummyBuf,2,100);

	   	     /***********************************Portion 2************************************************************/

	   	 	newMenu->portion2SetTemp = RecipieMenu[25];
	   	 	newMenu->portion2CutOffTemp = RecipieMenu[26];

	   	     ee24_write(Menu_base_addr+MENU_PORTION2_SET_TEMP,&newMenu->portion2SetTemp,1,100);   //cooking temp

	   	     ee24_write(Menu_base_addr+MENU_PORTION2_CUTOFF_TEMP,&newMenu->portion2CutOffTemp,1,100);     // Cutoff Temp

	   	     newMenu->portion2CTime1 = ((RecipieMenu[27] << 8) | RecipieMenu[28]);

	   	     dummyBuf[0] = newMenu->portion2CTime1 >> 8;
	   	     dummyBuf[1] = newMenu->portion2CTime1 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

	   	     newMenu->portion2CTime2 = ((RecipieMenu[29] << 8) | RecipieMenu[30]);

	   	     dummyBuf[0] = newMenu->portion2CTime2 >> 8;
	   	     dummyBuf[1] = newMenu->portion2CTime2 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

	   	     newMenu->portion2HTime = ((RecipieMenu[31] << 8) | RecipieMenu[32]);

	   	     dummyBuf[0] = newMenu->portion2HTime >> 8;
	   	     dummyBuf[1] = newMenu->portion2HTime & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION2_H_TIME,dummyBuf,2,100);		  //Holding time

	   	     newMenu->portion2Count = 0;												// Count

	   	 	dummyBuf[0] = 0;
	   	 	dummyBuf[1] = 0;
	   	 	ee24_write(Menu_base_addr+MENU_PORTION2_COUNT,dummyBuf,2,100);


	   	     /***********************************Portion 3************************************************************/

	   	 	newMenu->portion3SetTemp = RecipieMenu[33];
	   	 	newMenu->portion3CutOffTemp = RecipieMenu[34];

	   	     ee24_write(Menu_base_addr+MENU_PORTION3_SET_TEMP,&newMenu->portion3SetTemp,1,100);   //cooking temp

	   	     ee24_write(Menu_base_addr+MENU_PORTION3_CUTOFF_TEMP,&newMenu->portion3CutOffTemp,1,100);     // Cutoff Temp

	   	     newMenu->portion3CTime1 = ((RecipieMenu[35] << 8) | RecipieMenu[36]);

	   	     dummyBuf[0] = newMenu->portion3CTime1 >> 8;
	   	     dummyBuf[1] = newMenu->portion3CTime1 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

	   	     newMenu->portion3CTime2 = ((RecipieMenu[37] << 8) | RecipieMenu[38]);

	   	     dummyBuf[0] = newMenu->portion3CTime2 >> 8;
	   	     dummyBuf[1] = newMenu->portion3CTime2 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

	   	     newMenu->portion3HTime = ((RecipieMenu[39] << 8) | RecipieMenu[40]);

	   	     dummyBuf[0] = newMenu->portion3HTime >> 8;
	   	     dummyBuf[1] = newMenu->portion3HTime & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION3_H_TIME,dummyBuf,2,100);		  //Holding time

	   	     newMenu->portion3Count = 0;												// Count

	   	 	dummyBuf[0] = 0;
	   	 	dummyBuf[1] = 0;
	   	 	ee24_write(Menu_base_addr+MENU_PORTION3_COUNT,dummyBuf,2,100);


	   	     /***********************************Portion 4************************************************************/

	   	 	newMenu->portion4SetTemp = RecipieMenu[41];
	   	 	newMenu->portion4CutOffTemp = RecipieMenu[42];

	   	     ee24_write(Menu_base_addr+MENU_PORTION4_SET_TEMP,&newMenu->portion4SetTemp,1,100);   //cooking temp

	   	     ee24_write(Menu_base_addr+MENU_PORTION4_CUTOFF_TEMP,&newMenu->portion4CutOffTemp,1,100);     // Cutoff Temp

	   	     newMenu->portion4CTime1 = ((RecipieMenu[43] << 8) | RecipieMenu[44]);

	   	     dummyBuf[0] = newMenu->portion4CTime1 >> 8;
	   	     dummyBuf[1] = newMenu->portion4CTime1 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

	   	     newMenu->portion4CTime2 = ((RecipieMenu[45] << 8) | RecipieMenu[46]);

	   	     dummyBuf[0] = newMenu->portion4CTime2 >> 8;
	   	     dummyBuf[1] = newMenu->portion4CTime2 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

	   	     newMenu->portion4HTime = ((RecipieMenu[47] << 8) | RecipieMenu[48]);

	   	     dummyBuf[0] = newMenu->portion4HTime >> 8;
	   	     dummyBuf[1] = newMenu->portion4HTime & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION4_H_TIME,dummyBuf,2,100);		  //Holding time

	   	     newMenu->portion4Count = 0;												// Count

	   	 	dummyBuf[0] = 0;
	   	 	dummyBuf[1] = 0;
	   	 	ee24_write(Menu_base_addr+MENU_PORTION4_COUNT,dummyBuf,2,100);


	   	     /***********************************Portion 5************************************************************/

	   	 	newMenu->portion5SetTemp = RecipieMenu[49];
	   	 	newMenu->portion5CutOffTemp = RecipieMenu[50];

	   	     ee24_write(Menu_base_addr+MENU_PORTION5_SET_TEMP,&newMenu->portion5SetTemp,1,100);   //cooking temp

	   	     ee24_write(Menu_base_addr+MENU_PORTION5_CUTOFF_TEMP,&newMenu->portion5CutOffTemp,1,100);     // Cutoff Temp

	   	     newMenu->portion5CTime1 = ((RecipieMenu[51] << 8) | RecipieMenu[52]);

	   	     dummyBuf[0] = newMenu->portion5CTime1 >> 8;
	   	     dummyBuf[1] = newMenu->portion5CTime1 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

	   	     newMenu->portion5CTime2 = ((RecipieMenu[53] << 8) | RecipieMenu[54]);

	   	     dummyBuf[0] = newMenu->portion5CTime2 >> 8;
	   	     dummyBuf[1] = newMenu->portion5CTime2 & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

	   	     newMenu->portion5HTime = ((RecipieMenu[55] << 8) | RecipieMenu[56]);

	   	     dummyBuf[0] = newMenu->portion5HTime >> 8;
	   	     dummyBuf[1] = newMenu->portion5HTime & 0xff;
	   	     ee24_write(Menu_base_addr+MENU_PORTION5_H_TIME,dummyBuf,2,100);		  //Holding time

	   	     newMenu->portion5Count = 0;												// Count

	   	 	dummyBuf[0] = 0;
	   	 	dummyBuf[1] = 0;
	   	 	ee24_write(Menu_base_addr+MENU_PORTION5_COUNT,dummyBuf,2,100);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	   	    ee24_write(MENU_COUNT,&recipieCount,1,100);

	   	    /* 3. This new node is going to be the last node, so make next of
	   	          it as NULL*/
	   	    newMenu->next = NULL;

	   	    /* 4. If the Linked List is empty, then make the new node as head */
	   	    if (*head_ref == NULL)
	   	    {
	   	       *head_ref = newMenu;
	   	       return;
	   	    }

/*	   	     5. Else traverse till the last node
	   	    while (lastMenu->next != NULL)
	   	    	lastMenu = lastMenu->next;*/

	   	    /* 6. Change the next of last node */
	   	    Current->next = newMenu;
	   	    return;

	       } else {
	          //store reference to current link
	    	   previous = Current;
	          //move to next link
	          Current = Current->next;
	          MenuNo++;
	       }
	    }

	    Menu_base_addr = RECIPIE_BASE_ADDR(MenuNo);

	    /* 2. put in the data  */
	    for(i = 0; i < 15;i++)
	    {
	    	Current->name[i]  = (char )RecipieMenu[i+2];
	    }

	    Current->menuID = RecipieMenu[1];
#if 0

	    Current->cookingTemp = RecipieMenu[17];
	    Current->cutOffTemp = RecipieMenu[18];
	    Current->cookingTimeT1 = ((RecipieMenu[19] << 8) | RecipieMenu[20]);
	    Current->cookingTimeT2 = ((RecipieMenu[21] << 8) | (RecipieMenu[22]));
	    Current->holdingTime = ((RecipieMenu[23] << 8) | (RecipieMenu[24]));

	    ee24_write(Menu_base_addr+MENU_ID_ADDR,&Current->menuID,1,100); //id

	    nameSize = sizeof(Current->name);
	    ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)Current->name,nameSize,100); //name
	    ee24_write(Menu_base_addr+MENU_NAME_SIZE,&nameSize,1,100);

	    ee24_write(Menu_base_addr+MENU_COOKING_TEMP_ADDR,&Current->cookingTemp,1,100);   //cooking temp

	    ee24_write(Menu_base_addr+MENU_CUTOFF_TEMP_ADDR,&Current->cutOffTemp,1,100);     // Cutoff Temp


	    dummyBuf[0] = Current->cookingTimeT1 >> 8;
	    dummyBuf[1] = Current->cookingTimeT1 & 0xff;
	    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1,dummyBuf,2,100);    //Cooking time t1
	//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1+1,(uint8_t*)(newMenu->cookingTimeT1 &0xff),1,100);

	    dummyBuf[0] = Current->cookingTimeT2 >> 8;
	    dummyBuf[1] = Current->cookingTimeT2 & 0xff;
	    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2,dummyBuf,2,100);    //Cooking time t2
	//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2+1,(uint8_t*)(newMenu->cookingTimeT2 &0xff),1,100);

	    dummyBuf[0] = Current->holdingTime >> 8;
	    dummyBuf[1] = Current->holdingTime & 0xff;
	    ee24_write(Menu_base_addr+MENU_HOLDING_TIME,dummyBuf,2,100);		  //Holding time
	//    ee24_write(Menu_base_addr+MENU_HOLDING_TIME+1,(uint8_t*)(newMenu->holdingTime &0xff),1,100);

   	    dummyBuf[0] = 0;
   	    dummyBuf[1] = 0;
   	    dummyBuf[2] = 0;
   	    dummyBuf[3] = 0;
   	    ee24_write(Menu_base_addr+MENU_COUNT_ADDR,dummyBuf,4,100);
#endif

   	    ////////////////////////////////////////////////////////////////////////////////////

//   	    newMenu->menuID = menuParameters[0];

   	     ee24_write(Menu_base_addr+MENU_ID_ADDR,&Current->menuID,1,100); //id

   	     ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)Current->name,15,100); //name


   	     /***********************************Portion 1************************************************************/

   	     Current->portion1SetTemp = RecipieMenu[17];
   	     Current->portion1CutOffTemp = RecipieMenu[18];

   	     ee24_write(Menu_base_addr+MENU_PORTION1_SET_TEMP,&Current->portion1SetTemp,1,100);   //cooking temp

   	     ee24_write(Menu_base_addr+MENU_PORTION1_CUTOFF_TEMP,&Current->portion1CutOffTemp,1,100);     // Cutoff Temp

   	     Current->portion1CTime1 = ((RecipieMenu[19] << 8) | RecipieMenu[20]);

   	     dummyBuf[0] = Current->portion1CTime1 >> 8;
   	     dummyBuf[1] = Current->portion1CTime1 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

   	     Current->portion1CTime2 = ((RecipieMenu[21] << 8) | RecipieMenu[22]);

   	     dummyBuf[0] = Current->portion1CTime2 >> 8;
   	     dummyBuf[1] = Current->portion1CTime2 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

   	     Current->portion1HTime = ((RecipieMenu[23] << 8) | RecipieMenu[24]);

   	     dummyBuf[0] = Current->portion1HTime >> 8;
   	     dummyBuf[1] = Current->portion1HTime & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION1_H_TIME,dummyBuf,2,100);		  //Holding time

 /*  	  Current->portion1Count = 0;												// Count

   	 	dummyBuf[0] = 0;
   	 	dummyBuf[1] = 0;
   	 	ee24_write(Menu_base_addr+MENU_PORTION1_COUNT,dummyBuf,2,100);*/

   	     /***********************************Portion 2************************************************************/

   	     Current->portion2SetTemp = RecipieMenu[25];
   	     Current->portion2CutOffTemp = RecipieMenu[26];

   	     ee24_write(Menu_base_addr+MENU_PORTION2_SET_TEMP,&Current->portion2SetTemp,1,100);   //cooking temp

   	     ee24_write(Menu_base_addr+MENU_PORTION2_CUTOFF_TEMP,&Current->portion2CutOffTemp,1,100);     // Cutoff Temp

   	     Current->portion2CTime1 = ((RecipieMenu[27] << 8) | RecipieMenu[28]);

   	     dummyBuf[0] = Current->portion2CTime1 >> 8;
   	     dummyBuf[1] = Current->portion2CTime1 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

   	     Current->portion2CTime2 = ((RecipieMenu[29] << 8) | RecipieMenu[30]);

   	     dummyBuf[0] = Current->portion2CTime2 >> 8;
   	     dummyBuf[1] = Current->portion2CTime2 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

   	     Current->portion2HTime = ((RecipieMenu[31] << 8) | RecipieMenu[32]);

   	     dummyBuf[0] = Current->portion2HTime >> 8;
   	     dummyBuf[1] = Current->portion2HTime & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION2_H_TIME,dummyBuf,2,100);		  //Holding time

 /*  	     Current->portion2Count = 0;												// Count

   	 	dummyBuf[0] = 0;
   	 	dummyBuf[1] = 0;
   	 	ee24_write(Menu_base_addr+MENU_PORTION2_COUNT,dummyBuf,2,100);*/


   	     /***********************************Portion 3************************************************************/

   	     Current->portion3SetTemp = RecipieMenu[33];
   	     Current->portion3CutOffTemp = RecipieMenu[34];

   	     ee24_write(Menu_base_addr+MENU_PORTION3_SET_TEMP,&Current->portion3SetTemp,1,100);   //cooking temp

   	     ee24_write(Menu_base_addr+MENU_PORTION3_CUTOFF_TEMP,&Current->portion3CutOffTemp,1,100);     // Cutoff Temp

   	     Current->portion3CTime1 = ((RecipieMenu[35] << 8) | RecipieMenu[36]);

   	     dummyBuf[0] = Current->portion3CTime1 >> 8;
   	     dummyBuf[1] = Current->portion3CTime1 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

   	     Current->portion3CTime2 = ((RecipieMenu[37] << 8) | RecipieMenu[38]);

   	     dummyBuf[0] = Current->portion3CTime2 >> 8;
   	     dummyBuf[1] = Current->portion3CTime2 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

   	     Current->portion3HTime = ((RecipieMenu[39] << 8) | RecipieMenu[40]);

   	     dummyBuf[0] = Current->portion3HTime >> 8;
   	     dummyBuf[1] = Current->portion3HTime & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION3_H_TIME,dummyBuf,2,100);		  //Holding time

/*   	     Current->portion3Count = 0;												// Count

   	 	dummyBuf[0] = 0;
   	 	dummyBuf[1] = 0;
   	 	ee24_write(Menu_base_addr+MENU_PORTION3_COUNT,dummyBuf,2,100);*/


   	     /***********************************Portion 4************************************************************/

   	     Current->portion4SetTemp = RecipieMenu[41];
   	     Current->portion4CutOffTemp = RecipieMenu[42];

   	     ee24_write(Menu_base_addr+MENU_PORTION4_SET_TEMP,&Current->portion4SetTemp,1,100);   //cooking temp

   	     ee24_write(Menu_base_addr+MENU_PORTION4_CUTOFF_TEMP,&Current->portion4CutOffTemp,1,100);     // Cutoff Temp

   	     Current->portion4CTime1 = ((RecipieMenu[43] << 8) | RecipieMenu[44]);

   	     dummyBuf[0] = Current->portion4CTime1 >> 8;
   	     dummyBuf[1] = Current->portion4CTime1 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

   	     Current->portion4CTime2 = ((RecipieMenu[45] << 8) | RecipieMenu[46]);

   	     dummyBuf[0] = Current->portion4CTime2 >> 8;
   	     dummyBuf[1] = Current->portion4CTime2 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

   	     Current->portion4HTime = ((RecipieMenu[47] << 8) | RecipieMenu[48]);

   	     dummyBuf[0] = Current->portion4HTime >> 8;
   	     dummyBuf[1] = Current->portion4HTime & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION4_H_TIME,dummyBuf,2,100);		  //Holding time

/*   	     Current->portion4Count = 0;												// Count

   	 	dummyBuf[0] = 0;
   	 	dummyBuf[1] = 0;
   	 	ee24_write(Menu_base_addr+MENU_PORTION4_COUNT,dummyBuf,2,100);*/


   	     /***********************************Portion 5************************************************************/

   	     Current->portion5SetTemp = RecipieMenu[49];
   	     Current->portion5CutOffTemp = RecipieMenu[50];

   	     ee24_write(Menu_base_addr+MENU_PORTION5_SET_TEMP,&Current->portion5SetTemp,1,100);   //cooking temp

   	     ee24_write(Menu_base_addr+MENU_PORTION5_CUTOFF_TEMP,&Current->portion5CutOffTemp,1,100);     // Cutoff Temp

   	     Current->portion5CTime1 = ((RecipieMenu[51] << 8) | RecipieMenu[52]);

   	     dummyBuf[0] = Current->portion5CTime1 >> 8;
   	     dummyBuf[1] = Current->portion5CTime1 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

   	     Current->portion5CTime2 = ((RecipieMenu[53] << 8) | RecipieMenu[54]);

   	     dummyBuf[0] = Current->portion5CTime2 >> 8;
   	     dummyBuf[1] = Current->portion5CTime2 & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

   	     Current->portion5HTime = ((RecipieMenu[55] << 8) | RecipieMenu[56]);

   	     dummyBuf[0] = Current->portion5HTime >> 8;
   	     dummyBuf[1] = Current->portion5HTime & 0xff;
   	     ee24_write(Menu_base_addr+MENU_PORTION5_H_TIME,dummyBuf,2,100);		  //Holding time

/*   	     Current->portion5Count = 0;												// Count

   	 	dummyBuf[0] = 0;
   	 	dummyBuf[1] = 0;
   	 	ee24_write(Menu_base_addr+MENU_PORTION5_COUNT,dummyBuf,2,100);*/



   	    ////////////////////////////////////////////////////////////////////////////////////

//	    ee24_write(MENU_COUNT,&recipieCount,1,100);

	    return;
	}
	else if(RecipieMenu[0] == 0x14)
	{
		if(RecipieMenu[1] == 0x1)
		{
			return;
		}
		else
		{
			//delete(&*head_ref,RecipieMenu[1]);
			return;
		}

	}


}


#if 0

// This function prints contents of linked list starting from head
void printList(struct Menu *node)
{
	int i = 0;
	  ILI9341_FillScreen(ILI9341_WHITE);
	  HAL_Delay(10);

	  if(recipieCount >= 1){
		  while (node != NULL)
		  {
		//     printf(" %d ", node->data);
			  sprintf(temp_buff,"id:%d,name:%s,ct:%d,cu:%d,ct1:%d,ct2:%d,ht:%d",node->menuID,node->name,node->cookingTemp,node->cutOffTemp,node->cookingTimeT1,node->cookingTimeT2,node->holdingTime);
			  ILI9341_WriteString(0,( 4 * i)*10,temp_buff, Font_11x18, ILI9341_GREEN, ILI9341_WHITE);
			  i++;
		     node = node->next;

		     HAL_Delay(50);
		  }
	  }

}



//delete a link with given key
void delete(struct Menu** head_ref,uint16_t menuID)
{

   //start from the first link
//   struct Menu* current = head;
   struct Menu *CurrentMenu = *head_ref;
   struct Menu* previous = NULL;

   //if list is empty
   if(head_ref == NULL) {
      return;
   }

   //navigate through list
   while(CurrentMenu->menuID != menuID) {

      //if it is last node
      if(CurrentMenu->next == NULL) {
         return;
      } else {
         //store reference to current link
         previous = CurrentMenu;
         //move to next link
         CurrentMenu = CurrentMenu->next;
      }
   }
	recipieCount--;

	ee24_write(MENU_COUNT,&recipieCount,1,100);
   //found a match, update the link
   if(CurrentMenu == *head_ref) {
      //change first to point to next line

	   if(recipieCount == 0)
	   {
//		   memset(CurrentMenu->name,0,sizeof(CurrentMenu->name));
//		   CurrentMenu->menuID = 0;
//		   realloc(CurrentMenu,0);
		   *head_ref = NULL;
//		   CurrentMenu = NULL;
	   }
	   else
	   {
		   *(*head_ref) = *(*head_ref)->next;
	   }

   } else {
      //bypass the current link
      previous->next = CurrentMenu->next;
   }

   updateEeprom(&*head_ref);

   return;
}


void updateEeprom(struct Menu** head_ref)
{
	int i =0;
	ee24_read(MENU_COUNT,&recipieCount,1,100);
	uint16_t Menu_base_addr = 0;
	uint8_t nameSize = 0;
	uint8_t dummyBuf[5];
    /* 1. allocate node */
	struct Menu *CurrentMenu = *head_ref;

	for(i = 1; i <= recipieCount ; i++)
	{

		Menu_base_addr = RECIPIE_BASE_ADDR(i);

	    ee24_write(Menu_base_addr+MENU_ID_ADDR,&CurrentMenu->menuID,1,100); //id

	    nameSize = sizeof(CurrentMenu->name);
	    ee24_write(Menu_base_addr+MENU_NAME_START_ADDR,(uint8_t*)CurrentMenu->name,nameSize,100); //name
	    ee24_write(Menu_base_addr+MENU_NAME_SIZE,&nameSize,1,100);

	    ee24_write(Menu_base_addr+MENU_COOKING_TEMP_ADDR,&CurrentMenu->cookingTemp,1,100);   //cooking temp

	    ee24_write(Menu_base_addr+MENU_CUTOFF_TEMP_ADDR,&CurrentMenu->cutOffTemp,1,100);     // Cutoff Temp


	    dummyBuf[0] = CurrentMenu->cookingTimeT1 >> 8;
	    dummyBuf[1] = CurrentMenu->cookingTimeT1 & 0xff;
	    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1,dummyBuf,2,100);    //Cooking time t1
	//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T1+1,(uint8_t*)(newMenu->cookingTimeT1 &0xff),1,100);

	    dummyBuf[0] = CurrentMenu->cookingTimeT2 >> 8;
	    dummyBuf[1] = CurrentMenu->cookingTimeT2 & 0xff;
	    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2,dummyBuf,2,100);    //Cooking time t2
	//    ee24_write(Menu_base_addr+MENU_COOKING_TIME_T2+1,(uint8_t*)(newMenu->cookingTimeT2 &0xff),1,100);

	    dummyBuf[0] = CurrentMenu->holdingTime >> 8;
	    dummyBuf[1] = CurrentMenu->holdingTime & 0xff;
	    ee24_write(Menu_base_addr+MENU_HOLDING_TIME,dummyBuf,2,100);		  //Holding time

	    dummyBuf[0] = (CurrentMenu->cookingCount >> 24) & 0xff;
	    dummyBuf[1] = (CurrentMenu->cookingCount >> 16) & 0xff;
	    dummyBuf[2] = (CurrentMenu->cookingCount >> 8) & 0xff;
	    dummyBuf[3] = CurrentMenu->cookingCount & 0xff;
	    ee24_write(Menu_base_addr+MENU_COUNT_ADDR,dummyBuf,4,100);		  //Count Value


	    if(CurrentMenu->next != NULL)
	    	CurrentMenu = CurrentMenu->next;

	}

    return;
}

#endif

void updateMenu(struct Menu *RecipieMenu)
{


		uint16_t Menu_base_addr = 0;
//		uint8_t MenuNo = 1;
		uint8_t dummyBuf[5];

	    /* 1. allocate node */


	    Menu_base_addr = RECIPIE_BASE_ADDR(menuNo);

	    /* 2. put in the data  */

   	    /***********************************Portion 1************************************************************/

	    if(portionNoSelected == 1)
	    {
	   	    ee24_write(Menu_base_addr+MENU_PORTION1_SET_TEMP,&RecipieMenu->portion1SetTemp,1,100);   //cooking temp

	   	    ee24_write(Menu_base_addr+MENU_PORTION1_CUTOFF_TEMP,&RecipieMenu->portion1CutOffTemp,1,100);     // Cutoff Temp

	   	    dummyBuf[0] = RecipieMenu->portion1CTime1 >> 8;
	   	    dummyBuf[1] = RecipieMenu->portion1CTime1 & 0xff;
	   	    ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1


	   	    dummyBuf[0] = RecipieMenu->portion1CTime2 >> 8;
	   	    dummyBuf[1] = RecipieMenu->portion1CTime2 & 0xff;
	   	    ee24_write(Menu_base_addr+MENU_PORTION1_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

	   	    dummyBuf[0] = RecipieMenu->portion1HTime >> 8;
	   	    dummyBuf[1] = RecipieMenu->portion1HTime & 0xff;
	   	    ee24_write(Menu_base_addr+MENU_PORTION1_H_TIME,dummyBuf,2,100);		  //Holding time
	    }



   	    /***********************************Portion 2************************************************************/

	    else if(portionNoSelected == 2)
	    {

			ee24_write(Menu_base_addr+MENU_PORTION2_SET_TEMP,&RecipieMenu->portion2SetTemp,1,100);   //cooking temp

			ee24_write(Menu_base_addr+MENU_PORTION2_CUTOFF_TEMP,&RecipieMenu->portion2CutOffTemp,1,100);     // Cutoff Temp


			dummyBuf[0] = RecipieMenu->portion2CTime1 >> 8;
			dummyBuf[1] = RecipieMenu->portion2CTime1 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

			dummyBuf[0] = RecipieMenu->portion2CTime2 >> 8;
			dummyBuf[1] = RecipieMenu->portion2CTime2 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION2_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

			dummyBuf[0] = RecipieMenu->portion2HTime >> 8;
			dummyBuf[1] = RecipieMenu->portion2HTime & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION2_H_TIME,dummyBuf,2,100);		  //Holding time

	    }

   	    /***********************************Portion 3************************************************************/

	    else if(portionNoSelected == 3)
	    {
			ee24_write(Menu_base_addr+MENU_PORTION3_SET_TEMP,&RecipieMenu->portion3SetTemp,1,100);   //cooking temp

			ee24_write(Menu_base_addr+MENU_PORTION3_CUTOFF_TEMP,&RecipieMenu->portion3CutOffTemp,1,100);     // Cutoff Temp

			dummyBuf[0] = RecipieMenu->portion3CTime1 >> 8;
			dummyBuf[1] = RecipieMenu->portion3CTime1 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

			dummyBuf[0] = RecipieMenu->portion3CTime2 >> 8;
			dummyBuf[1] = RecipieMenu->portion3CTime2 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION3_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

			dummyBuf[0] = RecipieMenu->portion3HTime >> 8;
			dummyBuf[1] = RecipieMenu->portion3HTime & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION3_H_TIME,dummyBuf,2,100);		  //Holding time
	    }

   	    /***********************************Portion 4************************************************************/
	    else if(portionNoSelected == 4)
	    {
			ee24_write(Menu_base_addr+MENU_PORTION4_SET_TEMP,&RecipieMenu->portion4SetTemp,1,100);   //cooking temp

			ee24_write(Menu_base_addr+MENU_PORTION4_CUTOFF_TEMP,&RecipieMenu->portion4CutOffTemp,1,100);     // Cutoff Temp


			dummyBuf[0] = RecipieMenu->portion4CTime1 >> 8;
			dummyBuf[1] = RecipieMenu->portion4CTime1 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1

			dummyBuf[0] = RecipieMenu->portion4CTime2 >> 8;
			dummyBuf[1] = RecipieMenu->portion4CTime2 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION4_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

			dummyBuf[0] = RecipieMenu->portion4HTime >> 8;
			dummyBuf[1] = RecipieMenu->portion4HTime & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION4_H_TIME,dummyBuf,2,100);		  //Holding time
	    }

   	    /***********************************Portion 5************************************************************/

	    else if(portionNoSelected == 5)
	    {
			ee24_write(Menu_base_addr+MENU_PORTION5_SET_TEMP,&RecipieMenu->portion5SetTemp,1,100);   //cooking temp

			ee24_write(Menu_base_addr+MENU_PORTION5_CUTOFF_TEMP,&RecipieMenu->portion5CutOffTemp,1,100);     // Cutoff Temp


			dummyBuf[0] = RecipieMenu->portion5CTime1 >> 8;
			dummyBuf[1] = RecipieMenu->portion5CTime1 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T1,dummyBuf,2,100);    //Cooking time t1


			dummyBuf[0] = RecipieMenu->portion5CTime2 >> 8;
			dummyBuf[1] = RecipieMenu->portion5CTime2 & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION5_C_TIME_T2,dummyBuf,2,100);    //Cooking time t2

			dummyBuf[0] = RecipieMenu->portion5HTime >> 8;
			dummyBuf[1] = RecipieMenu->portion5HTime & 0xff;
			ee24_write(Menu_base_addr+MENU_PORTION5_H_TIME,dummyBuf,2,100);		  //Holding time
	    }

	    return;


}

void resetPortionCount(struct Menu *RecipieMenu)
{
	uint16_t Menu_base_addr = 0;
	uint8_t dummyBuf[5];

	Menu_base_addr = RECIPIE_BASE_ADDR(menuNo);


	if(RecipieMenu->portion1CTime1 > 0)
	{
		RecipieMenu->portion1Count = 0;												// Count

		dummyBuf[0] = 0;
		dummyBuf[1] = 0;
		ee24_write(Menu_base_addr+MENU_PORTION1_COUNT,dummyBuf,2,100);
	}

	if(RecipieMenu->portion2CTime1 > 0)
	{
		RecipieMenu->portion2Count = 0;												// Count

		dummyBuf[0] = 0;
		dummyBuf[1] = 0;
		ee24_write(Menu_base_addr+MENU_PORTION2_COUNT,dummyBuf,2,100);
	}

	if(RecipieMenu->portion3CTime1 > 0)
	{
		RecipieMenu->portion3Count = 0;												// Count

		dummyBuf[0] = 0;
		dummyBuf[1] = 0;
		ee24_write(Menu_base_addr+MENU_PORTION3_COUNT,dummyBuf,2,100);
	}

	if(RecipieMenu->portion4CTime1 > 0)
	{
		RecipieMenu->portion4Count = 0;												// Count

		dummyBuf[0] = 0;
		dummyBuf[1] = 0;
		ee24_write(Menu_base_addr+MENU_PORTION4_COUNT,dummyBuf,2,100);
	}

	if(RecipieMenu->portion5CTime1 > 0)
	{
		RecipieMenu->portion5Count = 0;												// Count

		dummyBuf[0] = 0;
		dummyBuf[1] = 0;
		ee24_write(Menu_base_addr+MENU_PORTION5_COUNT,dummyBuf,2,100);
	}

}








