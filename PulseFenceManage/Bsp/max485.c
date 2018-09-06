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
		HAL_UART_Transmit(&huart2,str,lenth,50);
		HAL_GPIO_WritePin(RS485_CNT1_GPIO_Port, RS485_CNT1_Pin, GPIO_PIN_RESET);
	}
	else if(device == MAX485_2)
	{
		HAL_GPIO_WritePin(RS485_CNT2_GPIO_Port, RS485_CNT2_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart4,str,lenth,50);
		HAL_GPIO_WritePin(RS485_CNT2_GPIO_Port, RS485_CNT2_Pin, GPIO_PIN_RESET);
	}
}

static void max_485_1_return_master_msg(void)
{
	uint16_t crc;
	
	max485_1_sendbuf[0] = 0xE7;
	max485_1_sendbuf[1] = 0xD9;
	max485_1_sendbuf[2] = 0xE7;
	max485_1_sendbuf[3] = 0xD9;	//数据头
	max485_1_sendbuf[4] = 0x02;	//设备类型
	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
	max485_1_sendbuf[6] = 0x01; //功能码
	max485_1_sendbuf[7] = 0x0F;	//数据长度
	
	max485_1_sendbuf[8] = zone_struct.zone_type;	//
	max485_1_sendbuf[9] = zone_struct.zone1_id;
	max485_1_sendbuf[10] = zone_struct.zone2_id;
	max485_1_sendbuf[11] = zone_struct.zone1_sta;
	max485_1_sendbuf[12] = zone_struct.zone2_sta;
	max485_1_sendbuf[13] = demolition_sta;
	max485_1_sendbuf[14] = zone_struct.zone_voltage_level;
	max485_1_sendbuf[15] = zone_struct.zone1_sensitivity;
	max485_1_sendbuf[16] = zone_struct.zone2_sensitivity;
	max485_1_sendbuf[17] = 0;//防区属性
	max485_1_sendbuf[18] = zone_struct.zone_mode;
	
	max485_1_sendbuf[19] = IP_ADDRESS[0];
	max485_1_sendbuf[20] = IP_ADDRESS[1];
	max485_1_sendbuf[21] = IP_ADDRESS[2];
	max485_1_sendbuf[22] = IP_ADDRESS[3];
	
	crc = CRC16(max485_1_sendbuf, 23);
	
	max485_1_sendbuf[23] = crc >> 8;
	max485_1_sendbuf[24] = crc;
	
	max485_1_sendbuf[25] = 0xF8;
	max485_1_sendbuf[26] = 0xC6;
	max485_1_sendbuf[27] = 0xF8;
	max485_1_sendbuf[28] = 0xC6;
	
	max485_send_str(MAX485_1, max485_1_sendbuf, 29);
}

///*返回主机类型、ID*/
//static void max_485_1_return_zone_type_id(void)
//{
//	uint16_t crc;
//	uint32_t i;
//	
//	max485_wait_usart1_flag = 1;
//	while(!max485_wait_usart1_finish)
//	{
//		if(++i >= 7000000000)
//		{
//			max485_wait_usart1_flag = 0;
//			break;
//		}
//		HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
//	};
//	
//	max485_1_sendbuf[0] = 0xE7;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;	//数据头
//	max485_1_sendbuf[4] = 0x02;	//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = 20; //功能码
//	max485_1_sendbuf[7] = 3;	//数据长度
//	
//	max485_1_sendbuf[8] = (uint8_t)zone_struct.zone_type;	//
//	max485_1_sendbuf[9] = zone_struct.zone1_id;
//	max485_1_sendbuf[10] = zone_struct.zone2_id;
//	
//	crc = CRC16(max485_1_sendbuf, 11);
//	
//	max485_1_sendbuf[11] = crc >> 8;
//	max485_1_sendbuf[12] = crc;
//	
//	max485_1_sendbuf[13] = 0xF8;
//	max485_1_sendbuf[14] = 0xC6;
//	max485_1_sendbuf[15] = 0xF8;
//	max485_1_sendbuf[16] = 0xC6;
//	
//	if(max485_wait_usart1_finish)
//	{
//		max485_wait_usart1_flag = 0;
//		max485_send_str(MAX485_1, max485_1_sendbuf, 17);
//	}
//	
//	max485_wait_usart1_finish = 0;	
//}

///*返回主机本地IP、子网掩码、网关*/
//static void max_485_1_return_local_ip(void)
//{
//	uint16_t crc;
//	
//	max485_1_sendbuf[0] = 0xE7;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;	//数据头
//	max485_1_sendbuf[4] = 0x02;	//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = 21; //功能码
//	max485_1_sendbuf[7] = 12;	//数据长度
//	
//	max485_1_sendbuf[8] = IP_ADDRESS[0];	
//	max485_1_sendbuf[9] = IP_ADDRESS[1];
//	max485_1_sendbuf[10] = IP_ADDRESS[2];
//	max485_1_sendbuf[11] = IP_ADDRESS[3];

