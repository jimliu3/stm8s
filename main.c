/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#include "stm8s.h"
#include "stm8s_conf.h"
#include "LF_Send.h"


#define BP_ON           GPIOD->ODR |=  (1<<4)               
#define BP_OFF          GPIOD->ODR &= ~(1<<4)
#define Learn_LED_OFF   GPIOD->ODR |=  (1<<2)               
#define Learn_LED_ON    GPIOD->ODR &= ~(1<<2)
#define Mode_LED_OFF    GPIOA->ODR |=  (1<<3)               
#define Mode_LED_ON     GPIOA->ODR &= ~(1<<3)        
#define Lfsend_LED_OFF  GPIOA->ODR |=  (1<<2)               
#define Lfsend_LED_ON   GPIOA->ODR &= ~(1<<2)
#define Out_LED_OFF     GPIOA->ODR |=  (1<<1)              
#define Out_LED_ON      GPIOA->ODR &= ~(1<<1)

#define Learn_Key       GPIO_ReadInputPin(GPIOC, GPIO_PIN_5)
#define Mode_Key        GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)
#define Lfsend_Key      GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)

#define RFin            GPIO_ReadInputPin(GPIOD, GPIO_PIN_3)


#define BIT_TEST(x, y)  (((x)&(1<<(y)))!= 0) 
#define setbit( b, n)   ((b) |= ( 1 << (n)))   
#define getbit( b, n)   (((b) & ((u32)1<<n)) ? 1 : 0)   
#define ifbit(x,y)      if (getbit(x,y)) 

#define   TRUE     1
#define   FALSE    0
#define   TOUT     600           

#define RF_NUM       10         
#define RF_Byte_LEN  3           
#define RF_LEN       24          

unsigned char RFFull = 0;       
unsigned char RFBit;            
unsigned char LL_w = 0;         
unsigned char First_flag = 0;   
unsigned char Buff_B[3];        
unsigned char BitCount;         

unsigned char FLearn = 0;		
unsigned int  COut = 0;                
unsigned int  CLearn = 0;               
unsigned int  CTLearn = 0;
unsigned char LF_Send_flag = 0;		
unsigned char Mode_Key_Old = 0;         
unsigned char Send_Key_Old = 0;       
unsigned char MLearn = 0;		
unsigned char CSend = 0;                

unsigned char Time_1ms = 0;
unsigned char Time_Nms = 0;

unsigned char User_LF_Send = 0;
void Key_Scan(void);

const unsigned int wCRCTalbeAbs[] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};

unsigned int GetCRC16(unsigned char *pchMsg,  unsigned char wDataLen)
{ 
    unsigned int wCRC = 0xFFFF;
    unsigned int i;
    unsigned char chChar;  
    
    for (i = 0; i < wDataLen; i++)
    {
         chChar = *pchMsg++;
         wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
         wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
    }

     return wCRC;    
} 

void SystemClock_Init(void)
{
	//enable internal HSI clock(16MHZ)
	CLK_HSICmd(ENABLE);

	//make sure internal clock(HSI) is stable
	while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);

	//set HSI DIV(High speed internal clock prescaler: 1)
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

}

void TIM2_Init(void)
{
     TIM2_DeInit();
     TIM2_TimeBaseInit(TIM2_PRESCALER_16,100); 
     TIM2_ITConfig(TIM2_IT_UPDATE , ENABLE);
     TIM2_SetCounter(0x0000);
     TIM2_Cmd(ENABLE);
}

void Read_EEpeomData(void)
{
    unsigned int x;
    unsigned char EEprom_Buff[SET_BUFF_MAX];
    
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    for(x = 0;x < SET_BUFF_MAX;x ++)  EEprom_Buff[x] = FLASH_ReadByte(0x00004000 + x); 
    
    FLASH_Lock(FLASH_MEMTYPE_DATA); 
    
    if((EEprom_Buff[0] == 0xa5)&&(EEprom_Buff[1] == 0x5a)) 
    {
	for(x = 0;x < SET_BUFF_MAX;x ++)   Set_Buff[x] = EEprom_Buff[x];
        LF_PLL_SET(LF_PLL);           
    }					 
       
    if(LF_ENABLE != 1) Mode_LED_ON;	
    else
	Mode_LED_OFF;
}   

void BEEP_BEEP(void)
{
     unsigned char i;

     for(i = 0; i < 100; i ++)
     {
          BP_ON;
          Delay_us(150);

          BP_OFF;
          Delay_us(150);
          
     }
}

