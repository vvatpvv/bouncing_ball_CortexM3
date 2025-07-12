#ifndef __EIE3810_LED_H
#define __EIE3810_LED_H
#include "stm32f10x.h"

void EIE3810_LED_Init(void);
int PB5State(void);
int PE5State(void);
void turnonLED0(void);
void turnoffLED0(void);
void turnonLED1(void);
void turnoffLED1(void);

#endif
