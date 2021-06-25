/*
 * eeprom.c
 *
 *  Created on: Jul 4, 2020
 *      Author: CEPL
 */
#include "eeprom.h"
#include "macro.h"
#include "x_display.h"
#include "xvariables.h"


//#if 0
//#include "i2c.h"
//#include "ee24.h"
#if (_EEPROM_USE_FREERTOS == 1)
#include "cmsis_os.h"
#define ee24_delay(x)   osDelay(x)
#else
#define ee24_delay(x)   HAL_Delay(x)
#endif

#if (_EEPROM_SIZE_KBIT == 1) || (_EEPROM_SIZE_KBIT == 2)
#define _EEPROM_PSIZE     8
#elif (_EEPROM_SIZE_KBIT == 4) || (_EEPROM_SIZE_KBIT == 8) || (_EEPROM_SIZE_KBIT == 16)
#define _EEPROM_PSIZE     16
#else
#define _EEPROM_PSIZE     32
#endif

uint8_t ee24_lock = 0;
extern uint32_t pohaCnt, upmaCnt, kesarCnt;
uint8_t retVal;
extern uint8_t eeprom_buff_writeBuff[2];
extern uint8_t eeprom_buff_writeCnt[4];
extern uint8_t eeprom_buff_read[2];
extern uint8_t eeprom_buff_readCnt[4];
//################################################################################################################
bool ee24_isConnected(void)
{
  #if (_EEPROM_USE_WP_PIN==1)
  HAL_GPIO_WritePin(_EEPROM_WP_GPIO,_EEPROM_WP_PIN,GPIO_PIN_SET);
  #endif
  if (HAL_I2C_IsDeviceReady(&_EEPROM_I2C, _EEPROM_ADDRESS, 2, 100)==HAL_OK)
    return true;
  else
    return false;
}
//################################################################################################################
bool ee24_write(uint16_t address, uint8_t *data, size_t len, uint32_t timeout)
{
  if (ee24_lock == 1)
    return false;
  ee24_lock = 1;
  uint16_t w;
  uint32_t startTime = HAL_GetTick();

	#if	(_EEPROM_USE_WP_PIN==1)
	HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN,GPIO_PIN_RESET);
	#endif

  while (1)
  {
    w = _EEPROM_PSIZE - (address  % _EEPROM_PSIZE);
    if (w > len)
      w = len;
    #if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
    if (HAL_I2C_Mem_Write(&_EEPROM_I2C, _EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, data, w, 100) == HAL_OK)
    #elif (_EEPROM_SIZE_KBIT==4)
    if (HAL_I2C_Mem_Write(&_EEPROM_I2C, _EEPROM_ADDRESS | ((address & 0x0100 >> 7)), (address & 0xff), I2C_MEMADD_SIZE_8BIT, data, w, 100) == HAL_OK)
    #elif (_EEPROM_SIZE_KBIT==8)
    if (HAL_I2C_Mem_Write(&_EEPROM_I2C, _EEPROM_ADDRESS | ((address & 0x0300 >> 7)), (address & 0xff), I2C_MEMADD_SIZE_8BIT, data, w, 100) == HAL_OK)
    #elif (_EEPROM_SIZE_KBIT==16)
    if (HAL_I2C_Mem_Write(&_EEPROM_I2C, _EEPROM_ADDRESS | ((address & 0x0700 >> 7)), (address & 0xff), I2C_MEMADD_SIZE_8BIT, data, w, 100) == HAL_OK)
    #else
    if (HAL_I2C_Mem_Write(&_EEPROM_I2C, _EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, data, w, 500) == HAL_OK)
    #endif
    {
//      ee24_delay(10);
      HAL_Delay(10);
      len -= w;
      data += w;
      address += w;
      if (len == 0)
      {
        #if (_EEPROM_USE_WP_PIN==1)
        HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN, GPIO_PIN_SET);
        #endif
        ee24_lock = 0;
        return true;
      }
      if (HAL_GetTick() - startTime >= timeout)
      {
        ee24_lock = 0;
        return false;
      }
    }
    else
    {
      #if (_EEPROM_USE_WP_PIN==1)
      HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN, GPIO_PIN_SET);
      #endif
      ee24_lock = 0;
      return false;
    }
  }
}
//################################################################################################################
bool ee24_read(uint16_t address, uint8_t* data, size_t len, uint32_t timeout)
{
  if (ee24_lock == 1)
    return false;
  ee24_lock = 1;
  #if (_EEPROM_USE_WP_PIN==1)
  HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN, GPIO_PIN_SET);
  #endif
  #if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
  if (HAL_I2C_Mem_Read(&_EEPROM_I2C, _EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK)
  #elif (_EEPROM_SIZE_KBIT == 4)
  if (HAL_I2C_Mem_Read(&_EEPROM_I2C, _EEPROM_ADDRESS | ((address & 0x0100 >> 7)), (Address & 0xff), I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK)
  #elif (_EEPROM_SIZE_KBIT == 8)
  if (HAL_I2C_Mem_Read(&_EEPROM_I2C, _EEPROM_ADDRESS | ((address & 0x0300 >> 7)), (Address & 0xff), I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK)
  #elif (_EEPROM_SIZE_KBIT==16)
  if (HAL_I2C_Mem_Read(&_EEPROM_I2C, _EEPROM_ADDRESS | ((address & 0x0700>> 7)), (Address & 0xff), I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK)
  #else
  if (HAL_I2C_Mem_Read(&_EEPROM_I2C, _EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, data, len, timeout) == HAL_OK)
  #endif
  {
    ee24_lock = 0;
    return true;
  }
  else
  {
    ee24_lock = 0;
    return false;
  }
}

void eeprom_init()
{
	  eeprom_buff_writeBuff[0] =0x14;
	  eeprom_buff_writeCnt[0] = 0x00;//0x05;
	  eeprom_buff_writeCnt[1] = 0x00;//0x0A;
	  eeprom_buff_writeCnt[2] = 0x00;// 0x0F;
	  ee24_write(EEPROM_DEFAULT,&eeprom_buff_writeBuff[0],1,300);
	  HAL_Delay(500);
//	  ee24_read((EEPROM_UPMA_COUNT_ADDR),(uint8_t*)&eeprom_buff_writeCnt,sizeof(eeprom_buff_writeCnt),1000);
//	  retVal = ee24_read((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)eeprom_buff_readCnt,3,300);
//	  HAL_Delay(5000);
//	  HAL_Delay(100);
//	  ee24_read((EEPROM_KESAR_COUNT_ADDR),(uint8_t*)&eeprom_buff_writeCnt[1],1,300);
//	  HAL_Delay(100);
//	  ee24_read((EEPROM_POHA_COUNT_ADDR),(uint8_t*)&eeprom_buff_writeCnt[2],1,300);
//	  HAL_Delay(100);
	  ee24_read(EEPROM_DEFAULT,(uint8_t *)&eeprom_buff_read[0],1,200);
	  /////////////////////////COUNTER TO ZERO//////////////////
	  /*ee24_write((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)&eeprom_buff_writeCnt[0],1,300);
//	  ee24_read((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)&eeprom_buff_write[0],1,300);
	  ee24_write((EEPROM_KESAR_COUNT_ADDR),(uint8_t *)&eeprom_buff_writeCnt[0],1,300);
//	  ee24_read((EEPROM_KESAR_COUNT_ADDR),(uint8_t *)&eeprom_buff_write[0],1,300);
	  ee24_write((EEPROM_POHA_COUNT_ADDR),(uint8_t *)&eeprom_buff_writeCnt[0],1,300);
//	  ee24_read((EEPROM_POHA_COUNT_ADDR),(uint8_t *)&eeprom_buff_write[0],1,300);*/
	  /////////////////////////COUNTER TO ZERO//////////////////
	  HAL_Delay(500);
	  /*else
	  	  {
	  	  keyFlags.okFlag = 0;
	  	  switchKeys.OK = 0;
	  	  			}*/
	  if (eeprom_buff_read[0] == 0x14)
	  {
		  ee24_read((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)&eeprom_buff_readCnt[0],1,300);
		  HAL_Delay(10);
		  ee24_read((EEPROM_KESAR_COUNT_ADDR),(uint8_t *)&eeprom_buff_readCnt[1],1,300);
		  HAL_Delay(10);
		  ee24_read((EEPROM_POHA_COUNT_ADDR),(uint8_t *)&eeprom_buff_readCnt[2],1,300);
		  HAL_Delay(10);
		  upmaCnt = eeprom_buff_readCnt[0];
		  kesarCnt = eeprom_buff_readCnt[1];
		  pohaCnt = eeprom_buff_readCnt[2];
	  }
//	  HAL_Delay(5000);
//	  if(eeprom_buff_read[0] == 0x14)
//	  {
//		  retVal = ee24_read((EEPROM_UPMA_COUNT_ADDR),(uint8_t *)&eeprom_buff_readCnt[0],1,300);
//		  HAL_Delay(10);
//		  ee24_read((EEPROM_KESAR_COUNT_ADDR),(uint8_t *)&eeprom_buff_readCnt[1],1,300);
//		  HAL_Delay(10);
//		  ee24_read((EEPROM_POHA_COUNT_ADDR),(uint8_t *)&eeprom_buff_readCnt[2],1,300);
//		  HAL_Delay(10);
//	  }
	  /*ee24_read((EEPROM_DEFAULT+4),&eeprom_buff_read[1],1,200);
	  HAL_Delay(5000);
	  ee24_write((EEPROM_DEFAULT + 4),eeprom_buff_write,1,300);
	  HAL_Delay(5000);
	  ee24_read(EEPROM_DEFAULT,&eeprom_buff_read[0],1,200);
	  ee24_read((EEPROM_DEFAULT+4),&eeprom_buff_read[1],1,200);*/
	  /*for(int i=0;i<size__of_dispaly_par();i++)
	  {
		  disp_param[i].eeprom_address=EEPROM_START_ADDR+i*disp_param[i].datatype;

	  }
	  if(eeprom_buff_read[0]==0XEF)
	  {
		//  ee24_read(EEPROM_DEFAULT,eeprom_buff_read,1,300);

		  _long_to_char._4bytes=++count_sve[UPMA];
		  for(int i=0;i<(disp_param[Cleaning].eeprom_address/4);i++)
		  {
			  ee24_read(disp_param[i].eeprom_address,_long_to_char.char_,disp_param[UPMA].datatype,disp_param[UPMA].datatype*200);
			  count_sve[i]=_long_to_char._4bytes;
		  }
	  }
	  else
	  {
		  eeprom_buff_read[0]=0X00;
		  for(int i=0;i<EEPROM_DEFAULT;i++)
		  {
			  ee24_write(i,eeprom_buff_read,1,200);
			  HAL_Delay(10);
		  }
		  eeprom_buff_read[0]=0XEF;
		  ee24_write(EEPROM_DEFAULT,eeprom_buff_read,1,300);
	  }*/

}

void save_count(unsigned int address)
{
	_long_to_char._4bytes = ++count_sve[address];
	ee24_write(disp_param[address].eeprom_address,&_long_to_char.char_[0],1,200);
	ee24_write(disp_param[address].eeprom_address+1,&_long_to_char.char_[1],1,200);
	ee24_write(disp_param[address].eeprom_address+2,&_long_to_char.char_[2],1,200);
	ee24_write(disp_param[address].eeprom_address+3,&_long_to_char.char_[3],1,200);

}
//################################################################################################################

//#endif








