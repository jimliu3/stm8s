/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#include "stm8s.h"
#include "stm8s_conf.h"
#include "lcd_i2c.h"

main()
{
	//-----max7219---SPI--------------
	//SPI_Clock_Config();   //enable SPI feature
	//SPI_GPIO_setup();
	//SPI_setup();
	//MAX7219_init();       //MAX 7219 setup
	
	//display_clear();      //Clearing the display
	//Delay_ms_int(1000);
	//--------------------------------

	//GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST); //toggle gpio test pin
	
	MX_TIM4_Init();          //enable tim4 init and interrupt
  //-----LCD 1602---I2C-------------
	//I2C_Clock_Config();      //enable i2c feature
	//lcd_setup();             //LCD 1602 setup
  //--------------------------------
	
/* gpio ket interrupt
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_IT);
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC,EXTI_SENSITIVITY_FALL_LOW);
*/
	//rim();                 //enable interrupt

	while (1) {
	//GPIO_WriteReverse(GPIOA, GPIO_PIN_3);   //toggle gpio
	
	//lcd_count();

	//SPI_print();
	
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
}


