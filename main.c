/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#include "stm8s.h"
#include "stm8s_conf.h"
#include "lcd_i2c.h"

int testValue = 0;
void lcd_count(void);

main()
{
	/*
	SPI_Clock_Config();
	SPI_GPIO_setup();
	SPI_setup();

	//GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST); //toggle gpio test pin
	//I2C_Clock_Config();  //enable i2c feature

	//MX_TIM4_Init();  //enable tim4 init and interrupt
	MAX7219_init();
	//lcd_setup();

	rim();    //enable interrupt

	display_clear(); //Clearing the display
	delay_ms(1000);

	while (1) {
	//Delay_ms_int(100);    //toggle gpio delay
	//GPIO_WriteReverse(GPIOA, GPIO_PIN_3);   //toggle gpio

	//lcd_count();

	SPI_print();
	}
	*/
	
		GPIO_Init_LED(); 
    Flash_WriteData(); 
    Flash_Verify(); 

	while (1){
		if (data1 == 0x01 && data2 == 0x02) {
        LED_Toggle();
				for (i = 0; i < 5000; i++);
    } else {
				GPIO_WriteHigh(LED_PORT, LED_PIN);
				
    }
	} 
}

void lcd_count(void)
{
	LCD_Set_Cursor(2,8);
	LCD_Print_Integer(testValue);
	delay_ms(100);
	testValue++;
}
