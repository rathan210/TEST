/* vim: set ai et ts=4 sw=4: */
#include "main.h"
#include "ili9341.h"

//#include "fonts.c"

extern const uint16_t Font11x18 [];
extern const uint8_t Times_New_Roman_bold16x16[];
extern const uint16_t Font16x26 [];
extern const uint8_t Times_New_Roman19x22[];
extern const uint8_t Times_New_Roman16x16[];

extern TIM_HandleTypeDef htim1;

static void ILI9341_Select() {
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
}

void ILI9341_Unselect() {
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_SET);
}

static void ILI9341_Reset() {
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(500);

//    HAL_GPIO_WritePin(ILI9341_Reset_GPIO_Port, ILI9341_Reset_Pin, GPIO_PIN_SET);
//    HAL_Delay(200);
//    HAL_GPIO_WritePin(ILI9341_Reset_GPIO_Port, ILI9341_Reset_Pin, GPIO_PIN_RESET);
//    HAL_Delay(200);
//
//    HAL_GPIO_WritePin(ILI9341_Reset_GPIO_Port, ILI9341_Reset_Pin, GPIO_PIN_SET);
//
//    HAL_Delay(200);
}

 void ILI9341_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

 void ILI9341_WriteCommand1(uint8_t cmd) {
	 ILI9341_Select();
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    ILI9341_Unselect();
}

 void ILI9341_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while(buff_size > 0) {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&hspi1, buff, chunk_size, HAL_MAX_DELAY);
        buff += chunk_size;
        buff_size -= chunk_size;
    }
}

 void ILI9341_WriteData1(uint8_t* buff, size_t buff_size) {
	 ILI9341_Select();
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while(buff_size > 0) {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&hspi1, buff, chunk_size, HAL_MAX_DELAY);
        buff += chunk_size;
        buff_size -= chunk_size;
    }
    ILI9341_Unselect();
}

