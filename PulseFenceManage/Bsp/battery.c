/**
  ******************************************************************************
  * File Name          : battery.c
  * Description        : 电池功能函数
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "battery.h"

extern ADC_HandleTypeDef hadc1;

uint8_t ac_connected_flag = 2;

/***********************************************************************/
//*******函数名:relay_battery(uint8_t cmd)
//*******用途:开启或关闭 电池控制继电器
//*******说明:
//*******参数:cmd：RELAY_ON :开启继电器  RELAY_OFF:关闭继电器
//*******返回值:无
/***********************************************************************/
void relay_battery(uint8_t cmd)
{
	if(cmd == RELAY_ON)
	{
		HAL_GPIO_WritePin(RELAY_BAT_GPIO_Port, RELAY_BAT_Pin, GPIO_PIN_SET);
	}
	else if(cmd == RELAY_OFF)
	{
		HAL_GPIO_WritePin(RELAY_BAT_GPIO_Port, RELAY_BAT_Pin, GPIO_PIN_RESET);
	}
}

/***********************************************************************/
//*******函数名:uint8_t ac_detect(void)
//*******用途:检测是否有交流电接入
//*******说明:当交流电掉电时 应轮询检测电池剩余电量 电池电量过低时自动断电
//*******参数:cmd：RELAY_ON :开启继电器  RELAY_OFF:关闭继电器
//*******返回值:AC_CONNECTED:交流电已连接		AC_DISCONNECTED:交流电已断开
/***********************************************************************/
uint8_t ac_detect(void)
{
	return HAL_GPIO_ReadPin(AC_DETECT_GPIO_Port, AC_DETECT_Pin);
}

/***********************************************************************/
//*******函数名:get_battery_voltage(void)
//*******用途:获取当前电池电压
//*******说明:当交流电掉电时 应轮询检测电池剩余电量 电池电量过低时自动断电
//*******参数:
//*******返回值:电池当前电压值
/***********************************************************************/
float get_battery_voltage(void)
{
	ADC_ChannelConfTypeDef sConfig;
	float reference_voltage = 3.6f;		//参考电压
	uint16_t max_value = 4096;				
	uint16_t get_value;
	float get_voltage;
	float battery_voltage;
	
	sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	sConfig.Offset = 0;
	
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	
	get_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
	
	get_voltage = (float)(reference_voltage / max_value * get_value);
	
	battery_voltage = get_voltage * 3.811f;
	
	return battery_voltage;
}

/***********************************************************************/
//*******函数名:battery_capacity_detect_processing(void)
//*******用途:电池容量检测 当电量为5%时断开电池
//*******说明:当交流电掉电时 应轮询检测电池剩余电量 电池电量过低时自动断电
//*******参数:
//*******返回值:电池当前剩余电量百分比
/***********************************************************************/
float battery_capacity_detect_processing(void)
{
	float battery_capacity;
	//
	
	return battery_capacity;
}


















