#include "myfile.h"
#include "lcd.h"
#include "display.h"
#include "key.h"
#include "stdio.h"
#include "i2c_hal.h"
#include "main.h"

//�����ⲿ����
extern uint8_t is_open,led1_sta,h,m,s,menu,change_time;
extern float k,volt;
extern struct key keys[4];
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;

//�������
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
uint8_t flag=0;
/********************************EEPROM*****************************/

//eeprom ��ʼ��������Ϣ
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

//������eeprom�е�Kֵ
void Update_Data()
{
		uint8_t k_1=0,k_2=k*10;
		Eeprom_Write_Bytes(0xa2,&k_1,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0xa3,&k_2,1);
		HAL_Delay(10);
	
}
/********************************����ģ��***************************/
char RXbuff[30],TXbuff[30];
uint8_t Rxdat,BufIndex=0;

//���ڻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RXbuff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1,&Rxdat, 1);
	}

}

//���ô���ͨ���޸�K��ֵ
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

//��ʱ�ϱ�����
void Upload_Data_Ontime()
{
	sprintf(TXbuff,"%.2f+%.1f+%d%d%d\n\r",volt,k,h,m,s);
	HAL_UART_Transmit_IT(&huart1,(uint8_t*)TXbuff,sizeof(TXbuff));

}
/*******************************ADCģ��************************/
void Get_adc()
{
	uint16_t adc;
	HAL_ADC_Start(&hadc2);
	adc=HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	volt= adc*3.3/4096;
}
/*******************************��������********************/
void Key_Function()
{
	//����ָʾ����˸��������
	if(keys[0].key_flg==1)
	{
		is_open=0;
		keys[0].key_flg=0;
	}
	
	//�л���ʾ����
	if(keys[1].key_flg==1)
	{
		menu++;
		keys[1].key_flg=0;
	}
	
	//�л���Ҫ������ʱ����ʱ�����Ƿ֣�������
	if(keys[2].key_flg==1)
	{
		change_time++;
		keys[2].key_flg=0;
	}
	
	//���µ�ǰҪ������ʱ��
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
		
		//���°������������õ�ʱ��
		sTime.Hours=h;
		sTime.Minutes=m;
		sTime.Seconds=s;
		HAL_RTC_SetTime(&hrtc, &sTime,RTC_FORMAT_BIN);
		
		//�ϱ���ѹ
		Upload_Data_Ontime();
		//�Զ��л���ϵͳ����
		menu++;
		keys[3].key_flg=0;
	}
	
	Display_led();
}



/****************************�Զ���ϵͳ����******************/

//��ʼ��
void sys_init()
{
	
	//LCD��ʼ��
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);

	Display_sys_lcd();
	
	Invoid_Complict();
	//LED��ʼ��
	Control_All_Led();
	
	//eeprom��ʼ��
	I2CInit();
	Save_Data_Init();
	//�򿪶�ʱ��
	HAL_TIM_Base_Start_IT(&htim2);
	
	HAL_UART_Receive_IT(&huart1,&Rxdat, 1);
	
	
}

void sys_work()
{
	//��ȡ��ǰ��ʱ��
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
	
	
	//��ʱ�ϱ���ѹ
	//ʹ�ñ�־λ�������ظ�����
	if(flag==0 && (m==30 || s==30 || m==59 || s==59))
	{
		Upload_Data_Ontime();
		flag=1;
	}
	if(!(m==30 || s==30 || m==59 || s==59))
		flag=0;
}
