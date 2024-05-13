#include "interrupt.h"

struct keys key[4]={0,0,0,0};
//中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 //获取当前四个按键的高低电平情况
	 key[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	 key[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	 key[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	 key[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	//依次判断四个按键的情况
	for (int i=0;i<4;i++)
	{
		//根据按键的三个过程来分别判断并进行记录
		
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
					if(key[i].key_time>70)
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




