#include "lcd_i2c.h"

u8 currentValue; // Current status of all 8 pins of PCF7584
u8 d10,d9,d8,d7,d6,d5,d4,d3,d2,d1; // Digits for integer print
int Input_Clock; // Clock speed in MHz
uint16_t testValue = 0;

void delay_ms (int ms) //milli second delay
{
  int a =0 ;
  int b=0;
  for (a=0; a<=ms; a++)
  {
    for (b=0; b<120; b++) 
      _asm("nop"); //Perform no operation 
  }
}

void PCF7584write8(u8 iData) // Write all 8 pins of PCF7584
{
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(LCD_I2C_Address, I2C_DIRECTION_TX);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(iData);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(ENABLE);
  currentValue=iData;
}

void PCF7584write(uint8_t pin, uint8_t value) // Write selected pin of PCF7584
{
      if (value == 0)
      {
          currentValue &= ~(1 << pin); // Set the pin value 0 or 1
      }
      else
      {
          currentValue |= (1 << pin); // Set the pin value 0 or 1
      }
      PCF7584write8(currentValue);
}

void LCD_SetBit(char data_bit) //Based on the Hex value Set the Bits of the Data Lines
{
    if(data_bit& 1) 
        PCF7584write(4,1); //D4 = 1
    else
        PCF7584write(4,0); //D4 = 0

    if(data_bit& 2)
        PCF7584write(5,1); //D5 = 1
    else
        PCF7584write(5,0); //D5 = 0

    if(data_bit& 4)
        PCF7584write(6,1); //D6 = 1
    else
        PCF7584write(6,0); //D6 = 0

    if(data_bit& 8) 
        PCF7584write(7,1); //D7 = 1
    else
        PCF7584write(7,0); //D7 = 0
}

void LCD_Cmd(char a)
{
    PCF7584write(0,0); //RS = 0          
    LCD_SetBit(a); //Incoming Hex value
    PCF7584write(2,1); //EN  = 1         
		Delay_ms_int(2);
		PCF7584write(2,0); //EN  = 0      
}

 void LCD_Begin(void)
 {
	  Delay_ms_int(10);
	  Input_Clock = CLK_GetClockFreq()/1000000;  // Clock speed in MHz
    I2C_Init(100000, LCD_I2C_Address, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
	  PCF7584write8(0b00000000);
	  LCD_SetBit(0x00);
	  Delay_ms_int(1000); 
    LCD_Cmd(0x03);
	  Delay_ms_int(5);
		LCD_Cmd(0x03);
	  Delay_ms_int(11);
    LCD_Cmd(0x03); 
    LCD_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
    LCD_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
    LCD_Cmd(0x08); //Select Row 1
    LCD_Cmd(0x00); //Clear Row 1 Display
    LCD_Cmd(0x0C); //Select Row 2
    LCD_Cmd(0x00); //Clear Row 2 Display
    LCD_Cmd(0x06);
 }
 
void LCD_Clear(void)
{
    LCD_Cmd(0); //Clear the LCD
    LCD_Cmd(1); //Move the curser to first position
}


void LCD_Set_Cursor(char a, char b)
{
    char temp,z,y;
    if(a== 1)
    {
      temp = 0x80 + b - 1; //80H is used to move the curser
        z = temp>>4; //Lower 8-bits
        y = temp & 0x0F; //Upper 8-bits
        LCD_Cmd(z); //Set Row
        LCD_Cmd(y); //Set Column
    }
    else if(a== 2)
    {
        temp = 0xC0 + b - 1;
        z = temp>>4; //Lower 8-bits
        y = temp & 0x0F; //Upper 8-bits
        LCD_Cmd(z); //Set Row
        LCD_Cmd(y); //Set Column
    }
}

void LCD_Print_Char(char data)  //Send 8-bits through 4-bit mode
{
   char Lower_Nibble,Upper_Nibble;
   Lower_Nibble = data&0x0F;
   Upper_Nibble = data&0xF0;
   PCF7584write(0,1);             // => RS = 1
	 
   LCD_SetBit(Upper_Nibble>>4);  //Send upper half by shifting by 4
   PCF7584write(2,1); //EN = 1
   Delay_ms_int(5); 
   PCF7584write(2,0); //EN = 0
	 
   LCD_SetBit(Lower_Nibble); //Send Lower half
   PCF7584write(2,1); //EN = 1
   Delay_ms_int(5); 
   PCF7584write(2,0); //EN = 0
}

void LCD_Print_String(char *a)
{
    int i;
    for(i=0;a[i]!='\0';i++)
		LCD_Print_Char(a[i]);  //Split the string using pointers and call the Char function 
}

void LCD_Print_Integer(long a)
{
	u8 check0 = 0; // to avoid printing 0 in the front
	d10 = a%10; // Last digit of the number
  d9 = (a/10)%10; // Second last digit of the number
  d8 = (a/100)%10; // And so on..
  d7 = (a/1000)%10;	
  d6 = (a/10000)%10;
  d5 = (a/100000)%10;
  d4 = (a/1000000)%10;
  d3 = (a/10000000)%10;
	d2 = (a/100000000)%10;
	d1 = (a/1000000000); 
	if(d1!=0) 
	{
	  LCD_Print_Char(d1+48); // ascii value of zero is 48
		check0 = 1;
	}
  if(d2!=0 || check0) 
	{
	  LCD_Print_Char(d2+48);
		check0 = 1;
	}
  if(d3!=0 || check0) 
	{
	  LCD_Print_Char(d3+48);
		check0 = 1;
	}
  if(d4!=0 || check0) 
	{
	  LCD_Print_Char(d4+48);
		check0 = 1;
	}
  if(d5!=0 || check0) 
	{
	  LCD_Print_Char(d5+48);
		check0 = 1;
	}
  if(d6!=0 || check0) 
	{
	  LCD_Print_Char(d6+48);
		check0 = 1;
	}
  if(d7!=0 || check0) 
	{
	  LCD_Print_Char(d7+48);
		check0 = 1;
	}
	if(d8!=0 || check0) 
	{
	  LCD_Print_Char(d8+48);
		check0 = 1;
	}
	if(d9!=0 || check0) 
	{
	  LCD_Print_Char(d9+48); // Second last digit 
	}
  LCD_Print_Char(d10+48); // Print the last digit whether or not 0
}

void LCD_BL_On(void) // Back light on
{
	PCF7584write(3,1);
}

void LCD_BL_Off(void) // Back light off
{
	PCF7584write(3,0);
}

void lcd_setup(void)
{
	LCD_Begin();
	LCD_Clear();
	LCD_BL_On();  // Back light on
	LCD_Set_Cursor(1,1);
	LCD_Print_String("STM8 LCD 1602");
	LCD_Set_Cursor(2,1);
	LCD_Print_String("by MICROPETA");
	Delay_ms_int(2000);
	LCD_Clear();
	LCD_BL_Off();  // Back light off
	LCD_Set_Cursor(1,1);
	LCD_Print_String("Nizar Mohideen");
	Delay_ms_int(2000);
	LCD_BL_On();  // Back light on
	LCD_Set_Cursor(2,1);
	LCD_Print_String("Score: ");
}

void I2C_Clock_Config(void)
{
	//enable internal HSI clock(16MHZ)
	CLK_HSICmd(ENABLE);

	//make sure internal clock(HSI) is stable
	while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);

	//set HSI DIV(High speed internal clock prescaler: 1)
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

	//enable i2c function
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
}
void lcd_count(void)
{
	LCD_Set_Cursor(2,8);
	LCD_Print_Integer(testValue);
	Delay_ms_int(100);
	testValue++;
}