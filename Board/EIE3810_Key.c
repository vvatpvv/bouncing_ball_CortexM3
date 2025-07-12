#include "stm32f10x.h"
#include "EIE3810_KEY.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_LED.h"

extern u8 Page;
extern u16 x_playerB;
extern u8 difficulty;
extern u8 CPU;
extern u8 dir_x;

u8 count = 0;
u8 acce = 0;

void EXTI4_IRQHandler(void)
{
	if (((GPIOE->IDR>>4) & 0x00000001) & (Page == 2)){
		EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
		ShowCharALine(20, 400, "Use USART for a random direction 0-7", RED, WHITE);
		Page = 3;
	}
	if (((GPIOE->IDR>>4) & 0x00000001) & (Page == 4)){
		EIE3810_TFTLCD_FillRectangle(x_playerB, 80, 795, 5, WHITE); //delete previous box of catcher A
		if (x_playerB < 400){
			x_playerB = x_playerB + 20; //shift position of the catcher A to right by 20 every time pressed
		}
		EIE3810_TFTLCD_FillRectangle(x_playerB, 80, 795, 5, BLACK); //draw box at catcher position after shift right
	}
	EXTI->PR = 1<<4;
}

void EXTI0_IRQHandler(void)
{
	if (((GPIOA->IDR) &= 0x00000001) & (Page == 2)){
		ShowCharALine(20, 350, "Easy", WHITE, BLUE); //easy highlighted
		ShowCharALine(20, 400, "Hard", BLUE, WHITE);
		difficulty = 1; //easy
	}
	EXTI->PR = 1;
}

void EXTI3_IRQHandler(void)
{
	if (((GPIOE->IDR>>3) & 0x00000001) & (Page == 2)){
		ShowCharALine(20, 350, "Easy", BLUE, WHITE);
		ShowCharALine(20, 400, "Hard", WHITE, BLUE); //hard highlighted
		difficulty = 2; //hard
	}
	if (((GPIOE->IDR>>3) & 0x00000001) && (Page == 4) && (count == 0)){
		Page = 6; //paused
	}
	if (((GPIOE->IDR>>3) & 0x00000001) && (Page == 6) && (count == 1)){
		Page = 4; //play
		ShowCharALine(200, 400, "Paused", WHITE, WHITE);//delete the 'Paused'
	}
	EXTI->PR = 1<<3;
}

void EXTI2_IRQHandler(void)
{
	if (((GPIOE->IDR>>2) & 0x00000001) & (Page == 4)){
		EIE3810_TFTLCD_FillRectangle(x_playerB, 80, 795, 5, WHITE);  //delete previous box of catcher A
		if (x_playerB > 0){
			x_playerB = x_playerB - 20; //shift position of the catcher A to left by 20 every time pressed
		}
		EIE3810_TFTLCD_FillRectangle(x_playerB, 80, 795, 5, BLACK); //draw box at catcher position after shift left
	}
	EXTI->PR = 1<<2;
}

void EIE3810_KEY_Init(void)
{
	RCC->APB2ENR|=1<<6;
	GPIOE->CRL &=0xFFFFF0FF;
	GPIOE->CRL |=0x00000800;
	GPIOE->ODR =1<<2;
	RCC->APB2ENR|=1<<6;
	GPIOE->CRL &=0xFFFF0FFF;
	GPIOE->CRL |=0x00008000;
	GPIOE->ODR |=1<<3;
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL &=0xFFFFFFF0;
	GPIOA->CRL |=0x00000008;
	GPIOA->ODR |=0<<8;
	RCC->APB2ENR|=1<<4;
	GPIOE->CRL &=0xFFF0FFFF;
	GPIOE->CRL |=0x00080000;
	GPIOE->ODR =1<<2;
}

void EIE3810_KeyUp_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<2; //Enable clock for GPIOA
	GPIOA->CRL &=0xFFFFFFF0;
	GPIOA->CRL |=0x00000008; //State of PA0 to 8
	GPIOA->ODR &= 0xE; //0 instead of 1 since is PA0 is 0 when not pressed
	RCC->APB2ENR |=0x01;
	AFIO->EXTICR[0] &=0xFFFFFFF0; //The 0 at PA0 is at bit 0
	EXTI->IMR |=1<<0; //Configure mask bit
	EXTI->RTSR |=1<<0; //Rising edge trigger used since PA0 is 0 when not pressed
	NVIC->IP[6] = 0x75; //EXTI0 is at position 6, priority set to 0x35
	//NVIC->IP[6] = 0x95; //line 55 replaced with this line at Experiment 3
	//NVIC->IP[6] = 0x35; //line 55 replaced with this line at Experiment 4
	NVIC->ISER[0] |= (1<<6); //position 6 for EXTI Line0 interrupt
}

void EIE3810_Key0_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<6;
	GPIOE->CRL &= 0xFFF0FFFF;
	GPIOE->CRL |= 0x00080000;
	GPIOE->ODR |= 1<<4;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[1] &= 0xFFFFFFF0;
	AFIO->EXTICR[1] |= 0x00000004;
	EXTI->IMR |= 1<<4;
	EXTI->RTSR |= 1<<4;
	NVIC->IP[10] = 0x65;
	NVIC->ISER[0] |= (1<<10);
}

void EIE3810_Key1_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<6;
	GPIOE->CRL &= 0xFFFF0FFF;
	GPIOE->CRL |= 0x00008000;
	GPIOE->ODR |= 1<<3;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFF0FFF;
	AFIO->EXTICR[0] |= 0x00004000;
	EXTI->IMR |= 1<<3;
	EXTI->RTSR |= 1<<3;
	NVIC->IP[9] = 0x35;
	NVIC->ISER[0] |= (1<<9);
}

void EIE3810_Key2_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<6;
	GPIOE->CRL &= 0xFFFFF0FF;
	GPIOE->CRL |= 0x00000800;
	GPIOE->ODR |= 1<<2;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFF0FF;
	AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR |= 1<<2;
	EXTI->RTSR |= 1<<2;
	NVIC->IP[8] = 0x65;
	NVIC->ISER[0] |= (1<<8);
}