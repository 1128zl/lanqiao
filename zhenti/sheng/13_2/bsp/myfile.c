#include "main.h"
#include "myfile.h"
#include "lcd.h"
#include "stdio.h"
#include "led.h"
#include "key.h"
#include "i2c_hal.h"

//定义外部变量
extern uint8_t menu;										//显示界面
extern uint8_t buy_x_count,buy_y_count;	//各类商品的购买数量
extern float x_price,y_price;						//记录各类商品的价格
extern uint8_t stock_x,stock_y;					//记录各类商品的库存量
extern TIM_HandleTypeDef htim4,htim3,htim16;
extern struct key keys[4];
extern uint16_t time_count;
extern uint8_t led2_sta,is_enputy;
extern char RXbuff[30],TXbuff[30];
extern unsigned char Rxdat,BufIndex;
extern UART_HandleTypeDef huart1;

//定义变量
char st[25];
float pay=0.0;	//购买的各类商品的总金额
uint8_t is_buy=0;	//记录是否购买成功

/*********************LCD模块*****************************/
/************************BEGIN****************************/

void Display_Lcd()
{
	//商品购买界面
	if(menu%3==0)
	{
		sprintf(st,"        SHOP        ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(st,"     X:%d            ",buy_x_count);
		LCD_DisplayStringLine(Line3,(u8*)st);
		sprintf(st,"     Y:%d            ",buy_y_count);
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	
	//商品价格界面
	else if(menu%3==1)
	{
		sprintf(st,"        PRICE       ");
		LCD_DisplayStringLine(Line1,(u8*)st);
		sprintf(st,"     X:%.1f          ",x_price);
		LCD_DisplayStringLine(Line3,(u8*)st);
		sprintf(st,"     Y:%.1f          ",y_price);
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	
	//库存信息界面
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






/*************************按键模块*************************/
/************************BEGIN****************************/
//处理按键函数


void Key_Function()
{
	
	//按键B1切换界面
	if(keys[0].key_flg==1)
	{
		menu++;
		keys[0].key_flg=0;
	}
	
	//商品X的相关信息处理
	if(keys[1].key_flg==1)
	{
		//商品购买界面下
		if(menu%3==0)
		{
			if(buy_x_count!=stock_x)
			{
				buy_x_count++;
			}
			
		}
		
		//商品价格界面下
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
		
		//库存信息界面下
		else if(menu%3==2)
		{
			stock_x++;
			
			Eeprom_Write_Bytes(0x00,&stock_x,1);
			HAL_Delay(2);
		}
		
		keys[1].key_flg=0;
	}
	
	//商品Y的相关信息处理
	if(keys[2].key_flg==1)
	{
			//商品购买界面下
		if(menu%3==0)
		{
			if(buy_y_count!=stock_y)
			{
				buy_y_count++;
			}
		}
		
		//商品价格界面下
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
		
		//库存信息界面下
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
			//发送购买的各类商品数量以及总价格
			
			HAL_UART_Transmit_IT(&huart1, (u8*)TXbuff, sizeof(TXbuff));
			
			//商品购买成功，LED1点亮5s后熄灭
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














/********************串口模块*****************************/
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



/******************自定义系统初始化函数编写****************************/
/************************BEGIN****************************/

void Sys_init()
{
	
	//LCD初始化
	LCD_Init();
	LCD_Clear(Black);	//清空LCD显示
	LCD_SetTextColor(White);	//设置文本色为白色
	LCD_SetBackColor(Black);	//设置背景色为黑色
	//LED初始化
	Contr_All_led();
	
	//I2C初始化
	I2CInit();
	//打开定时器TIM4
	HAL_TIM_Base_Start_IT(&htim4);
	
	//打开串口接收
	HAL_UART_Receive_IT(&huart1,&Rxdat,1);
	//使能PWM输出
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
	
	//eeprom上电初始化
	Eeprom_Init_Information();
	//LCD上电初始状态显示
	Display_Lcd();
	

}



void Sys_work()
{

	Receive_Deal();
	Display_Lcd();
	Key_Function();
	
//	这里是用来检测eeprom存储的信息是否更新成功
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
