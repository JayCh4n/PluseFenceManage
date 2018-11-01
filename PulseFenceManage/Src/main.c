
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "lwip.h"

/* USER CODE BEGIN Includes */
#include "tcp_client.h"
#include "display.h"
#include "keyboard.h"
#include "alarm.h"
#include "battery.h"
#include "max485.h"
#include "sim800c.h"
#include "master_manage.h"
#include "flash.h"
#include "udp_client.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern UART_RX_TypeDef uart6_rx_struct;
extern uint8_t uart6_rx_buff;
extern uint8_t uart1_rx_buff;
extern uint8_t logo_256x160[5120];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_USART6_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
	
	read_data_from_flash();
	MX_LWIP_Init();
	udp_monitor_conf(udp_remote_ip, udp_port_num);
	lcd_init();
	lcd_show_256x160(logo_256x160);
	sim800c_init_flag = sim800c_init(5, 5);
	init_control_uint();
	lcd_show_main_page();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		MX_LWIP_Process();
		dynamic_lcd_process();
		communication_icon_update_process();
		key_drive();
		demolition_detect_process();
		bettery_manage_process();
		write_flash_process();
		inquire_ctrl_sta_process();
		alarm_sta_detect_process();
		if(sim800c_init_flag)
		{
			sim800c_process();
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* UART4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UART4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* TIM1_UP_TIM10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 168;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* UART4 init function */
static void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, BETTERY_EN_Pin|RELAY_ZONE2_ALARM_Pin|RELAY_ZONE1_ALARM_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, BUZZ_Pin|RELAY_ZONE2_SIREN_Pin|RELAY_ZONE1_SIREN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ETH_RESET_Pin|DISPLAY_RS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISPLAY_WR_Pin|DISPLAY_E_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DISPLAY_D0_Pin|DISPLAY_D1_Pin|DISPLAY_D2_Pin|DISPLAY_D3_Pin 
                          |DISPLAY_D4_Pin|DISPLAY_D5_Pin|DISPLAY_D6_Pin|DISPLAY_D7_Pin 
                          |LED_DISMANTLE_Pin|LED_ZONE2_ALARM_Pin|LED_ZONE1_ALARM_Pin|LED_ARMING_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SIM800_PWR_GPIO_Port, SIM800_PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISPLAY_RES_GPIO_Port, DISPLAY_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DISPLAY_PWR_Pin|RS485_CNT1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CHRG_STA_Pin KEY5_Pin KEY4_Pin KEY3_Pin 
                           KEY2_Pin */
  GPIO_InitStruct.Pin = CHRG_STA_Pin|KEY5_Pin|KEY4_Pin|KEY3_Pin 
                          |KEY2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BETTERY_EN_Pin BUZZ_Pin RELAY_ZONE2_ALARM_Pin RELAY_ZONE2_SIREN_Pin 
                           RELAY_ZONE1_ALARM_Pin RELAY_ZONE1_SIREN_Pin */
  GPIO_InitStruct.Pin = BETTERY_EN_Pin|BUZZ_Pin|RELAY_ZONE2_ALARM_Pin|RELAY_ZONE2_SIREN_Pin 
                          |RELAY_ZONE1_ALARM_Pin|RELAY_ZONE1_SIREN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ETH_RESET_Pin SIM800_PWR_Pin */
  GPIO_InitStruct.Pin = ETH_RESET_Pin|SIM800_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DISMANTLE_DETECT_Pin */
  GPIO_InitStruct.Pin = DISMANTLE_DETECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DISMANTLE_DETECT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY1_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPLAY_WR_Pin DISPLAY_E_Pin */
  GPIO_InitStruct.Pin = DISPLAY_WR_Pin|DISPLAY_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPLAY_D0_Pin DISPLAY_D1_Pin DISPLAY_D2_Pin DISPLAY_D3_Pin 
                           DISPLAY_D4_Pin DISPLAY_D5_Pin DISPLAY_D6_Pin DISPLAY_D7_Pin */
  GPIO_InitStruct.Pin = DISPLAY_D0_Pin|DISPLAY_D1_Pin|DISPLAY_D2_Pin|DISPLAY_D3_Pin 
                          |DISPLAY_D4_Pin|DISPLAY_D5_Pin|DISPLAY_D6_Pin|DISPLAY_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : SIM800_STATUS_Pin */
  GPIO_InitStruct.Pin = SIM800_STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SIM800_STATUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPLAY_RES_Pin DISPLAY_CS_Pin */
  GPIO_InitStruct.Pin = DISPLAY_RES_Pin|DISPLAY_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DISPLAY_RS_Pin */
  GPIO_InitStruct.Pin = DISPLAY_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DISPLAY_RS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPLAY_PWR_Pin LED_DISMANTLE_Pin LED_ZONE2_ALARM_Pin LED_ZONE1_ALARM_Pin 
                           LED_ARMING_Pin RS485_CNT1_Pin */
  GPIO_InitStruct.Pin = DISPLAY_PWR_Pin|LED_DISMANTLE_Pin|LED_ZONE2_ALARM_Pin|LED_ZONE1_ALARM_Pin 
                          |LED_ARMING_Pin|RS485_CNT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t connect_to_server_cnt = 0;
	static uint16_t dynamic_lcd_cnt = 0;
	static uint32_t demolition_detect_cnt = 0;
	static uint16_t bettery_manage_cnt = 0;
	static uint32_t demolition_alarm_delay_cnt = 0;
	static uint16_t communication_sta_cnt = 0;
	static uint16_t sim800c_try_connect_cnt = 0;
	static uint16_t sim800c_heart_cnt = 0;
	static uint16_t inquire_ctrl_sta_cnt = 0;
	static uint16_t zone1_sta_detect_cnt = 0;
	static uint16_t zone2_sta_detect_cnt = 0;
	
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
	if(htim->Instance == htim1.Instance)
	{
		HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
		HAL_UART_Receive_IT(&huart2, &max485_1_receivebuf, 1);
		HAL_UART_Receive_IT(&huart6, &uart6_rx_buff, 1);
		
		key_scan();
		
		if(++connect_to_server_cnt >= 2000)
		{
			connect_to_server_cnt = 0;
			detect_net_sta_mask = 1;
		}
		
		if(++dynamic_lcd_cnt >= 600)
		{			
			dynamic_lcd_cnt = 0;
			dynamic_lcd_mask = 1;
		}
		
		if(++demolition_detect_cnt >= 5000)
		{
			demolition_detect_cnt = 0;
			demolition_detect_mask = 1;
		}
		
		if(++bettery_manage_cnt >= 1000)
		{
			bettery_manage_cnt = 0;
			bettery_manage_mask = 1;
		}
		
		if(++write_flash_time_cnt >= 6000)
		{
			write_flash_time_cnt = 0;
			write_flash_time_mask = 1;
		}
		
		if(zone1_alarm_reset_flag)
		{
			if(++zone1_alarm_delay_cnt >= zone1_alarm_reset_time)
			{
				zone1_alarm_delay_cnt = 0;
				zone1_alarm_reset_flag = 0;
				alarm_output(ZONE1, RESET_ALARM);
			}
		}
		
		if(zone2_alarm_reset_flag)
		{
			if(++zone2_alarm_delay_cnt >= zone2_alarm_reset_time)
			{
				zone2_alarm_delay_cnt = 0;
				zone2_alarm_reset_flag = 0;
				alarm_output(ZONE2, RESET_ALARM);
			}	
		}
		
		if(demolition_alarm_reset_flag)
		{
			if(++demolition_alarm_delay_cnt >= demolition_alarm_reset_time)
			{
				demolition_alarm_delay_cnt = 0;
				demolition_alarm_reset_flag = 0;
				if(zone_struct.zone1_sta == DISARMING || zone_struct.zone1_sta == ARMING)
				{
						relay_alarm(ZONE1, RELAY_ON);
				}
				led_dismantle(LED_OFF);
			}
		}
		
		if(zone1_trigger_flag)
		{
			if(++zone1_trigger_delay_cnt >= zone1_trigger_delay_time)
			{
				zone1_trigger_delay_cnt = 0;
				zone1_trigger_flag = 0;
			}
		}
		
		if(zone2_trigger_flag)
		{
			if(++zone2_trigger_delay_cnt >= zone2_trigger_delay_time)
			{
				zone2_trigger_delay_cnt = 0;
				zone2_trigger_flag = 0;
			}
		}
		
		
		if(communication_sta == COMMUNICATING)
		{
			if(++communication_sta_cnt >= 5000)
			{
				communication_sta_cnt = 0;
				communication_sta = DISCOMMUNICAT;
			}
		}

		
		if(++sim800c_try_connect_cnt >= 2000)
		{
			sim800c_try_connect_cnt = 0;
			sim800c_try_connect_mask = 1;
		}
		
		if(wait_tcp_contect_flag)
		{
			if(++wait_tcp_contect_cnt >= 10000)
			{
				wait_tcp_contect_cnt = 0;
				process_step = 6;
				sim800c_link_flag = 0;
			}
		}
		
		if(++sim800c_heart_cnt >= 5000)
		{
			sim800c_heart_cnt = 0;
			sim800c_heart_mask = 1;
		}
		
		if(uart6_rx_start)
		{
			if(++uart6_rx_time_cnt >= 2)
			{
				uart6_rx_struct.rx_data[uart6_rx_struct.rx_cnt++] = '\0';
				uart6_rx_start = 0;
				uart6_rx_struct.rx_cnt = 0;
				transparent_data_processing();
			}
		}
		
		if(++update_communication_icon_cnt >= 200)
		{
			update_communication_icon_cnt = 0;
			update_communication_icon_flag = 1;
		}
		
		if(++inquire_ctrl_sta_cnt >= 1000)
		{
			inquire_ctrl_sta_cnt = 0;
			inquire_ctrl_sta_flag = 1;
		}
		
		if(++zone1_sta_detect_cnt >= (zone_struct.zone1_sensitivity == SENSITIVITY_3 ? 900 : zone_struct.zone1_sensitivity == SENSITIVITY_2 ? 2500 : 5000))
		{
			zone1_sta_detect_cnt = 0;
			zone1_sta_detect_flag = 1;
		}
		
		if(++zone2_sta_detect_cnt >= (zone_struct.zone2_sensitivity == SENSITIVITY_3 ? 900 : zone_struct.zone2_sensitivity == SENSITIVITY_2 ? 2500 : 5000))
		{
			zone2_sta_detect_cnt = 0;
			zone2_sta_detect_flag = 1;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	static uint8_t uart1_rx_cnt = 0;
	static uint8_t uart1_package_lenth = 0;
	
	static uint8_t uart2_rx_cnt = 0;
	static uint8_t uart2_package_lenth = 0;	
	
//	uint16_t crc;
	
  UNUSED(huart);
	
	if(huart->Instance == huart1.Instance)
	{
		uart1_rx_cnt++;
		
		if(uart1_rx_cnt == 1)
		{
			if(uart1_rx_buff == 0xA5)
			{
				uart1_rx_data[0] = 0xA5;
			}
			else
			{
				uart1_rx_cnt = 0;
			}
		}
		else if(uart1_rx_cnt == 2)
		{
			if(uart1_rx_buff == 0x5A)
			{
				uart1_rx_data[1] = 0x5A;
			}
			else
			{
				uart1_rx_cnt = 0;
			}
		}
		else if(uart1_rx_cnt == 3)
		{
			uart1_package_lenth = uart1_rx_buff + 5;
			uart1_rx_data[2] = uart1_rx_buff;
		}
		/*后面加上校验位数据判断*/
		else if(uart1_rx_cnt == uart1_package_lenth)
		{
			uart1_rx_data[uart1_rx_cnt - 1] = uart1_rx_buff; 
			uart1_rx_cnt = 0;
			if(max485_wait_usart1_flag)
			{
				max485_wait_usart1_finish = 1;
			}
			uart1_deal(uart1_rx_data);
		}
		else if(uart1_rx_cnt > 50)
		{
			uart1_rx_cnt = 0;
		}
		else
		{
			uart1_rx_data[uart1_rx_cnt - 1] = uart1_rx_buff;
		}
	}		
	
	if(huart->Instance == huart6.Instance)
	{
		if(uart6_rx_struct.rx_enable)		//是否使能接收
		{	
			if(!sim800c_link_flag)	//如果没连接到tcp
			{
				if(!uart6_rx_struct.cmd_data_flag)						//是否为命令接收
				{
					uart6_rx_struct.rx_cmd[uart6_rx_struct.rx_cnt++] = uart6_rx_buff;
					if(uart6_rx_buff == '\n')
					{
						uart6_rx_struct.cmd_data_flag = 1;
						uart6_rx_struct.rx_cnt = 0;
					}
				}
				else if(uart6_rx_struct.cmd_data_flag)						//是否为数据接收
				{
					uart6_rx_struct.rx_data[uart6_rx_struct.rx_cnt++] = uart6_rx_buff;
					if(uart6_rx_buff == '\n')
					{
						uart6_rx_struct.cmd_data_flag = 0;
						uart6_rx_struct.rx_cnt = 0;
						uart6_rx_struct.rx_enable = 0;
						uart6_rx_struct.rx_end = 1;
					}					
				}
			}
			else	//如果已经连接到tcp服务器
			{
				uart6_rx_start = 1;
				uart6_rx_time_cnt = 0;
				
				uart6_rx_struct.rx_data[uart6_rx_struct.rx_cnt++] = uart6_rx_buff;
			}
		}
	}
	if(huart->Instance == huart2.Instance)
	{
		uart2_rx_cnt++;
		
		if(uart2_rx_cnt == 1)
		{
			if(max485_1_receivebuf == 0xE7)
			{
				max485_1_receive_data[0] = 0xE7;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == 2)
		{
			if(max485_1_receivebuf == 0xD9)
			{
				max485_1_receive_data[1] = 0xD9;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == 3)
		{
			if(max485_1_receivebuf == 0xE7)
			{
				max485_1_receive_data[2] = 0xE7;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == 4)
		{
			if(max485_1_receivebuf == 0xD9)
			{
				max485_1_receive_data[3] = 0xD9;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == 5)
		{
			if(max485_1_receivebuf == 0x02)
			{
				max485_1_receive_data[4] = 0x02;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == 6)
		{
			if((max485_1_receivebuf == zone_struct.zone1_id) || (max485_1_receivebuf == 0xFF))
			{
				max485_1_receive_data[5] = max485_1_receivebuf;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == 8)
		{
			uart2_package_lenth = max485_1_receivebuf + 14;
			max485_1_receive_data[7] = max485_1_receivebuf;
		}
		else if(uart2_rx_cnt == uart2_package_lenth - 3)
		{
			if(max485_1_receivebuf == 0xF8)
			{
				max485_1_receive_data[uart2_rx_cnt - 1] = 0xF8;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == uart2_package_lenth - 2)
		{
			if(max485_1_receivebuf == 0xC6)
			{
				max485_1_receive_data[uart2_rx_cnt - 1] = 0xC6;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt == uart2_package_lenth - 1)
		{
			if(max485_1_receivebuf == 0xF8)
			{
				max485_1_receive_data[uart2_rx_cnt - 1] = 0xF8;
			}
			else
			{
				uart2_rx_cnt = 0;
			}
		}
		/*后面加上校验位数据判断*/
		else if(uart2_rx_cnt == uart2_package_lenth)
		{
			if(max485_1_receivebuf == 0xC6)
			{
				max485_1_receive_data[uart2_rx_cnt - 1] = 0xC6;
//				crc = CRC16(max485_1_receive_data, uart2_package_lenth - 6);
//				if((max485_1_receive_data[uart1_package_lenth - 6] == (crc >> 8)) && (max485_1_receive_data[uart1_package_lenth - 5] == crc))
//				{
					max_485_1_deal(max485_1_receive_data);
//				}
				uart2_rx_cnt = 0;
			}
		}
		else if(uart2_rx_cnt > 50)
		{
			uart1_rx_cnt = 0;
		}
		else
		{
			max485_1_receive_data[uart2_rx_cnt - 1] = max485_1_receivebuf;
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
