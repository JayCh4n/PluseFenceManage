#include "alarm.h"

uint8_t demolition_detect_mask = 0; //定时器计时防拆检测标志
uint8_t demolition_sta = DISDEMOLITED;

uint8_t zone1_alarm_reset_flag = 0;
uint8_t zone2_alarm_reset_flag = 0;
uint8_t demolition_alarm_reset_flag = 0;

uint32_t zone1_alarm_delay_cnt = 0;
uint32_t zone2_alarm_delay_cnt = 0;

uint32_t zone1_alarm_reset_time = 5000;
uint32_t zone2_alarm_reset_time = 5000;					//报警延时时间 单位ms 最大设定999秒
uint32_t demolition_alarm_reset_time = 5000; 		//防拆报警延时  固定5秒

uint8_t zone1_trigger_flag = 0;				//当标志为1时 代表在触发计时中  不进行报警输出
uint8_t zone2_trigger_flag = 0;

uint32_t zone1_trigger_delay_cnt = 0;
uint32_t zone2_trigger_delay_cnt = 0;

uint32_t zone1_trigger_delay_time = 5000; 				//触发延时单位毫秒  最大设定99秒
uint32_t zone2_trigger_delay_time = 5000;

uint8_t zone1_sta_detect_flag = 0;							//状态检测标志		与灵敏度相关
uint8_t zone2_sta_detect_flag = 0;

void relay_alarm(uint8_t zone_num, uint8_t cmd)
{
	if (cmd == RELAY_ON)
	{
		if (zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_ALARM_GPIO_Port, RELAY_ZONE1_ALARM_Pin, GPIO_PIN_SET);
		}
		else if (zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_ALARM_GPIO_Port, RELAY_ZONE2_ALARM_Pin, GPIO_PIN_SET);
		}
	}
	else if (cmd == RELAY_OFF)
	{
		if (zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_ALARM_GPIO_Port, RELAY_ZONE1_ALARM_Pin, GPIO_PIN_RESET);
		}
		else if (zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_ALARM_GPIO_Port, RELAY_ZONE2_ALARM_Pin, GPIO_PIN_RESET);
		}
	}
}

void relay_siren(uint8_t zone_num, uint8_t cmd)
{
	if (cmd == RELAY_ON)
	{
		if (zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_SIREN_GPIO_Port, RELAY_ZONE1_SIREN_Pin, GPIO_PIN_SET);
		}
		else if (zone_num == 2)
		{
			HAL_GPIO_WritePin(RELAY_ZONE2_SIREN_GPIO_Port, RELAY_ZONE2_SIREN_Pin, GPIO_PIN_SET);
		}
	}
	else if (cmd == RELAY_OFF)
	{
		if (zone_num == 1)
		{
			HAL_GPIO_WritePin(RELAY_ZONE1_SIREN_GPIO_Port, RELAY_ZONE1_SIREN_Pin, GPIO_PIN_RESET);
		}
		else if (zone_num == 2)
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
	if(zone_num == ZONE1)
	{
		if (demolition_sta == DISDEMOLITED)			 //如果没有防拆报警  则对防区1报警继电器做相应处理
			relay_alarm(zone_num, sta ^ 0x01);
	}
	else
	{
		relay_alarm(zone_num, sta ^ 0x01);
	}

	relay_siren(zone_num, sta);
	led_alarm(zone_num, sta);
	buzz(sta);
}

/***********************************************************************/
//*******函数名:demolition_detect()
//*******用途:检测有没有打开机箱门
//*******说明:
//*******参数:
//*******返回值:DEMOLITED:机箱门打开  DISDEMOLITED:机箱门未被打开
/***********************************************************************/
static uint8_t demolition_detect(void)
{
	uint8_t sta; //拆除状态

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
	if (!demolition_detect_mask)
	{
		return;
	}

	demolition_detect_mask = 0;

	if ((demolition_sta = demolition_detect()) != pre_demolition_sta)
	{
		pre_demolition_sta = demolition_sta;
		if (demolition_sta == DEMOLITED)
		{
			//当防区1没有其他告警时，开启（断开）防区1告警继电器
			demolition_alarm_reset_flag = 0;
			if (zone_struct.zone1_sta == DISARMING || zone_struct.zone1_sta == ARMING)
			{
				relay_alarm(ZONE1, RELAY_OFF);
			}
			led_dismantle(LED_ON);
		}
		else if (demolition_sta == DISDEMOLITED)
		{
			//当防区1没有其他告警时，关闭（闭合）防区1告警继电器
			demolition_alarm_reset_flag = 1;
		}
	}
}

