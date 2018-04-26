#ifndef __EEPROM_H
#define __EEPROM_H

extern void I2C3_Int (void);
extern void I2C3_WriteChar (char Adr, char Dat);
extern char I2C3_ReadChar (char Adr);

#endif