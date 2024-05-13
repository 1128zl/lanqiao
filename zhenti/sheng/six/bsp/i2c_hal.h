#ifndef __I2C_HAL_H
#define __I2C_HAL_H

#include "stm32g4xx_hal.h"

void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void Eeprom_Write_Bytes(uint8_t addr,uint8_t* arry ,uint8_t n);
void Eeprom_Read_Bytes(uint8_t addr,uint8_t* arry ,uint8_t n);
void I2CInit(void);

#endif
