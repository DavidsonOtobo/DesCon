#include "Setup.h"

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
int muxState = 0;
int buttonPressed = 0;
uint32_t adcValue = 0;
//float loggedData[10];
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

/* Function that initialises timer 3 interrupt for updating measurement value (delay = 0.25s)*/
void TIM3I_Init (void)
{
	RCC->APB1ENR	|=	RCC_APB1ENR_TIM3EN; //enable timer 3 clock
	
	TIM3->PSC	=	599;  //prescaler	=	600	(PSC+1)
	TIM3->ARR	=	15000; //auto	reload value	
  TIM3->CR1	=	TIM_CR1_CEN; //enable	timer	
  TIM3->DIER |= 1UL	<<0;	//enable interrupt	
  NVIC_EnableIRQ(TIM3_IRQn); //enable	TIM3 interrupt in	NVIC	
	
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
  Function that initializes Buzzer
 *----------------------------------------------------------------------------*/
void BUZZ_Init(void) {

  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOEEN;        /* Enable GPIOE clock         */

  GPIOE->MODER    |= ((1UL << 2*3)   );        /* PE.3 is output             */
	GPIOE->OTYPER   &= !((1UL <<   3)  );				 /* PE.3 is output Push-Pull   */     
  GPIOE->OSPEEDR  |=  ((2UL << 2*3)  ); 			 /* PE.3 is 50MHz Fast Speed */       
	GPIOE->PUPDR    |=  ((3UL << 2*3)  );				 /* PE.3 is no Pull up         */
}

void A_Init(void) {

  GPIOE->MODER    |= ((1UL << 2*4)   );        /* PE.4 is output             */
	GPIOE->OTYPER   &= !((1UL <<   4)  );				 /* PE.4 is output Push-Pull   */     
  GPIOE->OSPEEDR  |=  ((2UL << 2*4)  ); 			 /* PE.4 is 50MHz Fast Speed */       
	GPIOE->PUPDR    |=  ((3UL << 2*4)  );				 /* PE.4 is no Pull up         */
	
	GPIOE->MODER    |= ((1UL << 2*5)   );        /* PE.5 is output             */
	GPIOE->OTYPER   &= !((1UL <<   5)  );				 /* PE.5 is output Push-Pull   */     
  GPIOE->OSPEEDR  |=  ((2UL << 2*5)  ); 			 /* PE.5 is 50MHz Fast Speed */       
	GPIOE->PUPDR    |=  ((3UL << 2*5)  );				 /* PE.5 is no Pull up         */
	
	GPIOE->MODER    |= ((1UL << 2*6)   );        /* PE.6 is output             */
	GPIOE->OTYPER   &= !((1UL <<   6)  );				 /* PE.6 is output Push-Pull   */     
  GPIOE->OSPEEDR  |=  ((2UL << 2*6)  ); 			 /* PE.6 is 50MHz Fast Speed */       
	GPIOE->PUPDR    |=  ((3UL << 2*6)  );				 /* PE.6 is no Pull up         */
}




/* Function to intiialise ADC1    */

void ADC1_init(void) {
	/* Enable clocks */
	RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;
	
	/* ADC12_IN14 is the channel we shall use. It is connected to 
	 * PC4 which is connected to the board edge connectors */
	GPIOC->MODER = 0x3 << (2 * 4);
	GPIOC->PUPDR = 0;
	
	/* Set ADC to discontinuous conversion mode. */
	ADC1->CR1 = 0x00;
	ADC1->CR1 |= ADC_CR1_DISCEN;
	
	/* Ensure CR2 is set to zero. This means data will be right aligned, 
	 * DMA is disabled and there are no extrnal triggers/injected channels */
	ADC1->CR2 = 0x00;
	
	/* Set to one conversion at a time, and set that first conversion 
	 * to come from channel 14 (connected to PC4) */
	ADC1->SQR1 &= ~ADC_SQR1_L;
	ADC1->SQR3 = 14 & ADC_SQR3_SQ1;
	
	/* Enable the ADC */
	ADC1->CR2 |= ADC_CR2_ADON;
}
	


void Setup(void)
{
	
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	

	
		// Initializes LEDs, Buttons and ADC
  LED_Init();
  BTN_Init();  
	ADC1_init();
	SWT_Init();
	BUZZ_Init();
	A_Init();
	TIM3I_Init();
	
	//Initializes LCD
	LCD_Initpins();	
	LCD_DriverOn();
	Delay(10);
	LCD_Init();
	LCD_On(1);
	Delay(20);
	LCD_Clear();
	
  buttonPressed = 0;//clear buttons
	
}
