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
void LIN_SendBreak(){
	USART_SendBreak(USART1);
}

void LIN_SendSync(){
	USART_SendData(USART1, 0x55);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
}
uint8_t LIN_Calculate_Parity(uint8_t id){
	uint8_t p0 = ((id >>0) & 0x01) ^ ((id >>1) &0x01) ^ ((id >> 2) &0x01);
	uint8_t p1 = (((id >> 1) & 0x01) ^ ((id >> 3) &0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01));
	return (p0 | (p1<<1)) << 6;
}
void LIN_SendID(uint8_t id){
	uint8_t id_with_parity = id | LIN_Calculate_Parity(id);
	USART_SendData(USART1, id_with_parity);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
void LIN_SendData(uint8_t * data, uint8_t length){
	for(int i =0; i < length; i++ )
	{
		USART_SendData(USART1, data[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	}
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
void LIN_SendCheckSum(uint8_t* data, uint8_t length){
	uint8_t CheckSum = LIN_Calculate_CheckSum (data, length);
	USART_SendData(USART1, CheckSum);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
void LIN_MasterSend(uint8_t id, uint8_t* data, uint8_t length)
{
	LIN_SendBreak();
	LIN_SendSync();
	LIN_SendID(id);
	LIN_SendData(data, length);
	LIN_SendCheckSum(data, length);
}
int main() 
{
	RCC_Config();
	GPIO_Config();
	USART_Lin_init();
	
	uint8_t data_Send[4] = {0x34, 0x89, 0x23, 0x01};
  while(1){
	LIN_MasterSend(0x01, data_Send, 4);
	}
}