#include "alarm.h"

uint8_t demolition_detect_mask = 0; //��ʱ����ʱ�������־ 
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
//*******������:demolition_detect()
//*******��;:�����û�д򿪻�����
//*******˵��:
//*******����:
//*******����ֵ:DEMOLITED:�����Ŵ�  DISDEMOLITED:������δ����
/***********************************************************************/
static uint8_t demolition_detect(void)
{
	uint8_t sta;	//���״̬
	
	sta = (uint8_t)HAL_GPIO_ReadPin(DISMANTLE_DETECT_GPIO_Port, DISMANTLE_DETECT_Pin);
	
	return sta;
}

/***********************************************************************/
//*******������:demolition_detect()
//*******��;:�����û�д򿪻�����
//*******˵��:
//*******����:
//*******����ֵ:DEMOLITED:�����Ŵ�  DISDEMOLITED:������δ����
/***********************************************************************/

void demolition_detect_process(void)
{
	static uint8_t pre_demolition_sta = DISDEMOLITED;

	/*���û���������ʱ����ֹ����ִ�м�����*/
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
			//������1û�������澯ʱ���������Ͽ�������1�澯�̵���
			demolition_alarm_reset_flag = 0;
			if(zone_struct.zone1_sta == DISARMING || zone_struct.zone1_sta == ARMING)
			{
				relay_alarm(ZONE1, RELAY_OFF);
			}
			led_dismantle(LED_ON);
		}
		else if(demolition_sta == DISDEMOLITED)
		{
			//������1û�������澯ʱ���رգ��պϣ�����1�澯�̵���
			demolition_alarm_reset_flag = 1;
		}
	}
}

