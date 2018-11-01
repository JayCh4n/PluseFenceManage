/**
  ******************************************************************************
  * File Name          : max485.c
  * Description        : 485芯片驱动
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "max485.h"

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern uint8_t uart1_rx_buff;

uint8_t max485_1_sendbuf[50];
uint8_t max485_1_receive_data[50];
uint8_t max485_1_receivebuf;
uint8_t max485_wait_usart1_flag = 0;
uint8_t max485_wait_usart1_finish = 0;

/*  */
uint16_t CRC16(uint8_t *arr_buff, uint8_t len)  
{
	uint16_t  crc=0xFFFF;
	uint8_t i, j;
	for(j=0;j<len;j++)
	{
		crc=crc^*(arr_buff+j);
		for ( i=0; i<8; i++)
		{
			if((crc&0x0001)>0)
			{
				crc>>=1;
				crc^=0xA001;
			}
			else
				crc>>=1;			
		}
	}
	return (crc);
}

void max485_send_str(uint8_t device, uint8_t *str, uint16_t lenth)
{
	if(device == MAX485_1)
	{
		HAL_GPIO_WritePin(RS485_CNT1_GPIO_Port, RS485_CNT1_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart2, str, lenth, 1000);
		HAL_GPIO_WritePin(RS485_CNT1_GPIO_Port, RS485_CNT1_Pin, GPIO_PIN_RESET);
	}
}

static void max_485_1_return_master_msg(void)
{
	uint16_t crc;
	
	max485_1_sendbuf[0] = 0xC8;
	max485_1_sendbuf[1] = 0xD9;
	max485_1_sendbuf[2] = 0xE7;
	max485_1_sendbuf[3] = 0xD9;										//数据头
	max485_1_sendbuf[4] = 0x02;										//设备类型
	max485_1_sendbuf[5] = zone_struct.zone1_id;		//设备ID（防区1ID）
	max485_1_sendbuf[6] = 0x01; 									//功能码
	max485_1_sendbuf[7] = 0x19;										//数据长度
	
	max485_1_sendbuf[8] = zone_struct.zone_type;	//
	max485_1_sendbuf[9] = zone_struct.zone1_id;
	max485_1_sendbuf[10] = zone_struct.zone2_id;
	
	max485_1_sendbuf[11] = zone_struct.zone1_sta <= 3 ? 1:zone_struct.zone1_sta - 2;
	max485_1_sendbuf[12] = zone_struct.zone2_sta <= 3 ? 1:zone_struct.zone2_sta - 2;
	
//	max485_1_sendbuf[11] = zone_struct.zone1_sta;
//	max485_1_sendbuf[12] = zone_struct.zone2_sta;
	max485_1_sendbuf[13] = demolition_sta;
	max485_1_sendbuf[14] = zone_struct.zone1_voltage_level;
	max485_1_sendbuf[15] = zone_struct.zone2_voltage_level;
	max485_1_sendbuf[16] = zone_struct.zone1_sensitivity;
	max485_1_sendbuf[17] = zone_struct.zone2_sensitivity;
	max485_1_sendbuf[18] = 4;											//防区属性（非延时防区）
	max485_1_sendbuf[19] = zone_struct.zone1_mode;
	max485_1_sendbuf[20] = zone_struct.zone2_mode;
	
	max485_1_sendbuf[21] = IP_ADDRESS[0];
	max485_1_sendbuf[22] = IP_ADDRESS[1];
	max485_1_sendbuf[23] = IP_ADDRESS[2];
	max485_1_sendbuf[24] = IP_ADDRESS[3];
	
	max485_1_sendbuf[25] = NETMASK_ADDRESS[0];
	max485_1_sendbuf[26] = NETMASK_ADDRESS[1];
	max485_1_sendbuf[27] = NETMASK_ADDRESS[2];
	max485_1_sendbuf[28] = NETMASK_ADDRESS[3];
	
	max485_1_sendbuf[29] = GATEWAY_ADDRESS[0];
	max485_1_sendbuf[30] = GATEWAY_ADDRESS[1];
	max485_1_sendbuf[31] = GATEWAY_ADDRESS[2];
	max485_1_sendbuf[32] = GATEWAY_ADDRESS[3];	
	
	crc = CRC16(max485_1_sendbuf, 33);
	
	max485_1_sendbuf[33] = crc >> 8;
	max485_1_sendbuf[34] = crc;
	
	max485_1_sendbuf[35] = 0xF8;
	max485_1_sendbuf[36] = 0xC6;
	max485_1_sendbuf[37] = 0xF8;
	max485_1_sendbuf[38] = 0xC6;
	
	max485_send_str(MAX485_1, max485_1_sendbuf, 39);
}

