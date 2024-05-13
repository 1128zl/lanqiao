#include "key.h"

struct key keys[4]={0,0,0,0};
extern unsigned int signal1;
extern unsigned int signal2;
extern unsigned int plat_flag;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
	
			keys[0].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
			keys[1].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
			keys[2].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
			keys[3].state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
				
		 for(int i=0;i<4;i++)
		 {
				switch(keys[i].step)
				{
					case 0:
						{
							if(keys[i].state==0)
								keys[i].step=1;
						
						}
						break;
						
				 case 1:
						{
							if(keys[i].state==0)
								keys[i].step=2;
							else
								keys[i].step=0;
						
						}
						break;
					
					case 2:
						{
							if(keys[i].state==1)
								{
									keys[i].flag=1;
									keys[i].step=0;
									signal2=i+1;
									if(signal2-signal1>0)
												plat_flag=0;
									else
												plat_flag=1;
									signal1=i+1;
								}
										
										}
								}
						break;
						}
						
				
				}
		 
		 
	 
	 

}


//根据按下的不同建采取不同的操作
void Lcd_Disp(unsigned signal)
{
	switch(signal)
	{
		case 1:
			{
			
			
			}
		break;
	
		case 2:
			{
			
			
			}
		break;
		
		case 3:
			{
			
			
			}
		break;
		
		case 4:
			{
			
			
			}
		break;
	}

}