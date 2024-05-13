#include "display.h"
#include "key.h"
#include "stdio.h"
#include "lcd.h"

//�����ⲿ����
extern float v_max,v_min,v_37;
extern unsigned char led_upper,led_lower,led_sta;
extern uint8_t menu,change_type,upper_led,low_led;
extern TIM_HandleTypeDef htim2,htim3;

//�������

char str[4][30];
char led[8][4]={"LD1","LD2","LD3","LD4","LD5","LD6","LD7","LD8"};	//���ں���LCD����ʾ

/**************************LED************************************/

//ȫ��LED�Ƴ�ʼ��
void Led_Init()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//���Ƶ���LED������
void Control_Single_Led(unsigned char location,unsigned char state)
{
	HAL_GPIO_WritePin(GPIOC,location<<8,state);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//LED������������LCD��ʾ��ͻ
void Invoid_Complict()
{
	HAL_GPIO_WritePin(GPIOC,0xff00, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}


void Display_led()
{
	//Invoid_Complict();
	
	//��������
	if(v_37>v_max)
	{
		led_sta=0;
		Control_Single_Led(led_upper,led_sta);
		HAL_TIM_Base_Start_IT(&htim3);
	}
	
	//��������
	else if(v_37<v_min)
	{
		led_sta=0;
		Control_Single_Led(led_lower,led_sta);
		HAL_TIM_Base_Start_IT(&htim3);
	}
	
	//����ֵ
	else
	{
		Led_Init();
		HAL_TIM_Base_Stop(&htim3);
	}
}
/*******************************LCD************************************/


//������ʾ������ʾ
void Data_Show()
{
		char st[30];
		sprintf(st,"       Main         ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(st,"    Volt:%.2fV         ",v_37);
		LCD_ClearLine(Line2);
		LCD_DisplayStringLine(Line3,(u8*)st);
		LCD_ClearLine(Line4);
		if(v_37>v_max)
			sprintf(st,"    Status:Upper         ");
		else if(v_37<v_min)
			sprintf(st,"    Status:Lower         ");
		else
			sprintf(st,"    Status:Normal         ");
		LCD_DisplayStringLine(Line5,(u8*)st);
}

//������ʾ������ʾ
void Para_Show()
{
		char st[30];
		sprintf(st,"       Setting         ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(str[0],"  Max Volt:%.1fV       ",v_max);
		sprintf(str[1],"  Min Volt:%.1fV       ",v_min);
		sprintf(str[2],"  Upper:%s       ",led[upper_led%8]);
		sprintf(str[3],"  Lower:%s       ",led[low_led]);
		
		//ʵ�ֶ��ڵ�ǰ�����Ĳ����������
		if(change_type%4==0)
			{
				LCD_SetBackColor(Blue);
				LCD_DisplayStringLine(Line2,(u8*)str[0]);
				LCD_SetBackColor(Black);
			}
		else
			LCD_DisplayStringLine(Line2,(u8*)str[0]);
		
		if(change_type%4==1)
			{
				LCD_SetBackColor(Blue);
				LCD_DisplayStringLine(Line3,(u8*)str[1]);
				LCD_SetBackColor(Black);
			}
		else
			LCD_DisplayStringLine(Line3,(u8*)str[1]);
			
		if(change_type%4==2)
			{
				LCD_SetBackColor(Blue);
				LCD_DisplayStringLine(Line4,(u8*)str[2]);
				LCD_SetBackColor(Black);
			}
		else
			LCD_DisplayStringLine(Line4,(u8*)str[2]);
			
		if(change_type%4==3)
			{
				LCD_SetBackColor(Blue);
				LCD_DisplayStringLine(Line5,(u8*)str[3]);
				LCD_SetBackColor(Black);
			}
		else
			LCD_DisplayStringLine(Line5,(u8*)str[3]);
}
void Display_lcd()
{
	if(menu%2==0)
		Data_Show();
	else
		Para_Show();
}