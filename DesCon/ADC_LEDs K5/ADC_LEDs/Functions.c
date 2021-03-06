#include "Functions.h"
#include "Setup.h"

void EXTI9_5_IRQHandler(void) {
	if 	(EXTI->PR & (1<<8)){ //check if button 8 is pressed
		EXTI->PR |= (1 << 8);
		buttonPressed = BTN1; //log button press
	}
	
	if 	(EXTI->PR & (1<<9)){
		EXTI->PR |= (1 << 9);
		GPIOD->ODR =0x0f  <<8;
		buttonPressed = BTN2;
	}
	
}



void EXTI15_10_IRQHandler(void){
	
	
	if (EXTI->PR & (1<<10)){//check if button is pressed
		EXTI->PR |= (1 << 10);//clear flagGPIOD->ODR =0xff  <<8;
		buttonPressed = BTN3;
	}
	
	if (EXTI->PR & (1<<11)){//check if button is pressed
		EXTI->PR |= (1 << 11);//clear flag
		buttonPressed = BTN4;
	}
	
  if (EXTI->PR & (1<<12)){//check if button is pressed
		EXTI->PR |= (1 << 12);//clear flag
		buttonPressed = BTN5;
	}
	
	if (EXTI->PR & (1<<13)){//check if button is pressed
		EXTI->PR |= (1 << 13);//clear flag
		buttonPressed = BTN6;
	}
	
	if (EXTI->PR & (1<<14)){//check if button is pressed
		EXTI->PR |= (1 << 14);//clear flag
		buttonPressed = BTN7;
	}
	
	if (EXTI->PR & (1<<15)){//check if button is pressed
		EXTI->PR |= (1 << 15);//clear flag
		buttonPressed = BTN8;
	}
	
}

void	TIM3_IRQHandler	(void)
{
	if(TIM3->SR & TIM_SR_UIF) //check if timer flag is set
	{
		TIM3->SR&=~TIM_SR_UIF; //clear flag
		
		//GPIOD->ODR =0xff  <<8; //test
		
		adcValue = read_ADC1(); // Gets ADC value
	}
	
}

void ledsON (uint8_t leds)
{
	GPIOD->ODR = 0x00 << 8;//clears LEDs
	GPIOD->BSRR = (leds & 0xf0) <<4;
	GPIOD->BSRR = (leds & 0x0f) <<12;
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
		checkVal = value/1000.0f;
	}
	else if (muxState == 2 && measurement == 1) {
		checkVal = value/100.0f;
	}
	else if (muxState == 3 && measurement == 1) {
		checkVal = value/10.0f;
	}
	
	// Voltage
	// Gain of 1000 if between +-0.9mV
	if ((checkVal < 0.0009f && checkVal > - 0.0009f && measurement == 1) || 
		(checkVal > 2.99f && measurement == 3)) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(1);
		muxState = 1;
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value/1000.0f;
	}
		
	// Gain of 100 if between +-9mV
	else if ((checkVal < 0.09f && checkVal > - 0.09f && measurement == 1)
		|| (checkVal > 2.99f && checkVal == 3)) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(0);
		muxState = 2;
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value/100.0f;
	}
		
	// Gain of 10 if between +-900mV
	else if ((checkVal < 0.9f && checkVal > -0.9f && measurement == 1) 
		|| (checkVal > 2.99f && measurement == 3)) {
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(1);
		muxState = 3;
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value/10.0f;
	}
		// No gain
	else if (measurement == 1){
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(0); 
		muxState = 0;
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value;
	}
		//current//unfinshed, waiting for hardware team
	if ( checkVal < 1 && checkVal > 1.8f && measurement == 2){
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
	if (checkVal > 2.7f && checkVal < 3.1f && measurement == 3) {
		muxState = muxState + 1;
	}
	// Decreases the gain when the voltage is 3V
	else if (checkVal > 1.3f && checkVal < 1.6f && measurement == 3) {
		muxState = muxState - 1;
	}
	
	// No gain state
	if (muxState == 0 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(0);
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value/0.000988f;
	}
	// Increments by 1 in binary if muxState increases and vice versa
	else if (muxState == 1 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(0);
		A0_Trig(1);
		value = (value/3.0f) * 3.0225f; // Tweaks to the resolution for precision
		return value/0.0005332f;
	}
	else if (muxState == 2 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(0);
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value/0.00004771f;
	}
	else if (muxState == 3 && measurement == 3) {
		A2_Trig(0);
		A1_Trig(1);
		A0_Trig(1);
		value = (value/3.0f) * 3.0f; // Tweaks to the resolution for precision
		return value/0.00002471f;
	}
	else if (muxState == 4 && measurement == 3) {
		A2_Trig(1);
		A1_Trig(0);
		A0_Trig(0);
		value = (value/3.0f) * 2.729f; // Tweaks to the resolution for precision
		return value/0.00000084f;
	}	
	
	return 0;
}


