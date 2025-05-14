#define RCC 	((RCC_TypeDef*)0x40021000)
#define GPIOC ((GPIO_TypeDef*)0x40011000)
#define GPIOA ((GPIO_TypeDef*)0x40010800)


typedef struct
{
  unsigned int CRL;
  unsigned int CRH;
  unsigned int IDR;
  unsigned int ODR;
  unsigned int BSRR;
  unsigned int BRR;
  unsigned int LCKR;
} GPIO_TypeDef;
typedef struct
{
  unsigned int CR;
  unsigned int CFGR;
  unsigned int CIR;
  unsigned int APB2RSTR;
  unsigned int APB1RSTR;
  unsigned int AHBENR;
  unsigned int APB2ENR;
  unsigned int APB1ENR;
  unsigned int BDCR;
  unsigned int CSR;
} RCC_TypeDef;

void Writepin(GPIO_TypeDef *port, char pin, char state)
	{
		if(state == 1)
	port->ODR |= (1 << pin);
	else
	port->ODR &= ~(1 << pin);

	}
	void GPIO_Config()
		{
		RCC->APB2ENR |=(1<<4) | (1<<2);
			// PC13
		GPIOC->CRH &= ~((1<<23) | (1<<22));
		GPIOC->CRH |= ((1<<21) | (1<<20));
				// PA0
		GPIOA->CRL &= ~((1<<0) | (1<<1) | (1<<2));
		GPIOA->CRL |= (1<<3);
		GPIOA->ODR |= 1;
		}
int main()
	{
		GPIO_Config();
		while(1)
			{
				if((GPIOA->IDR & (1<<0))==0)
			{
			Writepin(GPIOC,13,0);
			}
			Writepin(GPIOC,13,1);
 			}
		
	}
	

