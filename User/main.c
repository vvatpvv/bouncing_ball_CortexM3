#include "stm32f10x.h"
#include "EIE3810_Clock.h"
#include "EIE3810_Key.h"
#include "EIE3810_Buzzer.h"
#include "EIE3810_LED.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_USART.h"
#include "stm32f10x_it.h"
#include <stdio.h>
void Delay(u32);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);
void EIE3810_TIM3_Init(u16 arr, u16 psc);
void EIE3810_TIM4_Init(u16 arr, u16 psc);
void JOYPAD_Init();
void EIE3810_SYSTICK_Init();

u32 button = 0;
u32 Page;
u32 difficulty = 1;
u32 buffer;
u32 Up_playerb = 1;
u32 Down_playera = 1;
u32 Bounces = 1;
u32 Timer;
u8 task1HeartBeat;

u32 x = 240; //initial x of ball
u32 y = 730; //initial y of ball
u32 x_playerA = 200; //location of catcher of Player A
u32 x_playerB = 200; //location of catcher of Player B
u32 dir_x; //ball direction (left or right the screen)
u32 dir_y = 1; //ball direction (up or down the screen), start with up

char t[10];
char up_playerb[10];
char down_playera[10];
char bounces[10];
extern u8 count;

#define LED0_PWM_VAL TIM3->CCR2 //capture/compare

