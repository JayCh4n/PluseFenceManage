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
extern uint8_t battery_icon_32x16[6][64];

uint8_t bettery_manage_mask = 0;  //电池管理定时器标志
uint8_t charged_flag = 1;		//每充一次电  改变量都置1  防止电量反弹
//uint8_t ac_connected_flag = 2;

/***********************************************************************/
//*******函数名:relay_battery(uint8_t cmd)
//*******用途:开启或关闭 电池控制继电器  电量过低时关闭继电器 进入低功耗模式
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
	return (uint8_t)HAL_GPIO_ReadPin(AC_DETECT_GPIO_Port, AC_DETECT_Pin);
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
	float reference_voltage = 1.24f;		//参考电压
//	uint16_t max_value = 4096;
	uint16_t get_value;
	uint16_t vref_value;
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
	
	sConfig.Channel = ADC_CHANNEL_10;
	
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	vref_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
	
	get_voltage = (float)(reference_voltage*get_value/vref_value);
	
//	get_voltage = (float)(reference_voltage / max_value * get_value);
	
	battery_voltage = get_voltage * 5.0f;
	
	return battery_voltage;
}

/***********************************************************************/
//*******函数名:get_battery_capacity(void)
//*******用途:电池容量检测 当电量为5%时断开电池
//*******说明:当交流电掉电时 应轮询检测电池剩余电量 电池电量过低时自动断电
//*******参数:
//*******返回值:电池当前剩余电量百分比
/***********************************************************************/
uint8_t get_battery_capacity(void)
{
	uint8_t i;
	float battery_voltage = 0.0f;
	static float pre_battery_voltage = 0.0f;			//前一次检测电池电量%
	float battery_voltage_buff[20];
	uint8_t battery_capacity;
	
//	battery_voltage = get_battery_voltage();
	
	for(i=0; i<20; i++)
	{
		battery_voltage_buff[i] = get_battery_voltage();
		battery_voltage += battery_voltage_buff[i];
	}
	
	battery_voltage /= 20.0f;
	
	if(battery_voltage < 10.8f)
	{
		battery_capacity = 0;
	}
	else if(battery_voltage >= 10.8f && battery_voltage < 11.8f)
	{
		battery_capacity = 20;
	}
	else if(battery_voltage >= 11.8f && battery_voltage < 12.4f)
	{
		battery_capacity = 40;
	}
	else if(battery_voltage >= 12.4f && battery_voltage < 13.0f)
	{
		battery_capacity = 60;
	}
	else if(battery_voltage >= 13.0f && battery_voltage < 13.4f)
	{
		battery_capacity = 80;
	}
	else if(battery_voltage >= 13.4f)
	{
		battery_capacity = 100;
	}
	
	//电量检测防反弹  充过电以后  重新检测
	if(charged_flag)
	{
		charged_flag = 0;
		pre_battery_voltage = battery_capacity;
	}
	else
	{
		if(battery_capacity < pre_battery_voltage)
		{
			pre_battery_voltage = battery_capacity;
		}
		else
		{
			return pre_battery_voltage;
		}
	}
	
	return battery_capacity;
}

/***********************************************************************/
//*******函数名:get_charge_sta(void)
//*******用途:获取充电状态  电池图标格数递增
//*******说明:当交流电接入时 不进行电池电量检测
//*******参数:
//*******返回值:CHARGEING：正在充电  DISCHARGE：没有在充电
/***********************************************************************/
uint8_t get_charge_sta(void)
{
	uint8_t sta;
	
	sta = !HAL_GPIO_ReadPin(CHRG_STA_GPIO_Port, CHRG_STA_Pin);
	
	return sta;
}

void bettery_manage_process(void)
{
	static uint8_t bettery_icon_mask = 0;
	uint8_t battery_capacity;
	
	//定时未到 终止
	if(!bettery_manage_mask)
	{
		return;
	}
	
	bettery_manage_mask = 0;
	
	if(ac_detect() == AC_CONNECTED)		//如果交流电接入
	{
		if(page_sta == IN_MAIN_PAGE)
		{
			if(get_charge_sta() == CHARGEING)	//如果正在充电
			{
				charged_flag = 1;
				lcd_show_32x16(1,209,battery_icon_32x16[bettery_icon_mask]);
				if(++bettery_icon_mask >= 6)
				{
					bettery_icon_mask = 0;
				}
			}
			else			//如果没在充电
			{
				clear_screen(1, 209, 2, 32);
			}
		}
	}
	else			//如果交流电未接入
	{
		if(page_sta == IN_MAIN_PAGE)
		{
			battery_capacity = get_battery_capacity();
			bettery_icon_mask = battery_capacity / 20;
			lcd_show_32x16(1,209,battery_icon_32x16[bettery_icon_mask]);
		}
	}
}


















