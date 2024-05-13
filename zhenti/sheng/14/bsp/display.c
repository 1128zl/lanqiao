#include "display.h"
#include "stdio.h"
#include "lcd.h"
#include "key.h"
#include "myfile.h"

//�����ⲿ����
extern uint8_t menu,mode,r,k,n,status,led2_sta;
extern float speed_high_max,speed_low_max,speed_l,speed_h,duty;
extern uint16_t freq_l,freq_h,freq;
extern struct key keys[4];
/***************************************LED��ʾ����*******************************************/
//��������LED�Ƶ�����
void Control_All_Led()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

//���Ƶ���LED�Ƶ�����
void Control_Single_Led(unsigned char location,unsigned char state)
{
	 HAL_GPIO_WritePin(GPIOC, (location<<8), state);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

}

//Ϊ�˱���LED��LCD��ʾ��ͻ���еĴ���
void Invoid_Complict()
{
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}


void Display_Led()
{
	Invoid_Complict();
	
	//LED1
	if(menu%3==0)
		Control_Single_Led(0x01,0);
	else
		Control_Single_Led(0x01,1);
		
	//LED2
		if(freq==4000 || freq==8000)
		{
			
			Control_Single_Led(0x02,1);
			//�򿪶�ʱ��	
		}
		else 
		{
			led2_sta^=1;
			Control_Single_Led(0x02,led2_sta);
		}
	//LED3
	if(status)
		Control_Single_Led(0x04,1);
	else
		Control_Single_Led(0x04,0);
}
/***************************************LCD��ʾ����*******************************************/

void Display_Menu()
{
	char str[30];
	
	//���ݽ�����ʾ
	if(menu%3==0)
	{
		sprintf(str,"        DATA        ");
		LCD_DisplayStringLine(Line1,(u8*)str);
		if(mode==0)
		{
			
			sprintf(str,"     M=L           ");
			LCD_DisplayStringLine(Line3,(u8*)str);
			speed_l=(freq_l*2*3.14*r)/(100*k);
			sprintf(str,"     V=%.1f          ",speed_l);
			LCD_DisplayStringLine(Line5,(u8*)str);
		}
		else
		{
			sprintf(str,"     M=H           ");
			LCD_DisplayStringLine(Line3,(u8*)str);
			speed_h=(freq_h*2*3.14*r)/(100*k);
			sprintf(str,"     V=%.1f          ",speed_h);
			LCD_DisplayStringLine(Line5,(u8*)str);
		}
		
		sprintf(str,"     P=%.1f%%          ",duty);
		LCD_DisplayStringLine(Line4,(u8*)str);
		
		sprintf(str,"freq=%d  ",freq);
		LCD_DisplayStringLine(Line8,(u8*)str);
	}
	
	//����������ʾ
	else if(menu%3==1)
	{
		sprintf(str,"        PARA        ");
		LCD_DisplayStringLine(Line1,(u8*)str);
		sprintf(str,"     R=%d            ",r);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"     K=%d            ",k);
		LCD_DisplayStringLine(Line4,(u8*)str);
		sprintf(str,"                     ");
		LCD_DisplayStringLine(Line5,(u8*)str);
	}
	
	//ͳ�ƽ�����ʾ
	else 
	{
		sprintf(str,"        RECD        ");
		LCD_DisplayStringLine(Line1,(u8*)str);
		sprintf(str,"     N=%d           ",n);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"     MH=%.1f             ",speed_high_max);
		LCD_DisplayStringLine(Line4,(u8*)str);
		sprintf(str,"     ML=%.1f             ",speed_low_max);
		LCD_DisplayStringLine(Line5,(u8*)str);
	}
}