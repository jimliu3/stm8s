/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#include "stm8s.h"
#include "stm8s_conf.h"
#include "lcd_i2c.h"

int testValue = 0;

void I2C_Clock_Config(void);
void SPI_Clock_Config(void);
void lcd_setup(void);
void GPIO_setup(void);
void lcd_count(void);
void SPI_print(void);

const char input_string2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

main()
{
	//I2C_Clock_Config();
	SPI_Clock_Config();
	GPIO_setup();
	SPI_setup();
	MX_TIM4_Init();
	//lcd_setup();
	MAX7219_init();
	rim();
	
	//
	  display_clear(); //Clearing the display
		delay_ms(1000);

	while (1) {
		//Delay_ms_int(100);
		//GPIO_WriteReverse(GPIOA, GPIO_PIN_3);
		
		//lcd_count();
		
		SPI_print();
	}

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

void SPI_Clock_Config(void)
{
     CLK_DeInit();
                
 
     CLK_HSICmd(ENABLE);
     while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
     CLK_ClockSwitchCmd(ENABLE);
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
     CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);                
     CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, 
     DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
                
     CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
 
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
	delay_ms(2000);
	LCD_Clear();
	LCD_BL_Off();  // Back light off
	LCD_Set_Cursor(1,1);
	LCD_Print_String("Nizar Mohideen");
	delay_ms(2000);
	LCD_BL_On();  // Back light on
	LCD_Set_Cursor(2,1);
	LCD_Print_String("Score: ");
}

void GPIO_setup(void)
{
     GPIO_DeInit(GPIOC);
     GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_5 | GPIO_PIN_6), 
          GPIO_MODE_OUT_PP_HIGH_FAST);
		 
		 GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
}

void lcd_count(void)
{
	LCD_Set_Cursor(2,8);
	LCD_Print_Integer(testValue);
	delay_ms(100);
	testValue++;
}

void SPI_print(void)
{
		display_clear(); //Clearing the Display
		display_string(input_string2);  //Displaying a String
		delay_ms(2000);
}
