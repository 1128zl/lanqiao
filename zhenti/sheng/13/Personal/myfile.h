#ifndef _MYFILE_H_
#define _MYFILE_H_

#include "main.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Lcd_Display_State();
void Lcd_Display_Code();
void Get_Data();
void sys_Init();
void sys_work();
#endif