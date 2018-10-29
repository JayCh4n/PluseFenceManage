#ifndef __MASTER_MANAGE_H
#define __MASTER_MANAGE_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "display.h"
#include "flash.h"
#include "alarm.h"

#define COMMUNICATING	1	//通讯
#define DISCOMMUNICAT	0	//不通讯

/*防区控制命令*/
typedef enum{
	AMING_DISARM = 1,					//布防/撤防命令   		数据：0x00 撤防   0x01布防
	SINGLE_DOUBLE_ZONE,				//单/双防区命令				数据：0x01 单			0x02双
	HIGH_LOW_VOLTAGE,					//高/低压模式命令			数据：0x00 低压   0x01高压
	ZONE1_SENSITIVITY,				//防区1灵敏度命令			数据：0x01 - 0x03	对应1——3等级灵敏度
	ZONE2_SENSITIVITY,				//防区2灵敏度命令			数据：0x01 - 0x03	对应1——3等级灵敏度
	TOUCH_NET_MODE,						//触网功能开关				数据:	0x00 关闭   0x01开启
	AUTO_DETECT,							//自动检测命令				数据:	0x01 开始自动检测  0x02:自动检测完成
	TARGE_DELAY,							//触发延时时间				数据：时间		单位：秒
	ZONE1_STA = 0x11,					//防区1状态返回命令		数据: 0x01 断线 0x02短路 0x03触网
	ZONE2_STA									//防区2状态返回命令		数据: 0x01 断线 0x02短路 0x03触网
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
	zone_mode_def zone_mode;
	zone_voltage_level_def zone_voltage_level;
	zone_status_def zone1_sta;
	zone_status_def zone2_sta;
	zone_sensitivity_def zone1_sensitivity;
	zone_sensitivity_def zone2_sensitivity;
	uint8_t arm_sta;				//布防状态 0：撤防 		1：布防
//	uint8_t dismantle_sta; 	//防拆状态 0：未拆除 	1：已拆除
}zone_typedef;

extern uint8_t uart1_tx_data[50];
extern uint8_t uart1_rx_data[50];
extern uint8_t uart6_rx_buff;
extern zone_typedef zone_struct;
extern zone_typedef zone_struct_set_buff;
extern uint8_t local_address_set_buff[4];
extern uint8_t subnet_mask_set_buff[4];
extern uint8_t gateway_address_set_buff[4];
extern uint8_t remote_address_set_buff[4];
extern uint16_t remote_port_set_buff;

extern uint8_t pre_communication_sta;	//上次通讯状态
extern uint8_t communication_sta;			//当前通讯状态
extern uint16_t communication_cnt;

void uart1_deal(uint8_t *data_package);
void set_ctrl_unit(uint8_t cmd, uint8_t data);
void init_control_uint(void);
#endif /*__MASTER_MANAGE_H*/

