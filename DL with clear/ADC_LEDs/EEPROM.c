#include "STM32F4xx.h"

/*----------------------------------------------------------------------------
  initialize I2C Pins
 *----------------------------------------------------------------------------*/
void I2C3_Int (void) {
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;         /* Enable GPIOA clock                */
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;         /* Enable GPIOC clock                */
	GPIOA->AFR[2] |= 0x00000004; // select AF4 (I2C) for PA8 -> I2C3 
	GPIOC->AFR[2] |= 0x00000040; // select AF4 (I2C) for PC9 -> I2C3 
	GPIOA->MODER |= 0x00020000; // PA8 => alternate functions 
	GPIOC->MODER |= 0x00080000; // PC9 => alternate functions 
	GPIOA->OTYPER |= 0x0100; // use open-drain output on these pins! 
	GPIOC->OTYPER |= 0x0200; // use open-drain output on these pins! 
	
	I2C3->CR2 |= 0x0008; // clock == 8MHz!
	I2C3->CCR |= 0x0040; // clock control register (270kHz)
	I2C3->TRISE |= 0x0009; // rise time register
	I2C3->CR1 |= 0x0001; // I2C3 enable 
	
}

void I2C3_WriteChar (char Adr, char Dat) {
	I2C3->CR1 |= 0x0100; // send START bit
	while (!(I2C3->SR1 & 0x0001)) {}; // wait for START condition (SB=1)
	I2C3->DR = 0xd0; // slave address -> DR & write
	while (!(I2C3->SR1 & 0x0002)) {}; // wait for ADDRESS sent (ADDR=1)
	int Status2 = I2C3->SR2; // read status to clear flag
	I2C3->DR = Adr; // Address in chip -> DR & write
	while (!(I2C3->SR1 & 0x0080)) {}; // wait for DR empty (TxE)
	I2C3->DR = Dat; // Dat -> DR & write
	while (!(I2C3->SR1 & 0x0080)) {}; // wait for DR empty (TxE)
	while (!(I2C3->SR1 & 0x0004)) {}; // wait for Byte sent (BTF)
	I2C3->CR1 |= 0x0200; // send STOP bit
} 

char I2C3_ReadChar (char Adr) { // procedure: RM0090, pg. 584!

	I2C3->CR1 |= 0x0100; // send START bit
	while (!(I2C3->SR1 & 0x0001)) {}; // wait for START condition (SB=1)
	I2C3->DR = 0xd0; // slave address -> DR (LSB=1)
	while (!(I2C3->SR1 & 0x0002)) {}; // wait for ADDRESS sent (ADDR=1)
	int Status2 = I2C3->SR2; // read SR2 to clear flag
	I2C3->DR = Adr; // register in chip -> DR
	while (!(I2C3->SR1 & 0x0080)) {}; // wait for DR empty (TxE=1)
	while (!(I2C3->SR1 & 0x0004)) {}; // wait for ByteTransferred (BTF=1)

	I2C3->CR1 |= 0x0100; // send START bit
	while (!(I2C3->SR1 & 0x0001)) {}; // wait for START condition (SB=1)
	I2C3->DR = 0xd1; // slave address -> DR (LSB=0)
	while (!(I2C3->SR1 & 0x0002)) {}; // wait for ADDRESS sent (ADDR=1)
	int Status4 = I2C3->SR2; // read status to clear flag
	while (!(I2C3->SR1 & 0x0040)) {}; // wait for ByteReceived (RxNE=1)
	I2C3->CR1 |= 0x0200; // send STOP bit
	return ((char)I2C3->DR); // return byte
} 