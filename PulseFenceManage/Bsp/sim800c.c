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

uint8_t sim800c_init_flag = 0;
uint8_t sim800c_link_flag = 0;
uint8_t uart6_rx_buff;
uint8_t process_step = 1;
uint8_t sim800c_try_connect_mask = 0;
uint8_t uart6_rx_start = 0;
uint8_t uart6_rx_time_cnt = 0;
uint8_t wait_tcp_contect_flag = 0;
uint16_t wait_tcp_contect_cnt = 0;
uint8_t sim800c_heart_mask = 0;
UART_RX_TypeDef uart6_rx_struct = {0};

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
		if(*(orig_cmd++) != *(comp_cmd++))
		{
			return 0;
		}
	}while((*comp_cmd) != '\n');
	
	do
	{
		if(*(orig_data++) != *(comp_data++))
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
//*******������:check_sim_signal_level()
//*******��;:���simcard�ź�ǿ�ȵȼ�
//*******˵��:
//*******����:
//*******����ֵ:�ź�ǿ��
/***********************************************************************/
static uint8_t check_sim_signal_level(void)		 //Ӧ���볢�����ɴ� ȡ�����Է���  ����
{
	uint8_t signal_level;
	uint8_t *orig_cmd;
	uint8_t *comp_cmd;
	uint8_t cmd[8] = "AT+CSQ\r\n";
	
	orig_cmd = uart6_rx_struct.rx_cmd;
	comp_cmd = cmd;
	
	do
	{
		HAL_UART_Transmit(&huart6,  "AT+CSQ\n", 7, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(50);
	}while(!uart6_rx_struct.rx_end);
	
	do
	{
		if(*(orig_cmd++) != *(comp_cmd++))
		{
			return 0;
		}
	}while((*comp_cmd) != '\n');
	
	if(uart6_rx_struct.rx_data[7] == ',')
	{
		return 0;
	}
	else
	{
		signal_level = uart6_rx_struct.rx_data[6] - 0x30;
		signal_level = (signal_level << 4) | (uart6_rx_struct.rx_data[7] - 0x30);
	}
	
	return signal_level;
}

/***********************************************************************/
//*******������:check_sim_network_registration()
//*******��;:�������ע��״̬
//*******˵����
//*******����:
//*******����ֵ��
/***********************************************************************/
static uint8_t check_sim_network_registration(void)
{
	uint8_t reg_sta;	//ע��״̬
	uint8_t *orig_cmd;
	uint8_t *comp_cmd;
	uint8_t cmd[10] = "AT+CREG?\r\n";
	
	orig_cmd = uart6_rx_struct.rx_cmd;
	comp_cmd = cmd;
	
	do
	{
		HAL_UART_Transmit(&huart6,  "AT+CREG?\n", 9, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(50);
	}while(!uart6_rx_struct.rx_end);
	
	do
	{
		if(*(orig_cmd++) != *(comp_cmd++))
		{
			return 0;
		}
	}while((*comp_cmd) != '\n');
	
	reg_sta = uart6_rx_struct.rx_data[9] - 0x30;
	
	if((reg_sta == 1) || (reg_sta == 5))
	{
		return 1;
	}

	return 0;
}

/***********************************************************************/
//*******������:check_sim_attach_gprs()
//*******��;:���GPRS����״̬
//*******˵����
//*******����:
//*******����ֵ��
/***********************************************************************/
static uint8_t check_sim_attach_gprs(void)
{
	uint8_t attach_sta;	//ע��״̬
	uint8_t *orig_cmd;
	uint8_t *comp_cmd;
	uint8_t cmd[11] = "AT+CGATT?\r\n";
	orig_cmd = uart6_rx_struct.rx_cmd;
	comp_cmd = cmd;
	
	do
	{
		HAL_UART_Transmit(&huart6,  "AT+CGATT?\n", 10, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(50);
	}while(!uart6_rx_struct.rx_end);
	
	do
	{
		if(*(orig_cmd++) != *(comp_cmd++))
		{
			return 0;
		}
	}while((*comp_cmd) != '\n');
	
	attach_sta = uart6_rx_struct.rx_data[8] - 0x30;
	
	return attach_sta;
}

static uint8_t set_transparent_mode(void)
{
	do
	{
		HAL_UART_Transmit(&huart6,  "AT+CIPMODE=1\n", 13, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(50);
	}while(!uart6_rx_struct.rx_end);
	
	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT+CIPMODE=1\r\n", "OK\r\n"))
	{
		return 0;
	}
	
	return 1;
}

////��ѯ��Ӫ��
////����ֵ 0����ѯʧ�� 1���й��ƶ� 2���й���ͨ
//static uint8_t inquire_operator(void)
//{
//	uint8_t i = 0;
//	char operator_name[20];	//��Ӫ������
//	uint8_t *orig_cmd;
//	uint8_t *comp_cmd;
//	uint8_t cmd[10] = "AT+COPS?\r\n";
//	
//	orig_cmd = uart6_rx_struct.rx_cmd;
//	comp_cmd = cmd;
//	
//	do
//	{
//		HAL_UART_Transmit(&huart6,  "AT+COPS?\n", 9, 1000);
//		uart6_rx_struct.rx_enable = 1;
//		uart6_rx_struct.rx_cnt = 0;
//		uart6_rx_struct.rx_end = 0;
//		uart6_rx_struct.cmd_data_flag = 0;
//		HAL_Delay(50);
//	}while(!uart6_rx_struct.rx_end);
//	
//	do
//	{
//		if(*(orig_cmd++) != *(comp_cmd++))
//		{
//			return 0;
//		}
//	}while((*comp_cmd) != '\n');
//	
//	do
//	{
//		*(operator_name+i) = *(uart6_rx_struct.rx_data+12+i);
//		i++;
//	}while(*(uart6_rx_struct.rx_data+12+i) != '\r');
//	
//	*(operator_name+i-1) = '\0';
//	
//	if(strcmp(operator_name, "CHN-UNICOM") == 0)
//	{
//		return CHINA_UNICOM;
//	}
//	else if(strcmp(operator_name, "CHINA MOBILE") == 0)
//	{
//		return CHINA_MOBILE;
//	}
//	
//	return 0;
//}

//static uint8_t set_sim_apn(uint8_t operator_cor)
//{
//	do
//	{
//		if(operator_cor == CHINA_MOBILE)
//		{
//			HAL_UART_Transmit(&huart6,  "AT+CSTT=\"CMNET\"\n", 16, 1000);
//		}
//		else if(operator_cor == CHINA_UNICOM)
//		{
//			HAL_UART_Transmit(&huart6,  "AT+CSTT=\"UNINET\"\n", 17, 1000);
//		}
//		
//		uart6_rx_struct.rx_enable = 1;
//		uart6_rx_struct.rx_cnt = 0;
//		uart6_rx_struct.rx_end = 0;
//		uart6_rx_struct.cmd_data_flag = 0;
//		HAL_Delay(50);
//	}while(!uart6_rx_struct.rx_end);
//	
//	if(operator_cor == CHINA_MOBILE)
//	{
//		if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT+CSTT=\"CMNET\"\r\n", "OK\r\n"))
//		{
//			return 0;
//		}
//	}
//	else if(operator_cor == CHINA_UNICOM)
//	{
//		if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT+CSTT=\"UNINET\"\r\n", "OK\r\n"))
//		{
//			return 0;
//		}		
//	}
//		
//	return 1;	
//}

//static uint8_t built_wireless_link(void)
//{
//	do
//	{
//		HAL_UART_Transmit(&huart6,  "AT+CIICR\n", 9, 1000);
//		uart6_rx_struct.rx_enable = 1;
//		uart6_rx_struct.rx_cnt = 0;
//		uart6_rx_struct.rx_end = 0;
//		uart6_rx_struct.cmd_data_flag = 0;
//		HAL_Delay(50);
//	}while(!uart6_rx_struct.rx_end);
//	
//	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "\r\n+CIICR", "OK\r\n"))		//������
//	{
//		return 0;
//	}
//	
//	return 1;
//}

static uint8_t built_tcp_link(void)
{
	char sim_tcp_addr[] = "AT+CIPSTART=\"TCP\",\"47.104.184.9\",\"22\"\r\n";
	
	do
	{
		HAL_UART_Transmit(&huart6, (uint8_t *)sim_tcp_addr, strlen(sim_tcp_addr), 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(50);
	}while(!uart6_rx_struct.rx_end);
	
	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, (uint8_t *)sim_tcp_addr, "OK\r\n"))		//������
	{
		return 0;
	}
	
	return 1;
}

static uint8_t sim800c_connect_tcp(void)
{
	
//	switch(inquire_operator())
//	{
//		case 0: return 0;
//		case CHINA_MOBILE:
//			if(!set_sim_apn(CHINA_MOBILE))
//			{
//				return 0;
//			}
//			break;
//		case CHINA_UNICOM:
//			if(!set_sim_apn(CHINA_UNICOM))
//			{
//				return 0;
//			}
//			break;
//		default: break;
//	}
	
//	if(!built_wireless_link())
//	{
//		return 0;
//	}
	
	if(!built_tcp_link())
	{
		return 0;
	}
	
	return 1;
}

static uint8_t sim800c_close_tcp(void)
{
	do
	{
		HAL_UART_Transmit(&huart6, "AT+CIPCLOSE\r\n", 13, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(50);
	}while(!uart6_rx_struct.rx_end);
	
//	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT+CIPCLOSE\r\n", "CLOSE OK\r\n"))
//	{
//		return 0;
//	}
	
	return 1;
}

//��ʼ��sim800c
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
	
	return 1;
}

/***********************************************************************/
//*******������:sim800c_process()
//*******��;:sim800c���̴�����
//*******˵��:
//*******����:
//*******����ֵ��
/***********************************************************************/
void sim800c_process(void)
{
	if(!sim800c_init_flag)
	{
		return;
	}
	
	if(sim800c_link_flag)
	{
		if(sim800c_heart_mask)
		{
			sim800c_heart_mask = 0;
			if(!wait_tcp_contect_flag)
			{
//				HAL_UART_Transmit(&huart6, "HEART\n", 6, 1000);
			}
		}
		return;
	}
	
	if(!sim800c_try_connect_mask)
	{
		return;
	}
	
	sim800c_try_connect_mask = 0;
	
	if(process_step == 1)
	{
		if(check_sim_exist())
		{
			process_step = 2;
		}
		else
		{
			process_step = 0;		//���û�忨��ֹ
		}
	}
	
	if(process_step == 2)
	{
		if(check_sim_signal_level() >= 15)
		{
			process_step = 3;
		}		
	}
	
	if(process_step == 3)
	{
		if(check_sim_network_registration())
		{
			process_step = 4;
		}			
	}
	
	if(process_step == 4)
	{
		if(check_sim_attach_gprs())
		{
			process_step = 5;
		}		
	}
	
	if(process_step == 5)
	{
		if(set_transparent_mode())
		{
			process_step = 6;
		}
	}
	
	if(process_step == 6)
	{
		sim800c_link_flag = sim800c_connect_tcp();
		if(sim800c_link_flag)
		{
			wait_tcp_contect_flag = 1;
			uart6_rx_struct.rx_enable = 1;
			uart6_rx_struct.rx_cnt = 0;
			uart6_rx_struct.rx_end = 0;
			uart6_rx_struct.cmd_data_flag = 1;
		}
	}
}

void transparent_data_processing(void)
{
	if(strcmp((char *)uart6_rx_struct.rx_data, "\r\nCLOSED\r\n") == 0)				//tcp�������ر� ���µ�tcp�Ͽ�����
	{
		process_step = 6;
		sim800c_link_flag = 0;
	}
	
	if(strcmp((char *)uart6_rx_struct.rx_data, "\r\nCONNECT\r\n") == 0)
	{
		wait_tcp_contect_flag = 0;
		HAL_UART_Transmit(&huart6, "Pulse: Connect!\r\n", 17, 1000);
	}
	
	if(strcmp((char *)uart6_rx_struct.rx_data, "\r\n+PDP: DEACT\r\n") == 0)		//�����;���sim��  �����źŹ��� ��������ԭ��Ͽ�tcp����
	{
		sim800c_link_flag = 0;
		
		if(check_sim_exist())
		{
			if(sim800c_close_tcp())
			{
				process_step = 6;	//
			}
			else
			{
				process_step = 0;
			}
		}
		else
		{
			process_step = 0;		//���û�忨��ֹ
		}
	}
}


















