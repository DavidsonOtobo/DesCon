/*----------------------------------------------------------------------------
 * Name:    ADC_LEDs.c

reads ADC channel and displays upper 8 bits (of 12) on LEDs*/


#include <stdio.h>
#include "STM32F4xx.h"
#include "LED.h"
#include "LCD.h"
#include "SWT.h"
#include "math.h"



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

  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;              /* Enable GPIOA clock         */

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

// Outputs value of voltage
float voltage (float reading)
{
	reading = ((((float)reading/4096.0)*3.0)/0.15)-10; // Gets value of voltage
	return (reading);
}

float current (float reading)
{
	return(reading);
}

float resistance (float reading)
{
	return (reading);
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
 
	uint32_t value = 0;
	float calcVal = 0.0;
	uint32_t btns = 0;
	int32_t menu = 0;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }

	// Initializes LEDs, Buttons and ADC
  LED_Init();
  BTN_Init();  
	ADC1_init();
	SWT_Init();
	
	//Initializes LCD
	LCD_Initpins();	
	LCD_DriverOn();
	
	Delay(10);
	LCD_Init();
	
	LCD_DriverOn();
	LCD_On(1);
	Delay(20);
	LCD_Clear();

	
	
	GPIOD->ODR = 0;	
	
	
	// Stores key press
	char keyPressed[2];
	// Represents unit of measurement
	char unit[2] = "  ";
	
	uint32_t default_btns = SWT_Get(); 
 
  while(1) {      

		/* Loop forever               */

		btns = SWT_Get(); //Gets value of button pressed
		
		if(btns != default_btns){
			GPIOD->ODR = btns; //Outputs button press to LEDs
			menu = (((int)log2(btns >> 8))) + 1;
		}			
		
		value = read_ADC1(); 												/* Gets a 12 bit right-aligned value from the ADC */
		//value = (value << 4) & 0xFF00; 						/* Shift and AND to isolate bits 15-8 */
		
		if(menu == 1) {
			calcVal = voltage(value);
			if(menu == 4) {
				sprintf(unit,"mV");
			}
			else {
				sprintf(unit,"V");
			}
		}
		
		sprintf(keyPressed,"%.2f%s", calcVal, unit); //Outputs dp
		LCD_PutS(keyPressed); //Outputs to LCD
		Delay(500);
		LCD_Clear();
		

		
		}
  
}

