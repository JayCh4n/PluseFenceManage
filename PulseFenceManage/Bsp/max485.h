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

/*485ͨѶ����壨�����룩*/
typedef enum
{
	INQUIRE_MSTAER_MAG = 1,
	MODIFY_ZONE_TYPE_ID = 20,		//20 �������� ����ID�޸����� -- �������ͣ�1��������1ID��1��������2ID��1��
	MODIFY_LOCAL_IP,						//21 ����IP�޸����� -- �豸IP��4�������루4�������أ�4��
	MODIFY_VOLTAGE_LAVEL,				//22 ��ѹ�ȼ��޸����� -- ��ѹ�ȼ���1��
	MODIFY_ZONE_SENSITIVITY,		//23 �����������޸����� -- ������0��˫������1��1������2��2����  1byte���������ȣ�1��
	MODIFY_ALARM_DELAY,					//24 �澯��ʱ�޸�����	-- ��ʱʱ��/�루1��
	MODIFY_TRIGGER_DELAY,				//25 ������ʱ�޸�����	-- ��ʱʱ��/�� (1)
	MODIFY_ARM_DISARM						//26 ����/�������� -- ����/������1��
}MAX485_1_cmd_def;

void max485_send_str(uint8_t device, uint8_t *str, uint16_t lenth);
void max_485_1_deal(uint8_t *data_pakge);
uint16_t CRC16(uint8_t *arr_buff, uint8_t len);
#endif /*__MAX_485_H */


