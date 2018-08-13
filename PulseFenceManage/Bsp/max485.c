/**
  ******************************************************************************
  * File Name          : max485.c
  * Description        : 485Ð¾Æ¬Çý¶¯
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "max485.h"

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart2;

void max485_send_str(uint8_t device, uint8_t *str, uint16_t lenth)
{
	if(device == MAX485_1)
	{
		HAL_GPIO_WritePin(RS485_CNT1_GPIO_Port, RS485_CNT1_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart2,str,lenth,1000);
		HAL_GPIO_WritePin(RS485_CNT1_GPIO_Port, RS485_CNT1_Pin, GPIO_PIN_RESET);
	}
	else if(device == MAX485_2)
	{
		HAL_GPIO_WritePin(RS485_CNT2_GPIO_Port, RS485_CNT2_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart4,str,lenth,1000);
		HAL_GPIO_WritePin(RS485_CNT2_GPIO_Port, RS485_CNT2_Pin, GPIO_PIN_RESET);
	}
}

