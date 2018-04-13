#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "Setup.h"

void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void ledsON (uint8_t leds);
void BUZZ_Trig(int type);
void A2_Trig(int type);
void A1_Trig(int type);
void A0_Trig(int type);
uint32_t BTN_Get(void);
unsigned int read_ADC1 (void);
float autoRange (float value, int measurement);
float voltage (float reading);
float current (float reading);
float resistance (float reading);
void startUpMenu (void);
void mode (int btnPressed);




#endif
