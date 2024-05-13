#include "key.h"
#include "led.h"
#include "main.h"
#include "lcd.h"
#include "stdio.h"
//���尴����Ӧ�ı���
struct Key keys[4]={0,0,0,0};

extern int8_t code_1,code_2,code_3,origin_code,code_correct,error_counts;
extern uint8_t menu;
extern void Lcd_Display_State();
extern void Lcd_Display_Code();
extern TIM_HandleTypeDef htim2,htim3,htim4;
extern uint8_t counts;
extern unsigned char sta;

int correct_flag=0;
//��ʱ���ص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//�Ͱ�����صĶ�ʱ��
	if(htim->Instance==TIM4)
		{
			//��ȡ��ǰ�жϰ����Ƿ񱻰��������Ĺ���
			keys[0].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
			keys[1].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
			keys[2].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
			keys[3].key_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
			
			//��һ���ж�
			int i;
			for(i=0;i<4;i++)
			{
//				//�ж�ǰ�����־������������ж�
//				keys[i].key_flag=0;
				switch(keys[i].key_process)
				{
					case 0:
						{
							if(keys[i].key_state==0)
								{
									keys[i].key_process=1;
								}
						}
						break;
						
					case 1:
						{
								if(keys[i].key_state==0)
									keys[i].key_process=2;
								else
									keys[i].key_process=0;
						}
						break;
						
					case 2:
						{
								if(keys[i].key_state==1)
								{
									keys[i].key_flag=1;
									keys[i].key_process=0;
								}
						}
						break;
				
				}
			
			}
		}

	//��ʱ����ʱ������
	if(htim->Instance==TIM3)
	{
			if(code_correct==1)
			{
				Close_All_LED();
				
				code_correct=0;
			}
			
	}
	
//	//��ʱ����ʱ0.1���л�LED2�������
	if(htim->Instance==TIM2)
	{
			if(error_counts>=3)
			{
				counts++;
				sta=sta^1;
				Change_One_LED(0x02,sta);
				if(counts==25)
				{
				  Close_All_LED();
					HAL_TIM_Base_Stop(&htim2);
				}
			}
	}
	
	
}

/*******************************************************************************
* ������  : Key_Function
* ����    : ����ڰ��µļ���ʵ�ֶ�Ӧ�Ĺ���
* ����    : ��
* ����ֵ  : ��
*******************************************************************************/

void Key_Function()
{
	//����B1������һ����������һλ����+1��ÿλ����ķ�Χ��0-9
	if(keys[0].key_flag)
		{
			keys[0].key_flag=0;
			if(menu==0)
			{
			//�޸������һλ
			code_1++;
			if(code_1==10)
				code_1=0;
			}
			
		}
	//����B2�����ڶ����������ڶ�λ����+1��ÿλ����ķ�Χ��0-9
	if(keys[1].key_flag)
		{
			keys[1].key_flag=0;
			
			if(menu==0)
			{
			//�޸�����ڶ�λ
			code_2++;
			if(code_2==10)
				code_2=0;
			}
		
		}
	//����B3��������������������λ����+1��ÿλ����ķ�Χ��0-9
	if(keys[2].key_flag)
		{
			keys[2].key_flag=0;
			if(menu==0)
			{
			//�޸��������λ
			code_3++;
			if(code_3==10)
				code_3=0;
			}
		
		}
	//����B4�������ĸ���������ȷ����������룬������һ�����ж�
	if(keys[3].key_flag)
		{
			keys[3].key_flag=0;
			
			//�ж������Ƿ�������ȷ

			if((code_1*100+code_2*10+code_3) == origin_code)
			{
				//��ʾ���״̬����
				menu=1;
				code_correct=1;
				error_counts=0;
				
			}
			else
			{
				//��¼������������������
				error_counts++;
				code_correct=-1;

					//��ʾ�����������
				code_1=code_2=code_3=-1;
			
			}
			Led_Disp();
		}
}