#ifndef __MAX_485_H
#define __MAX_485_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "master_manage.h"
#include "lwip.h"
#include "alarm.h"
#include "display.h"

#define MAX485_1		1
#define MAX485_2		2

extern uint8_t max485_1_sendbuf[50];
extern uint8_t max485_1_receive_data[50];
extern uint8_t max485_1_receivebuf;
extern uint8_t max485_wait_usart1_flag;
extern uint8_t max485_wait_usart1_finish;

/*485通讯命令定义（功能码）*/
typedef enum
{
	INQUIRE_MSTAER_MAG = 1,
	MODIFY_ZONE_TYPE_ID = 20,		//20 防区类型 防区ID修改命令 -- 防区类型（1）、防区1ID（1）、防区2ID（1）
	MODIFY_LOCAL_IP,						//21 本地IP修改命令 -- 设备IP（4）、掩码（4）、网关（4）
	MODIFY_VOLTAGE_LAVEL,				//22 电压等级修改命令 -- 电压等级（1）
	MODIFY_ZONE_SENSITIVITY,		//23 防区灵敏度修改命令 -- 防区（0：双防区，1：1防区，2：2防区  1byte）、灵敏度（1）
	MODIFY_ALARM_DELAY,					//24 告警延时修改命令	-- 延时时间/秒（1）
	MODIFY_TRIGGER_DELAY,				//25 触发延时修改命令	-- 延时时间/秒 (1)
	MODIFY_ARM_DISARM						//26 布防/撤防命令 -- 布防/撤防（1）
}MAX485_1_cmd_def;

void max485_send_str(uint8_t device, uint8_t *str, uint16_t lenth);
void max_485_1_deal(uint8_t *data_pakge);
uint16_t CRC16(uint8_t *arr_buff, uint8_t len);
#endif /*__MAX_485_H */


