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

uint8_t ac_connected_flag = 2;

/***********************************************************************/
//*******������:relay_battery(uint8_t cmd)
//*******��;:������ر� ��ؿ��Ƽ̵���
//*******˵��:
//*******����:cmd��RELAY_ON :�����̵���  RELAY_OFF:�رռ̵���
//*******����ֵ:��
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
//*******������:uint8_t ac_detect(void)
//*******��;:����Ƿ��н��������
//*******˵��:�����������ʱ Ӧ��ѯ�����ʣ����� ��ص�������ʱ�Զ��ϵ�
//*******����:cmd��RELAY_ON :�����̵���  RELAY_OFF:�رռ̵���
//*******����ֵ:AC_CONNECTED:������������		AC_DISCONNECTED:�������ѶϿ�
/***********************************************************************/
uint8_t ac_detect(void)
{
	return HAL_GPIO_ReadPin(AC_DETECT_GPIO_Port, AC_DETECT_Pin);
}

/***********************************************************************/
//*******������:get_battery_voltage(void)
//*******��;:��ȡ��ǰ��ص�ѹ
//*******˵��:�����������ʱ Ӧ��ѯ�����ʣ����� ��ص�������ʱ�Զ��ϵ�
//*******����:
//*******����ֵ:��ص�ǰ��ѹֵ
/***********************************************************************/
float get_battery_voltage(void)
{
	ADC_ChannelConfTypeDef sConfig;
	float reference_voltage = 3.6f;		//�ο���ѹ
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
//*******������:battery_capacity_detect_processing(void)
//*******��;:���������� ������Ϊ5%ʱ�Ͽ����
//*******˵��:�����������ʱ Ӧ��ѯ�����ʣ����� ��ص�������ʱ�Զ��ϵ�
//*******����:
//*******����ֵ:��ص�ǰʣ������ٷֱ�
/***********************************************************************/
float battery_capacity_detect_processing(void)
{
	float battery_capacity;
	//
	
	return battery_capacity;
}


















