#include "alarm.h"

void relay_alarm(uint8_t cmd)
{
	if(cmd == RELAY_ON)
	{
		HAL_GPIO_WritePin(RELAY_ALARM_GPIO_Port, RELAY_ALARM_Pin, GPIO_PIN_SET);
	}
	else if(cmd == RELAY_OFF)
	{
		HAL_GPIO_WritePin(RELAY_ALARM_GPIO_Port, RELAY_ALARM_Pin, GPIO_PIN_RESET);
	}
}

void relay_siren(uint8_t cmd)
{
	if(cmd == RELAY_ON)
	{
		HAL_GPIO_WritePin(RELAY_SIREN_GPIO_Port, RELAY_SIREN_Pin, GPIO_PIN_SET);
	}
	else if(cmd == RELAY_OFF)
	{
		HAL_GPIO_WritePin(RELAY_SIREN_GPIO_Port, RELAY_SIREN_Pin, GPIO_PIN_RESET);
	}	
}

void buzz_on(void)
{
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_SET);
}

void buzz_off(void)
{
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);
}

/***********************************************************************/
//*******函数名:demolition_detect(uint8_t cmd)
//*******用途:检测有没有打开机箱门
//*******说明:
//*******参数:
//*******返回值:DEMOLITED:机箱门打开  DISDEMOLITED:机箱门未被打开
/***********************************************************************/
uint8_t demolition_detect(void)
{
	uint8_t demolition_sta;	//拆除状态
	//
	return demolition_sta;
}



