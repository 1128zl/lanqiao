#ifndef _MYFILE_H_
#define _MYFILE_H_

#include "main.h"

void Add_Value_Up(float value);
void Add_Value_Down(float value);
void Sub_Value_Up(float value);
void Sub_Value_Down(float value);
void Key_Proc();
void sys_Init();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Usart_Function();
void sys_work();
#endif