void zone1_alarm_sta_detect(void)
{
	if(zone1_trigger_flag)
	{
		return;
	}
	
	if(!zone1_sta_detect_flag)
	{
		return;
	}
	zone1_sta_detect_flag = 0;
	
	if (zone_struct.zone1_sta != pre_zone1_sta)
	{
		lcd_show_zone1_sta();
		lcd_show_zone1_voltage();
		if ((zone_struct.zone1_sta != DISARMING) && (zone_struct.zone1_sta != ARMING))
		{
			if ((pre_zone1_sta == DISARMING) || (pre_zone1_sta == ARMING))
			{
				alarm_output(ZONE1, SET_ALARM);
				zone1_alarm_reset_flag = 0;
				zone1_alarm_delay_cnt = 0;
			}
		}
		else if ((pre_zone1_sta != DISARMING) && (pre_zone1_sta != ARMING))
		{
			if (zone_struct.zone1_sta == ARMING)
			{
				zone1_alarm_reset_flag = 1;
				zone1_alarm_delay_cnt = 0;
			}
			else if (zone_struct.zone1_sta == DISARMING)
			{
				alarm_output(ZONE1, RESET_ALARM);
				zone1_alarm_reset_flag = 0;
				zone1_alarm_delay_cnt = 0;
			}
		}
		pre_zone1_sta = zone_struct.zone1_sta;
	}
}

void zone2_alarm_sta_detect(void)
{
	if(zone2_trigger_flag)
	{
		return;
	}
	
	if(!zone2_sta_detect_flag)
	{
		return;
	}
	zone2_sta_detect_flag = 0;
	
	if (zone_struct.zone2_sta != pre_zone2_sta)
	{
		lcd_show_zone2_sta();
		lcd_show_zone2_voltage();
		if ((zone_struct.zone2_sta != DISARMING) && (zone_struct.zone2_sta != ARMING))
		{
			if ((pre_zone2_sta == DISARMING) || (pre_zone2_sta == ARMING))
			{
				alarm_output(ZONE2, SET_ALARM);
				zone2_alarm_reset_flag = 0;
				zone2_alarm_delay_cnt = 0;
			}
		}
		else if ((pre_zone2_sta != DISARMING) && (pre_zone2_sta != ARMING))
		{
			if (zone_struct.zone2_sta == ARMING)
			{
				zone2_alarm_reset_flag = 1;
				zone2_alarm_delay_cnt = 0;
			}
			else if (zone_struct.zone2_sta == DISARMING)
			{
				alarm_output(ZONE2, RESET_ALARM);
				zone2_alarm_reset_flag = 0;
				zone2_alarm_delay_cnt = 0;
			}
		}
		pre_zone2_sta = zone_struct.zone2_sta;
	}
}


/*主机状态检测进程*/
/*屏幕显示对应状态，报警时继电器输出*/
void alarm_sta_detect_process(void)
{
	zone1_alarm_sta_detect();
	zone2_alarm_sta_detect();
}
/*
	if(zone_struct_set_buff.zone1_sta != zone_struct.zone1_sta)
	{

		if((zone_struct_set_buff.zone1_sta != DISARMING) && (zone_struct_set_buff.zone1_sta != ARMING))
		{
			alarm_output(ZONE1, SET_ALARM);
			zone1_alarm_reset_flag = 0;
			zone1_alarm_delay_cnt = 0;
		}
		else if((zone_struct.zone1_sta != DISARMING) && (zone_struct.zone1_sta != ARMING))
		{
			if(zone_struct_set_buff.zone1_sta == DISARMING)
			{
				alarm_output(ZONE1, RESET_ALARM);
				zone1_alarm_reset_flag = 0;
				zone1_alarm_delay_cnt = 0;
			}
			else
			{
				zone1_alarm_reset_flag = 1;
				zone1_alarm_delay_cnt = 0;
			}
		}
		
		zone_struct.zone1_sta = zone_struct_set_buff.zone1_sta;
*/

