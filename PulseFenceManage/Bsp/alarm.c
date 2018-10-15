#include "alarm.h"

uint8_t demolition_detect_mask = 0; //定时器计时防拆检测标志 
uint8_t demolition_sta = DISDEMOLITED;

uint8_t zone1_alarm_reset_flag = 0;
uint8_t zone2_alarm_reset_flag = 0;
uint8_t demolition_alarm_reset_flag = 0;

uint16_t zone1_alarm_delay_cnt = 0;
uint16_t zone2_alarm_delay_cnt = 0;

uint32_t zone1_alarm_reset_time = 5000;
uint32_t zone2_alarm_reset_time = 5000;
uint32_t demolition_alarm_reset_time = 5000;

void relay_alarm(uint8_t zone_num, uint8_t cmd)
{
	if(cmd == RELAY_ON)
	{
		if(zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_ALARM_GPIO_Port, RELAY_ZONE1_ALARM_Pin, GPIO_PIN_SET);
		}
		else if(zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_ALARM_GPIO_Port, RELAY_ZONE2_ALARM_Pin, GPIO_PIN_SET);
		}
	}
	else if(cmd == RELAY_OFF)
	{
		if(zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_ALARM_GPIO_Port, RELAY_ZONE1_ALARM_Pin, GPIO_PIN_RESET);
		}
		else if(zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_ALARM_GPIO_Port, RELAY_ZONE2_ALARM_Pin, GPIO_PIN_RESET);
		}
	}
}

void relay_siren(uint8_t zone_num, uint8_t cmd)
{
	if(cmd == RELAY_ON)
	{
		if(zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_SIREN_GPIO_Port, RELAY_ZONE1_SIREN_Pin, GPIO_PIN_SET);
		}
		else if(zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_SIREN_GPIO_Port, RELAY_ZONE2_SIREN_Pin, GPIO_PIN_SET);
		}
	}
	else if(cmd == RELAY_OFF)
	{
		if(zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_SIREN_GPIO_Port, RELAY_ZONE1_SIREN_Pin, GPIO_PIN_RESET);
		}
		else if(zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_SIREN_GPIO_Port, RELAY_ZONE2_SIREN_Pin, GPIO_PIN_RESET);
		}
	}
}

void buzz(uint8_t sta)
{
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, (GPIO_PinState)sta);
}

void alarm_output(uint8_t zone_num, uint8_t sta)
{
		if(demolition_sta == DISDEMOLITED) relay_alarm(zone_num, sta ^ 0x01);
		relay_siren(zone_num, sta);
		led_alarm(zone_num, sta);
//		buzz(sta);
}

//void alarm_detect_output(void)
//{
//	if((zone_struct.zone1_sta != DISARMING) && (zone_struct.zone1_sta != ARMING))
//	{
//		alarm_output(ZONE1, SET_ALARM);
//	}
//	else
//	{
//		alarm_output(ZONE1, RESET_ALARM);
//	}
//	
//	if((zone_struct.zone2_sta != DISARMING) && (zone_struct.zone2_sta != ARMING))
//	{
//		alarm_output(ZONE2, SET_ALARM);
//	}
//	else
//	{
//		alarm_output(ZONE2, RESET_ALARM);
//	}
//}

/***********************************************************************/
//*******函数名:demolition_detect()
//*******用途:检测有没有打开机箱门
//*******说明:
//*******参数:
//*******返回值:DEMOLITED:机箱门打开  DISDEMOLITED:机箱门未被打开
/***********************************************************************/
static uint8_t demolition_detect(void)
{
	uint8_t sta;	//拆除状态
	
	sta = (uint8_t)HAL_GPIO_ReadPin(DISMANTLE_DETECT_GPIO_Port, DISMANTLE_DETECT_Pin);
	
	return sta;
}

/***********************************************************************/
//*******函数名:demolition_detect()
//*******用途:检测有没有打开机箱门
//*******说明:
//*******参数:
//*******返回值:DEMOLITED:机箱门打开  DISDEMOLITED:机箱门未被打开
/***********************************************************************/

void demolition_detect_process(void)
{
	static uint8_t pre_demolition_sta = DISDEMOLITED;

	/*如果没到防拆检测计时，终止，不执行检测程序*/
	if(!demolition_detect_mask)
	{
		return;
	}
	
	demolition_detect_mask = 0;
	
	if((demolition_sta = demolition_detect()) != pre_demolition_sta)
	{
		pre_demolition_sta = demolition_sta;
		if(demolition_sta == DEMOLITED)
		{
			//当防区1没有其他告警时，开启（断开）防区1告警继电器
			demolition_alarm_reset_flag = 0;
			if(zone_struct.zone1_sta == DISARMING || zone_struct.zone1_sta == ARMING)
			{
				relay_alarm(ZONE1, RELAY_OFF);
			}
			led_dismantle(LED_ON);
		}
		else if(demolition_sta == DISDEMOLITED)
		{
			//当防区1没有其他告警时，关闭（闭合）防区1告警继电器
			demolition_alarm_reset_flag = 1;
		}
	}
}

