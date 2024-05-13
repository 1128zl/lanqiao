#ifndef _MYFILE_H_
#define	_MYFILE_H_

#include "main.h"

void sys_init();
void Disp_Code_Enter();
void Disp_Para();
void Display_lcd(uint8_t menu);
void Display_led();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void GetData_uart();
void sys_work();

#endif