//	max485_1_sendbuf[12] = NETMASK_ADDRESS[0];	
//	max485_1_sendbuf[13] = NETMASK_ADDRESS[1];
//	max485_1_sendbuf[14] = NETMASK_ADDRESS[2];
//	max485_1_sendbuf[15] = NETMASK_ADDRESS[3];	
//	
//	max485_1_sendbuf[16] = NETMASK_ADDRESS[0];	
//	max485_1_sendbuf[17] = NETMASK_ADDRESS[1];
//	max485_1_sendbuf[18] = NETMASK_ADDRESS[2];
//	max485_1_sendbuf[19] = NETMASK_ADDRESS[3];
//	
//	crc = CRC16(max485_1_sendbuf, 20);
//	
//	max485_1_sendbuf[20] = crc >> 8;
//	max485_1_sendbuf[21] = crc;
//	
//	max485_1_sendbuf[22] = 0xF8;
//	max485_1_sendbuf[23] = 0xC6;
//	max485_1_sendbuf[24] = 0xF8;
//	max485_1_sendbuf[25] = 0xC6;
//	
//	max485_send_str(MAX485_1, max485_1_sendbuf, 26);	
//}

///*返回高低压模式*/
//static void max_485_1_return_zone_voltage_level(void)
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
//	max485_1_sendbuf[0] = 0xE7;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;	//数据头
//	max485_1_sendbuf[4] = 0x02;	//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = 22; //功能码
//	max485_1_sendbuf[7] = 1;	//数据长度
//	
//	max485_1_sendbuf[8] = (uint8_t)zone_struct.zone_type;	//
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

///*返回防区灵敏度*/
//static void max_485_1_return_zone_sensitivity(uint8_t zone_num, zone_sensitivity_def sensitivity)
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
//	max485_1_sendbuf[0] = 0xE7;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;	//数据头
//	max485_1_sendbuf[4] = 0x02;	//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = 23; //功能码
//	max485_1_sendbuf[7] = 2;	//数据长度
//	
//	max485_1_sendbuf[8] = zone_num;	//
//	max485_1_sendbuf[9] = (uint8_t)sensitivity;
//	
//	crc = CRC16(max485_1_sendbuf, 10);
//	
//	max485_1_sendbuf[10] = crc >> 8;
//	max485_1_sendbuf[11] = crc;
//	
//	max485_1_sendbuf[12] = 0xF8;
//	max485_1_sendbuf[13] = 0xC6;
//	max485_1_sendbuf[14] = 0xF8;
//	max485_1_sendbuf[15] = 0xC6;
//	
//	if(max485_wait_usart1_finish)
//	{
//		max485_wait_usart1_flag = 0;
//		max485_send_str(MAX485_1, max485_1_sendbuf, 16);
//	}
//	
//	max485_wait_usart1_finish = 0;	
//}

///*返回布防撤防*/
//static void max_485_1_return_zone_arm_sta(void)
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
//	max485_1_sendbuf[0] = 0xE7;
//	max485_1_sendbuf[1] = 0xD9;
//	max485_1_sendbuf[2] = 0xE7;
//	max485_1_sendbuf[3] = 0xD9;	//数据头
//	max485_1_sendbuf[4] = 0x02;	//设备类型
//	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	max485_1_sendbuf[6] = 26; //功能码
//	max485_1_sendbuf[7] = 1;	//数据长度
//	
//	max485_1_sendbuf[8] = zone_struct.arm_sta;	//
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

/*返回设置成功*/
static void max_485_1_return_set_ok(uint8_t cmd)
{
	uint16_t crc;
	uint32_t i;
	
	max485_wait_usart1_flag = 1;
	while(!max485_wait_usart1_finish)
	{
		if(++i >= 70000000)
		{
			max485_wait_usart1_flag = 0;
			break;
		}
		HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
	};
	
	max485_1_sendbuf[0] = 0xE7;
	max485_1_sendbuf[1] = 0xD9;
	max485_1_sendbuf[2] = 0xE7;
	max485_1_sendbuf[3] = 0xD9;	//数据头
	max485_1_sendbuf[4] = 0x02;	//设备类型
	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
	max485_1_sendbuf[6] = cmd; //功能码
	max485_1_sendbuf[7] = 1;	//数据长度
	
	max485_1_sendbuf[8] = 0xA9;	//
	
	crc = CRC16(max485_1_sendbuf, 9);
	
	max485_1_sendbuf[9] = crc >> 8;
	max485_1_sendbuf[10] = crc;
	
	max485_1_sendbuf[11] = 0xF8;
	max485_1_sendbuf[12] = 0xC6;
	max485_1_sendbuf[13] = 0xF8;
	max485_1_sendbuf[14] = 0xC6;
	
	if(max485_wait_usart1_finish)
	{
		max485_wait_usart1_flag = 0;
		max485_send_str(MAX485_1, max485_1_sendbuf, 15);
	}
	
	max485_wait_usart1_finish = 0;
}
//	max485_wait_usart1_finish = 0;
//}

