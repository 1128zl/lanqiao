#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

//定义结构体记录四个按键的状态
struct Key {

	int key_state;//按键对应的引脚的电平情况
	int key_flag;//按键被按下的标志	1--被按下 0--未被按下
	int key_process;//记录此时处于判断按键是否按下的哪个过程
};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Key_Function();
#endif