void InIt(void)
{
    unsigned char OPT2 = 0x01;
    unsigned char OPT_Temp = 0;
    
    SystemClock_Init();
    
    GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
    
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);
    
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
     
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
    
    
    Delay_InIt(16);         
    TIM2_Init();           
    LF_ClockOccurs(125);    
    Read_EEpeomData(); 
    
    OPT_Temp =  *((unsigned char*)0x4803);
    
    if(OPT_Temp != OPT2)
    {
        FLASH_Unlock(FLASH_MEMTYPE_DATA);
        while(!(FLASH->IAPSR & 0x08));
        FLASH->CR2 |= 0x80;
        FLASH->NCR2 &= 0x7f;
        *((unsigned char*)0x4803) = OPT2;
        *((unsigned char*)0x4804) = ~OPT2;
        FLASH->CR2 &= 0x7f;
        FLASH->NCR2 |= 0x80;
        FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
    BEEP_BEEP();  
    enableInterrupts();
}

void RF_Remote(void)                  
{
    unsigned char i,j;
    unsigned char HF_Key = 0;
    unsigned char Buffer[RF_Byte_LEN];  
    unsigned char RF_UartSend[RF_Byte_LEN + 8];
    unsigned char RF_num;
    unsigned int  crc;
       
    for(i = 0; i < RF_Byte_LEN; i ++)  Buffer[i] = Buff_B[i]; 
                    
    HF_Key = Buffer[RF_Byte_LEN - 1] & 0x0F;                  
    Buffer[RF_Byte_LEN - 1] = Buffer[RF_Byte_LEN - 1] & 0xF0; 

    for(i = 0; i < RF_Byte_LEN + 1; i ++)
	      RF_UartSend[i + 2] = Buffer[i];    
        
    RF_UartSend[0] = 0;     
    RF_UartSend[1] = 0;
    RF_UartSend[2] = 0xFA; 
    RF_UartSend[3] = 0xDD;
    for(i = 0; i < RF_Byte_LEN; i ++)  RF_UartSend[4 + i] = Buffer[i];   
    RF_UartSend[RF_Byte_LEN + 4] = HF_Key;
    
    crc = GetCRC16(&RF_UartSend[2], RF_Byte_LEN + 3);   
    
    RF_UartSend[RF_Byte_LEN + 5] = crc >> 8;
    RF_UartSend[RF_Byte_LEN + 6] = crc & 0xFF;   
    RF_UartSend[RF_Byte_LEN + 7] = 0xEE;	
  
    RFFull = 0;                                        

    if(FLearn) 		                              
    {    
        disableInterrupts();                          
        FLASH_Unlock(FLASH_MEMTYPE_DATA);
        for(i = 0; i < RF_NUM; i ++) 		        
        {
            for(j = 0; j < RF_Byte_LEN; j ++)
            {
                if(Buffer[j] != FLASH_ReadByte(0x00004000  + SET_BUFF_MAX + i * RF_Byte_LEN + j + 1))  break;
            }
            if(j == RF_Byte_LEN)                 
            {
                CTLearn = 1000;
                FLASH_Lock(FLASH_MEMTYPE_DATA);
                enableInterrupts();             
                return;
            }
        }
        
        RF_num = FLASH_ReadByte(0x00004000 + SET_BUFF_MAX);            
        if(RF_num > RF_NUM) RF_num = 0;        
        
        for(i = 0; i < RF_Byte_LEN; i ++)
              FLASH_ProgramByte(0x00004000 + SET_BUFF_MAX + RF_num * RF_Byte_LEN + i + 1,Buffer[i]);
        RF_num = RF_num + 1;
        FLASH_ProgramByte(0x00004000 + SET_BUFF_MAX , RF_num);
     
        FLASH_Lock(FLASH_MEMTYPE_DATA);  
        
        Learn_LED_OFF;
        FLearn = 0;                    
        enableInterrupts();                                     
    } 
    else
    {
          disableInterrupts();
          
          for(i = 0; i < RF_NUM; i ++)
          {
              for(j = 0;j < RF_Byte_LEN;j ++)	
	      {
                   if(Buffer[j] != FLASH_ReadByte(0x00004000  + SET_BUFF_MAX + i * RF_Byte_LEN + j + 1))  
					   break; 			
	      }
              if(j == RF_Byte_LEN)
              {
                  if(COut == 0)
                  {      
                      if((HF_Key & 0x01) == 0x01)  Out_LED_ON;
                      if((HF_Key & 0x02) == 0x02)  Out_LED_ON;
                      if((HF_Key & 0x04) == 0x04)  Out_LED_ON;
                      if((HF_Key & 0x08) == 0x08)  Out_LED_ON;        
                      BEEP_BEEP();
                  }                 
                  COut = TOUT;
                  enableInterrupts(); 
		  break;                   
              }
          }
          enableInterrupts();
    }
}

