#define RCC_APB2ENR *((unsigned int*)0x40021018)
#define GPIOC_CRH *((unsigned int*)0x40011004)
#define GPIOC_ODR *((unsigned int*)0x4001100C)
void Delay(unsigned int time)
{
    for(int i=0; i< time;i++){}
}
int main()
	{
		
		RCC_APB2ENR |=(1<<4);
		GPIOC_CRH &= ~((1<<23) | (1<<22));
		GPIOC_CRH |= ((1<<21) | (1<<20));
		while(1)
			{
			GPIOC_ODR |= (1<<13);
			Delay(10000000);
			GPIOC_ODR &= ~(1<<13);
			Delay(10000000);
 			}
		
	}
	