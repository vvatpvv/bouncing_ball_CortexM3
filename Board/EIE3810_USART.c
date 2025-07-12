#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "EIE3810_LED.h"
#include "EIE3810_USART.h"
#include "EIE3810_TFTLCD.h"
extern u8 Page;
extern u32 buffer;

void EIE3810_USART2_init(u32 pclk1, u32 baudrate)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk1*1000000)/(baudrate*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<=4;
	mantissa+=fraction;
	RCC->APB2ENR |= 1<<2; //2 for IOPA EN
	GPIOA->CRL &= 0xFFFF00FF; //Set up only PA2 and PA3
	GPIOA->CRL |= 0x00008B00; //Set PA2 to alternate function push-pull, PA3 to input pull-up or pull-down
	RCC->APB1RSTR |= 1<<17; //17 for USART2 RST
	RCC->APB1RSTR &= ~(1<<17); //17 for USART2 RST
	USART2->BRR=mantissa;//Set the baud rate
	USART2->CR1=0x2008; //Control register 1 of USART2
}

void EIE3810_USART1_Init(u32 pclk1, u32 baudrate)
{
	//USART1
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk1*1000000)/(baudrate*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<=4;
mantissa+=fraction;
	RCC->APB2ENR |= 1<<2; //2 for IOPA EN	
	GPIOA->CRH &= 0xFFFFF00F; //Set up only PA9 and PA10
	GPIOA->CRH |= 0x000008B0; //Set PA9 to alternate function push-pull, PA10 to input pull-up or pull-down
	RCC->APB2RSTR |= 1<<14; //14 for for USART1 RST
	RCC->APB2RSTR &= ~(1<<14); //14 for for USART1 RST
	USART1->BRR=mantissa;//set rate
	USART1->CR1=0x202C; //enable transmitter, enable receiver, USART enable
	USART1->CR1 |= 1<<5; //enable receive interrupt
	USART1->CR1 |= 1<<2; //enable receiver
}

void EIE3810_USART1_EXTIInit(void)
{
	NVIC->IP[37] =0x65; //Position 37 is for USART1 global interrupt, Set priority 0x65
	NVIC->ISER[1] |=1<<5; //Enable NVIC interrupt by setting the bit
}

void USART1_IRQHandler(void)
{
	u32 CR1;
	if ((USART1->SR & (1<<5)) && (Page == 3))
	{
		buffer=USART1->DR + 0x30;
		CR1=USART1->CR1;
		ShowCharALine(20, 450, "The random number received is ", RED, WHITE);
		if (buffer== '0')
		{ShowCharALine2412(20 + 30*8, 448, "0", RED, WHITE);}
		if (buffer=='1')
		{ShowCharALine2412(20 + 30*8, 448, "1", RED, WHITE);}
		if (buffer=='2')
		{ShowCharALine2412(20 + 30*8, 448, "2", RED, WHITE);}
		if (buffer=='3')
		{ShowCharALine2412(20 + 30*8, 448, "3", RED, WHITE);}
		if (buffer=='4')
		{ShowCharALine2412(20 + 30*8, 448, "4", RED, WHITE);}
		if (buffer=='5')
		{ShowCharALine2412(20 + 30*8, 448, "5", RED, WHITE);}
		if (buffer=='6')
		{ShowCharALine2412(20 + 30*8, 448, "6", RED, WHITE);}
		if (buffer=='7')
		{ShowCharALine2412(20 + 30*8, 448, "7", RED, WHITE);}
	
	for (u32 i=0;i<10200000;i++);
	EIE3810_TFTLCD_Clear(WHITE);
	EIE3810_TFTLCD_SevenSegment(200, 330, 3, BLUE);
	for (u32 i=0;i<5100000;i++);
	EIE3810_TFTLCD_Clear(WHITE);
	EIE3810_TFTLCD_SevenSegment(200, 330, 2, BLUE);
	for (u32 i=0;i<5100000;i++);
	EIE3810_TFTLCD_Clear(WHITE);
	EIE3810_TFTLCD_SevenSegment(200, 330, 1, BLUE);
	for (u32 i=0;i<5100000;i++);
	EIE3810_TFTLCD_Clear(WHITE);
	EIE3810_TFTLCD_FillRectangle(200, 80, 795, 5, BLACK); //the catchers
	EIE3810_TFTLCD_FillRectangle(200, 80, 0, 5, BLACK);
	Page = 4;
	}
}