main()
{
	InIt();

	while (1)
    {      
              
        if(RFFull)  RF_Remote();     
        
        if(Time_Nms >= 10)           
        {
             Time_Nms = 0;
             Key_Scan();	
	     if(COut > 0)
	     {
		COut --;
		if(COut == 0)
		{
                     Out_LED_OFF;
		}
	     }
             if(LF_ENABLE == 1)  Mode_LED_OFF;	
        }
        if((LF_ENABLE == 1)&&(LF_Send_Tim == 0)) 
        {
            Lfsend_LED_ON;
            LF_SendData(PATTERN1,PATTERN2,PATTREN_BIT,LF_SEND_CH1);
	    LF_Send_Tim = (INTERVAL1 >> 4) * 1000 + (INTERVAL1 &0x0F) * 100 + (INTERVAL2 >> 4) * 10 + (INTERVAL2 &0x0F);
            Lfsend_LED_OFF;
        }
        else if(User_LF_Send)
        {
            User_LF_Send = 0;
            Lfsend_LED_ON;
            LF_SendData(PATTERN1,PATTERN2,PATTREN_BIT,LF_SEND_CH1);
            Lfsend_LED_OFF;          
        }
    } 

}

void Key_Scan(void)           
{
    unsigned char i;
    
    if(Learn_Key == 0) 
    {
        CLearn++;
	if(CLearn == 500)	   
	{
            Learn_LED_OFF;
            FLearn = 0;	
            
            FLASH_Unlock(FLASH_MEMTYPE_DATA);
            for(i = 0;i < RF_NUM * RF_Byte_LEN + 1;i ++)  
            {
                FLASH_ProgramByte(0x00004000 + SET_BUFF_MAX + i,0);
            }
            FLASH_Lock(FLASH_MEMTYPE_DATA); 
	    CLearn = 0;
	    while(!Learn_Key) ;	 
	} 
        if(CLearn == 10)         
        {
	    Learn_LED_ON;	
	    FLearn = 1;					
	    CTLearn = 1000;	         
        }  
    }
    else
    {
	CLearn = 0;
    }
    
    if(CTLearn > 0)		
    {
	CTLearn --;
	if(CTLearn == 0)
	{
            Learn_LED_OFF; 
            FLearn = 0;	
	}	
    }  
    
    if(!Mode_Key)              
    {
        if(Mode_Key != Mode_Key_Old)   MLearn ++; 

	if(MLearn >= 10)
        {	
             MLearn = 0;
	     Mode_Key_Old = Mode_Key;
							
             if(LF_ENABLE == 0)
             {
                 LF_ENABLE = 1; 	 
		 Mode_LED_OFF;
             }
	     else
	     {
		 LF_ENABLE = 0;		 
		 Mode_LED_ON;		
             }
             FLASH_Unlock(FLASH_MEMTYPE_DATA); 
             FLASH_ProgramByte(0x00004000 + 12,LF_ENABLE);
             FLASH_Lock(FLASH_MEMTYPE_DATA);                  
	 }					
   
    }
    else
    {
	MLearn = 0;
	Mode_Key_Old = 1;        
    }
    
    if(!Lfsend_Key)
    {
	if(Send_Key_Old)
	{
	     CSend ++;
	     if(CSend >= 2)
	     {
		CSend = 0;
		if(LF_ENABLE == 0)  User_LF_Send = 1;
		else	User_LF_Send = 0;	
		Send_Key_Old = 0;  			
	     }					  
	}      
    }
    else
    {
	 CSend = 0;
	 Send_Key_Old = 1;
    }
}


@far @interrupt void pc_irqhandler(void)
{
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
	Time_1ms ++;

    if(Time_1ms >= 10)
    {
        Time_1ms = 0;
        Time_Nms ++;

        if((LF_ENABLE == 1) && (LF_Send_Tim > 0))       LF_Send_Tim --; 
	else
		LF_Send_Tim = 0;
    }

    if(RFFull)   return; 
    
    if(!RFin) {LL_w ++; RFBit = 0;}
    else                                                                             
    {
         if(!RFBit)                   
         {
              if(!First_flag)       
              {
                    if((LL_w > 40)&&(LL_w < 60))   
                    {
			First_flag = 1;
			BitCount = 0; 	
                    }                
              }
              else           
              {
                    if((LL_w > 3)&&(LL_w <= 7))	 
                    {
			Buff_B[BitCount/8] = Buff_B[BitCount/8] << 1;    
			Buff_B[BitCount/8] = Buff_B[BitCount/8] | 0x01; 
			BitCount ++; 
			if(BitCount == 24)                              
			{
                              BitCount = 0;
                              First_flag = 0;
                              RFFull = 1; 
                        }
                    }
                    else if((LL_w >=8)&&(LL_w < 13))  
                    {
                        Buff_B[BitCount/8] = Buff_B[BitCount/8] << 1;
                        BitCount ++;
                        if(BitCount == 24)           		       
			{
                              BitCount = 0;
                              First_flag = 0;                             
                              RFFull = 1; 
			}	
                    }
		    else			
		    {
			First_flag = 0;
			BitCount = 0;
			RFFull = 0;											
		    }                    
              }
              LL_w = 0;
         }
         RFBit = 1;  
    }  
}