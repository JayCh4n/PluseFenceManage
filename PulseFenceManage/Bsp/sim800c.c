/**
  ******************************************************************************
  * File Name          : sim800c.c
  * Description        : sim800c����������
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "sim800c.h"

uint8_t uart6_rx_buff;
UART_RX_TypeDef uart6_rx_struct = {0,0,0,0};

extern UART_HandleTypeDef huart6;
/***********************************************************************/
//*******������:check_sim800c_cmd()
//*******��;:�Ƚ�sim800c���ص������״̬
//*******˵��:
//*******����:orig_cmd��ԭ�����ַ��� orig_data:ԭ���ݣ�״̬���ַ���  comp_cmd���Ƚ������ַ��� comp_data���Ƚ������ַ���
//*******����ֵ:0��ʧ�ܣ�1���ɹ�
/***********************************************************************/
static uint8_t check_sim800c_cmd(uint8_t *orig_cmd, uint8_t *orig_data, uint8_t *comp_cmd, uint8_t *comp_data)
{
	do
	{
		if(*(++orig_cmd) != *(++comp_cmd))
		{
			return 0;
		}
	}while((*comp_cmd) != '\n');
	
	do
	{
		if(*(++orig_data) != *(++comp_data))
		{
			return 0;
		}
	}while((*comp_data) != '\n');
	
	return 1;
}

/***********************************************************************/
//*******������:sim800c_pwr()
//*******��;:sim800c����
//*******˵��:����
//*******����:wait_time:�����ȴ���ʱʱ��
//*******����ֵ:0����������ʧ�ܣ�1���ɹ�
/***********************************************************************/
static uint8_t sim800c_pwr(int8_t wait_time)
{
	do
	{	
		HAL_GPIO_WritePin(SIM800_PWR_GPIO_Port, SIM800_PWR_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(SIM800_PWR_GPIO_Port, SIM800_PWR_Pin, GPIO_PIN_RESET);
		
		if(--wait_time < 0)
		{
			return 0;		//��ʼ��ʧ��
		}
		HAL_Delay(2500);
	}while(HAL_GPIO_ReadPin(SIM800_STATUS_GPIO_Port, SIM800_STATUS_Pin) == GPIO_PIN_RESET);	//���
	
	return 1;
}

/***********************************************************************/
//*******������:sim800c_sync_uart_baud()
//*******��;:ͬ��������
//*******˵��:
//*******����:wait_time:�ȴ���ʱʱ��
//*******����ֵ:ʧ�ܣ�1���ɹ�
/***********************************************************************/

static uint8_t sim800c_sync_uart_baud(int8_t wait_time)
{
	do
	{
		if(--wait_time < 0)
		{
			return 0;		//��ʼ��ʧ��
		}
		HAL_UART_Transmit(&huart6, "AT\n", 3, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(1000);
	}while(!uart6_rx_struct.rx_end);	//���
	
	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT\r\n", "OK\r\n"))
	{
		return 0;
	}
	
	return 1;
}

//���sim���Ƿ���� 0�������� 1������
static uint8_t check_sim_exist(void)
{
	do
	{
	HAL_UART_Transmit(&huart6,  "AT+CPIN?\n", 9, 1000);
	uart6_rx_struct.rx_enable = 1;
	uart6_rx_struct.rx_cnt = 0;
	uart6_rx_struct.rx_end = 0;
	uart6_rx_struct.cmd_data_flag = 0;
	HAL_Delay(50);	
	}while(!uart6_rx_struct.rx_end);
	
	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT+CPIN?\r\n", "+CPIN: READY\r\n"))
	{
		return 0;
	}
	
	return 1;
}

/***********************************************************************/
//*******������:sim800c_init()
//*******��;:sim800cģ���ʼ��
//*******˵��:����
//*******����:wait_time:��ʼ���ȴ���ʱʱ��
//*******����ֵ:0����ʼ����������ʧ�ܣ�1���ɹ�
/***********************************************************************/
uint8_t sim800c_init(int8_t pwr_wait_time, int8_t baud_wait_time)
{
	if(!sim800c_pwr(pwr_wait_time))
	{
		return 0;
	}
	HAL_Delay(2500);
	if(!sim800c_sync_uart_baud(baud_wait_time))
	{
		return 0;
	}
	
	if(!check_sim_exist())
	{
		return 0;
	}
	
	return 1;
}




