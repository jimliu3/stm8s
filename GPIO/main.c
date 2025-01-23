#include "stm8s.h"

main()
{
  
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

    while (1)
    {
        int i;
        GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
        for(i=0; i<10000; i++);
        GPIO_WriteLow(GPIOB, GPIO_PIN_5);
        for(i=0; i<10000; i++);
    }
}