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

//���ݽ�����ʾ
void data_menu_show(double v,uint menu_mode)
{
	//��ǰģʽ����ǰ��ʾ�����Ӧ��LED��ʾ
	Led_Display(menu_mode,view_chart);
	//��ʾ���ݽ���
	LCD_DisplayStringLine(Line0 ,(unsigned char *)"      Data          ");
	sprintf(text1,"    V:%.2fV         ",v);
	LCD_DisplayStringLine(Line2 ,(unsigned char *)text1);
	if(menu_mode)
		LCD_DisplayStringLine(Line4 ,(unsigned char *)"    Mode:MXNU       ");
	else
		LCD_DisplayStringLine(Line4 ,(unsigned char *)"    Mode:AUTO       ");
}
//����������ʾ
void para_menu_show(uint pa6_duty,uint pa7_duty)
{
	
	LCD_DisplayStringLine(Line0 ,(unsigned char *)"      Para          ");
	sprintf(text1,"    PA6:%d%%         ",pa6_duty);
	LCD_DisplayStringLine(Line2 ,(unsigned char *)text1);
		sprintf(text1,"    PA7:%d%%         ",pa7_duty);
	LCD_DisplayStringLine(Line4 ,(unsigned char *)text1);
}

//�Զ�ģʽ�£�ռ�ձȵ�����
void changePwmFrd_Auto(void)
{
	//�Զ�ģʽ ��ѹֵΪ0V
	if(menu_mode%2 == 0 && volt == 0)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,999);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,999);
	}
	//�Զ�ģʽ  ��ѹֵΪ3.3V 
	else if(menu_mode%2 == 0 && volt >= 3.3)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
	}
}

//LCD��ʾ��LED��ʾ
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
		