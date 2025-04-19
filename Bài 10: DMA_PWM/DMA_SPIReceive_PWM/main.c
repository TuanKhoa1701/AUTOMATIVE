#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI

#define CS_Pin 		GPIO_Pin_4
#define SCK_Pin 	GPIO_Pin_5
#define MISO_Pin 	GPIO_Pin_6
#define MOSI_Pin	GPIO_Pin_7

#define MIN_PULSE_WIDTH 1000
#define MAX_PULSE_WIDTH 2000

uint16_t buffer;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = CS_Pin | SCK_Pin | MOSI_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = MISO_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void SPI_Config()
{
	SPI_InitTypeDef SPI;
	
	SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI.SPI_Mode = SPI_Mode_Slave;
	SPI.SPI_CPHA = SPI_CPHA_1Edge;
	SPI.SPI_CPOL = SPI_CPOL_Low;
	SPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI.SPI_DataSize = SPI_DataSize_16b;
	SPI.SPI_CRCPolynomial = 7;
	SPI.SPI_NSS = SPI_NSS_Hard;
	
	SPI_Init(SPI1,&SPI);
	SPI_Cmd(SPI1, ENABLE);
}
void DMA_Config()
{
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_BufferSize = 1;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&buffer;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;

	DMA_Init(DMA1_Channel2, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel2, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
}


void TIM_config()
{
	TIM_TimeBaseInitTypeDef PWM;
	
	PWM.TIM_ClockDivision = TIM_CKD_DIV1;
	PWM.TIM_CounterMode = TIM_CounterMode_Up;
	PWM.TIM_Period = 20000-1 ;
	PWM.TIM_Prescaler = 71;
	TIM_TimeBaseInit(TIM2,&PWM);
	
	PWM.TIM_Period = 999 ;
	PWM.TIM_Prescaler = 71;
	TIM_TimeBaseInit(TIM3,&PWM);
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_OCInitTypeDef servo;
	
	servo.TIM_Pulse = MIN_PULSE_WIDTH;
	servo.TIM_OCMode = TIM_OCMode_PWM1;
	servo.TIM_OutputState = TIM_OutputState_Enable;
	servo.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2, &servo);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_Cmd(TIM2, ENABLE);
}	
void Delay_mS(uint32_t time){
	TIM_SetCounter(TIM3, 0);
	while(TIM_GetCounter(TIM3) < time);
}
void Servo_SetAngle(uint32_t angle)
{	
	if(angle > 180) angle =180;
	uint32_t pulse = MIN_PULSE_WIDTH + ((angle * (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH)) / 180);
	TIM_SetCompare1(TIM2, pulse);
}
uint16_t val;

int main()
	{
		RCC_Config();
		GPIO_Config();
		TIM_config();
	SPI_Config();
		DMA_Config();
		while(1){
				if(DMA_GetFlagStatus(DMA1_FLAG_TC2)){
          DMA_ClearFlag(DMA1_FLAG_TC2);
				
          uint16_t angle = (buffer * 180) / 4095;
            Servo_SetAngle(angle);
            Delay_mS(100);
//						Servo_SetAngle(180);
//            Delay_mS(1000);
        }
	}
	}
		