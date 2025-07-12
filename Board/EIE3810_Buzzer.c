#include "stm32f10x.h"
#include "EIE3810_Buzzer.h"

void EIE3810_Buzzer_Init(void)
{
	RCC->APB2ENR|=1<<3;
	GPIOB->CRH &=0xFFFFFFF0;
	GPIOB->CRH |=0x00000003; /* config PB8 0011: General purpose output, Push-pull, Max. output speed 50MHz*/
	
	GPIOB->BRR=1<<8;
}

int PB8State(void)
{
	return GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8);
}

void turnoffBuzzer(void)
{
	GPIOB->BRR=1<<8;
}

void turnonBuzzer(void)
{
	GPIOB->BSRR=1<<8;
}
