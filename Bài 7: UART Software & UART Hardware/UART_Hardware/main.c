#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

#define TX_Pin	GPIO_Pin_9
#define RX_Pin	GPIO_Pin_10
#define UART_GPIO	GPIOA

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}
void GPIO_Config(){
GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	//
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
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
void UART_Config(){
	USART_InitTypeDef USART_InitStruct;
	//USART
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}
uint8_t USART1_ReceiveByte(void){
	uint8_t temp = 0x00;
    // Wait until data is received (RXNE flag is set)
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    // Read the received data
	temp = USART_ReceiveData(USART1);
	return temp;
}
void USART1_TransmitByte(uint8_t byte) {
    // Wait until the transmit data register is empty (TXE flag is set)
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    // Transmit the byte
    USART_SendData(USART1, byte);

    // Wait until transmission is complete (TC flag is set)
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
void UART_SendString(const char* str)
	{
		while(*str)
		{
			while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
			USART_SendData(USART1, *str++);
			 while (!USART_GetFlagStatus(USART1, USART_FLAG_TC)); 
		}
	}
	#include "stm32f10x.h"

void UART_ReceiveString( char *buffer, uint16_t maxSize) {
    uint16_t count = 0;
    
    while (count < (maxSize - 1)) { 
        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); 
       char ch = USART_ReceiveData(USART1) & 0xFF; 

        if (ch == '\n' || ch == '\r') break; 
        buffer[count++] = ch; 
    }
    buffer[count] = '\0'; 
}

int main ()
{
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	UART_Config();
	char Receive[50];
	while(1)
	{
		UART_ReceiveString(Receive,sizeof(Receive));
		delay_us(100);
//		UART_SendString("Hello\n");
//		delay_S(1);
	}
}
