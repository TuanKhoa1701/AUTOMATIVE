#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
void USART_Lin_init(){
	USART_InitTypeDef USART_InitStructure;
	
  USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
	USART_LINCmd(USART1, ENABLE);
}

uint8_t LIN_Calculate_Parity(uint8_t id){
	uint8_t p0 = ((id >>0) & 0x01) ^ ((id >>1) &0x01) ^ ((id >> 2) &0x01);
	uint8_t p1 = (((id >> 1) & 0x01) ^ ((id >> 3) &0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01));
	return (p0 | (p1<<1)) << 6;
}

uint8_t LIN_Calculate_CheckSum(uint8_t *data, uint8_t length){
	uint16_t checkSum =0;
	for(int i=0; i < length; i++)
	{
	checkSum += data[i];
			if(checkSum > 0xFF)
		{
		checkSum -= 0xFF;
		}
	return ~checkSum;
	}
}

uint8_t LIN_Received(uint8_t *buffer, uint8_t length){
  for(int i=0; i < length; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		buffer[i] = USART_ReceiveData(USART1);
	}
	return LIN_Calculate_CheckSum(buffer, length);
}
void LIN_SlaveReceive(uint8_t id, uint8_t *buffer, uint8_t length){
	uint8_t received_id;
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	received_id = USART_ReceiveData(USART1);
	
	if(received_id == (id|LIN_Calculate_Parity(id))){
			LIN_Received(buffer, length);
			uint8_t received_CheckSum = USART_ReceiveData(USART1);
			if(received_CheckSum == LIN_Calculate_CheckSum(buffer, length)){
			
			}
			else{
			
			
			}
	}
}
int main() 
{
	RCC_Config();
	GPIO_Config();
	USART_Lin_init();
	
	uint8_t data_Receive[4] ;
  while(1){
		LIN_SlaveReceive(0x01, data_Receive, 4);
	}
}