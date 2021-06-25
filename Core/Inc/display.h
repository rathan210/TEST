/*
 * display.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Santhosh
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_



struct DISP__
{
	unsigned char name[20];
	unsigned char end_of_disp;
	unsigned char datatype;
	unsigned char min_val;
	unsigned char max_val;
	unsigned char def_val;
	unsigned int identifier;
	unsigned int eeprom_address;

};

struct DISP__ disp_param[]=
 {
		 //name,end_of_disp,datatype,min_val,max_val,def_val,identifier,eeprom_address
		 {" UPMA---------",	0,LONG,1,999,1,COUNT,0},
		 {" POHA---------",	0,LONG,1,999,1,COUNT,0},
		 {" KESARI-------",	0,LONG,1,999,1,COUNT,0},
 };

#endif /* INC_DISPLAY_H_ */
