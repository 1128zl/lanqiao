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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED1_ON HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define LED1_OFF HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)

#define LED2_ON HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET)
#define LED2_OFF HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET)

#define GPIOD_PIN2_ON HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET)
#define GPIOD_PIN2_OFF HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET)

uint8_t str[50];
uint8_t Value=0, Volt=0;
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
//灯光检测
void LED_Check()
{
		GPIOD_PIN2_ON;
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
		LED1_ON;
		HAL_Delay(200);
		LED2_ON;
		HAL_Delay(200);
		LED1_OFF;
		HAL_Delay(200);
		LED2_OFF;
		HAL_Delay(200);
		GPIOD_PIN2_OFF;
}
//上电开机显示
void Begin_Display()
{
	
	LED_Check();
	sprintf(str,"www.xiqianyangyi.com   ");
	LCD_DisplayStringLine(Line0,str);
	sprintf(str,"Demo:  ");
	LCD_DisplayStringLine(Line1,str);
	sprintf(str,"Volt:  ");
	LCD_DisplayStringLine(Line2,str);
}

//ADC 查询 电压数据采集
void ADC_Get_Value()
{
		
		HAL_ADC_Start(&hadc1);
		GPIOD_PIN2_ON;
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
		LED1_ON;
		if(HAL_OK==HAL_ADC_PollForConversion(&hadc1,10))
			{

				Value=HAL_ADC_GetValue(&hadc1);
				Volt=Value*3300/4096;
			}
		LED1_OFF;
		GPIOD_PIN2_OFF;
		HAL_ADC_Stop(&hadc1);
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
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	LCD_Clear(Black);
	Begin_Display();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		ADC_Get_Value();
		sprintf(str,"www.xiqianyangyi.com   ");
		LCD_DisplayStringLine(Line0,str);
		sprintf(str,"Demo:%d ",Value);
		LCD_DisplayStringLine(Line1,str);
		sprintf(str,"Volt:%d.%d%dV",Volt/1000,(Volt%1000)/100,(Volt%100)/10);
		LCD_DisplayStringLine(Line2,str);
		if(Volt<1300)
			{
				GPIOD_PIN2_ON;
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
				LED2_ON;
				GPIOD_PIN2_OFF;
			}
		else
			{
				GPIOD_PIN2_ON;
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
				LED2_OFF;
				GPIOD_PIN2_OFF;
			
			}
		HAL_Delay(300);
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
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
