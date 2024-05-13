#include "myfile.h"
#include "lcd.h"
#include "key.h"
#include "stdio.h"

extern TIM_HandleTypeDef htim2,htim3,htim4,htim5,htim6,htim7,htim16,htim17;
extern struct key keys[4];

int8_t platforms[20];	//�趨��Ŀ��ƽ̨
uint8_t platindex=0;
int8_t isup_or_down=0;
uint8_t plat_now=1;		//��ǰ����ƽ̨
uint8_t plat_future;	//��һ��Ҫ�����ƽ̨
uint8_t plat_pro=1;	//��ǰ������״̬��Ŀ��ƽ̨�趨�����ţ����У�����
void Key_Function()
{
	if(plat_pro%4==0)
	{
		if(keys[0].key_flg==1)
		{
			if(plat_now!=1)
			{
				platforms[platindex++]=1;
			}
			keys[0].key_flg=0;
		}
		
		if(keys[1].key_flg==1)
		{
			if(plat_now!=2)
			{
				platforms[platindex++]=2;
			}
			keys[1].key_flg=0;
		}
		
		if(keys[2].key_flg==1)
		{
			if(plat_now!=3)
			{
				platforms[platindex++]=3;
			}
			keys[2].key_flg=0;
		}
		
		if(keys[3].key_flg==1)
		{
			if(plat_now!=4)
			{
				platforms[platindex++]=4;
			}
			keys[3].key_flg=0;
		}
		HAL_TIM_Base_Start_IT(&htim3);
	}
}
void Up_or_Down()
{
	uint8_t i;
	for(i=0;i<platindex;i++)
	{
		if(platforms[i]>plat_now)
		{
			plat_future=platforms[i];
			platforms[i]=-1;
			isup_or_down=1;		//����
			break;
		}
		else if(platforms[i]!=-1 && platforms[i]<plat_now)
		{
			plat_future=platforms[i];
			isup_or_down=-1;	//�½�
			break;
		}
	}
	
}
void Tench_Operate()
{
	//����������
	if(plat_pro%4==1)
	{
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,500);
		
		HAL_TIM_Base_Start_IT(&htim6);	//����ά������
	}

	//����������
	if(plat_pro%4==2)
	{
		Up_or_Down();
		if(isup_or_down==1)
		{
			__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,800);
			
		}
		else if(isup_or_down==-1)
		{
			__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,600);
		}
		HAL_TIM_Base_Start_IT(&htim4);		//6������е�Ŀ��ƽ̨
	}
	
	//����������
	if(plat_pro%4==3)
	{
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,600);
		HAL_TIM_Base_Start_IT(&htim6);
	}
}
void  sys_init()
{
	//�򿪰��������Ķ�ʱ��
	HAL_TIM_Base_Start_IT(&htim2);
	
	//lcd��ʼ��
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
}

void sys_work()
{
	char str[30];
	sprintf(str,"%d   %d",plat_pro,platindex);
	LCD_DisplayStringLine(Line2,(u8*)str);
	sprintf(str,"     %d   %d",plat_now,plat_future);
	LCD_DisplayStringLine(Line4,(u8*)str);
	//Key_Function();
	Tench_Operate();
}