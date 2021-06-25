/*
 * fonts.h
 *
 *  Created on: Jul 1, 2020
 *      Author: CEPL
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include <stdint.h>

typedef struct {
    const uint8_t w;
    uint8_t h;
    const uint16_t *d;
}FontDef;


//extern FontDef Font_7x10;
extern FontDef Font_11x18;
//extern FontDef Font_16x26;

#endif /* INC_FONTS_H_ */
