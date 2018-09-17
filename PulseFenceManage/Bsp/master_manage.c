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

uint8_t init_ctrl_unit_flag = 0;

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
			write_flash_time_cnt = 0;
			led_arming(sta);
			break;
		case SINGLE_DOUBLE_ZONE:
			if(init_ctrl_unit_flag)
			{
				init_ctrl_unit_flag = 0;
			}
			else
			{
				zone_struct.zone_type = (zone_type_def)sta;
				zone_struct_set_buff.zone2_sta = (zone_status_def)(zone_struct.arm_sta+5);	//防止在不同状态下切换单双防区 造成下次开启双防区时 主界面显示防区2状态不
				flash_data_struct.flash_zone1_id = zone_struct_set_buff.zone1_id;
				flash_data_struct.flash_zone2_id = zone_struct_set_buff.zone2_id;
				flash_data_struct.flash_zone_type = (uint8_t)zone_struct.zone_type;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;
			}
			break;
		case HIGH_LOW_VOLTAGE:
			if(init_ctrl_unit_flag)
			{
				init_ctrl_unit_flag = 0;
			}
			else
			{
				zone_struct_set_buff.zone_voltage_level = (zone_voltage_level_def)sta;
				flash_data_struct.flash_voltage_level = (uint8_t)zone_struct_set_buff.zone_voltage_level;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;				
			}
			break;
		case ZONE1_SENSITIVITY:
			if(init_ctrl_unit_flag)
			{
				init_ctrl_unit_flag = 0;
			}
			else
			{
				zone_struct_set_buff.zone1_sensitivity = (zone_sensitivity_def)sta;
				flash_data_struct.flash_zone1_sensitivity = (uint8_t)zone_struct_set_buff.zone1_sensitivity;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;	
			}
			break;
		case ZONE2_SENSITIVITY:
			if(init_ctrl_unit_flag)
			{
				init_ctrl_unit_flag = 0;
			}
			else
			{
				zone_struct_set_buff.zone2_sensitivity = (zone_sensitivity_def)sta;
				flash_data_struct.flash_zone2_sensitivity = (uint8_t)zone_struct_set_buff.zone2_sensitivity;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;				
			}
			break;
		case TOUCH_NET_MODE:
			if(init_ctrl_unit_flag)
			{
				init_ctrl_unit_flag = 0;
			}
			else
			{
				zone_struct_set_buff.zone_mode = (zone_mode_def)sta;
				flash_data_struct.flash_zone_mode = (uint8_t)zone_struct_set_buff.zone_mode;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;
			}
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
		case AUTO_DETECT:
			if(sta == 0x01)
			{
				lcd_show_chs_16x16(9, 8, in_progress_gnk_code, 4);				//正在进行
				lcd_show_chs_16x16(9, 72, self_test_calibration_gbk_code, 4);	//自检校准
				lcd_show_str_8x16(9, 136, ",");
				lcd_show_chs_16x16(9, 144, please_wait_gbk_code, 3);	//请稍后
				lcd_show_str_8x16(9, 192, "...");
				auto_detect_page_cursor_sta = AT_AUTO_DETECTING;
			}
			else if(sta == 0x02)
			{
				lcd_show_chs_16x16(12, 8, self_test_calibration_gbk_code, 4);	//自检校准
				lcd_show_chs_16x16(12, 72, completed_gbk_code, 3);				//已完成
				lcd_show_str_8x16(12, 120, ",");
				lcd_show_chs_16x16(12, 128, please_wait_gbk_code, 1);			//请
				lcd_show_chs_16x16(12, 144, back_gbk_code, 2);					//返回
				lcd_show_str_8x16(12, 176, "...");
				auto_detect_page_cursor_sta = AT_AUTO_DETECT_COMPLATED;
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

/*开机从flash读取到数据后  发送给控制单元*/
void init_control_uint(void)
{
	uint32_t i = 0;
	uint8_t set_cnt; //设定计数  尝试发送5次没返回  break;

	do
	{
		if(++set_cnt > 5)
		{
			init_ctrl_unit_flag = 0;
			break;
		}
		set_ctrl_unit(SINGLE_DOUBLE_ZONE, (uint8_t)zone_struct.zone_type);
		init_ctrl_unit_flag = 1;
		
		for(i=0; i<10000000; i++)
		{
			HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
		}
	}while(init_ctrl_unit_flag);
	
	set_cnt = 0;
	
	do
	{
		if(++set_cnt > 5)
		{
			init_ctrl_unit_flag = 0;
			break;
		}
		set_ctrl_unit(TOUCH_NET_MODE, (uint8_t)zone_struct.zone_mode);
		init_ctrl_unit_flag = 1;		
		
		for(i=0; i<10000000; i++)
		{
			HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
		}		
	}while(init_ctrl_unit_flag);

	set_cnt = 0;
	
	do
	{
		if(++set_cnt > 5)
		{
			init_ctrl_unit_flag = 0;
			break;
		}
		set_ctrl_unit(HIGH_LOW_VOLTAGE, (uint8_t)zone_struct.zone_voltage_level);
		init_ctrl_unit_flag = 1;		
		
		for(i=0; i<10000000; i++)
		{
			HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
		}		
	}while(init_ctrl_unit_flag);

	set_cnt = 0;
	
	do
	{
		if(++set_cnt > 5)
		{
			init_ctrl_unit_flag = 0;
			break;
		}
		set_ctrl_unit(ZONE1_SENSITIVITY, (uint8_t)zone_struct.zone1_sensitivity);
		init_ctrl_unit_flag = 1;		
		
		for(i=0; i<10000000; i++)
		{
			HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
		}		
	}while(init_ctrl_unit_flag);
	
	set_cnt = 0;
	
	do
	{
		if(++set_cnt > 5)
		{
			init_ctrl_unit_flag = 0;
			break;
		}
		set_ctrl_unit(ZONE2_SENSITIVITY, (uint8_t)zone_struct.zone2_sensitivity);
		init_ctrl_unit_flag = 1;	
		
		for(i=0; i<10000000; i++)
		{
			HAL_UART_Receive_IT(&huart1, &uart1_rx_buff, 1);
		}
	}while(init_ctrl_unit_flag);
}




