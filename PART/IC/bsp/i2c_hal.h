#ifndef __I2C_HAL_H
#define __I2C_HAL_H

#include "stm32g4xx_hal.h"
#define uint unsigned int
#define uchar unsigned char
void I2CStart(void);
void I2CStop(void);
uchar I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(uchar cSendByte);
uchar I2CReceiveByte(void);
void I2CInit(void);
void eeprom_write(uchar addr,uchar dat);
uchar eeprom_read(uchar addr);

#endif
