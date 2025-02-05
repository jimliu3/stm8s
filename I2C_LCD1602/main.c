/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#include "stm8s.h"
#include "stm8s_conf.h"
#include "lcd_i2c.h"

void Clock_Config(void)
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

main()
{
	int testValue = 0;
	Clock_Config();
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
	MX_TIM4_Init();
	lcd_setup();
	rim();

	while (1) {
		//Delay_ms_int(100);
		//GPIO_WriteReverse(GPIOA, GPIO_PIN_3);
		LCD_Set_Cursor(2,8);
		LCD_Print_Integer(testValue);
		delay_ms(100);
		testValue++;
	}

}