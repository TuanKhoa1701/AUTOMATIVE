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

uint16_t val;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = CS_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // S?a: Output cho CS
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =  SCK_Pin | MOSI_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = MISO_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void SPI_Config()
{
	SPI_InitTypeDef SPI;
	
	SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI.SPI_Mode = SPI_Mode_Master;
	SPI.SPI_CPHA = SPI_CPHA_1Edge;
	SPI.SPI_CPOL = SPI_CPOL_Low;
	SPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI.SPI_DataSize = SPI_DataSize_16b;
	SPI.SPI_CRCPolynomial = 7;
	SPI.SPI_NSS = SPI_NSS_Hard;
	
	SPI_Init(SPI1,&SPI);
	SPI_SSOutputCmd(SPI1, ENABLE); // B?t di?u khi?n NSS t? d?ng
	SPI_Cmd(SPI1, ENABLE);
}
	void ADC_Config(){
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void DMA_Config()
{
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_BufferSize = 1;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&val;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;

	DMA_Init(DMA1_Channel3, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

void TIM_config()
{
	TIM_TimeBaseInitTypeDef Delay;
	
	Delay.TIM_ClockDivision = TIM_CKD_DIV1;
	Delay.TIM_CounterMode = TIM_CounterMode_Up;
	Delay.TIM_Period = 0xFFFF ;
	Delay.TIM_Prescaler = 71;
	
	TIM_TimeBaseInit(TIM2,&Delay);
	TIM_Cmd(TIM2, ENABLE);
}	
void Delay_ms(uint32_t time)
{
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < time*1000);
}

int main()
	{
	
		RCC_Config();
		GPIO_Config();
		TIM_config();
		ADC_Config();
		SPI_Config();
		DMA_Config();

    while (1) {
        val = ADC_GetConversionValue(ADC1); 
	while (!DMA_GetFlagStatus(DMA1_FLAG_TC3)); 
        DMA_ClearFlag(DMA1_FLAG_TC3);
        Delay_ms(50); 
    }
	}
