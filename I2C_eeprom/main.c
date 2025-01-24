#include "stm8s.h"
#include "stm8s_i2c.h"
//#include <stdint.h>


#define EEPROM_ADDRESS 0xA0 // AT24C系列的基本 I2C 地址
#define EEPROM_PAGE_SIZE 8  // EEPROM 每次寫入的頁面大小




void delay(uint32_t count)
{
     uint32_t i;
    for (i = 0; i < count; i++);
}
/**
 * @brief  初始化 I2C 接口
 */
 
 void Clock_Config(void)
{
    // 1. 啟用內部高速時鐘 (HSI)
    CLK_HSICmd(ENABLE);

    // 2. 確保內部高速時鐘 (HSI) 穩定
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);

    // 3. 設定 HSI 的分頻值 (確保系統運行在正確的頻率)
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    // 4. 啟用 I2C 周邊時鐘
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
}
 
void I2C_Config(void)
{
    // 初始化 I2C，時鐘頻率設置為 100kHz
    I2C_DeInit();
    I2C_Init(100000, 0x0011, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16);
}

/**
 * @brief  向 EEPROM 寫入數據
 * @param  mem_address: EEPROM 的內部存儲地址
 * @param  data: 要寫入的數據指針
 * @param  size: 要寫入的數據大小
 */
void EEPROM_Write(uint16_t mem_address, uint8_t *data, uint8_t size)
{
    uint8_t i;
		
		// 啟用 ACK       
		I2C_AcknowledgeConfig(I2C_ACK_CURR);

    // 開始 I2C 傳輸
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // 發送 EEPROM 地址（帶寫操作）
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // 發送內部存儲器地址高字節（適用於 16 位地址）
    I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // 發送內部存儲器地址低字節
    I2C_SendData((uint8_t)(mem_address & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // 發送數據
    for (i = 0; i < size; i++)
    {
        I2C_SendData(data[i]);
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    // 停止 I2C 傳輸
    I2C_GenerateSTOP(ENABLE);
}

/**
 * @brief  從 EEPROM 讀取數據
 * @param  mem_address: EEPROM 的內部存儲地址
 * @param  data: 存儲讀取數據的緩衝區
 * @param  size: 要讀取的數據大小
 */

/*
void EEPROM_Read(uint16_t mem_address, uint8_t *data, uint8_t size)
{
    uint16_t i;

    // 等待總線空閒
    while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
        ;

    // 發送起始信號
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;

    // 發送 EEPROM 地址（帶寫操作）
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    // 發送內部存儲器地址（高字節和低字節）
    I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData((uint8_t)(mem_address & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    // 重新啟動 I2C 傳輸
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;

    // 發送 EEPROM 地址（帶讀操作）
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;

    // 接收數據
		
		data[i] = I2C_ReceiveData();
		
    for (i = 0; i < size; i++)
    {
		
        if (i == size - 1)
        {
            // 最後一個字節，禁用 ACK
            I2C_AcknowledgeConfig(I2C_ACK_NONE);
        }
        else
        {
            // 其他字節，啟用 ACK
            I2C_AcknowledgeConfig(I2C_ACK_CURR);
        }

        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[i] = I2C_ReceiveData();
    }


    // 停止 I2C 傳輸
    I2C_GenerateSTOP(ENABLE);
}
*/


void EEPROM_Read(uint16_t mem_address, uint8_t *data, uint8_t size)
{
    uint8_t NumByteToRead;
		uint8_t i = 0;
		
		NumByteToRead= size;
		
		// 啟用 ACK       
		I2C_AcknowledgeConfig(I2C_ACK_CURR);

    // 等待 I2C 總線空閒
    while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));

    // 發送起始信號
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // 發送 EEPROM 地址（帶寫操作）
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // 發送內部存儲器地址（高字節和低字節）
    I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData((uint8_t)(mem_address & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // 發送重新啟動信號
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // 發送 EEPROM 地址（帶讀操作）
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // 根據需要讀取的字節數進行處理
    if (NumByteToRead == 1) // 單字節讀取
    {
        I2C_AcknowledgeConfig(I2C_ACK_NONE);  // 禁用 ACK
        I2C_GenerateSTOP(ENABLE);            // 發送 STOP 信號
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[0] = I2C_ReceiveData();         // 讀取數據
    }
    else if (NumByteToRead == 2) // 特殊處理兩字節讀取
    {
        I2C_AcknowledgeConfig(I2C_ACK_NONE); // 禁用 ACK
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[0] = I2C_ReceiveData();         // 讀取第一字節
        I2C_GenerateSTOP(ENABLE);            // 發送 STOP 信號
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[1] = I2C_ReceiveData();         // 讀取第二字節
    }
    else // 多字節讀取
    {
        for ( i = 0; i < NumByteToRead; i++)
        {
            if (i == (NumByteToRead - 1))    // 最後一個字節
            {
                I2C_AcknowledgeConfig(I2C_ACK_NONE);  // 禁用 ACK
                I2C_GenerateSTOP(ENABLE);            // 發送 STOP 信號
            }
            while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
            data[i] = I2C_ReceiveData();     // 讀取當前字節
        }
    }
}


/**
 * @brief  主函數測試
 */
 
void main(void)
{
    uint8_t write_data[8] = { 'H', 'e', 'l', 'l', 'o', ' ', '!', '\n' };
		
		uint8_t write_data1[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
																
	  uint8_t read_data[8] = {0};
		
		GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);

    // 初始化 I2C
		Clock_Config();
    I2C_Config();

    while (1){
			
		// 向 EEPROM 寫入數據
    //EEPROM_WriteRead(0x0000, write_data, 8, read_data, 8);
		
   // EEPROM_Write(0x0011, write_data1, 8);
	 
    // 延遲以等待 EEPROM 完成寫入
    delay(10000);

    // 從 EEPROM 讀取數據
    EEPROM_Read(0x0011,read_data, 8);
		
		delay(10000);
        
		GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
    delay(1000);
		GPIO_WriteLow(GPIOA, GPIO_PIN_3);
    delay(1000);
		
		}
}





