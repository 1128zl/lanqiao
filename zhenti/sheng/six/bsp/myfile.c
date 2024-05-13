#include "myfile.h"
#include "lcd.h"
#include "display.h"
#include "key.h"
#include "stdio.h"
#include "i2c_hal.h"
#include "main.h"

//定义外部变量
extern uint8_t is_open,led1_sta,h,m,s,menu,change_time;
extern float k,volt;
extern struct key keys[4];
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;

//定义变量
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
uint8_t flag=0;
/********************************EEPROM*****************************/

//eeprom 初始化保存信息
void  Save_Data_Init()
{
	uint8_t data1,data2,d1,d2;
	Eeprom_Read_Bytes(0xa6,&data1,1);
	Eeprom_Read_Bytes(0xa7,&data2,1);
	if(data1!=0x48 || data2!=0x68)
	{
		d1=0x55,d2=0x88;
		Eeprom_Write_Bytes(0xa6,&data1,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0xa7,&data2,1);
		HAL_Delay(10);
		
		uint8_t k_1=0x00,k_2=0x01;
		k=0.1;
		Eeprom_Write_Bytes(0xa2,&k_1,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0xa3,&k_2,1);
		HAL_Delay(10);
	}
}

//更新在eeprom中的K值
void Update_Data()
{
		uint8_t k_1=0,k_2=k*10;
		Eeprom_Write_Bytes(0xa2,&k_1,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0xa3,&k_2,1);
		HAL_Delay(10);
	
}
/********************************串口模块***************************/
char RXbuff[30],TXbuff[30];
uint8_t Rxdat,BufIndex=0;

//串口回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RXbuff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1,&Rxdat, 1);
	}

}

//利用串口通信修改K的值
void Deal_Information()
{
	if(BufIndex==0 || BufIndex<6)
		return ;
	if(RXbuff[0]=='k' && RXbuff[1]=='0' && RXbuff[2]=='.' && (RXbuff[3]>='1' && RXbuff[3]<='9') )
	{
		k=(RXbuff[3]-'0')/10.0;
		sprintf(TXbuff,"ok\n");
		HAL_UART_Transmit_IT(&huart1,(uint8_t *)TXbuff,sizeof(TXbuff));
	}
	BufIndex=0;
}

//定时上报数据
void Upload_Data_Ontime()
{
	sprintf(TXbuff,"%.2f+%.1f+%d%d%d\n\r",volt,k,h,m,s);
	HAL_UART_Transmit_IT(&huart1,(uint8_t*)TXbuff,sizeof(TXbuff));

}
/*******************************ADC模块************************/
void Get_adc()
{
	uint16_t adc;
	HAL_ADC_Start(&hadc2);
	adc=HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	volt= adc*3.3/4096;
}
/*******************************按键操作********************/
void Key_Function()
{
	//开关指示灯闪烁报警功能
	if(keys[0].key_flg==1)
	{
		is_open=0;
		keys[0].key_flg=0;
	}
	
	//切换显示界面
	if(keys[1].key_flg==1)
	{
		menu++;
		keys[1].key_flg=0;
	}
	
	//切换想要调整的时间是时，还是分，还是秒
	if(keys[2].key_flg==1)
	{
		change_time++;
		keys[2].key_flg=0;
	}
	
	//更新当前要调整的时间
	if(keys[3].key_flg==1)
	{
		if(change_time%3==0)
		{
			if(h==23)
				h=0;
			else
				h++;
		}
		
		else if(change_time%3==1)
		{
			if(m==59)
				m=0;
			else
				m++;
		}
		
		else
		{
			if(s==59)
				s=0;
			else
				s++;
		}
		
		//更新按键作用下设置的时间
		sTime.Hours=h;
		sTime.Minutes=m;
		sTime.Seconds=s;
		HAL_RTC_SetTime(&hrtc, &sTime,RTC_FORMAT_BIN);
		
		//上报电压
		Upload_Data_Ontime();
		//自动切换到系统界面
		menu++;
		keys[3].key_flg=0;
	}
	
	Display_led();
}



/****************************自定义系统函数******************/

//初始化
void sys_init()
{
	
	//LCD初始化
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);

	Display_sys_lcd();
	
	Invoid_Complict();
	//LED初始化
	Control_All_Led();
	
	//eeprom初始化
	I2CInit();
	Save_Data_Init();
	//打开定时器
	HAL_TIM_Base_Start_IT(&htim2);
	
	HAL_UART_Receive_IT(&huart1,&Rxdat, 1);
	
	
}

void sys_work()
{
	//获取当前的时间
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	h=sTime.Hours;
	m=sTime.Minutes;
	s=sTime.Seconds;
	
	Update_Data();
	Deal_Information();
	Get_adc();
	Display_lcd();
	Key_Function();
	
	
	//定时上报电压
	//使用标志位，避免重复发送
	if(flag==0 && (m==30 || s==30 || m==59 || s==59))
	{
		Upload_Data_Ontime();
		flag=1;
	}
	if(!(m==30 || s==30 || m==59 || s==59))
		flag=0;
}
