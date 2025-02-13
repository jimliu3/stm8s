#include "STM8S.h"
#include "stm8s_i2c.h"

#define LCD_I2C_Address  (0x27 << 1)

//void delay_ms (int ms);
void LCD_Begin(void);
void LCD_Clear(void);
void LCD_Set_Cursor(char a, char b);
void LCD_Print_Char(char data);
void LCD_Print_String(char *a);
void LCD_Print_Integer(long a);
void LCD_BL_On(void); // Back light on
void LCD_BL_Off(void); // Back light off
void I2C_Clock_Config(void);