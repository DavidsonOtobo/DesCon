/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: COPIES SWITCH VALUES TO LEDS ON CARRIER BOARD
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
 
 /* MODIFIED BY D. CHESMORE JANUARY 2013 */
 
#include <stdio.h>
#include "STM32F4xx.h"
#include "SWT.h"



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


/* Function to intiialise DAC1, connected to pin PA4     */
void DAC_init(void) {
	/* Enable clocks */
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR  |= RCC_APB1ENR_DACEN;
	
	/* set PA4 and PA5 to analogue */
	GPIOA->MODER |= 3UL << (2 * 4);		
	GPIOA->PUPDR &= ~(3UL << (2 * 4));
	
	/* turn DAC channel 1 on*/
  DAC->CR |= DAC_CR_EN1;
	
}
	
/* function to write DAC1. Uses a 12 bit right-aligned number */
void write_DAC(uint32_t val) {
	DAC->DHR12R1 = val;
}
	
/*----------------------------------------------------------------------------
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;              /* Enable GPIOA clock         */

  GPIOA->MODER    &= ~((3UL << 2*0)  );         /* PA.0 is input              */
  GPIOA->OSPEEDR  &= ~((3UL << 2*0)  );         /* PA.0 is 50MHz Fast Speed   */
  GPIOA->OSPEEDR  |=  ((2UL << 2*0)  ); 
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );         /* PA.0 is no Pull up         */

	

}

/*----------------------------------------------------------------------------
  Function that read User Button pin
 *----------------------------------------------------------------------------*/
 uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL<<0));
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

int main (void) {

  uint32_t btns = 0;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }

  BTN_Init();   
  SWT_Init();	
	DAC_init();
	
  while(1) {
    btns = SWT_Get();
		
		/* Shifts the buttons value right 4 because the buttons are between bits 15 
		 * and 8 and the DAC expects a 12 bit right-aligned number */
		write_DAC(20);
		
		Delay(50);
	}

	
				
}

