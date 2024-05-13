#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"
#include "stdbool.h"
#define uchar unsigned char
#define uint unsigned int

struct key_infrom{
		uchar key_state;	//按键此刻是否是被按下
		bool key_flag;		//确定按键是被按下的标志（经过了消抖）
		uint key_tip;			//用于决定进入判断过程的哪一步
};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Key_Contr(uint code,uint view_chart,uint mode);
#endif
