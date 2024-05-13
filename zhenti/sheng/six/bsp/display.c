#include "display.h"
#include "stdio.h"
#include "lcd.h"
#include "i2c_hal.h"


extern uint8_t led1_sta,is_open,h,m,s,menu;
extern float k,volt;
extern TIM_HandleTypeDef htim2,htim3;

/*********************LEDģ��**************************************/

//Ϩ������LED��
void Control_All_Led()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

}

//���Ƶ���LED��
void Control_Single_Led(unsigned char location,unsigned char state)
{
	HAL_GPIO_WritePin(GPIOC, location<<8,state);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

}

//����LED�Ʊ�����LCD��ʾ��ͻ
void Invoid_Complict()
{
	HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

/************************************END*******************************/











/*******************************LCDģ��*********************************/
//lcd �Զ���ϵͳ������ʾ
void Display_sys_lcd()
{
	char str[30];
	sprintf(str,"    V1:%.2fV        ",volt);
	LCD_DisplayStringLine(Line2,(u8*)str);
	sprintf(str,"    k:%.1f        ",k);
	LCD_DisplayStringLine(Line3,(u8*)str);
	if(is_open)
	{
		sprintf(str,"    LED:ON          ");
	}
	else
		sprintf(str,"    LED:OFF          ");
	LCD_DisplayStringLine(Line4,(u8*)str);
	sprintf(str,"    T:%d-%d-%d        ",h,m,s);
	LCD_DisplayStringLine(Line5,(u8*)str);
}

//LCD��ʾʱ�����ý���
void Display_time()
{
	char str[30];
	LCD_ClearLine(Line2);
	LCD_ClearLine(Line4);
	sprintf(str,"        Setting       ");
	LCD_DisplayStringLine(Line3,(u8*)str);
	sprintf(str,"      %d-%d-%d        ",h,m,s);
	LCD_DisplayStringLine(Line5,(u8*)str);

}



void Display_led()
{
	Invoid_Complict();
		
	//ָʾ����˸����
	if( volt>(3.3*k))
	{
		led1_sta=0;
		Control_Single_Led(0x01,led1_sta);
		HAL_TIM_Base_Start_IT(&htim3);
	}
	if(is_open)
	{
		Control_Single_Led(0x02,0);
	}
	else 
		Control_Single_Led(0x02,1);
}

void Display_lcd()
{
	if(menu%2==0)
	{
		Display_sys_lcd();
	}
	else
		Display_time();
}