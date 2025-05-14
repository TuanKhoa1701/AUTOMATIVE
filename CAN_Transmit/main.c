#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_can.h"              // Keil::Device:StdPeriph Drivers:CAN
void RCC_config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}
void GPIO_config(){
	GPIO_InitTypeDef GPIO;
	
	GPIO.GPIO_Pin = GPIO_Pin_11;
	GPIO.GPIO_Mode = GPIO_Mode_IPU;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO);
	
	GPIO.GPIO_Pin = GPIO_Pin_12;
	GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO);
}
void CAN_config(){
	CAN_InitTypeDef CAN;
	
	CAN.CAN_ABOM = ENABLE;
	CAN.CAN_AWUM = ENABLE;
	CAN.CAN_TTCM = DISABLE;
	CAN.CAN_NART = DISABLE;
	CAN.CAN_TXFP = ENABLE;
	CAN.CAN_RFLM = DISABLE;
	CAN.CAN_Mode = CAN_Mode_Normal;
	CAN.CAN_SJW = CAN_SJW_1tq;
	CAN.CAN_BS1 = CAN_BS1_6tq;
	CAN.CAN_BS2 = CAN_BS2_8tq;
	CAN.CAN_Prescaler = 6;
	CAN_Init(CAN1, &CAN);
}
void CAN_FilterConfig(){
	CAN_FilterInitTypeDef CAN;
	CAN.CAN_FilterFIFOAssignment= CAN_Filter_FIFO0	;
	CAN.CAN_FilterNumber = 1;
	CAN.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN.CAN_FilterIdHigh = (0x123 << 5);
	CAN.CAN_FilterIdLow = 0;
	CAN.CAN_FilterMaskIdHigh = 0xFFE0;
	CAN.CAN_FilterMaskIdLow = 0;
	CAN.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN);
}
void CAN_TransmitData(uint8_t* data, uint8_t length){
	CanTxMsg TxMsg;
	
	TxMsg.StdId = 0x123;
	TxMsg.DLC = length;
	TxMsg.RTR = CAN_RTR_DATA;
	TxMsg.IDE = CAN_ID_STD;
	for(int i=0; i < length; i++ ){
		TxMsg.Data[i]= data[i];
	}
	uint8_t mailbox = CAN_Transmit(CAN1, &TxMsg);
	while(CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok);
}
int  main()
{
	RCC_config();
	GPIO_config();
	CAN_config();
	CAN_FilterConfig();
uint8_t data[8] = {0x02, 0x04, 0x06,0x07};
	while (1)
	{
			CAN_TransmitData(data, 4);
	}
}