/*返回设置IP成功*/
static void max_485_1_return_ip_set_ok(void)
{
	uint16_t crc;

	max485_1_sendbuf[0] = 0xE7;
	max485_1_sendbuf[1] = 0xD9;
	max485_1_sendbuf[2] = 0xE7;
	max485_1_sendbuf[3] = 0xD9;	//数据头
	max485_1_sendbuf[4] = 0x02;	//设备类型
	max485_1_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
	max485_1_sendbuf[6] = 21; //功能码
	max485_1_sendbuf[7] = 1;	//数据长度
	
	max485_1_sendbuf[8] = 0xA9;	//
	
	crc = CRC16(max485_1_sendbuf, 9);
	
	max485_1_sendbuf[9] = crc >> 8;
	max485_1_sendbuf[10] = crc;
	
	max485_1_sendbuf[11] = 0xF8;
	max485_1_sendbuf[12] = 0xC6;
	max485_1_sendbuf[13] = 0xF8;
	max485_1_sendbuf[14] = 0xC6;
	
	max485_send_str(MAX485_1, max485_1_sendbuf, 15);
}

void max_485_1_deal(uint8_t *data_pakge)
{
	uint32_t i;
	
	MAX485_1_cmd_def cmd;
	uint8_t zone_num; 	// 0:双防区 1：1防区  2：2防区
	
	cmd = (MAX485_1_cmd_def)data_pakge[6];
		
	switch(cmd)
	{
		case INQUIRE_MSTAER_MAG:	max_485_1_return_master_msg(); break;
		case MODIFY_ZONE_TYPE_ID: 
			zone_struct_set_buff.zone_type = (zone_type_def)data_pakge[8];	//防区类型  单防区：1  双防区：2

			if(zone_struct_set_buff.zone_type == DOUBLE_ZONE)
			{
				zone_struct.zone1_id = data_pakge[9];
				zone_struct.zone2_id = data_pakge[10];				
				zone_struct_set_buff.zone1_id = zone_struct.zone1_id;
				zone_struct_set_buff.zone2_id = zone_struct.zone2_id;
			}
			else
			{
				zone_struct.zone1_id = data_pakge[9];
				zone_struct_set_buff.zone1_id = zone_struct.zone1_id;
			}
			set_ctrl_unit(SINGLE_DOUBLE_ZONE, (uint8_t)zone_struct_set_buff.zone_type);
//			max_485_1_return_zone_type_id();
			max_485_1_return_set_ok((uint8_t)cmd);
			lcd_show_main_page();
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
			}
			local_network_set(IP_ADDRESS, NETMASK_ADDRESS, GATEWAY_ADDRESS);
//			max_485_1_return_local_ip();
			max_485_1_return_ip_set_ok();
			break;
		case MODIFY_VOLTAGE_LAVEL:
			if(zone_struct.arm_sta) //如果在布防状态下
			{
				//低压：0  高压：1
				set_ctrl_unit(HIGH_LOW_VOLTAGE, data_pakge[8]);
//				max_485_1_return_zone_voltage_level();	
				max_485_1_return_set_ok((uint8_t)cmd);
			}
			break;
		case MODIFY_ZONE_SENSITIVITY:
			if(zone_struct.arm_sta) //如果在布防状态下
			{
				zone_num = data_pakge[8];
				if(zone_num == 0)		//全防区设定
				{	
					if(data_pakge[9] == 0 || data_pakge[9] >= 4)
					{
						return;
					}						
					set_ctrl_unit(ZONE1_SENSITIVITY, data_pakge[9]);
					while(++i <= 750000)
					{
						HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
					}
					set_ctrl_unit(ZONE2_SENSITIVITY, data_pakge[9]);
//					max_485_1_return_zone_sensitivity(zone_num, zone_struct.zone1_sensitivity);
					max_485_1_return_set_ok((uint8_t)cmd);
				}
				else if(zone_num == 1)
				{
					set_ctrl_unit(ZONE1_SENSITIVITY, data_pakge[9]);
//					max_485_1_return_zone_sensitivity(zone_num, zone_struct.zone1_sensitivity);
					max_485_1_return_set_ok((uint8_t)cmd);
				}
				else if(zone_num == 2)
				{
					set_ctrl_unit(ZONE2_SENSITIVITY, data_pakge[9]);
//					max_485_1_return_zone_sensitivity(zone_num, zone_struct.zone2_sensitivity);
					max_485_1_return_set_ok((uint8_t)cmd);
				}
			}
			break;
		case MODIFY_ALARM_DELAY: break;
			
		case MODIFY_TRIGGER_DELAY: break;
		
		case MODIFY_ARM_DISARM:
			//0:撤防  1：布防
			set_ctrl_unit(AMING_DISARM, data_pakge[8]);
//			max_485_1_return_zone_arm_sta();
			max_485_1_return_set_ok((uint8_t)cmd);
			break;
		default:	break;
	}
}















