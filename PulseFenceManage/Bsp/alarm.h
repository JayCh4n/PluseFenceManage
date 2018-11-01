#ifndef __ALARM_H
#define __ALARM_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "display.h"
#include "master_manage.h"

#define ZONE1		1
#define ZONE2 	2

#define SET_ALARM			1
#define RESET_ALARM		0

#define RELAY_ON		1
#define RELAY_OFF		0

#define DEMOLITED		1
#define DISDEMOLITED	0

extern uint8_t demolition_detect_mask;
extern uint8_t demolition_sta;

extern uint8_t zone1_alarm_reset_flag;
extern uint8_t zone2_alarm_reset_flag;
extern uint8_t demolition_alarm_reset_flag;

extern uint32_t zone1_alarm_delay_cnt;
extern uint32_t zone2_alarm_delay_cnt;

extern uint32_t zone1_alarm_reset_time;
extern uint32_t zone2_alarm_reset_time;
extern uint32_t demolition_alarm_reset_time;

extern uint8_t zone1_trigger_flag;				//当标志为1时 代表在触发计时中  不进行报警输出
extern uint8_t zone2_trigger_flag;

extern uint32_t zone1_trigger_delay_cnt;
extern uint32_t zone2_trigger_delay_cnt;

extern uint32_t zone1_trigger_delay_time; 				//触发延时单位毫秒  最大设定99秒
extern uint32_t zone2_trigger_delay_time;

extern uint8_t zone1_sta_detect_flag;							//状态检测标志		与灵敏度相关
extern uint8_t zone2_sta_detect_flag;

void relay_alarm(uint8_t zone_num, uint8_t cmd);
void relay_siren(uint8_t zone_num, uint8_t cmd);
void buzz(uint8_t sta);
//void alarm_detect_output(void);
void alarm_output(uint8_t zone_num, uint8_t sta);
void demolition_detect_process(void);
void alarm_sta_detect_process(void);
#endif /* __ALARM_H */

