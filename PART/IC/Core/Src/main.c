/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g4xx_hal_uart.h"
#include "led.h"
#include "lcd.h"
#include "stdio.h"
#include "interrupt.h"
#include "badc.h"
#include "i2c_hal.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern struct keys key[];
extern unsigned int frq1,frq2;
extern uint duty1,duty2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



//上位机结合stm32显示画面的串口设置
/*
UART_HandleTypeDef huart1;
uint8_t uart_rx_buf1[1024 * 6]; //串口接收数据缓冲区1
uint8_t uart_rx_buf2[1024 * 6]; //串口接收数据缓冲区2


// UART1 初始化
static void MX_UART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit); //配置USART1外设的时钟

	__HAL_RCC_USART1_CLK_ENABLE(); //打开USART1的时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 1000000; //设置波特率
	huart1.Init.WordLength = UART_WORDLENGTH_8B; //设置数据位
	huart1.Init.StopBits = UART_STOPBITS_1; //设置停止位
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	HAL_MultiProcessor_Init(&huart1, 0, UART_WAKEUPMETHOD_IDLELINE);
	HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8);

	HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_DisableFifoMode(&huart1);

	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; //复用开漏输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;//复用功能
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //初始化串口引脚PA9 PA10

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); //设置串口中断优先级
	HAL_NVIC_EnableIRQ(USART1_IRQn); //打开串口中断

	HAL_UART_Receive_IT(&huart1, uart_rx_buf1, 4800); //设置串口中断缓冲区及中断阈值(当前为1)
}

//串口中断
void USART1_IRQHandler(void) //串口中断
{
	HAL_UART_IRQHandler(&huart1);
}
int buf = 1; //记录当前使用的是buf1 还是 buf2
int disp = 0; //告诉main该显示什么内容，0不显示，1显示buf1，2显示buf2

//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(buf == 1) //若果当前是使用的是buf1
	{
		buf = 2; //下次使用buf2
		disp = 1;//告诉main显示buf1的内容到屏幕
		HAL_UART_Receive_IT(&huart1, uart_rx_buf2, 4800);  //切换为buf2
	}
	else
	{
		buf = 1; //下次使用buf1
		disp = 2;//告诉main显示buf2的内容到屏幕
		HAL_UART_Receive_IT(&huart1, uart_rx_buf1, 4800);  //切换为buf2
	}
} 
*/



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uchar pa6_duty;
	uchar pa7_duty;
	
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	LED_Display(0x00);			//led初始化
	LCD_Init();			//lcd初始化
  
	//清空背景为蓝色
	LCD_Clear(Black);
	//设置背景色为蓝色
	LCD_SetBackColor(Black);
	//设置前景色为白色
	LCD_SetTextColor(White);
	
	//普通定时器开启
	HAL_TIM_Base_Start_IT(&htim4);
	
	//pwm输出通道开启
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
	/*
	//将界面划分为四小格
	LCD_DrawLine(120,0,320,Horizontal);
	LCD_DrawLine(0,160,240,Vertical);
	HAL_Delay(1000);
	LCD_Clear(Blue);
	
	//在界面中心显示一个矩形
	LCD_DrawRect(70,210,100,100);
	HAL_Delay(1000);
	LCD_Clear(Blue);
	
	//在界面中心显示一个圆形
	LCD_DrawCircle(120,160,50);
	HAL_Delay(1000);
	
	
	//清空LCD背景为蓝色
	LCD_Clear(Blue);
	LCD_DisplayStringLine(Line4 ,(unsigned char *)"    Hello,world.   ");
	HAL_Delay(1000);
	
	
	LCD_SetBackColor(White);
	LCD_DisplayStringLine(Line0,(unsigned char *)"                    ");	
	LCD_SetBackColor(Black);
	LCD_DisplayStringLine(Line1,(unsigned char *)"                    ");	
	LCD_SetBackColor(Grey);
	LCD_DisplayStringLine(Line2,(unsigned char *)"                    ");
	LCD_SetBackColor(Blue);
	LCD_DisplayStringLine(Line3,(unsigned char *)"                    ");
	LCD_SetBackColor(Blue2);
	LCD_DisplayStringLine(Line4,(unsigned char *)"                    ");
	LCD_SetBackColor(Red);						
	LCD_DisplayStringLine(Line5,(unsigned char *)"                    ");
	LCD_SetBackColor(Magenta);	
	LCD_DisplayStringLine(Line6,(unsigned char *)"                    ");
	LCD_SetBackColor(Green);	
	LCD_DisplayStringLine(Line7,(unsigned char *)"                    ");	
	LCD_SetBackColor(Cyan);	
	LCD_DisplayStringLine(Line8,(unsigned char *)"                    ");
	LCD_SetBackColor(Yellow);		
	LCD_DisplayStringLine(Line9,(unsigned char *)"                    ");	
	*/
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		char text[30];
		if(key[0].single_flag==1)
		{
			uchar frq_h=1>>8;
			uchar frq_l=1&0xff;
			eeprom_write(1,frq_h);
			HAL_Delay(10);
			eeprom_write(2,frq_l);
			key[0].single_flag=0;
		}
//		if(key[0].long_flag==1)
//		{
//			sprintf(text," key0down_long				");
//			LCD_DisplayStringLine(Line8,(uint8_t *)text);
//			key[0].single_flag=0;
//		}
//		
		if(key[1].single_flag==1)
		{
			//sprintf(text," key1down				");
			//LCD_DisplayStringLine(Line7,(uint8_t *)text);
			pa6_duty+=10;
			if(pa6_duty==100) pa6_duty=10;
			__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,pa6_duty);
			sprintf(text," pwm1_duty:%d			",pa6_duty);
			LCD_DisplayStringLine(Line8,(uint8_t *)text);
			key[1].single_flag=0;
		}
		
		if(key[2].single_flag==1)
		{
			//sprintf(text," key2down				");
			//LCD_DisplayStringLine(Line6,(uint8_t *)text);
			pa7_duty+=10;
			if(pa7_duty==100) pa7_duty=10;
			__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,pa7_duty);
			sprintf(text," pwm2_duty:%d			",pa7_duty);
			LCD_DisplayStringLine(Line7,(uint8_t *)text);
			key[2].single_flag=0;
		}
//		
//		if(key[3].single_flag==1)
//		{
//			//sprintf(text," key3down				");
//			//LCD_DisplayStringLine(Line5,(uint8_t *)text);
//			//key[3].single_flag=0;
//		}
		
		sprintf(text,"            FRQ1=%d ",frq1);
		LCD_DisplayStringLine(Line0,(uint8_t *)text);
		sprintf(text,"            FRQ2=%d ",frq2);
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
//		sprintf(text,"            DUTY1=%d ",duty1);
//		LCD_DisplayStringLine(Line2,(uint8_t *)text);
//		sprintf(text,"            DUTY2=%d ",duty2);
//		LCD_DisplayStringLine(Line3,(uint8_t *)text);
//		sprintf(text,"            V=%.2f ",Get_ADC(&hadc1));
//		LCD_DisplayStringLine(Line4,(uint8_t *)text);
//		sprintf(text,"            V=%.2f ",Get_ADC(&hadc2));
//		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		
		uint frq_eep=(eeprom_read(1)<<8)+(eeprom_read(2));
		sprintf(text,"  frq_eep=%d ",frq_eep);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		
		
		char temp[30];
		sprintf(temp,"frq=%d\r\n",frq1);
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,strlen(temp),50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
