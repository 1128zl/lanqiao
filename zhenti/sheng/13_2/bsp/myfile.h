#ifndef _MYFILE_H_
#define _MYFILE_H_

#include "main.h"

void Display_Lcd();
void Key_Function();
void Eeprom_Init_Information();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Receive_Deal();
void Sys_init();
void Sys_work();

#endif