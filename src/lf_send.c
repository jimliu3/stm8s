#include "stm8s.h"
#include "lf_Send.h"
#include "uart.h"
#include "CommandAgreement.h"

unsigned char Set_Buff[SET_BUFF_MAX];     
unsigned int Bit_element;
unsigned int Carrier_Time;
unsigned int LF_Send_Tim;      

volatile unsigned char fac_us = 0;

void Delay_InIt(unsigned char clk)
{
    if(clk > 16) fac_us = (16 - 4)/4;
    else if(clk > 4) fac_us = (clk - 4)/4; 
    else fac_us = 1;
}

void Delay_us(unsigned int nus)
{
      __asm(
    "PUSH A          \n"
    "DELAY_XUS:      \n"
    "LD A,fac_us     \n"
    "DELAY_US_1:     \n"
    "NOP             \n"
    "DEC A           \n"
    "JRNE DELAY_US_1 \n"
    "NOP             \n"
    "DECW X          \n"
    "JRNE DELAY_XUS  \n"
    "POP A           \n"
    );  
}

void LF_ClockOccurs(unsigned char LF_Pll)
{      
    unsigned int Pll_num;
    
    Pll_num = (16000 / LF_Pll) - 1;  
  
    TIM1 -> CNTRH = 0;        
    TIM1 -> CNTRL = 0;   
    TIM1 -> PSCRH = 0;
    TIM1 -> PSCRL = 0;            
    TIM1 -> ARRH = (unsigned char)(Pll_num >> 8);  
    TIM1 -> ARRL = (unsigned char)(Pll_num);  
    TIM1 -> CR1 &= 0x8F;       
    
    TIM1 -> CCR1H = (unsigned char)((Pll_num / 2) >> 8);
    TIM1 -> CCR1L = (unsigned char)(Pll_num / 2);    
    
    TIM1 -> CCMR1 = 0x60;      
    TIM1 -> CCER1 &= 0xFD;     
    TIM1 -> CCER1 |= 0x01;     
    TIM1 -> OISR |= 0x01;      
        
    TIM1 -> BKR |= 0x80;
    TIM1 -> CR1 |= 0x01;
}

void LF_PLL_SET(unsigned char LF_Pll)
{
    unsigned int Pll_num;
    
    Pll_num = (16000 / LF_Pll) - 1;  //us
    
    TIM1 -> CR1 &= 0xFE;
    
    TIM1 -> ARRH = (unsigned char)(Pll_num >> 8);  
    TIM1 -> ARRL = (unsigned char)(Pll_num);  
    TIM1 -> CCR1H = (unsigned char)((Pll_num / 2) >> 8);
    TIM1 -> CCR1L = (unsigned char)(Pll_num / 2);  
    
    TIM1 -> CR1 |= 0x01;
}


void Out_125K(unsigned int tim, unsigned char LF_Send_CHx)	
{
     switch(LF_Send_CHx)
     {
          case LF_SEND_CH1:  CH1_GPIO_OPEN;	       
			      Delay_us(tim);				
			break;
          case LF_SEND_CH2:  CH2_GPIO_OPEN;	
			      Delay_us(tim);					
			break;	
          case LF_SEND_CH3:  CH3_GPIO_OPEN;	
                              Delay_us(tim);				
			break;	                        
          default:
			break;
     }
}

void Clock_125K(unsigned int tim, unsigned char LF_Send_CHx)	
{
     switch(LF_Send_CHx)
     {
          case LF_SEND_CH1:  CH1_GPIO_CLOCK;	        
			      Delay_us(tim);				
			break;
          case LF_SEND_CH2:  CH2_GPIO_CLOCK;	
			      Delay_us(tim);					
			break;	
          case LF_SEND_CH3:  CH3_GPIO_CLOCK;	
                              Delay_us(tim);				
			break;	                        
          default:
			break;
     }
}

void Timecalculate(void)	
{
    unsigned char  Tcarr;       
    unsigned char  Tclk;        
    
    Tcarr = 1000 / LF_PLL;  
    Tclk  = 1000000 / (RC_PLL1 * 256 + RC_PLL2);
    Tclk = 256 - Tclk;
   
    Bit_element = LFBIT_NxRC + Tclk * LFBIT_NxRC;  
      
    if(ONOFF_SCAN)                                 
    {
        if((LF_PLL >= 15) && (LF_PLL <= 23))
        {
            Carrier_Time = 220 * Tclk + 8 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    
        }
	else if((LF_PLL > 23) && (LF_PLL <= 40))
	{
            Carrier_Time = 224 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    
	}
	else if((LF_PLL > 40) && (LF_PLL <= 65))
	{
            Carrier_Time = 180 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    
	}	
	else if((LF_PLL > 65)&&(LF_PLL <= 95))
	{
            Carrier_Time = 92 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    
	}	
	else if((LF_PLL > 95)&&(LF_PLL <= 150))
	{
            Carrier_Time = 80 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    
	}
        else     
          Carrier_Time =  3000;      //us
    }
    else                //ON/OFF模式
    {
	if((LF_PLL >= 15) && (LF_PLL <= 23))
	{
            Carrier_Time = 92 * Tclk + 8 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    //us
	}
	else if((LF_PLL > 23) && (LF_PLL <= 40))
	{         
            Carrier_Time = 96 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    //us
	}
	else if((LF_PLL > 40) && (LF_PLL <= 65))
	{
            Carrier_Time = 52 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    //us
	}	
	else if((LF_PLL > 65) && (LF_PLL <= 95))
	{
            Carrier_Time = 28 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 500;    //us
	}	
	else if((LF_PLL > 95) && (LF_PLL <= 150))
	{
            Carrier_Time = 16 * Tclk + 16 * Tcarr;
            Carrier_Time =  Carrier_Time + 1000;   //us
	}
        else
            Carrier_Time =  2000;    //us
    }   
}

