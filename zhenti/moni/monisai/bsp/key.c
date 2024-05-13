#include "key.h"
#include "led.h"

struct key keys[4]={0,0,0,0};

//定义外部变量
extern uint8_t standard_37,standard_38;
extern int tim_count;
extern TIM_HandleTypeDef htim3;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	//用于按键消抖
	if(htim->Instance==TIM2)
	{
		keys[0].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		for(unsigned char i=0;i<4;i++)
		{
			switch(keys[i].key_process)
			{
				case 0:
					{
						if(keys[i].key_state==0)
							keys[i].key_process=1;
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
							
						}
						keys[i].key_process=0;
					}
					break;
				}
			}
	}

	
		
	
	
}