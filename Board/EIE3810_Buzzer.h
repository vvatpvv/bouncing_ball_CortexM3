#ifndef __EIE3810_BUZZER_H
#define __EIE3810_BUZZER_H
#include "stm32f10x.h"

void EIE3810_Buzzer_Init(void);
int PB8State(void);
void turnoffBuzzer(void);
void turnonBuzzer(void);

#endif
