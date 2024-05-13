#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "main.h"
#include "stdbool.h"
struct keys
{
	uchar judge_sta;//判断按键按下的动作进行到了哪一步
	bool key_sta;	//如果按键被按下，为0
	bool single_flag;//单击情况--如果确认被按下，为1
	uint key_time;	//按键持续按下的时间
	bool double_key_flag;
	bool long_flag;	//长按响应标志
	uint pwm_duty;
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
#endif