// Outputs value of voltage
float voltage (float reading)
{
	float V2,Vout;
	
	Vout = ((float)reading/4096.0f)*3.0f; // Gets value of voltage from ADC
	V2 = ((Vout - 1.5f)/0.15f)-0.2f;
	autoRange(V2, 1); // does autoranging on voltage
	
	// Turns on buzzer if the voltage is out of range
	if (V2 > 9.99f || V2 == -10.0f) {
		BUZZ_Trig(2);
	}
	// Buzzer starts beeping if it gets close to being out of range
	else if (V2 >= 9.5f || V2 <= -9.5f) {
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
	float V1,Vout;

	Vout = ((float)reading/4096.0f)*3.0f; // Gets value of voltage from ADC
	V1 = ((Vout - 1.5f)/0.15f)/10;
	
	// Turns on buzzer if the current is out of range
	if (V1 > 0.99f || V1 == -1.0f) {
		BUZZ_Trig(2);
	}
	// Buzzer starts beeping if it gets close to being out of range
	else if (V1 >= 0.8f || V1 <= -0.8f) {
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
	float Vout = ((float)reading/4096.0f)*3.0f;
	float R =  autoRange(Vout, 3); // Does autoranging for resistance
	R = R/1.5f;										//scaling factor  
	
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



void startUpMenu (void)
{
	int scroll = 0;
	
	char welcomeMes[10]; 
	char opts[16];

	
	LCD_Clear();
	sprintf(welcomeMes,"Multimeter");//First level of menu
	LCD_PutS(welcomeMes);
	LCD_GotoXY(0,1);
	
	scroll = 0;

	do{
		
		buttonPressed = 0;
		
		if(scroll == 0){
			LCD_GotoXY(0,1);
			sprintf(opts,"1=V,2=A,3=%c,4=DL", 222);
			LCD_PutS(opts);
			ledsON(0xF8);//lights available buttons
		}
		else if(scroll == 1){
			LCD_GotoXY(0,1);
			sprintf(opts,"5=L,6=T,7=Other ");
			LCD_PutS(opts);
			ledsON(0x0F);
		}

		while(buttonPressed == 0);//wait for button press
		scroll = !scroll;
	
	}while((buttonPressed == BTN8) || (buttonPressed == 0));
}

void mode (int modeSelect)
{
	char title[16];
	float calcVal = 0.0;
	char output[16];
	uint32_t adcPreviousVal = 0;
	char prefix; 
	//int logged[
  //char unit[3];
	//char suffix;
	
	
	buttonPressed = 0; //clear button press
	
	
	/*Voltage mode */
	
	if(modeSelect == BTN1) //if voltage button pressed
	{
		LCD_Clear();
		sprintf(title,"Voltmeter");
		LCD_PutS(title);
		
		while(buttonPressed != BTN8){
		
			adcPreviousVal =  adcValue;
				
			while(adcPreviousVal == adcValue && buttonPressed != BTN8)//wait until something changes 
			
			LCD_GotoXY(0,1);// set cursor to bottom line
			calcVal = voltage(adcValue); //get voltage measurement from ADC value
			
			if (calcVal > -1 && calcVal < 1){// if volts less than 1 convert to milli
				calcVal = calcVal*1000;
				prefix = 'm'; //set preffix
			}
			else{
				prefix = ' ';
			}
			snprintf(output,16,"%2.2f%cV", calcVal,prefix); //2.2 allows 2 places either side of dp
			LCD_PutS(output); //Outputs to LCD
		
		}
		
		
	}
	
	/*Current mode*/
	
	if(modeSelect == BTN2) // if current button pressed
	{
		LCD_Clear();
		sprintf(title,"Ammeter");//title message
		LCD_PutS(title);//instert title message

		while(buttonPressed != BTN8){ //checks back button has not been pressed

			adcPreviousVal =  adcValue; //hold the last ADC value 
				
			while(adcPreviousVal == adcValue && buttonPressed != BTN8)//wait until something changes 

			LCD_GotoXY(0,1);// set cursor to bottom line
			calcVal = current(adcValue); //get current measurement from ADC value

			if (calcVal > -1 && calcVal < 1)// if current less than 1 convert to milli
			{
				calcVal = calcVal*1000;
				prefix = 'm'; //set preffix
			}
			else {
				prefix = ' '; //otherwise no prefix
			} 

			sprintf(output,"%2.2f%cA", calcVal,prefix); //2.2 allows 2 places either side of dp
			LCD_PutS(output); //Outputs to LCD

		}
	}
	
	 /*Resistance mode*/
	
		if(modeSelect == BTN3) // if current button pressed
		{
			LCD_Clear();
			sprintf(title,"Ohmmeter");//title message
			LCD_PutS(title);//instert title message
		
			while(buttonPressed != BTN8){ //checks back button has not been pressed
			
				adcPreviousVal =  adcValue; //hold the last ADC value 
					
				while(adcPreviousVal == adcValue && buttonPressed != BTN8)//wait until something changes 
				
				LCD_GotoXY(0,1);// set cursor to bottom line
				calcVal = resistance(adcValue); //get resistance measurement from ADC value
				
				if (calcVal > -1 && calcVal < 1)// if resistance less than 1 convert to milli
				{
					calcVal = calcVal*1000;
					prefix = 'm'; //set preffix
				}
				else{
					prefix = ' '; //otherwise no prefix
				} 
					
				sprintf(output,"%2.2f%c%c",calcVal,prefix, 222); //2.2 allows 2 places either side of dp
				LCD_PutS(output); //Outputs to LCD
			
			}
	}
		
	
	
}

