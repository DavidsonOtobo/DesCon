/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "STM32F4xx.h"
#include "LED.h"



volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}


/*----------------------------------------------------------------------------
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;

  GPIOA->MODER    &= ~((3UL << 2*0)  );         /* PA.0 is input              */
  GPIOA->OSPEEDR  &= ~((3UL << 2*0)  );         /* PA.0 is 50MHz Fast Speed   */
  GPIOA->OSPEEDR  |=  ((2UL << 2*0)  ); 
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );         /* PA.0 is no Pull up         */
}

/*----------------------------------------------------------------------------
  Function that read Button pins
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL << 0));
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t num = -1; 
  int32_t dir =  1;
  uint32_t btns = 0;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }

  LED_Init();
  BTN_Init(); 

	int i = 0x00;	
	int btn = 0;
 
  while(1) {                                    /* Loop forever               */
    btns = BTN_Get();                           /* Read button states         */
		
    if ((btns & 0x1)) {
			if(btn == 0) {
				//i = 0xFF;
				btn = 1;
			}
			else if(btn == 1){
				//i = 0x00;
				btn = 0;
			}	
		}
		LED_Out(i);
		Delay(200);  
		
		if(btn == 0) {
			i++;
		}
		if(btn == 1) {
			i--;
		}
		if (i == 0xFF && btn == 0) {
			i = 0x00;
		} 
		else if (i == 0x00 && btn == 1){
			i = 0xFF;
		}
			
	}
}
  

