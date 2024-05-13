#include "myfile.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "main.h"
#include "stdio.h"
#include "adc.h"
#include "hadc.h"
#include "tim.h"

extern uint menu_mode;
extern uint signal;
extern uint view_chart;
extern char mode;
extern uint pa6_duty,pa7_duty ;
char text1[30];

extern float volt;

//数据界面显示
void data_menu_show(double v,uint menu_mode)
{
	//当前模式，当前显示界面对应的LED显示
	Led_Display(menu_mode,view_chart);
	//显示数据界面
	LCD_DisplayStringLine(Line0 ,(unsigned char *)"      Data          ");
	sprintf(text1,"    V:%.2fV         ",v);
	LCD_DisplayStringLine(Line2 ,(unsigned char *)text1);
	if(menu_mode)
		LCD_DisplayStringLine(Line4 ,(unsigned char *)"    Mode:MXNU       ");
	else
		LCD_DisplayStringLine(Line4 ,(unsigned char *)"    Mode:AUTO       ");
}
//参数界面显示
void para_menu_show(uint pa6_duty,uint pa7_duty)
{
	
	LCD_DisplayStringLine(Line0 ,(unsigned char *)"      Para          ");
	sprintf(text1,"    PA6:%d%%         ",pa6_duty);
	LCD_DisplayStringLine(Line2 ,(unsigned char *)text1);
		sprintf(text1,"    PA7:%d%%         ",pa7_duty);
	LCD_DisplayStringLine(Line4 ,(unsigned char *)text1);
}

//自动模式下，占空比的设置
void changePwmFrd_Auto(void)
{
	//自动模式 电压值为0V
	if(menu_mode%2 == 0 && volt == 0)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,999);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,999);
	}
	//自动模式  电压值为3.3V 
	else if(menu_mode%2 == 0 && volt >= 3.3)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
	}
}

//LCD显示，LED显示
void Display()
{
	if(view_chart%2==0)
		data_menu_show(volt,menu_mode);
	else
		para_menu_show(pa6_duty,pa7_duty);
	Led_Display (menu_mode,view_chart);

}

void Work()

{
	volt=getADC()*3.3/4096;
	Key_Contr(signal,view_chart,menu_mode);
	changePwmFrd_Auto();
	Display();
	
}
		