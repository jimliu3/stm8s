#include "stm8s.h"
#include "stm8s_conf.h"
#include "lcd_i2c.h"

int testValue = 0;

#define FLASH_START_ADDR  0x4000  
#define LED_PORT          GPIOB   
#define LED_PIN           GPIO_PIN_5  
uint8_t data1,data2;
uint32_t i;
uint8_t Flash_ReadData(uint32_t address);

void GPIO_setup(void);
void lcd_count(void);
void SPI_print(void);
void GPIO_Init_LED(void);
void LED_Toggle(void);
void Flash_WriteData(void);
void Flash_Verify(void);

const char input_string2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

main()
{
	//I2C_Clock_Config();
	//SPI_Clock_Config();
	//GPIO_setup();
	//SPI_setup();
	//MX_TIM4_Init();
	//lcd_setup();
	//MAX7219_init();
	//rim();
	
	//
	  //display_clear(); //Clearing the display
		//delay_ms(1000);
	
	GPIO_Init_LED(); 
    Flash_WriteData(); 
    Flash_Verify(); 
    
	while (1) {
		//Delay_ms_int(100);
		//GPIO_WriteReverse(GPIOA, GPIO_PIN_3);
		
		//lcd_count();
		
		//SPI_print();
		if (data1 == 0x01 && data2 == 0x02) {
        	LED_Toggle();
			for (i = 0; i < 5000; i++);
    	} else {
			GPIO_WriteHigh(LED_PORT, LED_PIN);		
    	}
	}

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

void GPIO_Init_LED(void) {
    GPIO_DeInit(LED_PORT);
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}


void LED_Toggle(void) {
    GPIO_WriteReverse(LED_PORT, LED_PIN);
}


void Flash_WriteData(void) {
    FLASH_Unlock(FLASH_MEMTYPE_DATA); 

    FLASH_ProgramByte(FLASH_START_ADDR, 0x01); 
    FLASH_ProgramByte(FLASH_START_ADDR + 1, 0x02); 
	FLASH_ProgramByte(FLASH_START_ADDR + 2, 0x03);
    FLASH_ProgramByte(FLASH_START_ADDR + 3, 0x04);
	FLASH_ProgramByte(FLASH_START_ADDR + 4, 0x05);
	FLASH_ProgramByte(FLASH_START_ADDR + 5, 0x0F);
    FLASH_Lock(FLASH_MEMTYPE_DATA); 
}


uint8_t Flash_ReadData(uint32_t address) {
    return FLASH_ReadByte(address);
}



void Flash_Verify(void) {	
    data1 = Flash_ReadData(FLASH_START_ADDR);
    data2 = Flash_ReadData(FLASH_START_ADDR + 1);
}
