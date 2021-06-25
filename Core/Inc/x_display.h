/*
 * x_display.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Santhosh
 */

#ifndef INC_X_DISPLAY_H_
#define INC_X_DISPLAY_H_

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

extern struct DISP__ disp_param[];



#endif /* INC_X_DISPLAY_H_ */
