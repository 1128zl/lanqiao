#ifndef _MYFILE_H_
#define _MYFILE_H_

#include "main.h"

void sys_init();
void sys_work();
void Key_Function();
float Get_ADC();
void Change_Freq();
void Freq_IC();
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void Get_Speed_Max();
#endif