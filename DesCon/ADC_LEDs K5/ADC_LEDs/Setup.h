#ifndef __SETUP_H__
#define __SETUP_H__

#include <stdio.h>
#include "STM32F4xx.h"
#include "LED.h"
#include "LCD.h"
#include "SWT.h"
#include "math.h"
#include "string.h"

/*External interrupt line definitions*/
#define EXTI_line8  ((uint32_t)0x00100)
#define EXTI_line9  ((uint32_t)0x00200)
#define EXTI_line10 ((uint32_t)0x00400)
#define EXTI_line11 ((uint32_t)0x00800)
#define EXTI_line12 ((uint32_t)0x01000)
#define EXTI_line13 ((uint32_t)0x02000)
#define EXTI_line14 ((uint32_t)0x04000)
#define EXTI_line15 ((uint32_t)0x08000)

/*External interrupt pin source definitions*/
#define EXTI_PinSource8   0x08
#define EXTI_PinSource9   0x09
#define EXTI_PinSource10  0x0A
#define EXTI_PinSource11  0x0B
#define EXTI_PinSource12  0x0C
#define EXTI_PinSource13  0x0D
#define EXTI_PinSource14  0x0E
#define EXTI_PinSource15  0x0F

/*External port source definition definition*/
#define EXTI_PortSourceGPIOD 0x04

/*BTN definitions*/
#define BTN1 10000000
#define BTN2 01000000
#define BTN3 00100000
#define BTN4 00010000
#define BTN5 00001000
#define BTN6 00000100
#define BTN7 00000010
#define BTN8 00000001


extern volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
extern int muxState;
extern int buttonPressed;  //stores buttons presses
extern uint32_t adcValue; //stores ADC value

void SysTick_Handler(void);
void Delay (uint32_t dlyTicks);
void TIM3I_Init (void);
void BTN_Init(void);
void BUZZ_Init(void);
void A_Init(void);

void ADC1_init(void);

void Setup(void);



#endif
