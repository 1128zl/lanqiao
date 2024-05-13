#include "key.h"
#include "stm32g4xx.h"                  // Device header
#include "tim.h"


struct key keys[4]={0,0,0,0};
extern int plats[4];
extern unsigned int plat_flag;
extern unsigned int platform;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance==TIM3)
	{
		platform++;
	}
	HAL_TIM_Base_Start(&htim3);
//	
//	if(htim->Instance==TIM4)
//	{
//	
//			keys[0].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
//			keys[1].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
//			keys[2].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
//			keys[3].state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
//				
//		 for(int i=0;i<4;i++)
//		 {
//				switch(keys[i].step)
//				{
//					case 0:
//						{
//							if(keys[i].state==0)
//								keys[i].step=1;
//						
//						}
//						break;
//						
//				 case 1:
//						{
//							if(keys[i].state==0)
//								keys[i].step=2;
//							else
//								keys[i].step=0;
//						
//						}
//						break;
//					
//					case 2:
//						{
//							if(keys[i].state==1)
//								{
//									keys[i].flag=1;
//									keys[i].step=0;
//									if((i+1)!=plat_flag)
//										plats[i]=1;
//								}
//										
//										}
//								}
//						break;
//						}
//						
//				
//				}
//		 
//		 
//	 
	 

}


void plat_oper(unsigned int plat)
{
	int floor;
	for(int i=plat;i<4;i++)
		{
			//有要上行的目标平台
			if(plats[i]==1)
				{
					floor=i+1;
				}
		}
		plat=floor;
	for(int i=plat-1;i>=0;i--)
		{
			//有要下行的目标平台
			if(plats[i]==1)
			{
			
			
			}
			
		}



}