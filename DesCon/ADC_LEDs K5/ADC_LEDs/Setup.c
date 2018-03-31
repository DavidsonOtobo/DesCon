#include "Setup.h"

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
int muxState = 0;

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


/*----------------------------------------------------------------------------
  Function that triggers Buzzer
 *----------------------------------------------------------------------------*/
void BUZZ_Trig(int type) {
	// Turn off buzzer
	if (type == 0) {
		GPIOE->BSRR|=(1UL << 3)<<16;
	}
	// Beeping buzzer
	else if (type == 1) {
		GPIOE->BSRR|=(1UL << 3);
		Delay(100);
		GPIOE->BSRR|=(1UL << 3)<<16;
	}
	// Turn on buzzer
	else if (type == 2) {
		GPIOE->BSRR|=(1UL << 3);
	}
}

/*----------------------------------------------------------------------------
  Function that triggers A2
 *----------------------------------------------------------------------------*/
void A2_Trig(int type) {
	// Turns off buzzer
	if (type == 0) {
		GPIOE->BSRR|=(1UL << 4)<<16;
	}
	// Turns on buzzer
	else if (type == 1) {
		GPIOE->BSRR|=(1UL << 4);
	}
}

/*----------------------------------------------------------------------------
  Function that triggers A1
 *----------------------------------------------------------------------------*/
void A1_Trig(int type) {
	// Turns off A1
	if (type == 0) {
		GPIOE->BSRR|=(1UL << 5)<<16;
	}
	// Turns on A1
	else if (type == 1) {
		GPIOE->BSRR|=(1UL << 5);
	}
}

/*----------------------------------------------------------------------------
  Function that triggers A0
 *----------------------------------------------------------------------------*/
void A0_Trig(int type) {
	// Turns off A0
	if (type == 0) {
		GPIOE->BSRR|=(1UL << 6)<<16;
	}
	// Turns on A0
	else if (type == 1) { 
		GPIOE->BSRR|=(1UL << 6);
	}
}

/*----------------------------------------------------------------------------
  Function that read Button pins
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL << 0));
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
	
/* function to read ADC and retun value */
unsigned int read_ADC1 (void) {
	/* set SWSTART to 1 to start conversion */
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
	/* Wait until End Of Conversion bit goes high */
	while (!(ADC1->SR & ADC_SR_EOC));
	
	/* Return data value */
	return (ADC1->DR);
}

/*----------------------------------------------------------------------------
  Autoranging functionality
 *----------------------------------------------------------------------------*/
