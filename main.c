/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#include "stm8s.h"
#include "stm8s_conf.h"

void Clock_Config(void)
{
	//enable internal HSI clock(16MHZ)
	CLK_HSICmd(ENABLE);

	//make sure internal clock(HSI) is stable
	while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);

	//set HSI DIV(High speed internal clock prescaler: 1)
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

	//enable i2c function
	//CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
}

main()
{
	Clock_Config();
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_IT);
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_IT);
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC,EXTI_SENSITIVITY_FALL_LOW);

	MX_TIM4_Init();
	rim();

	while (1) {
		Delay_ms_int(100);
		GPIO_WriteReverse(GPIOA, GPIO_PIN_3);
	}

}