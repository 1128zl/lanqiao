#include "led.h"

/*******************************************************************************
* 函数名  : Led_Display
* 功能    : 两种运行模式分别对应的LED亮灭情况
* 输入    : mode---当前运行模式 0---自动模式	1---手动模式	
				  :view_chart---当前显示界面 0---数据界面	1---参数界面
* 返回值  : 无
*******************************************************************************/



void Led_Display (unsigned int mode,unsigned int view_chart)
{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
		if(mode==0)
		{
			HAL_GPIO_WritePin(GPIOC,0x01<<8,GPIO_PIN_RESET);	
		}
		else
			HAL_GPIO_WritePin(GPIOC,0x01<<8,GPIO_PIN_SET);
		if(view_chart==0)
			HAL_GPIO_WritePin(GPIOC,0x02<<8,GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(GPIOC,0x02<<8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

	
	