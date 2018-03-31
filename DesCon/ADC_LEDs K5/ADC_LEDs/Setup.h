#ifndef __SETUP_H__
#define __SETUP_H__

#include <stdio.h>
#include "STM32F4xx.h"
#include "LED.h"
#include "LCD.h"
#include "SWT.h"
#include "math.h"
#include "string.h"

extern volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
extern int muxState;

void SysTick_Handler(void);
void Delay (uint32_t dlyTicks);
void BTN_Init(void);
void BUZZ_Init(void);
void A_Init(void);
void BUZZ_Trig(int type);
void A2_Trig(int type);
void A1_Trig(int type);
void A0_Trig(int type);
uint32_t BTN_Get(void);
void ADC1_init(void);
unsigned int read_ADC1 (void);
float autoRange (float value, int measurement);
float voltage (float reading);
float current (float reading);
float resistance (float reading);
void Setup(void);



#endif
