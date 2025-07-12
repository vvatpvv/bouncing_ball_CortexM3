#include "stm32f10x.h"
#include "EIE3810_Clock.h"

void EIE3810_clock_tree_init(void)
{
	u8 PLL=7;
	u8 temp=0;
	RCC->CR |= 0x00010000; //16
	while(!((RCC->CR>>17)&0x1));//Bit 17 is  HSERDY (if HSE clock ready)
	RCC->CFGR &= 0xFFFDFFFF; //For 16 in hex which is 00010000 in binary
	RCC->CFGR |= 1<<16; //PLLSRC (entry clock source)
	RCC->CFGR |= PLL<<18; //PLLMUL (multiplication factor)
	RCC->CR |=0x01000000;//72MHz
	while(!(RCC->CR>>25));//Bit 25 is PLLRDY (if PLL locked)
	RCC->CFGR &=0xFFFFFFFE;//Reset and clock control
	RCC->CFGR |=0x00000002;//binary 10 is for when PLL selected as system clock
	while(temp !=0x02) //while not 10
	{
		temp=RCC->CFGR>>2;
		temp &= 0x03; //11
	}	
	RCC->CFGR &= 0xFFFFFC0F;//Bit 2 and 3 are SWS (clock source used as system clock)
	RCC->CFGR |= 0x00000400;//binary 100
	FLASH->ACR = 0x32;//Set FLASH with 2 wait states		
	RCC->APB1ENR |= 1<<17; //17 is for USART2 EN
	RCC->APB2ENR |= 1<<14; //14 is for USART1 EN
}
