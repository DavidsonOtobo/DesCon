/*----------------------------------------------------------------------------
 * Name:    ADC_LEDs.c

reads ADC channel and displays upper 8 bits (of 12) on LEDs*/


#include "Setup.h"
#include "Functions.h"






/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	
	float loggedData[10];
	float *ptrLog;
	
	ptrLog = loggedData;
 
	//uint32_t value = 0; // Gets value from the ADC
	//float calcVal = 0.0; // Stores output on the board
//	uint32_t btns = 0; // gets button presses
	//int32_t menu = 0; // Menu of buttons
	//int32_t menu1 = 0; // first half of the buttons
	//int32_t menu2 = 0; // second half of the buttons
//	int dataLogTimer = 0; // timer used to control data logging
	// Stores display on LCD
	//char display[2];
	// Represents unit of measurement
	//char unit[2] = "  ";
	//char suffix[1] = " ";
	//char dataLog[1]; // Data logger values
	//int dataLogCount = 0; // Iterates through data logger

	
	Setup();
	

	//strcpy(dataLog, "\0"); // Defaults all data as NULL

	
	//dataLogCount = 0;
	
	
	//GPIOD->ODR = 0;	// Sets buttons to 0

	
	//uint32_t default_btns = SWT_Get(); // Gets initial value of the buttons
	
	
	
	while(1){
		
		startUpMenu();//Displays start up menu, user will choose mode with button
		
		mode(buttonPressed/*,loggedData*/);
		
	}
	
	
	
 	/* Loop forever */
//  while(1) {      

//		//Gets value of button pressed
//		btns = SWT_Get();
//		GPIOD->ODR = btns; //Outputs button press to LEDs
//	//	menu = ((int)(btns >> 8)); // checks buttons and changes menu
//		
//		if(btns != default_btns){
//			btns = GPIOD-> ODR ^ btns; // Stores buttons
//			GPIOD->ODR = btns; //Outputs button press to LEDs
//			menu = ((int)(btns >> 8)); // checks buttons and changes menu
//			menu1 = menu & 0x0F; // Menu1 is the first 4 buttons
//			menu2 = menu & 0xF0; // Menu 2 is the last 4 buttons
//		}			
//		

//		
//		
//		
//		value = read_ADC1(); 												/* Gets a 12 bit right-aligned value from the ADC */
//	//	value = (value << 4) & 0xFF00; 						/* Shift and AND to isolate bits 15-8 */
//		
//		BUZZ_Trig(0); // Turns off buzzer
//		
//		// Outputs voltage if first button is selected
//		if(menu1 == 1) {
//			calcVal = voltage(value); //gets voltage
//			sprintf(suffix,"V"); // The voltage suffix
//			// Sets menu 2 if autoranging occurs
//			if(calcVal < 1 && calcVal > -1) {
//				menu2 = 0x10;
//			}
//			else{
//				menu2 = 0x00;
//			}
//		}
//		// Outputs current if second button is selected
//		if(menu1 == 2) {
//			calcVal = current(value); // gets current
//			sprintf(suffix,"A"); // The current suffix
//		}
//		// Outputs resistance if second button is selected
//		if(menu1 == 4) {
//			
//			calcVal = resistance(value); // gets resistance
//			sprintf(suffix,"%c", 222); // The current suffix
//		}
//		
//		// MENU 2
//		// menu 2 only carries out functionality of menu1 is set
//		//if button 1 is pressed change scale to milli
//		if(menu2 == 0x10 && menu1 != 0) {
//			calcVal = calcVal*1000.0; // converts to mili
//			sprintf(unit,"m%s",suffix); // adds milli in front of unit
//			GPIOD->ODR |= (menu2 << 8);  // Turns menu2 light on
//		}
//		//if button 2 is pressed change scale to centi
//		else	if (menu2 == 0x20 && menu1 != 0) {
//			//calcVal = calcVal*100.0;
//			sprintf(unit,"c%s",suffix); // adds centi in front of unit
//		}
//		//if button 3 is pressed change scale to deci
//		else	if (menu2 == 0x40 && menu1 != 0) {
//		//calcVal = calcVal*1000.0;
//			sprintf(unit,"d%s",suffix); // adds deci in front of unit
//		}
//		//if no button is pressed 
//		else	if (menu2 == 0x00 && menu1 != 0) {
//			sprintf(unit,"%s",suffix); // Adds no units
//			GPIOD->ODR |= (menu2 << 8) << 16; // Turns light off
//		}
//		// Turns on datalogger
//		else	if (menu2 == 0x80 && menu1 == 0) {
//			dataLogCount = 0; // Starts from first value in array
//			LCD_Clear();
//			Delay(100);
//			LCD_PutS(dataLog); // Displays value on LCD

//		}
//		
//		else {
//			sprintf(unit,"%s",suffix);
//			menu2 = 0x00;
//			GPIOD->ODR |= (menu2 << 8) << 16; // Turns light off
//		}
//		
//		
//		sprintf(display,"%.2f%s", calcVal, unit); //Outputs dp
//		LCD_PutS(display); //Outputs to LCD
//		
//		// Data Logging functionality
//		dataLogTimer = dataLogTimer + 1; // Increments timer every 100 ms
//		
//		// If menu1 is set the datalogger works
//		if (dataLogTimer == 50 && menu1 != 0){ // data logs every 5 seconds
//			sprintf(dataLog,"%s",display); // Adds display to data logger
//			dataLogCount++; // increments through 10 values
//			dataLogTimer = 0; // resets timer

//		}

//		
//		//Delay(100);
//		LCD_Clear();
  
//	}

}
