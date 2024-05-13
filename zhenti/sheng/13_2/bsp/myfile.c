#include "main.h"
#include "myfile.h"
#include "lcd.h"
#include "stdio.h"
#include "led.h"
#include "key.h"
#include "i2c_hal.h"

//�����ⲿ����
extern uint8_t menu;										//��ʾ����
extern uint8_t buy_x_count,buy_y_count;	//������Ʒ�Ĺ�������
extern float x_price,y_price;						//��¼������Ʒ�ļ۸�
extern uint8_t stock_x,stock_y;					//��¼������Ʒ�Ŀ����
extern TIM_HandleTypeDef htim4,htim3,htim16;
extern struct key keys[4];
extern uint16_t time_count;
extern uint8_t led2_sta,is_enputy;
extern char RXbuff[30],TXbuff[30];
extern unsigned char Rxdat,BufIndex;
extern UART_HandleTypeDef huart1;

//�������
char st[25];
float pay=0.0;	//����ĸ�����Ʒ���ܽ��
uint8_t is_buy=0;	//��¼�Ƿ���ɹ�

/*********************LCDģ��*****************************/
/************************BEGIN****************************/

void Display_Lcd()
{
	//��Ʒ�������
	if(menu%3==0)
	{
		sprintf(st,"        SHOP        ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(st,"     X:%d            ",buy_x_count);
		LCD_DisplayStringLine(Line3,(u8*)st);
		sprintf(st,"     Y:%d            ",buy_y_count);
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	
	//��Ʒ�۸����
	else if(menu%3==1)
	{
		sprintf(st,"        PRICE       ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(st,"     X:%.1f          ",x_price);
		LCD_DisplayStringLine(Line3,(u8*)st);
		sprintf(st,"     Y:%.1f          ",y_price);
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	
	//�����Ϣ����
	else
	{
		sprintf(st,"        REP         ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(st,"     X:%d            ",stock_x);
		LCD_DisplayStringLine(Line3,(u8*)st);
		sprintf(st,"     Y:%d            ",stock_y);
		LCD_DisplayStringLine(Line4,(u8*)st);
	
	}
}
/************************END****************************/

/************************EEPROM****************************/
/************************BEGIN****************************/

void Eeprom_Init_Information()
{
	
	uint8_t temp[3];
	Eeprom_Read_Bytes(0xa4,temp,3);
	if(temp[0]!=0x44 && temp[1]!=0x55 && temp[2]!=0x66)
	{
		uint8_t inform=10;
		Eeprom_Write_Bytes(0x00,&inform,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0x01,&inform,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0x02,&inform,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0x03,&inform,1);
		HAL_Delay(10);
		stock_x=stock_y=10;
		x_price=y_price=1.0;
	}
}



/************************END****************************/






/*************************����ģ��*************************/
/************************BEGIN****************************/
//����������


void Key_Function()
{
	
	//����B1�л�����
	if(keys[0].key_flg==1)
	{
		menu++;
		keys[0].key_flg=0;
	}
	
	//��ƷX�������Ϣ����
	if(keys[1].key_flg==1)
	{
		//��Ʒ���������
		if(menu%3==0)
		{
			if(buy_x_count!=stock_x)
			{
				buy_x_count++;
			}
			
		}
		
		//��Ʒ�۸������
		else if(menu%3==1)
		{
			if((int)x_price==2)
				x_price=1.0;
			else
				x_price+=0.1;
			uint8_t xp=x_price*10;
			Eeprom_Write_Bytes(0x02,&xp,1);
			HAL_Delay(2);
		}
		
		//�����Ϣ������
		else if(menu%3==2)
		{
			stock_x++;
			
			Eeprom_Write_Bytes(0x00,&stock_x,1);
			HAL_Delay(2);
		}
		
		keys[1].key_flg=0;
	}
	
	//��ƷY�������Ϣ����
	if(keys[2].key_flg==1)
	{
			//��Ʒ���������
		if(menu%3==0)
		{
			if(buy_y_count!=stock_y)
			{
				buy_y_count++;
			}
		}
		
		//��Ʒ�۸������
		else if(menu%3==1)
		{
			if((int)y_price==2)
				y_price=1.0;
			else
				y_price+=0.1;
			uint8_t yp=y_price*10;
			Eeprom_Write_Bytes(0x03,&yp,1);
			HAL_Delay(2);
		}
		
		//�����Ϣ������
		else if(menu%3==2)
		{
			stock_y++;
			Eeprom_Write_Bytes(0x01,&stock_y,1);
			HAL_Delay(2);
		}
		
		keys[2].key_flg=0;
	}
	
	if(keys[3].key_flg==1)
	{
		if(menu%3==0)
		{
			pay=buy_x_count*x_price+buy_y_count*y_price;
			sprintf(TXbuff,"X:%d,Y:%d,Z:%.1f\r\n",buy_x_count,buy_y_count,pay);
			stock_x-=buy_x_count;
			stock_y-=buy_y_count;
			buy_x_count=buy_y_count=0;
			
			Eeprom_Write_Bytes(0x00,&stock_x,1);
			HAL_Delay(2);
			Eeprom_Write_Bytes(0x01,&stock_y,1);
			HAL_Delay(2);
			//���͹���ĸ�����Ʒ�����Լ��ܼ۸�
			
			HAL_UART_Transmit_IT(&huart1, (u8*)TXbuff, sizeof(TXbuff));
			
			//��Ʒ����ɹ���LED1����5s��Ϩ��
			is_buy=1;
			Display_Led();
			keys[3].key_flg=0;
			if(stock_x==0 && stock_y==0)
			{
				is_enputy=1;
				Display_Led();
			}
		}
	}
	
	if(!(stock_x==0 && stock_y==0))
	{
		is_enputy=0;
	}
	
	

}
/************************END****************************/














/********************����ģ��*****************************/
/************************BEGIN****************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RXbuff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1,&Rxdat,1);
	}

}



void Receive_Deal()
{
	if(BufIndex==0)
		return ;
	if(RXbuff[0]=='?')
		{
			sprintf(TXbuff,"X:%.1f,Y:%.1f\r\n",x_price,y_price);
			HAL_UART_Transmit_IT(&huart1, (u8*)TXbuff, sizeof(TXbuff));
			BufIndex=0;
		}

}

/************************END****************************/



/******************�Զ���ϵͳ��ʼ��������д****************************/
/************************BEGIN****************************/

void Sys_init()
{
	
	//LCD��ʼ��
	LCD_Init();
	LCD_Clear(Black);	//���LCD��ʾ
	LCD_SetTextColor(White);	//�����ı�ɫΪ��ɫ
	LCD_SetBackColor(Black);	//���ñ���ɫΪ��ɫ
	//LED��ʼ��
	Contr_All_led();
	
	//I2C��ʼ��
	I2CInit();
	//�򿪶�ʱ��TIM4
	HAL_TIM_Base_Start_IT(&htim4);
	
	//�򿪴��ڽ���
	HAL_UART_Receive_IT(&huart1,&Rxdat,1);
	//ʹ��PWM���
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
	
	//eeprom�ϵ��ʼ��
	Eeprom_Init_Information();
	//LCD�ϵ��ʼ״̬��ʾ
	Display_Lcd();
	

}



void Sys_work()
{

	Receive_Deal();
	Display_Lcd();
	Key_Function();
	
//	�������������eeprom�洢����Ϣ�Ƿ���³ɹ�
	uint8_t r1,r2,r3,r4;
	Eeprom_Read_Bytes(0x00,&r1,1);
	HAL_Delay(10);
	Eeprom_Read_Bytes(0x01,&r2,1);
	HAL_Delay(10);
	Eeprom_Read_Bytes(0x02,&r3,1);
	HAL_Delay(10);
	Eeprom_Read_Bytes(0x03,&r4,1);
	HAL_Delay(10);
	//char st[30];
	//sprintf(st,"%d, %d, %d, %d",r1,r2,r3,r4);
	//LCD_DisplayStringLine(Line8,(u8*)st);
	
}
/************************END****************************/
