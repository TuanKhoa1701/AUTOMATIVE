#define TIM2_BASE        0x40000000
#define TIM2_CR1         (*(volatile unsigned int*)(TIM2_BASE + 0x00))
#define TIM2_DIER        (*(volatile unsigned int*)(TIM2_BASE + 0x0C))
#define TIM2_SR          (*(volatile unsigned int*)(TIM2_BASE + 0x10))
#define TIM2_PSC         (*(volatile unsigned int*)(TIM2_BASE + 0x28))
#define TIM2_ARR         (*(volatile unsigned int*)(TIM2_BASE + 0x2C))
#define TIM2_CNT         (*(volatile unsigned int*)(TIM2_BASE + 0x24))

#define RCC_BASE         0x40021000
#define RCC_APB1ENR      (*(volatile unsigned int*)(RCC_BASE + 0x1C))
#define RCC_APB2ENR      (*(volatile unsigned int*)(RCC_BASE + 0x18))

#define GPIOC_BASE       0x40011000
#define GPIOC_CRH        (*(volatile unsigned int*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR        (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))

#define NVIC_ISER0       (*(volatile unsigned int*)0xE000E100)
void RCC_config()
	{
		RCC_APB1ENR |= 1;
		RCC_APB2ENR |= 1<<4 ;
	}
void GPIO_config()
	{
		GPIOC_CRH &= ~((1<<23) | (1<<22));
		GPIOC_CRH |= ((1<<21)|(1<<20));
	}
void Timer_config()
	{
		TIM2_PSC = 7200 -1;
		TIM2_ARR = 10000 -1;
		TIM2_DIER |= 1<<0;
		TIM2_CR1 |= 1<<0;
		NVIC_ISER0 |= (1<<28);
	}	
void TIM2_IRQHandler(void)
{
	if(TIM2_SR & (1<<0))
	{
		TIM2_SR &= ~1;
		GPIOC_ODR ^= (1<<13);
	}
}
		
int main() {
	RCC_config();
	GPIO_config();
	Timer_config();
while(1)
	{
		
	}
}