///*返回设置成功*/
//static void max_485_1_return_set_ok(uint8_t cmd)
//{
//	uint16_t crc;
//	uint32_t i;
//	
//	max485_wait_usart1_flag = 1;
//	while(!max485_wait_usart1_finish)
//	{
//		if(++i >= 70000000)
//		{
//			max485_wait_usart1_flag = 0;
//			break;
//		}
//		HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
//	};
//	
//	max485_1_sendbuf[0] = 0xC8;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;									//数据头
//	max485_1_sendbuf[4] = 0x02;									//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = cmd; 									//功能码
//	max485_1_sendbuf[7] = 1;										//数据长度
//	
//	max485_1_sendbuf[8] = 0xA9;	//
//	
//	crc = CRC16(max485_1_sendbuf, 9);
//	
//	max485_1_sendbuf[9] = crc >> 8;
//	max485_1_sendbuf[10] = crc;
//	
//	max485_1_sendbuf[11] = 0xF8;
//	max485_1_sendbuf[12] = 0xC6;
//	max485_1_sendbuf[13] = 0xF8;
//	max485_1_sendbuf[14] = 0xC6;
//	
//	if(max485_wait_usart1_finish)
//	{
//		max485_wait_usart1_flag = 0;
//		max485_send_str(MAX485_1, max485_1_sendbuf, 15);
//	}
//	
//	max485_wait_usart1_finish = 0;
//}

///*返回设置IP成功*/
//static void max_485_1_return_ip_set_ok(void)
//{
//	uint16_t crc;

//	max485_1_sendbuf[0] = 0xC8;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;									//数据头
//	max485_1_sendbuf[4] = 0x02;									//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = 21; 									//功能码
//	max485_1_sendbuf[7] = 1;										//数据长度
//	
//	max485_1_sendbuf[8] = 0xA9;	//
//	
//	crc = CRC16(max485_1_sendbuf, 9);
//	
//	max485_1_sendbuf[9] = crc >> 8;
//	max485_1_sendbuf[10] = crc;
//	
//	max485_1_sendbuf[11] = 0xF8;
//	max485_1_sendbuf[12] = 0xC6;
//	max485_1_sendbuf[13] = 0xF8;
//	max485_1_sendbuf[14] = 0xC6;
//	
//	max485_send_str(MAX485_1, max485_1_sendbuf, 15);
//}

