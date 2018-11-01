#include "alarm.h"

uint8_t demolition_detect_mask = 0; //��ʱ����ʱ�������־
uint8_t demolition_sta = DISDEMOLITED;

uint8_t zone1_alarm_reset_flag = 0;
uint8_t zone2_alarm_reset_flag = 0;
uint8_t demolition_alarm_reset_flag = 0;

uint32_t zone1_alarm_delay_cnt = 0;
uint32_t zone2_alarm_delay_cnt = 0;

uint32_t zone1_alarm_reset_time = 5000;
uint32_t zone2_alarm_reset_time = 5000;					//������ʱʱ�� ��λms ����趨999��
uint32_t demolition_alarm_reset_time = 5000; 		//���𱨾���ʱ  �̶�5��

uint8_t zone1_trigger_flag = 0;				//����־Ϊ1ʱ �����ڴ�����ʱ��  �����б������
uint8_t zone2_trigger_flag = 0;

uint32_t zone1_trigger_delay_cnt = 0;
uint32_t zone2_trigger_delay_cnt = 0;

uint32_t zone1_trigger_delay_time = 5000; 				//������ʱ��λ����  ����趨99��
uint32_t zone2_trigger_delay_time = 5000;

uint8_t zone1_sta_detect_flag = 0;							//״̬����־		�����������
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
		if (demolition_sta == DISDEMOLITED)			 //���û�з��𱨾�  ��Է���1�����̵�������Ӧ����
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
//*******������:demolition_detect()
//*******��;:�����û�д򿪻�����
//*******˵��:
//*******����:
//*******����ֵ:DEMOLITED:�����Ŵ�  DISDEMOLITED:������δ����
/***********************************************************************/
static uint8_t demolition_detect(void)
{
	uint8_t sta; //���״̬

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
			//������1û�������澯ʱ���������Ͽ�������1�澯�̵���
			demolition_alarm_reset_flag = 0;
			if (zone_struct.zone1_sta == DISARMING || zone_struct.zone1_sta == ARMING)
			{
				relay_alarm(ZONE1, RELAY_OFF);
			}
			led_dismantle(LED_ON);
		}
		else if (demolition_sta == DISDEMOLITED)
		{
			//������1û�������澯ʱ���رգ��պϣ�����1�澯�̵���
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


/*����״̬������*/
/*��Ļ��ʾ��Ӧ״̬������ʱ�̵������*/
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

