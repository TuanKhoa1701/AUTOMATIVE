#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void RCC_config()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA,ENABLE);
	}
void GPIO_config()
{
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_init.GPIO_Pin = GPIO_Pin_13;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_init);
	
	GPIO_init.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_init.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_init);
}
	
void Delay(unsigned int time)
{
    for(int i=0; i< time;i++){}
}
void chaseLed(uint8_t loop){
	uint16_t Ledval;
	for(int j = 0; j < loop; j++)
{
		Ledval = 0x0010; //0b0 0001 0000
		for(int i = 0; i < 4; i++)
		{
			Ledval = Ledval << 1;
			GPIO_Write(GPIOC, Ledval);
			Delay(10000000);
		}
	}
}

int main()
	{
		RCC_config();
		GPIO_config();
		while(1)
			{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);	
			if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
			{
						GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			} 
			else {
						GPIO_SetBits(GPIOC, GPIO_Pin_13);
					}
		}
 			}
	}
	