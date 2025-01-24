#include "stm8s.h"
#include "stm8s_i2c.h"
//#include <stdint.h>


#define EEPROM_ADDRESS 0xA0 // AT24C�t�C���� I2C �a�}
#define EEPROM_PAGE_SIZE 8  // EEPROM �C���g�J�������j�p




void delay(uint32_t count)
{
     uint32_t i;
    for (i = 0; i < count; i++);
}
/**
 * @brief  ��l�� I2C ���f
 */
 
 void Clock_Config(void)
{
    // 1. �ҥΤ������t���� (HSI)
    CLK_HSICmd(ENABLE);

    // 2. �T�O�������t���� (HSI) í�w
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);

    // 3. �]�w HSI �����W�� (�T�O�t�ιB��b���T���W�v)
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    // 4. �ҥ� I2C �P�����
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
}
 
void I2C_Config(void)
{
    // ��l�� I2C�A�����W�v�]�m�� 100kHz
    I2C_DeInit();
    I2C_Init(100000, 0x0011, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16);
}

/**
 * @brief  �V EEPROM �g�J�ƾ�
 * @param  mem_address: EEPROM �������s�x�a�}
 * @param  data: �n�g�J���ƾګ��w
 * @param  size: �n�g�J���ƾڤj�p
 */
void EEPROM_Write(uint16_t mem_address, uint8_t *data, uint8_t size)
{
    uint8_t i;
		
		// �ҥ� ACK       
		I2C_AcknowledgeConfig(I2C_ACK_CURR);

    // �}�l I2C �ǿ�
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // �o�e EEPROM �a�}�]�a�g�ާ@�^
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // �o�e�����s�x���a�}���r�`�]�A�Ω� 16 ��a�}�^
    I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // �o�e�����s�x���a�}�C�r�`
    I2C_SendData((uint8_t)(mem_address & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // �o�e�ƾ�
    for (i = 0; i < size; i++)
    {
        I2C_SendData(data[i]);
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    // ���� I2C �ǿ�
    I2C_GenerateSTOP(ENABLE);
}

/**
 * @brief  �q EEPROM Ū���ƾ�
 * @param  mem_address: EEPROM �������s�x�a�}
 * @param  data: �s�xŪ���ƾڪ��w�İ�
 * @param  size: �nŪ�����ƾڤj�p
 */

/*
void EEPROM_Read(uint16_t mem_address, uint8_t *data, uint8_t size)
{
    uint16_t i;

    // �����`�u�Ŷ�
    while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
        ;

    // �o�e�_�l�H��
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;

    // �o�e EEPROM �a�}�]�a�g�ާ@�^
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    // �o�e�����s�x���a�}�]���r�`�M�C�r�`�^
    I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData((uint8_t)(mem_address & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    // ���s�Ұ� I2C �ǿ�
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;

    // �o�e EEPROM �a�}�]�aŪ�ާ@�^
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;

    // �����ƾ�
		
		data[i] = I2C_ReceiveData();
		
    for (i = 0; i < size; i++)
    {
		
        if (i == size - 1)
        {
            // �̫�@�Ӧr�`�A�T�� ACK
            I2C_AcknowledgeConfig(I2C_ACK_NONE);
        }
        else
        {
            // ��L�r�`�A�ҥ� ACK
            I2C_AcknowledgeConfig(I2C_ACK_CURR);
        }

        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[i] = I2C_ReceiveData();
    }


    // ���� I2C �ǿ�
    I2C_GenerateSTOP(ENABLE);
}
*/


void EEPROM_Read(uint16_t mem_address, uint8_t *data, uint8_t size)
{
    uint8_t NumByteToRead;
		uint8_t i = 0;
		
		NumByteToRead= size;
		
		// �ҥ� ACK       
		I2C_AcknowledgeConfig(I2C_ACK_CURR);

    // ���� I2C �`�u�Ŷ�
    while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));

    // �o�e�_�l�H��
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // �o�e EEPROM �a�}�]�a�g�ާ@�^
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // �o�e�����s�x���a�}�]���r�`�M�C�r�`�^
    I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData((uint8_t)(mem_address & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // �o�e���s�ҰʫH��
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // �o�e EEPROM �a�}�]�aŪ�ާ@�^
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // �ھڻݭnŪ�����r�`�ƶi��B�z
    if (NumByteToRead == 1) // ��r�`Ū��
    {
        I2C_AcknowledgeConfig(I2C_ACK_NONE);  // �T�� ACK
        I2C_GenerateSTOP(ENABLE);            // �o�e STOP �H��
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[0] = I2C_ReceiveData();         // Ū���ƾ�
    }
    else if (NumByteToRead == 2) // �S��B�z��r�`Ū��
    {
        I2C_AcknowledgeConfig(I2C_ACK_NONE); // �T�� ACK
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[0] = I2C_ReceiveData();         // Ū���Ĥ@�r�`
        I2C_GenerateSTOP(ENABLE);            // �o�e STOP �H��
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[1] = I2C_ReceiveData();         // Ū���ĤG�r�`
    }
    else // �h�r�`Ū��
    {
        for ( i = 0; i < NumByteToRead; i++)
        {
            if (i == (NumByteToRead - 1))    // �̫�@�Ӧr�`
            {
                I2C_AcknowledgeConfig(I2C_ACK_NONE);  // �T�� ACK
                I2C_GenerateSTOP(ENABLE);            // �o�e STOP �H��
            }
            while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
            data[i] = I2C_ReceiveData();     // Ū����e�r�`
        }
    }
}


/**
 * @brief  �D��ƴ���
 */
 
void main(void)
{
    uint8_t write_data[8] = { 'H', 'e', 'l', 'l', 'o', ' ', '!', '\n' };
		
		uint8_t write_data1[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
																
	  uint8_t read_data[8] = {0};
		
		GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);

    // ��l�� I2C
		Clock_Config();
    I2C_Config();

    while (1){
			
		// �V EEPROM �g�J�ƾ�
    //EEPROM_WriteRead(0x0000, write_data, 8, read_data, 8);
		
   // EEPROM_Write(0x0011, write_data1, 8);
	 
    // ����H���� EEPROM �����g�J
    delay(10000);

    // �q EEPROM Ū���ƾ�
    EEPROM_Read(0x0011,read_data, 8);
		
		delay(10000);
        
		GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
    delay(1000);
		GPIO_WriteLow(GPIOA, GPIO_PIN_3);
    delay(1000);
		
		}
}





