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
#include "stdio.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "myfile.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//R37 R38电压值变量定义
float v_37,v_38;
//R37 R38电压值上下限定义
float standard_37_up=2.2;
float standard_37_down=1.2;
float standard_38_up=3.0;
float standard_38_down=1.4;

//总检测数，符合标准数，合格率的变量定义
uint8_t check_total_37=0;
uint8_t standard_total_37=0;
float qulify_ratio_37=0.0;

uint8_t check_total_38=0;
uint8_t standard_total_38=0;
float qulify_ratio_38=0.0;

//定义当前所处界面的变量
// 0--产品参数界面		1--标准设置界面	2--合格率界面
uint8_t menu=0;

//定义产品参数界面下要调整的检测标准变化的变量
//update_standard%4==0--调整产品R37上限  
//update_standard%4==1--调整产品R37下限
//update_standard%4==2--调整产品R38上限  
//update_standard%4==3--调整产品R38下限
int8_t update_standard=-1;

//定义R37 R38 是否检测合格的标志变量
uint8_t qulify_37=0;
uint8_t qulify_38=0;

//串口缓存
char TX_Buff[50];
char RX_Buff[50];

unsigned char BufIndex=0;
unsigned char Rxdat=0;


//标记R37,R38检测是否合格
uint8_t standard_37=0;
uint8_t standard_38=0;

//定时器计数
uint8_t tim_count=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//LCD显示函数
void Display_Lcd()
{
	
	char str[30];
	
	if((menu%3)==0)
	{
		sprintf(str,"       GOODS        ");
		LCD_DisplayStringLine(Line1,(u8*)str);
		sprintf(str,"     R37:%.2fV      ",v_37);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"     R38:%.2fV      ",v_38);
		LCD_DisplayStringLine(Line4,(u8*)str);
	}
	else if((menu%3)==1)
	{
		sprintf(str,"      STANDARD      ");
		LCD_DisplayStringLine(Line1,(u8*)str);
		sprintf(str,"    SR37:%.1f-%.1f    ",standard_37_down,standard_37_up);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"    SR38:%.1f-%.1f    ",standard_38_down,standard_38_up);
		LCD_DisplayStringLine(Line4,(u8*)str);
	}
	else
	{
		sprintf(str,"        PASS        ");
		LCD_DisplayStringLine(Line1,(u8*)str);
		sprintf(str,"     PR37:%.1f%%     ",qulify_ratio_37);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"     PR38:%.1f%%     ",qulify_ratio_38);
		LCD_DisplayStringLine(Line4,(u8*)str);
	}

}

//获取电压值
void Get_ADC(uint8_t type)
{
	
	if(type==38)
	{
		HAL_ADC_Start(&hadc1);
		v_38=HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		v_38=v_38*3.3/4096;
	}
	else
	{
		HAL_ADC_Start(&hadc2);
		v_37=HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		v_37=v_37*3.3/4096;
	}
	
}	

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_UART_Receive_IT(&huart1, &Rxdat, 1);
	sys_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		sys_work();
		
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