void max_485_1_deal(uint8_t *data_pakge)
{
	uint32_t i;
	
	master_ctrl_cmd_def cmd;
//	uint8_t zone_num; 													// 0:双防区 1：1防区  2：2防区
	uint16_t alarm_delay_s;	//报警延时 单位s
	uint32_t alarm_delay_ms;
	
	communication_sta = COMMUNICATING;
	cmd = (master_ctrl_cmd_def)data_pakge[6];
		
	switch(cmd)
	{
		case INQUIRE_MSTAER_MAG:	
			max_485_1_return_master_msg(); 
			break;
		case MODIFY_ZONE_TYPE_ID:
			if(zone_struct.zone1_arm_sta || zone_struct.zone2_arm_sta)		//必须在两个防区都撤防的情况下 才能设定
				break;
			if(data_pakge[8] == 2)		//如果双防区
			{
				temp_zone1_id = data_pakge[9];
				temp_zone2_id = data_pakge[10];
			}
			else if(data_pakge[8] == 1)	//如果单防区
			{
				temp_zone1_id = data_pakge[9];
			}
			set_ctrl_unit(SINGLE_DOUBLE_ZONE, 0xFF, data_pakge[8]);
//			max_485_1_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_LOCAL_IP:
			for(i=0; i<4; i++)
			{
				IP_ADDRESS[i] = data_pakge[8+i];
				NETMASK_ADDRESS[i] = data_pakge[12+i];
				GATEWAY_ADDRESS[i] = data_pakge[16+i];
				local_address_set_buff[i] = IP_ADDRESS[i];
				subnet_mask_set_buff[i] = NETMASK_ADDRESS[i];
				gateway_address_set_buff[i] = GATEWAY_ADDRESS[i];
				flash_data_struct.flash_local_ip[i] = IP_ADDRESS[i];
				flash_data_struct.flash_netmask[i] = NETMASK_ADDRESS[i];
				flash_data_struct.flash_gate_way[i] = GATEWAY_ADDRESS[i];
				write_flash_time_cnt = 0;
				write_flash_flag = 1;
			}
			local_network_set(IP_ADDRESS, NETMASK_ADDRESS, GATEWAY_ADDRESS);
//			max_485_1_return_ip_set_ok();
			break;
		case MODIFY_VOLTAGE_LAVEL:
			if(zone_struct.zone1_arm_sta || zone_struct.zone2_arm_sta) 						//如果在布防状态下
			{
				//低压：0  高压：1
				if(data_pakge[9] == 0x01)
				{
					set_ctrl_unit(HIGH_LOW_VOLTAGE, 0x01, data_pakge[8]);
				}
				else if(data_pakge[9] == 0x02)
				{
					if(zone_struct.zone_type == DOUBLE_ZONE)
						set_ctrl_unit(HIGH_LOW_VOLTAGE, 0x02, data_pakge[8]);
					else
						break;
				}
				else if(data_pakge[9] == 0xFF)
				{
					if(zone_struct.zone_type == DOUBLE_ZONE)
						set_ctrl_unit(HIGH_LOW_VOLTAGE, 0xFF, data_pakge[8]);
					else
						set_ctrl_unit(HIGH_LOW_VOLTAGE, 0x01, data_pakge[8]);
				}
//				max_485_1_return_set_ok((uint8_t)cmd);
			}
			break;
		case MODIFY_ZONE_SENSITIVITY:
			if(data_pakge[8] == 0 || data_pakge[8] >= 4)
			{
				break;
			}
			if(data_pakge[9] == 0x01)
			{
					set_ctrl_unit(SENSITIVITY, 0x01, data_pakge[8]);
			}
			else if(data_pakge[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(SENSITIVITY, 0x02, data_pakge[8]);
				else
					break;
			}
			else if(data_pakge[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(SENSITIVITY, 0xFF, data_pakge[8]);
				else
					set_ctrl_unit(SENSITIVITY, 0x01, data_pakge[8]);
			}
//			max_485_1_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_ALARM_DELAY:
			alarm_delay_s = data_pakge[8];
			alarm_delay_s = (alarm_delay_s << 8) | data_pakge[9];
			alarm_delay_ms = alarm_delay_s * 1000;
		
			if(data_pakge[10] == 0x01)
			{
				zone1_alarm_reset_time = alarm_delay_ms;
			}
			else if(data_pakge[10] == 0x02)
			{
				zone2_alarm_reset_time = alarm_delay_ms;
			}
			else if(data_pakge[10] == 0xFF)
			{
				zone1_alarm_reset_time = zone2_alarm_reset_time = alarm_delay_ms;
			}
			flash_data_struct.flash_zone1_alarm_reset_time = zone1_alarm_reset_time;
			flash_data_struct.flash_zone2_alarm_reset_time = zone2_alarm_reset_time;
			write_flash_time_cnt = 0;
			write_flash_flag = 1;
//			max_485_1_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_TRIGGER_DELAY:
			alarm_delay_s = data_pakge[8];
			alarm_delay_s = (alarm_delay_s << 8) | data_pakge[9];
			alarm_delay_ms = alarm_delay_s * 1000;
		
			if(data_pakge[10] == 0x01)
			{
				zone1_trigger_delay_time = alarm_delay_ms;
			}
			else if(data_pakge[10] == 0x02)
			{
				zone2_trigger_delay_time = alarm_delay_ms;
			}
			else if(data_pakge[10] == 0xFF)
			{
				zone1_trigger_delay_time = zone2_trigger_delay_time = alarm_delay_ms;
			}
			
			flash_data_struct.flash_zone1_trigger_delay_time = zone1_trigger_delay_time;
			flash_data_struct.flash_zone2_trigger_delay_time = zone2_trigger_delay_time;
			write_flash_time_cnt = 0;
			write_flash_flag = 1;
//			max_485_1_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_ARM_DISARM:
			//0:撤防  1：布防
			if(data_pakge[9] == 0x01)
			{
				set_ctrl_unit(AMING_DISARM, 0x01, data_pakge[8]);
			}
			else if(data_pakge[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(AMING_DISARM, 0x02, data_pakge[8]);
				else
					break;
			}
			else if(data_pakge[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(AMING_DISARM, 0xFF, data_pakge[8]);
				else
					set_ctrl_unit(AMING_DISARM, 0x01, data_pakge[8]);
			}
//			max_485_1_return_set_ok((uint8_t)cmd);
			break;
		case TMING_CMD:
			break;
		case MODIFY_TOUCH_NET:
			if(data_pakge[9] == 0x01)
			{
				set_ctrl_unit(TOUCH_NET_MODE, 0x01, data_pakge[8]);
			}
			else if(data_pakge[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(TOUCH_NET_MODE, 0x02, data_pakge[8]);
				else
					break;
			}
			else if(data_pakge[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(TOUCH_NET_MODE, 0xFF, data_pakge[8]);
				else
					set_ctrl_unit(TOUCH_NET_MODE, 0x01, data_pakge[8]);
			}
//				max_485_1_return_set_ok((uint8_t)cmd);
			break;
		default:	break;
	}
}