/********************************************************************************************/
/*                    void CarrierBurst(u16 Bit_Tim)                         */
/***************************          发送起始时间         **********************************/
/*                                                */
/*  Bit_Tim      编码的时间 * 30us                */
/********************************************************************************************/

void CarrierBurst(unsigned char LF_Send_CHx)
{
      unsigned char i;
      
      CH1_GPIO_CLOCK;
      CH2_GPIO_CLOCK;
      CH3_GPIO_CLOCK;
      
      Timecalculate();                                            
      Out_125K(Carrier_Time,LF_Send_CHx);	
      Clock_125K(Bit_element,LF_Send_CHx);		 
		
      if(LFSENDMODE)			                 					
      {
          for(i = 0;i < 8;i ++)	 	                 
          {
              Out_125K(Bit_element,LF_Send_CHx); 	 
              Clock_125K(Bit_element,LF_Send_CHx);	               
          }         
      }
}

/********************************************************************************************/
/*         void Pattern(u8 R6_Dat,u8 R5_Dat,u8 Patt16_32,u8 LF_Send_CHx,u16 Bit_Tim)        */
/***************************        发送Pattern数据        **********************************/
/*  R6_Dat ----- Pattern1 对应AS3933 R6寄存器的值 */ 
/*  R5_Dat ----- Pattern2 对应AS3933 R6寄存器的值 */
/*  Patt16_32    Pattern的编码位数                */
/*  LF_Send_CHx    发送的通道                     */
/********************************************************************************************/

void Pattern(unsigned char R6_Dat,unsigned char R5_Dat,unsigned char Patt16_32,unsigned char LF_Send_CHx)
{
     unsigned char i;
     unsigned int Patt_Data = 0;
     unsigned int Temp_x = 0;
     
     Patt_Data = R6_Dat * 256 + R5_Dat;
     Temp_x = 0x8000;
     
     if(Patt16_32 == Patt_32bit)
     {
	 for(i = 0;i < 16;i ++)
	 {          					
	    if(Patt_Data & Temp_x)	       //数据1
	    {
                Out_125K(Bit_element,LF_Send_CHx);
                Clock_125K(Bit_element,LF_Send_CHx);					
	    }
	    else
	    {
		Clock_125K(Bit_element,LF_Send_CHx);
		Out_125K(Bit_element,LF_Send_CHx);								
	    }
            Patt_Data = Patt_Data << 1; 
	 }			
     }
     else
     {
	 for(i = 0;i < 16;i ++)
	 {            					
	     if(Patt_Data & Temp_x)	
	     {
                  Out_125K(Bit_element,LF_Send_CHx);				
             }
	     else
	     {
                  Clock_125K(Bit_element,LF_Send_CHx);						
	     }	
             Patt_Data = Patt_Data << 1; 
	 }
     }
}

/********************************************************************************************/
/*  void LF_SendData(unsigned char R6_Dat,unsigned char R5_Dat,unsigned char Patt16_32,unsigned char LF_Send_CHx)  */
/*  R6_Dat ----- Pattern1 对应AS3933 R6寄存器的值 */ 
/*  R5_Dat ----- Pattern2 对应AS3933 R6寄存器的值 */
/*  Patt16_32    Pattern的编码位数                */
/*  Send_Dat     发送的数据(DAT) 这里只发送2个字节*/
/*  LF_Send_CHx  发送的通道             */
/********************************************************************************************/

void LF_SendData(unsigned char R6_Dat,unsigned char R5_Dat,unsigned char Patt16_32,unsigned char LF_Send_CHx)
{
    unsigned char i,j;
    unsigned char Data_Buff[20];		
	
    if((LFBIT_NxRC < 4)||(LFBIT_NxRC > 32))	  return;   
    
    Data_Buff[0] = ACTIVE_NUM + 2;      
    Data_Buff[1] = DEVICE_ID;           
    Data_Buff[ACTIVE_NUM + 2] = 0;      
    
    for(i = 0;i < ACTIVE_NUM;i ++)
        Data_Buff[i + 2] = Set_Buff[i + 16]; 
    for(i = 0;i < ACTIVE_NUM;i ++)    
        Data_Buff[ACTIVE_NUM + 2] = Data_Buff[ACTIVE_NUM + 2] + Set_Buff[i + 16]; 
    

    disableInterrupts();                        
    
    CarrierBurst(LF_Send_CHx);	                        	
    Pattern(R6_Dat,R5_Dat,Patt16_32,LF_Send_CHx);       	
		
    for(i = 0;i < ACTIVE_NUM + 3;i ++)		
    {	
	for(j = 0;j < 8;j ++)			
	{	  							
	    if(Data_Buff[i] & 0x80)
	    {
		Out_125K(Bit_element,LF_Send_CHx);
		Clock_125K(Bit_element,LF_Send_CHx);
	    }
	    else
	    {								
		Clock_125K(Bit_element,LF_Send_CHx);	
		Out_125K(Bit_element,LF_Send_CHx);						
	    }
            Data_Buff[i]= Data_Buff[i] << 1;
	}
    }
    enableInterrupts();        		
}