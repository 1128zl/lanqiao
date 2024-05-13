#include "lcd.h"
#include "led.h"
#include "stdio.h"
#include "key.h"

//�����ⲿ����
extern TIM_HandleTypeDef htim2,htim3,htim4;
extern int8_t code_1,code_2,code_3;
extern uint8_t code_one,code_two,code_thr;
extern uint8_t duty;
extern uint16_t freq,count;
extern struct key keys[4];
extern uint8_t menu,error_counts,is_correct;
extern unsigned char led2_sta;
extern UART_HandleTypeDef huart1;

//�������
char st[30];
char RXbuff[30],TXbuff[30];	//���ڽ��շ��ͻ�����
unsigned char Rxdat,BufIndex=0;	

// �������������ʾ
void Disp_Code_Enter()
{
	sprintf(st,"       PSD          ");
	LCD_DisplayStringLine(Line1,(u8*)st);
	if(code_1==-1)
	{
		sprintf(st,"    B1:@            ");
		LCD_DisplayStringLine(Line3,(u8*)st);
	}
	else
	{
		sprintf(st,"    B1:%d             ",code_1);
		LCD_DisplayStringLine(Line3,(u8*)st);
	}
	
	if(code_2==-1)
	{
		sprintf(st,"    B2:@            ");
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	else
	{
		sprintf(st,"    B2:%d             ",code_2);
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	
	if(code_3==-1)
	{
		sprintf(st,"    B3:@            ");
		LCD_DisplayStringLine(Line5,(u8*)st);
	}
	else
	{
		sprintf(st,"    B3:%d             ",code_3);
		LCD_DisplayStringLine(Line5,(u8*)st);
	}
}

// ����������ʾ
void Disp_Para()
{
	
	sprintf(st,"       STA          ");
	LCD_DisplayStringLine(Line1,(u8*)st);
	sprintf(st,"    F:%dHz             ",freq);
	LCD_DisplayStringLine(Line3,(u8*)st);
	sprintf(st,"    D:%d%%           ",duty);
	LCD_DisplayStringLine(Line4,(u8*)st);
	sprintf(st,"                    ");
	LCD_DisplayStringLine(Line5,(u8*)st);
}
//ϵͳ��ʼ������
void sys_init()
{
	//lcd��ʼ��
	LCD_Init();
	LCD_Clear(Black);
  LCD_SetTextColor(White);
  LCD_SetBackColor(Black);
	
	//ʹ��PWM���
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	
	//�򿪶�ʱ��TIM2
	HAL_TIM_Base_Start_IT(&htim4);
	
	//�򿪴��ڽ���
	HAL_UART_Receive_IT(&huart1, &Rxdat, 1);
	//��ʼ״̬�Ľ���
	Disp_Code_Enter();
	//LED��ʼ��
	Contro_Led(1);
}

//LCD�ۺ�ʹ�ú���
void Display_lcd(uint8_t menu)
{
		if(menu==0)
		Disp_Code_Enter();
	else
		Disp_Para();
}

void Display_led()
{
	if(is_correct==1)
	{
		
		//LED1��5���Ϩ��
		count=0;
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		Contro_One_Led(0x01,0);
		HAL_TIM_Base_Start_IT(&htim3);
	
	}
	
	else if(is_correct==0 && error_counts>=3)
	{
		//	LED2ÿ��0.1���л����𣬱���5����
		count=0;
		led2_sta=1;
		
		//����ͬ���ǽ��LCD��LED��ʾ��ͻ��һ��С����
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		Contro_One_Led(0x02,led2_sta);
		
		//������򿪶�ʱ��3������ʱ���������
		HAL_TIM_Base_Start_IT(&htim3);
	}

}
/************************����ģ��****************************/
//���ڻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RXbuff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1, &Rxdat, 1);
	}

}
//���ڽ������ݴ�����
void GetData_uart()
{		
		//�ж��Ƿ��յ�����
		if(BufIndex<=6)
			return ;
		
		//�жϽ��յ������Ƿ����Ҫ��
		if(RXbuff[0]-48==code_one && RXbuff[1]-48==code_two && RXbuff[2]-48==code_thr && RXbuff[3]=='-')
		{
			if(RXbuff[4]-48==code_one && RXbuff[5]-48==code_two && RXbuff[6]-48==code_thr)
				return ;
			else
				{
					code_one=RXbuff[4]-48;
					code_two=RXbuff[5]-48;
					code_thr=RXbuff[6]-48;
					BufIndex=0;
					sprintf(TXbuff,"success\r\n");
					HAL_UART_Transmit_IT(&huart1, (u8*)TXbuff, sizeof(TXbuff));
				}
		}
		else
			return ;
}
void Key_Function()
{
	if(keys[0].key_flg==1)
	{
		//�������һλ���и�����ֵ
		code_1++;
		if(code_1==10)
			code_1=0;
		
		keys[0].key_flg=0;
	}
	if(keys[1].key_flg==1)
	{
		//������ڶ�λ���и�����ֵ
		code_2++;
		if(code_2==10)
			code_2=0;
		
		keys[1].key_flg=0;
	}

	if(keys[2].key_flg==1)
	{
		//���������λ���и�����ֵ
		code_3++;
		if(code_3==10)
			code_3=0;
		
		keys[2].key_flg=0;
	}
	
	if(keys[3].key_flg==1)
	{
		//����������ȷ
		if(code_1==code_one && code_2==code_two && code_3==code_thr)
			{
				is_correct=1;
				error_counts=0;
				menu=1;
				
				//�޸�pwm���Ƶ�ʣ�ռ�ձ�
				htim2.Init.Prescaler = 40-1;
				htim2.Init.Period = 999;
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,100);
				duty=10;
				freq=2000;
				HAL_TIM_Base_Start_IT(&htim3);
				Display_led();
			}
		
		//�����������
		else
		{
			is_correct=0;
			menu=0;
			error_counts++;
			code_1=code_2=code_3=-1;
			Display_led();
		}
		
		keys[3].key_flg=0;
	}
}

//���������ܺ���
void sys_work()
{
	//���ڽ��մ���
	GetData_uart();

	Display_lcd(menu);
	Key_Function();
}