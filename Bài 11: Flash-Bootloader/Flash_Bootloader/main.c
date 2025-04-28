#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"            // Keil::Device:StdPeriph Drivers:Flash
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define ADDR_STR_Blink 0x08008000

void Erase_1Page(uint32_t address)
{
	FLASH_Unlock();
	FLASH -> CR |= FLASH_CR_PER;
	FLASH -> AR = address;
	FLASH -> CR |= FLASH_CR_STRT;
	while((FLASH -> SR & FLASH_SR_BSY)== SET);
	FLASH -> CR &= ~FLASH_CR_PER;
	FLASH_Lock();
}
void FLASH_PROGRAMMING(uint32_t address, uint32_t data)
{
    FLASH_Unlock();
    FLASH->CR |= FLASH_CR_PG;

    // Ghi lower 16 bits
    *(volatile uint16_t*)address = (uint16_t)(data & 0xFFFF);
    while (FLASH->SR & FLASH_SR_BSY);

    // Ghi upper 16 bits
    *(volatile uint16_t*)(address + 2) = (uint16_t)((data >> 16) & 0xFFFF);
    while (FLASH->SR & FLASH_SR_BSY);

    FLASH->CR &= ~FLASH_CR_PG;
    FLASH_Lock();
}
void Boot()
	{
		RCC_DeInit();
		SCB ->SHCSR &= ~(SCB_SHCSR_USGFAULTACT_Msk | SCB_SHCSR_BUSFAULTACT_Msk | SCB_SHCSR_MEMFAULTACT_Msk);
		__set_MSP(*(__IO uint32_t*)(ADDR_STR_Blink));
		SCB -> VTOR = ADDR_STR_Blink;
		uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_Blink + 4);
		void (*reset_handler)(void) = (void(*)(void)) JumpAddress;
		reset_handler();
	}
int main()
{
	Boot();
	while(1){}	
}