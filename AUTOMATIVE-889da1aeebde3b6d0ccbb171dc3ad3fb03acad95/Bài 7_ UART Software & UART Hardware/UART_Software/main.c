#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define TX_Pin GPIO_Pin_9
#define RX_Pin GPIO_Pin_10
#define UART_GPIO GPIOA
uint32_t Bit_Time;

void RCC_Config(){
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
typedef enum{
	Parity_Mode_None,
	Parity_Mode_Odd,
	Parity_Mode_Even
}Parity_Mode;
typedef enum
{
	Baud_110 = 110,
	Baud_300 = 300,
	Baud_600 = 600,
	Baud_1200 = 1200,
	Baud_2400 = 2400,
	Baud_4800 = 4800,
	Baud_9600 = 9600,
	Baud_19200 = 19200,
	Baud_28800 = 28800
}Baudrate;
typedef enum
{
	Stop_Bits_1,
	Stop_Bits_2
}Stop_Bits;
typedef struct
{
	Parity_Mode Parity;
	Baudrate Baudrate;
	Stop_Bits Stop_Bits;
}UART_Config;
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = TX_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(UART_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = RX_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(UART_GPIO, &GPIO_InitStructure);
}
void TIM_Config() {
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_Prescaler = 72 - 1;  
    TIM_InitStruct.TIM_Period = 0xFFFF;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
}
void delay_us(uint32_t time) {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < time) {} 
}
void delay_S(uint32_t time)
	{
		for(int i=0; i < time*1000 ;i++){
		delay_us(1000);
		}
	}
void UARTSoftware_Init(UART_Config Config){
	Bit_Time = (int)(1000000/Config.Baudrate) ;
	GPIO_SetBits(UART_GPIO, TX_Pin);
	delay_us(1);
}
void clock(uint32_t Bit_Time)
	{
		delay_us(Bit_Time);
	}
void UARTSoftware_Transmit(UART_Config Config,char c) {
	uint16_t count =0;
    // Start bit
    GPIO_ResetBits(GPIOA, TX_Pin);
    clock(Bit_Time);

    for (int i = 0; i < 8; i++) {
        if (c & (1 << i)) {
            GPIO_SetBits(GPIOA, TX_Pin);
						count++;
        } else {
            GPIO_ResetBits(GPIOA, TX_Pin);
        }
        clock(Bit_Time);
    }
		switch(Config.Parity){
			case Parity_Mode_None:
			break;

			case Parity_Mode_Odd:
				if(count % 2){
					GPIO_SetBits(GPIOA, TX_Pin);
				} else {
					GPIO_ResetBits(GPIOA, TX_Pin);
				}

			case Parity_Mode_Even:
				if(!(count % 2)){
					GPIO_SetBits(GPIOA, TX_Pin);
				} else {
					GPIO_ResetBits(GPIOA, TX_Pin);
				}
	}
    // Stop bit
    GPIO_SetBits(GPIOA, TX_Pin);
		clock(Bit_Time);
		if(Config.Stop_Bits == Stop_Bits_2)
		clock(Bit_Time);
		
}

char UARTSoftware_Receive(UART_Config Config) {
    char c = 0;
		uint16_t count=0;
    while (GPIO_ReadInputDataBit(GPIOA, RX_Pin) == 1);

    delay_us( Bit_Time + Bit_Time / 2);

    for (int i = 0; i < 8; i++) {
				
        if (GPIO_ReadInputDataBit(GPIOA, RX_Pin)) {
            c |= (1 << i);
					count++;
        }
				clock(Bit_Time); 
    }
		
    delay_us(Bit_Time / 2);
		
		if(Config.Stop_Bits == Stop_Bits_2)
		delay_us(Bit_Time);
    return c;
}
void UARTSoftware_TransmitString(UART_Config config, const char* str)
{
	while(*str)
	{
		UARTSoftware_Transmit(config, *str++);
	}
}
void UARTSoftware_ReceiveString(UART_Config config, char* Buffer, uint16_t len)
{
	uint16_t count =0;
	while(count < len -1){
	char c = UARTSoftware_Receive(config);
		if ( c=='\n' || c == '\r'){
			break;
		}
	Buffer[count++] = c;
	}
	Buffer[count] = '\0';

}

uint8_t Parity_Check(UART_Config config,uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	for(int i = 0; i < 8; i++){
		if(data & 0x01){
			count++;
		}
		data >>= 1;
	}
	switch(config.Parity){
		case Parity_Mode_None:
			return 1; 
		case Parity_Mode_Odd:
			return (count % 2);
		case Parity_Mode_Even:
			return (!(count % 2));
		default:
			return 0;
	}
}
int main()
	{
		RCC_Config();
		GPIO_Config();
		TIM_Config();
		
		UART_Config Uart_config;
		Uart_config.Baudrate = Baud_4800;
		Uart_config.Parity = Parity_Mode_Even;
		Uart_config.Stop_Bits = Stop_Bits_2;
		
		UARTSoftware_Init(Uart_config);
		char Receive[5];
		while(1)
			{
			
//			UARTSoftware_ReceiveString(Uart_config,Receive, sizeof(Receive));
			UARTSoftware_TransmitString(Uart_config, "HELLO\n");
			delay_S(1);
			}
	}
	
