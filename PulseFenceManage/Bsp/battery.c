/**
  ******************************************************************************
  * File Name          : battery.c
  * Description        : ��ع��ܺ���
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "battery.h"

extern ADC_HandleTypeDef hadc1;
extern uint8_t battery_icon_32x16[6][64];

uint8_t bettery_manage_mask = 0;  //��ع���ʱ����־
uint8_t charged_flag = 1;		//ÿ��һ�ε�  �ı�������1  ��ֹ��������
//uint8_t ac_connected_flag = 2;

/***********************************************************************/
//*******������:relay_battery(uint8_t cmd)
//*******��;:�����ر� ��ؿ��Ƽ̵���  ��������ʱ�رռ̵��� ����͹���ģʽ
//*******˵��:
//*******����:cmd��RELAY_ON :����̵���  RELAY_OFF:�رռ̵���
//*******����ֵ:��
/***********************************************************************/
//void relay_battery(uint8_t cmd)
//{
//	if(cmd == RELAY_ON)
//	{
//		HAL_GPIO_WritePin(RELAY_BAT_GPIO_Port, RELAY_BAT_Pin, GPIO_PIN_SET);
//	}
//	else if(cmd == RELAY_OFF)
//	{
//		HAL_GPIO_WritePin(RELAY_BAT_GPIO_Port, RELAY_BAT_Pin, GPIO_PIN_RESET);
//	}
//}

/***********************************************************************/
//*******������:uint8_t ac_detect(void)
//*******��;:����Ƿ��н��������
//*******˵��:����������ʱ Ӧ��ѯ�����ʣ����� ��ص�������ʱ�Զ��ϵ�
//*******����:cmd��RELAY_ON :����̵���  RELAY_OFF:�رռ̵���
//*******����ֵ:AC_CONNECTED:������������		AC_DISCONNECTED:�������ѶϿ�
/***********************************************************************/
uint8_t ac_detect(void)
{
	ADC_ChannelConfTypeDef sConfig;
	float reference_voltage = 1.24f;		//�ο���ѹ
//	uint16_t max_value = 4096;
	uint16_t get_value;
	uint16_t vref_value;
	float get_voltage;
	
	sConfig.Channel = ADC_CHANNEL_0;
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
	
	if(get_voltage >= 2.5f)
	{
		return AC_CONNECTED;
	}
	else
	{
		return AC_DISCONNECTED;
	}
	
//	return (uint8_t)HAL_GPIO_ReadPin(AC_DETECT_GPIO_Port, AC_DETECT_Pin);
}

/***********************************************************************/
//*******������:get_battery_voltage(void)
//*******��;:��ȡ��ǰ��ص�ѹ
//*******˵��:����������ʱ Ӧ��ѯ�����ʣ����� ��ص�������ʱ�Զ��ϵ�
//*******����:
//*******����ֵ:��ص�ǰ��ѹֵ
/***********************************************************************/
float get_battery_voltage(void)
{
	ADC_ChannelConfTypeDef sConfig;
	float reference_voltage = 1.24f;		//�ο���ѹ
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
	
	battery_voltage = get_voltage * 4.66666666f;
	
	return battery_voltage;
}

/***********************************************************************/
//*******������:get_battery_capacity(void)
//*******��;:���������� ������Ϊ5%ʱ�Ͽ����
//*******˵��:����������ʱ Ӧ��ѯ�����ʣ����� ��ص�������ʱ�Զ��ϵ�
//*******����:
//*******����ֵ:��ص�ǰʣ������ٷֱ�
/***********************************************************************/
uint8_t get_battery_capacity(void)
{
	uint8_t i;
	float battery_voltage = 0.0f;
	static float pre_battery_voltage = 0.0f;			//ǰһ�μ���ص���%
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
	
	//������������  ������Ժ�  ���¼��
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
//*******������:get_charge_sta(void)
//*******��;:��ȡ���״̬  ���ͼ���������
//*******˵��:�����������ʱ �����е�ص������
//*******����:
//*******����ֵ:CHARGEING�����ڳ��  DISCHARGE��û���ڳ��
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
	
	//��ʱδ�� ��ֹ
	if(!bettery_manage_mask)
	{
		return;
	}
	
	bettery_manage_mask = 0;
	
	if(ac_detect() == AC_CONNECTED)		//������������
	{
		if(page_sta == IN_MAIN_PAGE)
		{
			if(get_charge_sta() == CHARGEING)	//������ڳ��
			{
				charged_flag = 1;
				lcd_show_32x16(1,209,battery_icon_32x16[bettery_icon_mask]);
				if(++bettery_icon_mask >= 6)
				{
					bettery_icon_mask = 0;
				}
			}
			else			//���û�ڳ��
			{
				clear_screen(1, 209, 2, 32);
			}
		}
	}
	else			//���������δ����
	{
		if(page_sta == IN_MAIN_PAGE)
		{
			battery_capacity = get_battery_capacity();
			bettery_icon_mask = battery_capacity / 20;
			lcd_show_32x16(1,209,battery_icon_32x16[bettery_icon_mask]);
		}
	}
}


















