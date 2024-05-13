#include "interrupt.h"

struct keys key[4]={0,0,0,0};
//�жϻص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
	 //��ȡ��ǰ�ĸ������ĸߵ͵�ƽ���
	 key[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	 key[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	 key[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	 key[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		//�����ж��ĸ����������
		for (int i=0;i<4;i++)
		{
			//���ݰ����������������ֱ��жϲ����м�¼
			
			key[i].long_flag=0;
			key[i].single_flag=0;
			switch(key[i].judge_sta)
			{
				case 0 :
				{
					if(key[i].key_sta==0)
					{
						key[i].judge_sta=1;
					}
				}
				break;
				
				case 1 :
				{
					if(key[i].key_sta==0)
					{
						key[i].key_time++;
						key[i].judge_sta=2;
					}
					
					else
						key[i].judge_sta=0;
				}
				break;
				
				case 2 :
				{
					if(key[i].key_sta==1)
					{
						if(key[i].key_time>50)
						{
							key[i].long_flag=1;
							key[i].key_time=0;
						}
						else
						{
							key[i].single_flag=1;
							
						}
						key[i].judge_sta=0;
					}
					
					else
					{
						key[i].key_time++;
					}
				}
				break;
			}
		}	
	}
}




//���벶���ж�
uint ccrl_val1_a=0,ccrl_val2_a=0,ccrl_val1_b=0,ccrl_val2_b=0;
uint frq1=0,frq2=0;
uint duty1=0,duty2=0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		//��ȡ��ʱֵ
		ccrl_val1_a=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		ccrl_val1_b=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		//��ʱֵ����
		__HAL_TIM_SetCounter(htim,0);
		//����ʱ��Ƶ��
		frq1=(80000000/80)/ccrl_val1_a;
		duty1=(ccrl_val1_b/ccrl_val1_a)*100;
		//���¿�����ʱ��  ---Ϊʲô
		HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);
		HAL_TIM_IC_Start(htim,TIM_CHANNEL_2);
	}
	
	if(htim->Instance==TIM3)
	{
		//��ȡ��ʱֵ
		ccrl_val2_a=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		ccrl_val2_b=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		//��ʱֵ����
		__HAL_TIM_SetCounter(htim,0);
		//����ʱ��Ƶ��
		frq2=(80000000/80)/ccrl_val2_a;
		duty2=(ccrl_val2_b/ccrl_val2_a)*100;
		//���¿�����ʱ��  ---Ϊʲô
		HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);
		HAL_TIM_IC_Start(htim,TIM_CHANNEL_2);
	}
}