static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9341_WriteCommand(0x2A); // CASET
    {
        uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(0x2B); // RASET
    {
        uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(0x2C); // RAMWR
}

#if 0
static void ILI9341_SetAddressWindow1(uint16_t x, uint16_t y)//, uint16_t x1, uint16_t y1)
{
    // column address set
    ILI9341_WriteCommand(0x2A); // CASET
    {
        uint8_t data[] = {(x >> 8) & 0xFF, x & 0xFF};//{ (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(0x2B); // RASET
    {
        uint8_t data[] = {(y >> 8) & 0xFF, y & 0xFF};//{ (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(0x2C); // RAMWR
}
#endif

void ILI9341_Init() {


	ILI9341_Select();

	// SOFTWARE RESET
	    ILI9341_WriteCommand(0x01);
	    HAL_Delay(100);

	ILI9341_Reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

/////////////////////////////////////////////////

    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    HAL_Delay(1000);

    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint8_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint8_t data[] = { 0x00, 0xC1, 0x30 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint8_t data[] = { 0x85, 0x00, 0x78 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0xEA);
    {
        uint8_t data[] = { 0x00, 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint8_t data[] = { 0x64, 0x03, 0x12, 0x81 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint8_t data[] = { 0x20 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint8_t data[] = { 0x23 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint8_t data[] = { 0x10 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint8_t data[] = { 0x3E, 0x28 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint8_t data[] = { 0x86 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // MEMORY ACCESS CONTROL
/*    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = {0x40|0x80|0x20|0x08 };
        ILI9341_WriteData(data, sizeof(data));
    }*/
    HAL_Delay(10);
    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint8_t data[] = { 0x55 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint8_t data[] = { 0x00, 0x18 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint8_t data[] = { 0x0A, 0x82, 0x27 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
/*    ILI9341_WriteCommand(0xB7);
    {
        uint8_t data[] = { 0x06};
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);*/
    ILI9341_WriteCommand(0x0B);
    {
        uint8_t data[] = { 0x08};
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);

    ILI9341_WriteCommand(0x33);
    {
        uint8_t data[] = { 0x00,0x00,0x01,0x40,0x00,0x00};
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // Vertical Scrolling
    ILI9341_WriteCommand(0x37);
    {
        uint8_t data[] = { 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint8_t data[] = { 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint8_t data[] = { 0x01 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint8_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint8_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        ILI9341_WriteData(data, sizeof(data));
    }
    HAL_Delay(10);
    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    HAL_Delay(120);
//    HAL_Delay(1);
    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);
//    HAL_Delay(1);
    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = { ILI9341_ROTATION };
        ILI9341_WriteData(data, sizeof(data));
    }
        HAL_Delay(1);
    ILI9341_Unselect();
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
        return;

    ILI9341_Select();

    ILI9341_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ILI9341_WriteData(data, sizeof(data));

    ILI9341_Unselect();

}

static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ILI9341_SetAddressWindow(x, y, x+font.w-1, y+font.h-1);

    for(i = 0; i < font.h; i++) {
        b = font.d[(ch - 32) * font.h + i];
        for(j = 0; j < font.w; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
        }
    }
}

#if 0
void times_of_new_roman_bold16x16_hor(char row,long adx,long ady,long fg_clr,long bg_clr)
{
  long ax,ay;
  char n,tx;
  int m;
  ax = adx;
  ay = ady;
  row = row-0x20;
  // Print Text 1 Charecter(data 14 Byte)
  for(m=0;m<32;m++)			  //14
  {
	  tx= Times_New_Roman_bold16x16[row][m];

		ax = ax-1;		//Read data Ascii
	  for(n=0;n<8;n++)		   //8    															//Loop Sent data  1 byte(8bit)
	  {
	  if(tx & 0x80)				   															//if data bit7 = 1 ,Plot Color area Charecter
	  {
	      GLCD_Write_Command(0x2A);  															//Command Set Adddress Hor(X)
        GLCD_Write_Data(ay);  																//Sent X_Address CGRAM
        GLCD_Write_Command(0x2B);  															//Command Set Address Ver(Y)
        GLCD_Write_Data(ax);  																//Sent Y_Address CGRAM
		    GLCD_Write_Command(0x2C);  															//Command Write data
        GLCD_Write_Data(fg_clr);
	   }
	   else						   															//if data bit7 = 0 ,Plot Color area back ground Charecter
	   {
	     if(bg_clr != 1)
         {
		        GLCD_Write_Command(0x2A);  														//Command Set Adddress Hor(X)
            GLCD_Write_Data(ay);  															//Sent X_Address CGRAM
            GLCD_Write_Command(0x2B);  														//Command Set Adddress Ver(Y)
            GLCD_Write_Data(ax);  															//Sent Y_Address CGRAM
			      GLCD_Write_Command(0x2C);  														//Command Write data
            GLCD_Write_Data(bg_clr);
			 														//Sent Data
		  }
		}
		tx <<= 1;  																			// Shift Right data 1 bit
		ay = ay+1;  																		// Increment Y-address
	}
	 m++;
	ax = ax+1;
	//ay = ay-16;
  tx = Times_New_Roman_bold16x16[row][m];  																//Read data Ascii
	  for(n=0;n<8;n++)		   //8    															//Loop Sent data  1 byte(8bit)
	  {
	  if(tx & 0x80)				   															//if data bit7 = 1 ,Plot Color area Charecter
	  {
	      GLCD_Write_Command(0x2A);  															//Command Set Adddress Hor(X)
        GLCD_Write_Data(ay);  																//Sent X_Address CGRAM
        GLCD_Write_Command(0x2B);  															//Command Set Address Ver(Y)
        GLCD_Write_Data(ax);  																//Sent Y_Address CGRAM
		    GLCD_Write_Command(0x2C);  															//Command Write data
        GLCD_Write_Data(fg_clr);

	   }
	   else						   															//if data bit7 = 0 ,Plot Color area back ground Charecter
	   {
	     if(bg_clr != 1)
         {
		        GLCD_Write_Command(0x2A);  														//Command Set Adddress Hor(X)
            GLCD_Write_Data(ay);  															//Sent X_Address CGRAM
            GLCD_Write_Command(0x2B);  														//Command Set Adddress Ver(Y)
            GLCD_Write_Data(ax);  															//Sent Y_Address CGRAM
			      GLCD_Write_Command(0x2C);  														//Command Write data
            GLCD_Write_Data(bg_clr); 			 														//Sent Data
		  }
		}
		tx <<= 1;  																			// Shift Right data 1 bit
		ay = ay+1;  																		// Increment Y-address
	}
		ax = ax+1;
		//Complet sent data 2 byte(11bit) Increment X-Address
	ay = ady; 																				//Set Position Y-address old
  }
  // Fill Back ground Color Position space between Charecter 1 Colum
  if(bg_clr != 1)
  {
    for(n=0;n<16;n++)		//11
	{
	  GLCD_Write_Command(0x2A);  															//Command Set Adddress Hor(X)
      GLCD_Write_Data(ay);  																//Sent X_Address CGRAM
      GLCD_Write_Command(0x2B);  															//Command Set Adddress Ver(Y)
      GLCD_Write_Data(ax);  																//Sent Y_Address CGRAM
	  GLCD_Write_Command(0x2C);  															//Command Write data
      GLCD_Write_Data(bg_clr);
	  ay = ay+1;  																			//Increment Y-Address
    }
  }
}

#endif






//11x18

static void ILI9341_WriteChar1(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ILI9341_SetAddressWindow(x, y, x+11-1, y+18-1);

    for(i = 0; i < 18; i++) {
        b = Font11x18[(ch - 32) * 18 + i];
        for(j = 0; j < 11; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
        }
    }
}


#if 0
//16x26
static void ILI9341_WriteChar1(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ILI9341_SetAddressWindow(x, y, x+16-1, y+26-1);

    for(i = 0; i < 26; i++) {
        b = Font16x26[(ch - 32) * 26 + i];
        for(j = 0; j < 16; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
        }
    }
}




static void ILI9341_WriteChar1(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bgcolor) {
    uint8_t i,j; //b, j;
    uint16_t ay,ax;
    uint8_t b;


    ay = y + 16;
    ax = x;

//    ILI9341_SetAddressWindow1(ax, y);

    for(i = 0; i < 32; i++) {
        b = Times_New_Roman_bold16x16[(ch - 32) * 33 + i];

//        b = 0xff;
       // ax = ax-1;

        for(j = 0; j < 8; j++) {

        	ILI9341_SetAddressWindow1(ax, ay);

            if((b << j) & 0x80)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
//                ay = ay+1;

            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));

//                ILI9341_SetAddressWindow1(ax, ay);
            }

            ay = ay-1;
        }
        ax = ax-1;
       // ay = y;
//        ay = ay+1;

        i++;

        b = Times_New_Roman_bold16x16[(ch - 32) * 33 + i];


        for(j = 0; j < 8; j++) {

        	ILI9341_SetAddressWindow1(ax, ay);

            if((b << j) & 0x80)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
//                ay = ay+1;

            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));

//                ILI9341_SetAddressWindow1(ax, ay);
            }

            ay = ay-1;
        }

        ax = x+1;
        ay = y + 16;

    }
}
#endif

static void ILI9341_WriteCharScroll(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ILI9341_SetAddressWindow(x, y, x+11-1, y+18-1);

    for(i = 0; i < 18; i++) {
        b = Font11x18[(ch - 32) * 18 + i];
        for(j = 0; j < 11; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
        }
    }
}

#if 0
static void ILI9341_WriteCharBold(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ILI9341_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x6000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
        }
    }
}

#endif


void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    ILI9341_Select();

    while(*str) {
        if(x + font.w >= ILI9341_WIDTH) {
            x = 0;
            y += font.h;
            if(y + font.w >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.w;
        str++;
    }

    ILI9341_Unselect();
}

//11x18
void ILI9341_WriteString1(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bgcolor) {
    ILI9341_Select();
//    HAL_TIM_Base_Stop_IT(&htim1);

    while(*str) {
        if(x + 11 >= ILI9341_WIDTH) {
            x = 0;
            y += 18;
            if(y + 11 >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteChar1(x, y, *str, color, bgcolor);
        x += 11;
        str++;
    }
//    HAL_TIM_Base_Start_IT(&htim1);
    ILI9341_Unselect();
    HAL_Delay(1);
}

#if 0

//16x26
void ILI9341_WriteString1(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bgcolor) {
    ILI9341_Select();

    while(*str) {
        if(x + 16 >= ILI9341_WIDTH) {
            x = 0;
            y += 26;
            if(y + 16 >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteChar1(x, y, *str, color, bgcolor);
        x += 16;
        str++;
    }

    ILI9341_Unselect();
}




void ILI9341_WriteString1(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bgcolor) {
    ILI9341_Select();

    while(*str) {
        if(x + 16 >= ILI9341_WIDTH) {
            x = 0;
            y += 16;
            if(y + 16 >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteChar1(x, y, *str, color, bgcolor);
        x += 16;
        str++;
    }

    ILI9341_Unselect();
}
#endif

void ILI9341_WriteStringScroll(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bgcolor){
    ILI9341_Select();

    while(*str) {
        if(x + 11 >= ILI9341_WIDTH) {
            x = 0;
            y += 18;
            if(y + 11 >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteCharScroll(x, y, *str, color, bgcolor);
        x += 11;
        str++;
    }

    ILI9341_Unselect();

}

#if 0
void ILI9341_WriteStringBold(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    ILI9341_Select();

    while(*str) {
        if(x + font.width >= ILI9341_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.width >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9341_WriteCharBold(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }

    ILI9341_Unselect();
}

#endif



void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ILI9341_Select();
    HAL_Delay(1);
    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            HAL_SPI_Transmit(&hspi1, data, sizeof(data), HAL_MAX_DELAY);
        }
    }

    ILI9341_Unselect();
    HAL_Delay(1);
}

void ILI9341_FillScreen(uint16_t color) {
    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ILI9341_FillScreenOffset(uint16_t color) {
    ILI9341_FillRectangle(0, 20, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ILI9341_FillScreenOffsetManual(uint16_t color) {
    ILI9341_FillRectangle(0, 120, ILI9341_WIDTH, 40, color);
}

void ILI9341_FillScreenTemp(uint16_t color) {
    ILI9341_FillRectangle(ERROR_OFFSET, 0, ILI9341_WIDTH, 20, color);
}

void ILI9341_FillScreenTemp1(uint16_t color) {
    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, 20, color);
}

// Moving Up Please Wait

// Moving Down Please Wait

void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) return;
    if((y + h - 1) >= ILI9341_HEIGHT) return;

    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);
    ILI9341_WriteData((uint8_t*)data, sizeof(uint16_t)*w*h);
    ILI9341_Unselect();
}

void ILI9341_InvertColors(bool invert) {
    ILI9341_Select();
    ILI9341_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
    ILI9341_Unselect();
}

