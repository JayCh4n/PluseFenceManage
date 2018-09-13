#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "master_manage.h"
#include "lwip.h"
#include "tcp_client.h"
#include "display.h"

typedef struct{
	uint8_t flash_first_start;
	uint8_t flash_zone_type;
	uint8_t flash_zone1_id;
	uint8_t flash_zone2_id;
	uint8_t flash_zone_mode;
	uint8_t flash_voltage_level;
	uint8_t flash_zone1_sensitivity;
	uint8_t flash_zone2_sensitivity;
	uint8_t flash_local_ip[4];
	uint8_t flash_netmask[4];
	uint8_t flash_gate_way[4];
	uint8_t flash_remote_ip[4];
	uint16_t flash_remote_port;
}flash_data_typedef;

#define FIRST_START_FLASH_ADDR	((uint32_t)0x08080000)	//�Ƿ�Ϊ��һ���ϵ�������־ 	һ���ֽ�
////#define ZONE_TYPE_FLASH_ADDR		((uint32_t)0x0807FFFE)	//�������ͣ���˫������
////#define ZONE1_ID_FLASH_ADDR			((uint32_t)0x0807FFFD)	//����1ID
////#define ZONE2_ID_FLASH_ADDR			((uint32_t)0x0807FFFC)	//����2ID
////#define	ZONE_MODE_FLASH_ADDR		((uint32_t)0x0807FFFB)	//����ģʽ�����������壩
////#define	VOLTAGE_LAVEL_FLASH_ADDR	((uint32_t)0x0807FFFA)	//��ѹ�ȼ����ߵ�ѹ��
////#define	ZONE1_SENSITIVITY_FLASH_ADDR	((uint32_t)0x0807FFF9)	//����1������
////#define ZONE2_SENSITIVITY_FLASH_ADDR	((uint32_t)0x0807FFF8)	//����2������

////#define LOCAL_IP0_FLASH_ADDR		((uint32_t)0x0807FFF7)	//����IP0
////#define LOCAL_IP1_FLASH_ADDR		((uint32_t)0x0807FFF6)	//����IP1
////#define LOCAL_IP2_FLASH_ADDR		((uint32_t)0x0807FFF5)	//����IP2
////#define LOCAL_IP3_FLASH_ADDR		((uint32_t)0x0807FFF4)	//����IP3

////#define SUBNET_MASK0_FLASH_ADDR		((uint32_t)0x0807FFF3)	//��������0
////#define SUBNET_MASK1_FLASH_ADDR		((uint32_t)0x0807FFF2)	//��������1
////#define SUBNET_MASK2_FLASH_ADDR		((uint32_t)0x0807FFF1)	//��������2
////#define SUBNET_MASK3_FLASH_ADDR		((uint32_t)0x0807FFF0)	//��������3

////#define GATE_WAY0_FLASH_ADDR		((uint32_t)0x0807FFEF)	//����0
////#define GATE_WAY1_FLASH_ADDR		((uint32_t)0x0807FFEE)	//����1
////#define GATE_WAY2_FLASH_ADDR		((uint32_t)0x0807FFED)	//����2
////#define GATE_WAY3_FLASH_ADDR		((uint32_t)0x0807FFEC)	//����3

////#define REMOTE_IP0_FLASH_ADDR		((uint32_t)0x0807FFEB)	//Զ��IP0
////#define REMOTE_IP1_FLASH_ADDR		((uint32_t)0x0807FFEA)	//Զ��IP1
////#define REMOTE_IP2_FLASH_ADDR		((uint32_t)0x0807FFE9)	//Զ��IP2
////#define REMOTE_IP3_FLASH_ADDR		((uint32_t)0x0807FFE8)	//Զ��IP3

////#define	REMOTE_PORT_FLASH_ADDR	((uint32_t)0x0807FFE6)	//Զ�˶˿ں�	2���ֽ�	

extern flash_data_typedef flash_data_struct;

extern uint8_t write_flash_flag;
void write_flash_process(uint8_t cycle_quantity);
void read_data_from_flash(void);
uint8_t flash_read_byte(uint32_t Address);
void flash_read(uint32_t Sector, uint8_t *p_data, uint16_t data_lenth);
void flash_write(uint32_t Sector, uint8_t *p_data, uint16_t data_lenth);

#endif /* __FLASH_H */

