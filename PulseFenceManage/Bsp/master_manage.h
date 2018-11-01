#ifndef __MASTER_MANAGE_H
#define __MASTER_MANAGE_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "display.h"
#include "flash.h"
#include "alarm.h"

#define COMMUNICATING	1	//ͨѶ
#define DISCOMMUNICAT	0	//��ͨѶ

/*������������*/
typedef enum{
	AMING_DISARM = 1,					//����/��������   		���ݣ�0x00 ����   0x01����
	SINGLE_DOUBLE_ZONE,				//��/˫��������				���ݣ�0x01 ��			0x02˫
	HIGH_LOW_VOLTAGE,					//��/��ѹģʽ����			���ݣ�0x00 ��ѹ   0x01��ѹ
	SENSITIVITY,				//����1����������			���ݣ�0x01 - 0x03	��Ӧ1����3�ȼ�������
//	ZONE2_SENSITIVITY,				//����2����������			���ݣ�0x01 - 0x03	��Ӧ1����3�ȼ�������
	TOUCH_NET_MODE,						//�������ܿ���				����:	0x00 �ر�   0x01����
	AUTO_DETECT,							//�Զ��������				����:	0x01 ��ʼ�Զ����  0x02:�Զ�������
	TARGE_DELAY,							//������ʱʱ��				���ݣ�ʱ��		��λ����
	ZONE1_STA = 0x11,					//����1״̬��������		����: 0x01 ���� 0x02��· 0x03����
	ZONE2_STA									//����2״̬��������		����: 0x01 ���� 0x02��· 0x03����
}zone_ctrl_cmd_def;

typedef enum{
	SINGLE_ZONE = 1,
	DOUBLE_ZONE
}zone_type_def;

typedef enum{
	PULSE_MODE = 0,
	TOUCH_MODE
}zone_mode_def;

typedef enum{
	SENSITIVITY_1 = 1,
	SENSITIVITY_2,
	SENSITIVITY_3
}zone_sensitivity_def;

typedef enum{
	P_BROKEN_LINE = 1,
	N_BROKEN_LINE,
	PN_BROKEN_LINE,
	SHORT_CIRCUIT,
	TOUCH_NET,
	BYPASS,
	DISARMING,
	ARMING
}zone_status_def;

typedef enum{
	LOW_VOLTAGE = 0,
	HIGH_VOLTAGE
}zone_voltage_level_def;

typedef struct
{
	zone_type_def zone_type;
	uint8_t zone1_id;
	uint8_t zone2_id;
	zone_mode_def zone1_mode;
	zone_mode_def zone2_mode;
	zone_voltage_level_def zone1_voltage_level;
	zone_voltage_level_def zone2_voltage_level;
	zone_status_def zone1_sta;
	zone_status_def zone2_sta;
	zone_sensitivity_def zone1_sensitivity;
	zone_sensitivity_def zone2_sensitivity;
	uint8_t zone1_arm_sta;				//����״̬ 0������ 		1������
	uint8_t zone2_arm_sta;
}zone_typedef;

extern uint8_t uart1_tx_data[50];
extern uint8_t uart1_rx_data[50];
extern uint8_t uart6_rx_buff;
extern zone_typedef zone_struct;
extern uint8_t local_address_set_buff[4];
extern uint8_t subnet_mask_set_buff[4];
extern uint8_t gateway_address_set_buff[4];
extern uint8_t remote_address_set_buff[4];
extern uint16_t remote_port_set_buff;
extern zone_status_def pre_zone1_sta;
extern zone_status_def pre_zone2_sta;
extern zone_type_def temp_zone_type;
extern uint8_t temp_zone1_id;		//����ID�趨����   �����Ƶ�Ԫ��������ʱ  �ٸ������ID
extern uint8_t temp_zone2_id;

extern uint8_t communication_sta;			//��ǰͨѶ״̬
extern uint8_t inquire_ctrl_sta_flag;

void uart1_deal(uint8_t *data_package);
void set_ctrl_unit(zone_ctrl_cmd_def cmd, uint8_t zone_num, uint8_t data);
void init_control_uint(void);
void inquire_ctrl_sta(void);
void inquire_ctrl_sta_process(void);
#endif /*__MASTER_MANAGE_H*/