int main(void)
{
	EIE3810_LED_Init();
	EIE3810_clock_tree_init();
	EIE3810_TFTLCD_Init();
	EIE3810_Buzzer_Init();
	
	EIE3810_KEY_Init();
	EIE3810_Key1_EXTIInit();
	EIE3810_Key2_EXTIInit();
	EIE3810_KeyUp_EXTIInit();
	EIE3810_Key0_EXTIInit();
	
	EIE3810_NVIC_SetPriorityGroup(5);//Set PRIGROUP
	EIE3810_USART1_Init(72, 14440);
	EIE3810_USART1_EXTIInit();
	EIE3810_SYSTICK_Init();
	EIE3810_TIM3_Init(99,7199); //100Hz
	JOYPAD_Init();
	
	turnoffLED1();
	EIE3810_TFTLCD_Clear(WHITE);
	Delay(5100000);
	Page = 1;
	
	if (Page == 1){ //first Page
			EIE3810_TFTLCD_Clear(WHITE);
		
			EIE3810_TFTLCD_FillRectangle(110, 20, 400, 20, RED);
			EIE3810_TFTLCD_FillRectangle(130, 20, 380, 60, RED);
			EIE3810_TFTLCD_FillRectangle(150, 20, 400, 20, RED);
			
			EIE3810_TFTLCD_FillRectangle(290, 20, 400, 20, RED);
			EIE3810_TFTLCD_FillRectangle(310, 20, 380, 60, RED);
			EIE3810_TFTLCD_FillRectangle(330, 20, 400, 20, RED);
		
			EIE3810_TFTLCD_FillRectangle(110, 20, 500, 20, RED);
			EIE3810_TFTLCD_FillRectangle(130, 20, 520, 20, RED);
			EIE3810_TFTLCD_FillRectangle(150, 40, 540, 20, RED);
			EIE3810_TFTLCD_FillRectangle(190, 80, 560, 20, RED);
			EIE3810_TFTLCD_FillRectangle(270, 40, 540, 20, RED);
			EIE3810_TFTLCD_FillRectangle(310, 20, 520, 20, RED);
			EIE3810_TFTLCD_FillRectangle(330, 20, 500, 20, RED);
		
			ShowCharALine2412(20, 90, "Welcome to mini project!", BLUE, WHITE);
			Delay(10000000);
			ShowCharALine(20, 150, "This is the final lab (^-^)", BLUE, WHITE);
			Delay(10000000);
			ShowCharALine(20, 200, "Are you ready?", BLUE, WHITE);
			Delay(10000000);
			ShowCharALine(20, 250, "Let's start!", BLUE, WHITE);
			Delay(10000000);
			EIE3810_TFTLCD_Clear(WHITE);
			Page = 2;
		}
	if (Page == 2){ //choose easy/hard
			ShowCharALine(20, 300, "Please select difficulty level:", RED, WHITE);
			ShowCharALine(20, 350, "Easy", WHITE, BLUE);
			ShowCharALine(20, 400, "Hard", BLUE, WHITE);
			ShowCharALine(20, 450, "Press KEY0 to enter.", RED, WHITE);
		
			EIE3810_TFTLCD_FillRectangle(255+20, 10, 200+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(265+20, 10, 190+200, 30, RED);
			EIE3810_TFTLCD_FillRectangle(275+20, 10, 200+200, 10, RED);
			
			EIE3810_TFTLCD_FillRectangle(345+20, 10, 200+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(355+20, 10, 190+200, 30, RED);
			EIE3810_TFTLCD_FillRectangle(365+20, 10, 200+200, 10, RED);
		
			EIE3810_TFTLCD_FillRectangle(255+20, 10, 250+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(265+20, 10, 260+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(275+20, 20, 270+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(295+20, 40, 280+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(335+20, 20, 270+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(355+20, 10, 260+200, 10, RED);
			EIE3810_TFTLCD_FillRectangle(365+20, 10, 250+200, 10, RED);
			Delay(5000000);
	}
	while(1){		
		if (Page == 4){ //start playing
			count = 0;
			EIE3810_TIM4_Init(9999,7199); //timer
			if (!dir_x){ //initial direction of x
				if (buffer <= 51){
					dir_x = 1;
				}
				else{
					dir_x = -1;
				}
			}
			while ((task1HeartBeat >= 2) && (Page == 4)){
				EIE3810_TFTLCD_DrawCircle(x, y, 10, 1, WHITE);

				if ((10 <= x) && (x <= 470)){
					if (dir_x == 1){
						x = x + difficulty*2; //difficulty affects how fast x shifts
					}
					if (dir_x == -1){
						x = x - difficulty*2;
					}
				}
				else if (x > 470){
					dir_x = -1;
					x = x - 5;
					turnonBuzzer();
					Delay(51000);
					turnoffBuzzer();
					sprintf(bounces,"%d",Bounces);
					ShowCharALine(800, 400, bounces, WHITE, WHITE);
					Bounces ++;
					ShowCharALine(800, 400, bounces, BLUE, WHITE);
				}
				
				else if (x < 10){
					dir_x = 1;
					x = x + 5;
					turnonBuzzer();
					Delay(51000);
					turnoffBuzzer();
					sprintf(bounces,"%d",Bounces);
					ShowCharALine(800, 400, bounces, WHITE, WHITE);
					Bounces ++;
					ShowCharALine(800, 400, bounces, BLUE, WHITE);
				}
				
				if (y < 20){ //ball at top (player B section)
					if ((x >= x_playerA - 5) && (x <= x_playerA + 85)){ //if ball catched by Player B
						y = y + 6;
						dir_y = -1;
						turnonBuzzer();
						Delay(51000);
						turnoffBuzzer();			
						sprintf(bounces,"%d",Bounces);
						ShowCharALine(800, 400, bounces, WHITE, WHITE);
						Bounces ++;
						ShowCharALine(800, 400, bounces, BLUE,WHITE);
						sprintf(up_playerb,"%d",Up_playerb);
						ShowCharALine(800, 450, up_playerb, WHITE, WHITE);
						Up_playerb ++; //add score to Player B
						ShowCharALine(800 ,450, up_playerb, BLUE, WHITE);
					}
					else{ //if ball not catched by Player B
						Delay(5100000);
						ShowCharALine2412(160, 200, "Player A wins!", RED, WHITE);
						Delay(10200000);
						Page = 5; //'Thanks for playing!' page
					}
				}
				else if (y > 780){ //ball at bottom (player A section)
					if ((x >= x_playerB - 5) && (x <= x_playerB + 85)){ //if ball catched by Player A
						y = y - 6;
						dir_y = 1;
						turnonBuzzer();
						Delay(51000);
						turnoffBuzzer();
						sprintf(bounces,"%d",Bounces);
						ShowCharALine(800, 400, bounces, WHITE, WHITE);
						Bounces ++;
						ShowCharALine(800, 400, bounces, BLUE, WHITE);
						sprintf(down_playera,"%d",Down_playera);
						ShowCharALine(800, 500, down_playera, WHITE, WHITE);
						Down_playera ++; //add score to Player A
						ShowCharALine(800, 500, down_playera, BLUE, WHITE);
					}
					else{ //if ball not catched by Player A
						Delay(5100000);
						ShowCharALine2412(160, 200, "Player B wins!", RED, WHITE);
						Delay(10200000);
						Page = 5; //'Thanks for playing!' page
					}
				}				
				else if ((20 <= y) && (y <= 780)){ //ball is not at player A or B section
					if (dir_y == -1){
						if (buffer <= 51){
							y = y + difficulty*((buffer - 48)*2 + 1);  //difficulty affects how fast y shifts
						}
						else{
							y = y + difficulty*((buffer - 48 - 4)*2 + 1);
						}
					}
					if (dir_y == 1){
						if (buffer <= 51){
							y = y - difficulty*((buffer - 48)*2 + 1);
						}
						else{
							y = y - difficulty*((buffer - 48 - 4)*2 + 1);
						}
					}
				}
				
				if (Timer%2 == 0){EIE3810_TFTLCD_DrawCircle(x, y, 10, 1, RED);}
				else{EIE3810_TFTLCD_DrawCircle(x, y, 10, 1, BLACK);}
				
				EIE3810_TFTLCD_FillRectangle(x_playerB, 80, 795, 5, BLACK);
				
				task1HeartBeat = 0;
			}
		}
				
		if (Page == 5){ //finish!
			EIE3810_TFTLCD_Clear(WHITE);
			ShowCharALine2412(20, 100, "Thanks for playing!", BLUE, WHITE);
			
			EIE3810_TFTLCD_FillRectangle(110, 20, 300, 20, RED); //draw left eyes
			EIE3810_TFTLCD_FillRectangle(130, 20, 280, 60, RED);
			EIE3810_TFTLCD_FillRectangle(150, 20, 300, 20, RED);
			
			EIE3810_TFTLCD_FillRectangle(290, 20, 300, 20, RED); //draw right eyes
			EIE3810_TFTLCD_FillRectangle(310, 20, 280, 60, RED);
			EIE3810_TFTLCD_FillRectangle(330, 20, 300, 20, RED);
			
			EIE3810_TFTLCD_FillRectangle(110, 20, 400, 20, RED); //draw mouth
			EIE3810_TFTLCD_FillRectangle(130, 20, 420, 20, RED);
			EIE3810_TFTLCD_FillRectangle(150, 40, 440, 20, RED);
			EIE3810_TFTLCD_FillRectangle(190, 80, 460, 20, RED);
			EIE3810_TFTLCD_FillRectangle(270, 40, 440, 20, RED);
			EIE3810_TFTLCD_FillRectangle(310, 20, 420, 20, RED);
			EIE3810_TFTLCD_FillRectangle(330, 20, 400, 20, RED);
			
			for (u8 m=0;m<70;m++){ //draw line
					EIE3810_TFTLCD_FillRectangle(20+3*2*m, 3, 153, 3, WHITE);
					EIE3810_TFTLCD_FillRectangle(20+3*(2*m+1), 3, 150, 3, WHITE);
					EIE3810_TFTLCD_FillRectangle(20+3*2*m, 3, 150, 3, BLUE);
					EIE3810_TFTLCD_FillRectangle(20+3*(2*m+1), 3, 153, 3, BLUE);
			}
			while(1);
		}
		
		if (Page == 6){ //paused
			ShowCharALine(200, 400, "Paused", WHITE, BLACK);
			count = 1;
			while(Page == 6);
		}
	}
}

void Delay(u32 count)
{
	u32 i;
	for (i=0;i<count;i++);
}
	
void JOYPAD_Init(void)
{
	RCC->APB2ENR|=1<<3; //en GPIOB
	RCC->APB2ENR|=1<<5; //en GPIOD
	GPIOB->CRH &=0xFFFF00FF;
	GPIOB->CRH |=0x00003800; /* config PB10 and PB11 to 1000 and 0011 respectively*/
	GPIOB->ODR|=3<<10; //set PB10 input pull up mode, set PB11
	GPIOD->CRL &=0xFFFF0FFF;
	GPIOD->CRL |=0x00003000; /* config PD3 0011: General purpose output, Push-pull, Max. output speed 50MHz*/
	GPIOD->ODR |=1<<3; //set PD3
}

u8 JOYPAD_Read(void)
{
	vu8 temp=0;
	u8 t;
	GPIOB->BSRR |= 1<<11;//set PB11
	Delay(80);
	GPIOB->BSRR|=1<<27;//reset PB11 (low signal to joypad)
	for (t=0;t<8;t++)
	{
		temp>>=1;//temp go right by 1 bit
		if((((GPIOB->IDR)>>10)&0x01)==0)temp|=0x80;//read from PB10
		GPIOD->BSRR |=(1<<3);//set PD3
		Delay(80);
		GPIOD->BSRR |=  (1<<19);//reset PD3
		Delay(80);
	}
	return temp;
}

void EIE3810_TIM3_Init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->DIER|=1<<0;
	TIM3->CR1|=0x01;
	NVIC->IP[29] = 0x45;
	NVIC->ISER[0] |= (1<<29);
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR&1<<0)
	{
		u8 Input;
		Input=JOYPAD_Read(); //read joypad to variable Input
		if (Input)
		{
			u8 pause = Input & 0x08; //pause and play
			u8 button5 = Input & 0x10; //if UP, button5 is 1
			u8 button6 = Input & 0x20; //if DOWN, button6 is 1
			u8 button7 = Input & 0x40; //if LEFT, button7 is 1
			u8 button8 = Input & 0x80; //if RIGHT, button8 is 1
			
			if (button5>>4){
				button++;
				if ((Page == 2) && (button > 2)){
					ShowCharALine(20, 350, "Easy", WHITE, BLUE); //easy highlighted
					ShowCharALine(20, 400, "Hard", BLUE, WHITE);
					difficulty = 1; //easy
					button = 0;
				}
			}
			else if (button6>>5){
				button++;
				if ((Page == 2)  && (button > 2)){
					ShowCharALine(20, 350, "Easy", BLUE, WHITE);
					ShowCharALine(20, 400, "Hard", WHITE, BLUE); //hard highlighted
					difficulty = 2; //hard
					button = 0;
				}
			}
			
			else if ((Page == 4) && (button7 >> 6)){
				EIE3810_TFTLCD_FillRectangle(x_playerA, 80, 0, 5, WHITE);  //delete previous box of catcher B
				if (x_playerA > 0){
					x_playerA--;  //shift position of the catcher A to left (can press ctsly)
				}
				EIE3810_TFTLCD_FillRectangle(x_playerA, 80, 0, 5, BLACK); //draw box at catcher position after shift left
			}	
			else if ((Page == 4) && (button8 >> 7)){
				EIE3810_TFTLCD_FillRectangle(x_playerA, 80, 0, 5, WHITE);  //delete previous box of catcher B
				if (x_playerA < 400){
					x_playerA++;  //shift position of the catcher B to right (can press ctsly)
				}
				EIE3810_TFTLCD_FillRectangle(x_playerA, 80, 0, 5, BLACK); //draw box at catcher position after shift right
			}
			else if (pause){
				button++;
				if ((button > 2) && (Page == 4) && (count == 0)){
					Page = 6; //paused
					button = 0;
				}
				if ((button > 2) && (Page == 6) && (count == 1)){
					Page = 4; //play
					ShowCharALine(200, 400, "Paused", WHITE, WHITE); //delete the 'Paused'
					button = 0;
				}
			}	
			else{
				count = 0;
			}
		}
	}
	TIM3->SR &=~(1<<0);
}


void EIE3810_TIM4_Init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1<<2; //enable TIM4
	TIM4->ARR=arr; //set counter value for TIM4
	TIM4->PSC=psc; //set pre-scalar value for TIM4
	TIM4->DIER|=1<<0; //en bits
	TIM4->CR1|=0x01; //counter
	NVIC->IP[30] = 0x45; //set priority to 0x45
	NVIC->ISER[0] |= (1<<30); //position for TIM4 is 30
}

void TIM4_IRQHandler(void){
    if ((TIM4->SR & 1<<0) && (Page == 4)){
			ShowCharALine(10,400,"All Bounces=", BLUE, WHITE);
			ShowCharALine(10,450,"B score=", BLUE, WHITE);
			ShowCharALine(10,500,"A score=", BLUE, WHITE);
			ShowCharALine(10,550,"Timer= ", BLUE, WHITE);
			ShowCharALine(800,400, bounces, BLUE,WHITE); //total bounces (horizontal and vertical)
			ShowCharALine(800,450, up_playerb , BLUE, WHITE); //score of player B
			ShowCharALine(800,500, down_playera, BLUE, WHITE); //score of player A
			ShowCharALine(800,550, t, WHITE, WHITE); //timer
			Timer ++;
			sprintf(t,"%d",Timer);
			ShowCharALine(800,550, t, BLUE, WHITE);
    }
    TIM4->SR &= ~(1<<0);
}

void EIE3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp1, temp2;
	temp2 = prigroup & 0x00000007;
	temp2 <<=8;
	temp1 = SCB->AIRCR;
	temp1 &= 0x0000F8FF;
	temp1 |=0x05FA0000;
	temp1 |= temp2;
	SCB->AIRCR = temp1;
}

void EIE3810_SYSTICK_Init()
{
	SysTick->CTRL = 0; //Clear SysTick->CTRL setting
	SysTick->LOAD = 719999; //100Hz
	SysTick->CTRL = 0x7; //CLKSOURCE=0, counter enable
}
