/**
  ******************************************************************************
  * @file    discover_board.h
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   Input/Output defines
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DISCOVER_BOARD_H
#define __DISCOVER_BOARD_H

#define GPIO_HIGH(a,b) 		a->ODR|=b
#define GPIO_LOW(a,b)		a->ODR&=~b
#define GPIO_TOGGLE(a,b) 	a->ODR^=b    


#define LED_ON                GPIO_HIGH(GPIOA, GPIO_Pin_3)              //PA3
#define LED_OFF               GPIO_LOW(GPIOA, GPIO_Pin_3)
#define LED_TOGGLE            GPIO_TOGGLE(GPIOA, GPIO_Pin_3)
    
//#define RF_ON                 GPIO_WriteBit(GPIOC, GPIO_Pin_5,SET)      //RFout
//#define RF_OFF                GPIO_WriteBit(GPIOC, GPIO_Pin_5,RESET)

#define RF_ON                 GPIO_WriteBit(GPIOD, GPIO_Pin_0,SET)      //RFout
#define RF_OFF                GPIO_WriteBit(GPIOD, GPIO_Pin_0,RESET)

#define AS3933_SDI_WriteOne   GPIO_WriteBit(GPIOB, GPIO_Pin_3,SET)      //SDI     PB3
#define AS3933_SDI_WriteZero  GPIO_WriteBit(GPIOB, GPIO_Pin_3,RESET)
#define AS3933_SCL_WriteOne   GPIO_HIGH(GPIOB, GPIO_Pin_4)              //SCL     PB4
#define AS3933_SCL_WriteZero  GPIO_LOW(GPIOB, GPIO_Pin_4)     
#define AS3933_CS_WriteOne    GPIO_WriteBit(GPIOB, GPIO_Pin_5,SET)      //CS      PB5
#define AS3933_CS_WriteZero   GPIO_WriteBit(GPIOB, GPIO_Pin_5,RESET)

#define User_Key_Read         GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define AS3933_Wake_Read      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define AS3933_SDO_Read       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)
#define AS3933_DAT_Read       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
#define AS3933_DAT_CLK_Read   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#endif


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
