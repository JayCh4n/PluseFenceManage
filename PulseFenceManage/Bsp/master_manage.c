/**
  ******************************************************************************
  * File Name          : master_manage.c
  * Description        : 主机管理程序
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "master_manage.h"

extern UART_HandleTypeDef huart1;

uint8_t uart1_tx_data[50];
uint8_t uart1_rx_data[50];
uint8_t uart1_rx_buff;

zone_typedef zone_struct = {DOUBLE_ZONE, 1,2, PULSE_MODE, HIGH_VOLTAGE, DISARMING, DISARMING, SENSITIVITY_3, SENSITIVITY_3, 0};
zone_typedef zone_struct_set_buff = {DOUBLE_ZONE, 1,2, PULSE_MODE, HIGH_VOLTAGE, DISARMING, DISARMING, SENSITIVITY_3, SENSITIVITY_3, 0};

uint8_t local_address_set_buff[4] = {192,168,19,10};
uint8_t subnet_mask_set_buff[4] = {255,255,255,0};
uint8_t gateway_address_set_buff[4] = {192,168,19,1};
uint8_t remote_address_set_buff[4] = {192,168,19,110};

uint16_t remote_port_set_buff = 8999;

void uart1_deal(uint8_t *data_package)
{
//	uint8_t package_lenth;
	zone_ctrl_cmd_def command;
	uint8_t sta;
	command = (zone_ctrl_cmd_def)data_package[3];
	sta = data_package[4];
	
	switch(command)
	{
		case AMING_DISARM:
			zone_struct_set_buff.zone1_sta = (zone_status_def)(sta+5);
			if(zone_struct.zone_type == DOUBLE_ZONE)
			{
				zone_struct_set_buff.zone2_sta = (zone_status_def)(sta+5);
			}
			zone_struct.arm_sta = sta;
			led_arming(sta);
			break;
		case SINGLE_DOUBLE_ZONE: 
			zone_struct.zone_type = (zone_type_def)sta;
			zone_struct_set_buff.zone2_sta = (zone_status_def)(zone_struct.arm_sta+5);	//防止在不同状态下切换单双防区 造成下次开启双防区时 主界面显示防区2状态不正确
			break;
		case HIGH_LOW_VOLTAGE: 
			zone_struct_set_buff.zone_voltage_level = (zone_voltage_level_def)sta;
			break;
		case ZONE1_SENSITIVITY:
			zone_struct_set_buff.zone1_sensitivity = (zone_sensitivity_def)sta;
			break;
		case ZONE2_SENSITIVITY:
			zone_struct_set_buff.zone2_sensitivity = (zone_sensitivity_def)sta;
			break;
		case TOUCH_NET_MODE:
			zone_struct_set_buff.zone_mode = (zone_mode_def)sta;
			break;
		case ZONE1_STA:
			if(!sta)
			{
				zone_struct_set_buff.zone1_sta = ARMING;
			}
			else
			{
				zone_struct_set_buff.zone1_sta = (zone_status_def)sta;
			}
			break;
		case ZONE2_STA:
			if(!sta)
			{
				zone_struct_set_buff.zone2_sta = ARMING;
			}
			else
			{
				zone_struct_set_buff.zone2_sta = (zone_status_def)sta;
			}
			break;
		default: break;
	}
}

void set_ctrl_unit(zone_ctrl_cmd_def cmd, uint8_t data)
{
	uart1_tx_data[0] = 0xA5;
	uart1_tx_data[1] = 0x5A;
	uart1_tx_data[2] = 0x02;
	uart1_tx_data[3] = cmd;
	uart1_tx_data[4] = data;
	uart1_tx_data[5] = 0x00;
	uart1_tx_data[6] = 0x00;
	HAL_UART_Transmit(&huart1,uart1_tx_data,7,1000);
}






