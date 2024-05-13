#ifndef _KEY_H_
#define _KEY_H_


#include "main.h"
#include "stdbool.h"
//按键结构体
struct key{
	unsigned int state;		//当前按键的电平情况
	bool flag;		//按键是否被按下	0--未按下		1--被按下
	unsigned int step;	//判断当前处于判断按键被按下的哪一步
};
//按键回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Lcd_Disp(unsigned signal);
#endif