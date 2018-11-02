/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define CHRG_STA_Pin GPIO_PIN_2
#define CHRG_STA_GPIO_Port GPIOE
#define BETTERY_EN_Pin GPIO_PIN_3
#define BETTERY_EN_GPIO_Port GPIOE
#define BUZZ_Pin GPIO_PIN_4
#define BUZZ_GPIO_Port GPIOE
#define VREF1_2_Pin GPIO_PIN_0
#define VREF1_2_GPIO_Port GPIOC
#define ETH_RESET_Pin GPIO_PIN_2
#define ETH_RESET_GPIO_Port GPIOC
#define BAT_DETECT_Pin GPIO_PIN_3
#define BAT_DETECT_GPIO_Port GPIOC
#define AC_DETECT_Pin GPIO_PIN_0
#define AC_DETECT_GPIO_Port GPIOA
#define DISMANTLE_DETECT_Pin GPIO_PIN_0
#define DISMANTLE_DETECT_GPIO_Port GPIOB
#define DISMANTLE_PULSE_Pin GPIO_PIN_1
#define DISMANTLE_PULSE_GPIO_Port GPIOB
#define RELAY_ZONE2_ALARM_Pin GPIO_PIN_8
#define RELAY_ZONE2_ALARM_GPIO_Port GPIOE
#define RELAY_ZONE2_SIREN_Pin GPIO_PIN_9
#define RELAY_ZONE2_SIREN_GPIO_Port GPIOE
#define RELAY_ZONE1_ALARM_Pin GPIO_PIN_10
#define RELAY_ZONE1_ALARM_GPIO_Port GPIOE
#define RELAY_ZONE1_SIREN_Pin GPIO_PIN_11
#define RELAY_ZONE1_SIREN_GPIO_Port GPIOE
#define KEY5_Pin GPIO_PIN_12
#define KEY5_GPIO_Port GPIOE
#define KEY4_Pin GPIO_PIN_13
#define KEY4_GPIO_Port GPIOE
#define KEY3_Pin GPIO_PIN_14
#define KEY3_GPIO_Port GPIOE
#define KEY2_Pin GPIO_PIN_15
#define KEY2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_10
#define KEY1_GPIO_Port GPIOB
#define DISPLAY_WR_Pin GPIO_PIN_14
#define DISPLAY_WR_GPIO_Port GPIOB
#define DISPLAY_E_Pin GPIO_PIN_15
#define DISPLAY_E_GPIO_Port GPIOB
#define DISPLAY_D0_Pin GPIO_PIN_8
#define DISPLAY_D0_GPIO_Port GPIOD
#define DISPLAY_D1_Pin GPIO_PIN_9
#define DISPLAY_D1_GPIO_Port GPIOD
#define DISPLAY_D2_Pin GPIO_PIN_10
#define DISPLAY_D2_GPIO_Port GPIOD
#define DISPLAY_D3_Pin GPIO_PIN_11
#define DISPLAY_D3_GPIO_Port GPIOD
#define DISPLAY_D4_Pin GPIO_PIN_12
#define DISPLAY_D4_GPIO_Port GPIOD
#define DISPLAY_D5_Pin GPIO_PIN_13
#define DISPLAY_D5_GPIO_Port GPIOD
#define DISPLAY_D6_Pin GPIO_PIN_14
#define DISPLAY_D6_GPIO_Port GPIOD
#define DISPLAY_D7_Pin GPIO_PIN_15
#define DISPLAY_D7_GPIO_Port GPIOD
#define SIM800_STATUS_Pin GPIO_PIN_8
#define SIM800_STATUS_GPIO_Port GPIOC
#define SIM800_PWR_Pin GPIO_PIN_9
#define SIM800_PWR_GPIO_Port GPIOC
#define DISPLAY_RES_Pin GPIO_PIN_8
#define DISPLAY_RES_GPIO_Port GPIOA
#define DISPLAY_CS_Pin GPIO_PIN_12
#define DISPLAY_CS_GPIO_Port GPIOA
#define DISPLAY_RS_Pin GPIO_PIN_12
#define DISPLAY_RS_GPIO_Port GPIOC
#define DISPLAY_PWR_Pin GPIO_PIN_0
#define DISPLAY_PWR_GPIO_Port GPIOD
#define LED_DISMANTLE_Pin GPIO_PIN_1
#define LED_DISMANTLE_GPIO_Port GPIOD
#define LED_ZONE2_ALARM_Pin GPIO_PIN_2
#define LED_ZONE2_ALARM_GPIO_Port GPIOD
#define LED_ZONE1_ALARM_Pin GPIO_PIN_3
#define LED_ZONE1_ALARM_GPIO_Port GPIOD
#define LED_ARMING_Pin GPIO_PIN_4
#define LED_ARMING_GPIO_Port GPIOD
#define RS485_CNT1_Pin GPIO_PIN_7
#define RS485_CNT1_GPIO_Port GPIOD

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
