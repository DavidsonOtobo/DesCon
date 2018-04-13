/*----------------------------------------------------------------------------
 * Name:    SWT.c
 * Purpose: low level SWT 'input switch' functions
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

#include "STM32F4xx.h"
#include "Setup.h"

const unsigned long SWT_mask[] = {1UL << 8, 1UL << 9, 1UL << 10, 1UL << 11, 1UL << 12, 1UL << 13, 1UL << 14, 1UL << 15};

/*----------------------------------------------------------------------------
  initialize SWT Pins
 *----------------------------------------------------------------------------*/
void SWT_Init (void) {
	uint32_t tmp = 0x00; 
	/* Enable GPIOE clock              */
  RCC->AHB1ENR    |=  RCC_AHB1ENR_GPIOEEN;
	
	/* Clock for SYSCFG system config controller */
	RCC->APB2ENR |= 0x00004000;

  GPIOE->MODER    &= ~((3UL << 2* 8) |
                       (3UL << 2* 9) |
                       (3UL << 2*10) |
                       (3UL << 2*11) |
                       (3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PE.8..15 is input               */
  GPIOE->OSPEEDR  &= ~((3UL << 2* 8) |
                       (3UL << 2* 9) |
                       (3UL << 2*10) |
                       (3UL << 2*11) |
                       (3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PE.8..15 is 50MHz Fast          */
  GPIOE->OSPEEDR  |=  ((2UL << 2* 8) |
                       (2UL << 2* 9) | 
                       (2UL << 2*10) | 
                       (2UL << 2*11) | 
                       (2UL << 2*12) | 
                       (2UL << 2*13) | 
                       (2UL << 2*14) | 
                       (2UL << 2*15)  ); 
  GPIOE->PUPDR    &= ~((3UL << 2* 8) |
                       (3UL << 2* 9) |
                       (3UL << 2*10) |
                       (3UL << 2*11) |
                       (3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PE.8..15 is no Pull up             */
											 
  /*Enable IRQ for ext signals 5-9 and 10-15*/
	NVIC_EnableIRQ(EXTI9_5_IRQn);   
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
 //port E = 0x04 //pin source 8 = 0x08 //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource8 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource8 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource8 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource8 & (uint8_t)0x03)));
 
 //port E = 0x04 //pin source 9 = 0x09 //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource9 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource9 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource9 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource9 & (uint8_t)0x03)));
 
 //port E = 0x04 //pin source 10 = 0x0A //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource10 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource10 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource10 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource10 & (uint8_t)0x03)));
 
 //port E = 0x04 //pin source 11 = 0x0B //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource11 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource11 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource11 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource11 & (uint8_t)0x03)));
 
 //port E = 0x04 //pin source 12 = 0x0C //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource12 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource12 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource12 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource12 & (uint8_t)0x03)));
 
 //port E = 0x04 //pin source 13 = 0x0D //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource13 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource13 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource13 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource13 & (uint8_t)0x03)));
	
 //port E = 0x04 //pin source 14 = 0x0E //select rising edge for pin
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource14 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource14 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource14 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource14 & (uint8_t)0x03)));

 //port E = 0x04 //pin source 15 = 0x0F //select rising edge for pin 
 tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSource15 & (uint8_t)0x03));
 SYSCFG->EXTICR[EXTI_PinSource15 >> 0x02] &= ~tmp;
 SYSCFG->EXTICR[EXTI_PinSource15 >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOD) << (0x04 * (EXTI_PinSource15 & (uint8_t)0x03)));
 
 /*Allow rising edge for the lines*/
 EXTI->RTSR |= EXTI_line8;
 EXTI->RTSR |= EXTI_line9;
 EXTI->RTSR |= EXTI_line10;
 EXTI->RTSR |= EXTI_line11;
 EXTI->RTSR |= EXTI_line12;
 EXTI->RTSR |= EXTI_line13;
 EXTI->RTSR |= EXTI_line14;
 EXTI->RTSR |= EXTI_line15;
 
 /*Enable interrupt for the lines*/
 EXTI->IMR |= EXTI_line8;
 EXTI->IMR |= EXTI_line9;
 EXTI->IMR |= EXTI_line10;
 EXTI->IMR |= EXTI_line11;
 EXTI->IMR |= EXTI_line12;
 EXTI->IMR |= EXTI_line13;
 EXTI->IMR |= EXTI_line14;
 EXTI->IMR |= EXTI_line15;

}



/*----------------------------------------------------------------------------
  Function that reads Switch states
 *----------------------------------------------------------------------------*/
uint32_t SWT_Get (void) {

  return (GPIOE->IDR);
}

/*----------------------------------------------------------------------------
  Function that checks the state of requested switch
 *----------------------------------------------------------------------------*/
int SWT_Check (unsigned int num) {
	
	if ( num > SWT_NUM ) {
		return 0;
	}
	else {
		return ( GPIOE->IDR & SWT_mask[num] );
	}
}
