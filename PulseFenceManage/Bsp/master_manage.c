/**
  ******************************************************************************
  * File Name          : master_manage.c
  * Description        : �����������
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

zone_typedef zone_struct = {DOUBLE_ZONE, 1, 2, PULSE_MODE, PULSE_MODE, HIGH_VOLTAGE, HIGH_VOLTAGE, DISARMING, DISARMING, SENSITIVITY_3, SENSITIVITY_3, 0, 0};

zone_status_def pre_zone1_sta = DISARMING;
zone_status_def pre_zone2_sta = DISARMING;

zone_type_def temp_zone_type;
uint8_t temp_zone1_id;		//����ID�趨����   �����Ƶ�Ԫ��������ʱ  �ٸ������ID
uint8_t temp_zone2_id;

uint8_t local_address_set_buff[4] = LOCAL_IPADDR;
uint8_t subnet_mask_set_buff[4] = NETMASK_ADDR;
uint8_t gateway_address_set_buff[4] = GATEWAY_ADDR;
uint8_t remote_address_set_buff[4] = REMOTE_IPADDR;
uint16_t remote_port_set_buff = REMOTE_PORT;

uint8_t communication_sta = DISCOMMUNICAT;			//��ǰͨѶ״̬

uint8_t inquire_ctrl_sta_flag = 0;

void uart1_deal(uint8_t *data_package)
{
//	uint8_t package_lenth;
	zone_ctrl_cmd_def command;
	uint8_t zone_num, sta;
	command = (zone_ctrl_cmd_def)data_package[3];
	zone_num = data_package[4];
	sta = data_package[5];
	
	switch(command)
	{
		case AMING_DISARM:
			if(zone_num == 0x01)
			{
				zone_struct.zone1_sta = (zone_status_def)(sta+7);
				zone_struct.zone1_arm_sta = sta;
			}
			else if(zone_num == 0x02)
			{
				zone_struct.zone2_sta = (zone_status_def)(sta+7);
				zone_struct.zone2_arm_sta = sta;
			}
			else if(zone_num == 0xFF)
			{
				zone_struct.zone1_sta = (zone_status_def)(sta+7);
				zone_struct.zone2_sta = (zone_status_def)(sta+7);
				zone_struct.zone1_arm_sta = zone_struct.zone2_arm_sta = sta;	
			}
			
			if(zone_struct.zone1_sta == ARMING)
			{
				zone1_trigger_flag = 1;
				zone1_trigger_delay_cnt = 0;
			}
			else if(zone_struct.zone1_sta == DISARMING)
			{
				pre_zone1_sta = zone_struct.zone1_sta;
				alarm_output(ZONE1, RESET_ALARM);
				zone1_alarm_reset_flag = 0;
				zone1_alarm_delay_cnt = 0;
			}
			if(zone_struct.zone2_sta == ARMING)
			{
				zone2_trigger_flag = 1;
				zone2_trigger_delay_cnt = 0;
			}
			else if(zone_struct.zone2_sta == DISARMING)
			{
				pre_zone2_sta = zone_struct.zone2_sta;
				alarm_output(ZONE2, RESET_ALARM);
				zone2_alarm_reset_flag = 0;
				zone2_alarm_delay_cnt = 0;
			}
			
			if(page_sta == IN_MAIN_PAGE) lcd_show_main_page();
			write_flash_time_cnt = 0;
			led_arming(sta);
			break;
		case SINGLE_DOUBLE_ZONE:
				zone_struct.zone_type = (zone_type_def)sta;
				if(zone_struct.zone_type == DOUBLE_ZONE)
				{
					zone_struct.zone1_id = temp_zone1_id;
					zone_struct.zone2_id = temp_zone2_id;
				}
				else if(zone_struct.zone_type == SINGLE_ZONE)
				{
					zone_struct.zone1_id = temp_zone1_id;
				}
				if(page_sta == IN_MAIN_PAGE)	lcd_show_main_page();
				flash_data_struct.flash_zone1_id = zone_struct.zone1_id;
				flash_data_struct.flash_zone2_id = zone_struct.zone2_id;
				flash_data_struct.flash_zone_type = (uint8_t)zone_struct.zone_type;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;
			break;
		case HIGH_LOW_VOLTAGE:
			if(zone_num == 0x01)
			{
				zone_struct.zone1_voltage_level = (zone_voltage_level_def)sta;
			}
			else if(zone_num == 0x02)
			{
				zone_struct.zone2_voltage_level = (zone_voltage_level_def)sta;
			}
			else if(zone_num == 0xFF)
			{
				zone_struct.zone1_voltage_level = zone_struct.zone2_voltage_level = (zone_voltage_level_def)sta;
			}
			if(page_sta == IN_MAIN_PAGE) lcd_show_main_page();
			flash_data_struct.flash_zone1_voltage_level = (uint8_t)zone_struct.zone1_voltage_level;
			flash_data_struct.flash_zone2_voltage_level = (uint8_t)zone_struct.zone2_voltage_level;
			write_flash_time_cnt = 0;
			write_flash_flag = 1;		
			break;
		case SENSITIVITY:
			if(zone_num == 0x01)
			{
				zone_struct.zone1_sensitivity = (zone_sensitivity_def)sta;
			}
			else if(zone_num == 0x02)
			{
				zone_struct.zone2_sensitivity = (zone_sensitivity_def)sta;
			}
			else if(zone_num == 0xFF)
			{
				zone_struct.zone1_sensitivity = zone_struct.zone2_sensitivity = (zone_sensitivity_def)sta;
			}
			if(page_sta == IN_MAIN_PAGE) lcd_show_main_page();
			flash_data_struct.flash_zone1_sensitivity = (uint8_t)zone_struct.zone1_sensitivity;
			flash_data_struct.flash_zone2_sensitivity = (uint8_t)zone_struct.zone2_sensitivity;
			write_flash_time_cnt = 0;
			write_flash_flag = 1;	
			break;
		case TOUCH_NET_MODE:
				if(zone_num == 0x01)
				{
					zone_struct.zone1_mode = (zone_mode_def)sta;
				}
				else if(zone_num == 0x02)
				{
					zone_struct.zone2_mode = (zone_mode_def)sta;
				}
				else if(zone_num == 0xFF)
				{
					zone_struct.zone1_mode = zone_struct.zone2_mode = (zone_mode_def)sta;
				}
				if(page_sta == IN_MAIN_PAGE) lcd_show_main_page();
				flash_data_struct.flash_zone1_mode = (uint8_t)zone_struct.zone1_mode;
				flash_data_struct.flash_zone2_mode = (uint8_t)zone_struct.zone2_mode;
				write_flash_time_cnt = 0;
				write_flash_flag = 1;
			break;
		case ZONE1_STA:
			break;
		case ZONE2_STA:
			break;
		case AUTO_DETECT:
			if(sta == 0x01)
			{
				lcd_show_chs_16x16(9, 8, in_progress_gnk_code, 4);						//���ڽ���
				lcd_show_chs_16x16(9, 72, self_test_calibration_gbk_code, 4);	//�Լ�У׼
				lcd_show_str_8x16(9, 136, ",");
				lcd_show_chs_16x16(9, 144, please_wait_gbk_code, 3);					//���Ժ�
				lcd_show_str_8x16(9, 192, "...");
				auto_detect_page_cursor_sta = AT_AUTO_DETECTING;
			}
			else if(sta == 0x02)
			{
				lcd_show_chs_16x16(12, 8, self_test_calibration_gbk_code, 4);	//�Լ�У׼
				lcd_show_chs_16x16(12, 72, completed_gbk_code, 3);						//�����
				lcd_show_str_8x16(12, 120, ",");
				lcd_show_chs_16x16(12, 128, please_wait_gbk_code, 1);					//��
				lcd_show_chs_16x16(12, 144, back_gbk_code, 2);								//����
				lcd_show_str_8x16(12, 176, "...");
				auto_detect_page_cursor_sta = AT_AUTO_DETECT_COMPLATED;
			}
			break;
		case TARGE_DELAY:
			break;
		case 0x30:
			if(data_package[4] == 0)
			{
				zone_struct.zone1_sta = (zone_status_def)(zone_struct.zone1_arm_sta + 7);
			}
			else
			{
				zone_struct.zone1_sta = (zone_status_def)data_package[4];
			}
			
			if(data_package[5] == 0)
			{
				zone_struct.zone2_sta = (zone_status_def)(zone_struct.zone2_arm_sta + 7);
			}
			else
			{
				zone_struct.zone2_sta = (zone_status_def)data_package[5];
			}
		default: break;
	}
}

void set_ctrl_unit(zone_ctrl_cmd_def cmd, uint8_t zone_num, uint8_t data)
{
	uart1_tx_data[0] = 0xA5;
	uart1_tx_data[1] = 0x5A;
	uart1_tx_data[2] = 0x03;
	uart1_tx_data[3] = cmd;
	uart1_tx_data[4] = zone_num;	//0x01��1����  0x02:2����  0xFF:˫���� ���� �����Ƿ���
	uart1_tx_data[5] = data;
	uart1_tx_data[6] = 0x00;
	uart1_tx_data[7] = 0x00;
	HAL_UART_Transmit(&huart1, uart1_tx_data, 8, 1000);
}

/*������flash��ȡ�����ݺ�  ���͸����Ƶ�Ԫ*/
void init_control_uint(void)
{
	uart1_tx_data[0] = 0xA5;
	uart1_tx_data[1] = 0x5A;
	uart1_tx_data[2] = 0x08;
	uart1_tx_data[3] = 0x20;					//����   �Կ��Ƶ�Ԫ��ʼ������
	uart1_tx_data[4] = (uint8_t)zone_struct.zone_type;
	uart1_tx_data[5] = (uint8_t)zone_struct.zone1_voltage_level;
	uart1_tx_data[6] = (uint8_t)zone_struct.zone2_voltage_level;
	uart1_tx_data[7] = (uint8_t)zone_struct.zone1_sensitivity;
	uart1_tx_data[8] = (uint8_t)zone_struct.zone2_sensitivity;
	uart1_tx_data[9] = (uint8_t)zone_struct.zone1_mode;
	uart1_tx_data[10] = (uint8_t)zone_struct.zone2_mode;
	uart1_tx_data[11] = 0;
	uart1_tx_data[12] = 0;
	
	HAL_UART_Transmit(&huart1, uart1_tx_data, 13, 1000);
}

void inquire_ctrl_sta(void)
{
	uart1_tx_data[0] = 0xA5;
	uart1_tx_data[1] = 0x5A;
	uart1_tx_data[2] = 0x01;
	uart1_tx_data[3] = 0x30;						//����  ��ѯ���Ƶ�Ԫ״̬
	uart1_tx_data[4] = 0x00;
	uart1_tx_data[5] = 0x00;
	
	HAL_UART_Transmit(&huart1, uart1_tx_data, 6, 1000);
}

void inquire_ctrl_sta_process(void)
{
	if(!inquire_ctrl_sta_flag)
	{
		return;
	}
	inquire_ctrl_sta_flag = 0;
	
	if(page_sta != IN_MAIN_PAGE)
	{
		return;
	}

	if(!(zone_struct.zone1_arm_sta || zone_struct.zone2_arm_sta))
	{
		return;
	}
	
	inquire_ctrl_sta();
}












