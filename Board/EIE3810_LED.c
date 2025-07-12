#include "stm32f10x.h"
#include "EIE3810_LED.h"

void EIE3810_LED_Init(void)
{
	RCC->APB2ENR|=1<<3;
	GPIOB->CRL &=0xFF0FFFFF;
	GPIOB->CRL |=0x00300000; /* config PB5 0011: General purpose output, Push-pull, Max. output speed 50MHz*/
	
	RCC->APB2ENR|=1<<6;
	GPIOE->CRL &=0xFF0FFFFF;
	GPIOE->CRL |=0x00300000; /* config PE5 0011: General purpose output, Push-pull, Max. output speed 50MHz*/
	
	GPIOB->BSRR=1<<5;
	GPIOE->BSRR=1<<5;
}

int PB5State(void)
{
	return GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5);
}

int PE5State(void)
{
	return GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5);
}

void turnoffLED0(void)
{
	GPIOB->BSRR=1<<5;
}

void turnonLED0(void)
{
	GPIOB->BRR=1<<5;
}

void turnoffLED1(void)
{
	GPIOE->BSRR=1<<5;
}

void turnonLED1(void)
{
	GPIOE->BRR=1<<5;
}
