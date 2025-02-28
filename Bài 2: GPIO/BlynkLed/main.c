#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void RCC_config()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	}
void GPIO_config()
{
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_init.GPIO_Pin = GPIO_Pin_13;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_init);
}
	
void Delay(unsigned int time)
{
    for(int i=0; i< time;i++){}
}
int main()
	{
		RCC_config();
		GPIO_config();
		while(1)
			{
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Delay(10000000);
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Delay(10000000);
 			}

	}
	