float autoRange (float value, int measurement)
{	
	/*float V2,Vout;
	Vout = ((float)value/4096.0)*3.0; // Gets value of voltage from ADC
	V2 = ((Vout - 1.5)/0.15)-0.2;*/
	
	float checkVal = value;
	//muxState = 0; //defaults at 0
	// Checks value at input after going through gains
	if (muxState == 1 && measurement == 1) {
		checkVal = value/1000.0;
	}
	else if (muxState == 2 && measurement == 1) {
		checkVal = value/100.0;
	}
	else if (muxState == 3 && measurement == 1) {
		checkVal = value/10.0;
	}
	
	// Voltage
	// Gain of 1000 if between +-0.9mV
	if ((checkVal < 0.0009 && checkVal > - 0.0009 && measurement == 1) || 
		(checkVal > 2.99 && measurement == 3)) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(1);
		muxState = 1;
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value/1000.0;
	}
		
	// Gain of 100 if between +-9mV
	else if ((checkVal < 0.09 && checkVal > - 0.09 && measurement == 1)
		|| (checkVal > 2.99 && checkVal == 3)) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(0);
		muxState = 2;
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value/100.0;
	}
		
	// Gain of 10 if between +-900mV
	else if ((checkVal < 0.9 && checkVal > -0.9 && measurement == 1) 
		|| (checkVal > 2.99 && measurement == 3)) {
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(1);
		muxState = 3;
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value/10.0;
	}
		// No gain
	else if (measurement == 1){
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(0); 
		muxState = 0;
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value;
	}
		//current//unfinshed, waiting for hardware team
	if ( checkVal < 1 && checkVal > 1.8 && measurement == 2){
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(1);
		muxState = 1;
		value = value;
    return value;		
	}
	else if (measurement == 2){
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(0);
		muxState = 0;
		value = value;
    return value;		
	}
	
	//Resistance
	// Increases the gain when the voltage is 3V
	if (checkVal > 2.7 && checkVal < 3.1 && measurement == 3) {
		muxState = muxState + 1;
	}
	// Decreases the gain when the voltage is 3V
	else if (checkVal > 1.3 && checkVal < 1.6 && measurement == 3) {
		muxState = muxState - 1;
	}
	
	// No gain state
	if (muxState == 0 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(0);
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value/0.000988;
	}
	// Increments by 1 in binary if muxState increases and vice versa
	else if (muxState == 1 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(1);
		value = (value/3.0) * 3.0225; // Tweaks to the resolution for precision
		return value/0.0005332;
	}
	else if (muxState == 2 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(0);
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value/0.00004771;
	}
	else if (muxState == 3 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(1);
		value = (value/3.0) * 3.0; // Tweaks to the resolution for precision
		return value/0.00002471;
	}
	else if (muxState == 4 && measurement == 3) {
		A2_Trig(1);
		A1_Trig(0);
		A0_Trig(0);
		value = (value/3.0) * 2.729; // Tweaks to the resolution for precision
		return value/0.00000084;
	}	
	

}


// Outputs value of voltage
float voltage (float reading)
{
	float V2,Vout;
	
	Vout = ((float)reading/4096.0)*3.0; // Gets value of voltage from ADC
	V2 = ((Vout - 1.5)/0.15)-0.2;
	autoRange(V2, 1); // does autoranging on voltage
	
	// Turns on buzzer if the voltage is out of range
	if (V2 > 9.99 || V2 == -10.0) {
		BUZZ_Trig(2);
	}
	// Buzzer starts beeping if it gets close to being out of range
	else if (V2 >= 9.5 || V2 <= -9.5) {
		BUZZ_Trig(1);
	}
	// If measurements are in range the buzzer will not beep
	else {
		BUZZ_Trig(0);
	}
	return (V2);
}

// Outputs value of voltage
float current (float reading)
{
	float R1,R2,R3,Rgain,Rf,Vref,V1,V2,Vout;

	Vout = ((float)reading/4096.0)*3.0; // Gets value of voltage from ADC
	V1 = ((Vout - 1.5)/0.15)/10;
	
	// Turns on buzzer if the current is out of range
	if (V1 > 0.99 || V1 == -1.0) {
		BUZZ_Trig(2);
	}
	// Buzzer starts beeping if it gets close to being out of range
	else if (V1 >= 0.8 || V1 <= -0.8) {
		BUZZ_Trig(1);
	}
	// If measurements are in range the buzzer will not beep
	else {
		BUZZ_Trig(0);
	}
	
	return (V1);
}

float resistance (float reading)
{
	float Vout = ((float)reading/4096.0)*3.0;
	float R =  autoRange(Vout, 3); // Does autoranging for resistance
	R = R/1.5;										//scaling factor  
	
	// Turns on buzzer if the resistance is out of range
	if (R == 0){
		BUZZ_Trig(2);
	}
	// If measurements are in range the buzzer will not beep
	else {
		BUZZ_Trig(0);
	}
	
	return (R);
}

void EXTI15_10_CONFIG (void)
{
	EXTI_initTypeDef EXTI15_10_IRQn; 
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
	
	//Initializes LCD
	LCD_Initpins();	
	LCD_DriverOn();
	Delay(10);
	LCD_Init();
	LCD_On(1);
	Delay(20);
	LCD_Clear();
	
  
	
}
