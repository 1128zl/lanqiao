#include "key.h"
#include "led.h"

struct key keys[4]={0,0,0,0};

extern uint16_t count,freq;
extern uint8_t is_correct,error_counts,duty;
extern TIM_HandleTypeDef htim2,htim3;
extern unsigned char led2_sta;

/**************************定时器回调函数******************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
		keys[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		for(unsigned char i=0;i<4;i++)
		{
			switch(keys[i].key_pro)
			{
				case 0:
					{
						if(keys[i].key_sta==0)
							keys[i].key_pro=1;
					}
					break;
				
				case 1:
					{
						if(keys[i].key_sta==0)
							keys[i].key_pro=2;
						else
							keys[i].key_pro=0;
					}
					break;
				
				case 2:
					{
						if(keys[i].key_sta==1)
							{
								keys[i].key_flg=1;
								keys[i].key_pro=0;
							}
						
					}
					break;	
			}
		}
	}
	
	if(htim->Instance==TIM3)
	{
		count++;
		//密码输入正确，LED1点亮5s后熄灭
		//改变PWM的输出频率，占空比
		if(is_correct==1 && count==50)
			{
				HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				Contro_One_Led(0x01,1);
				htim2.Init.Prescaler = 80-1;
				htim2.Init.Period = 999;
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,500);
				duty=50;
				freq=1000;
				HAL_TIM_Base_Start_IT(&htim3);
				HAL_TIM_Base_Stop(&htim3);
			}
			
		//密码输入错误次数到三次以上，LED2在5s内每隔0.1s切换状态
		if(is_correct==0 && error_counts>=3)
		{
			if(count!=50)
			{
				//这里为了避免LED和LCD发生冲突，所以在LED亮灭前进行了处理
				//前面的更新文章里面有提到，需要的话可以去看看
				HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				led2_sta=led2_sta^1;
				Contro_One_Led(0x02,led2_sta);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				Contro_One_Led(0x02,1);
				
				//这里在5s之后关闭定时器，以防后面计时错误
				HAL_TIM_Base_Stop(&htim3);
			}
		}
	}
}