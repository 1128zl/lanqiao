#include "key.h"
#include "myfile.h"
#include "display.h"



struct key keys[4]={0,0,0,0};
int16_t count=0;

//�����ⲿ����
extern uint8_t menu,level_flg,check_flg;
extern uint8_t led1_sta,led2_sta,led3_sta;//LED1��LED2��LED3�ĵ�ƽ״̬
extern uint8_t shrink_count_led2,shrink_count_led3;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		keys[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		for (int i=0;i<4;i++)
		{
			switch(keys[i].key_pro)
			{
				case 0:
					if(keys[i].key_sta==0)
						keys[i].key_pro=1;
					break;
				
				case 1:
					if(keys[i].key_sta==0)
						keys[i].key_pro=2;
					else
						keys[i].key_pro=0;
					break;
				
				case 2:
					if(keys[i].key_sta==1)
						{
							keys[i].key_pro=0;
							keys[i].key_flg=1;
						}
					break;
			}
		}
	}


	if(htim->Instance==TIM3)
	{
		Invoid_Complict(); 
		count++;
		
		//���һ�����R37�����ѹ
		if(count%5==0)
		{
			Get_adc();
			level_confirm();
		}
		
		//����״̬�¼��һ��LED1������˸
		if(count%5==0 && menu%2==0)
		{
			led1_sta^=1;
			Control_Single_Led(0x01,led1_sta);
		}
		else if(menu%2)
		{
			Control_Single_Led(0x01,1);
			led1_sta=1;
		}
		
		//Һλ�ȼ��仯��LED2���0.2����˸5��
		if(level_flg==1)
		{
			if(shrink_count_led2==10)
				{
					Control_Single_Led(0x02,1);
					led2_sta=1;
					level_flg=0;
					
					shrink_count_led2=0;
				}
			else
			{
				led2_sta^=1;
				Control_Single_Led(0x02,led2_sta);
				shrink_count_led2++;
			}
		}
		
		
		//ͨѶ״̬��LED3���0.2����˸5��
		if(check_flg==1)
		{
			if(shrink_count_led3==10)
				{
					Control_Single_Led(0x04,1);
					led3_sta=1;
					check_flg=0;
					shrink_count_led3=0;
				}
			else
			{
				led3_sta^=1;
				Control_Single_Led(0x04,led3_sta);
				shrink_count_led3++;
			}
		}
	}
}