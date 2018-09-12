#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "master_manage.h"
#include "lwip.h"
#include "tcp_client.h"
#include "display.h"

#define FIRST_START_FLASH_ADDR	((uint32_t)0x0807FFFF)	//是否为第一次上电启动标志 	一个字节
#define ZONE_TYPE_FLASH_ADDR		((uint32_t)0x0807FFFE)	//防区类型（单双防区）
#define ZONE1_ID_FLASH_ADDR			((uint32_t)0x0807FFFD)	//防区1ID
#define ZONE2_ID_FLASH_ADDR			((uint32_t)0x0807FFFC)	//防区2ID
#define	ZONE_MODE_FLASH_ADDR		((uint32_t)0x0807FFFB)	//防区模式（触网、脉冲）
#define	VOLTAGE_LAVEL_FLASH_ADDR	((uint32_t)0x0807FFFA)	//电压等级（高抵压）
#define	ZONE1_SENSITIVITY_FLASH_ADDR	((uint32_t)0x0807FFF9)	//防区1灵敏度
#define ZONE2_SENSITIVITY_FLASH_ADDR	((uint32_t)0x0807FFF8)	//防区2灵敏度

#define LOCAL_IP0_FLASH_ADDR		((uint32_t)0x0807FFF7)	//本地IP0
#define LOCAL_IP1_FLASH_ADDR		((uint32_t)0x0807FFF6)	//本地IP1
#define LOCAL_IP2_FLASH_ADDR		((uint32_t)0x0807FFF5)	//本地IP2
#define LOCAL_IP3_FLASH_ADDR		((uint32_t)0x0807FFF4)	//本地IP3

#define SUBNET_MASK0_FLASH_ADDR		((uint32_t)0x0807FFF3)	//子网掩码0
#define SUBNET_MASK1_FLASH_ADDR		((uint32_t)0x0807FFF2)	//子网掩码1
#define SUBNET_MASK2_FLASH_ADDR		((uint32_t)0x0807FFF1)	//子网掩码2
#define SUBNET_MASK3_FLASH_ADDR		((uint32_t)0x0807FFF0)	//子网掩码3

#define GATE_WAY0_FLASH_ADDR		((uint32_t)0x0807FFEF)	//网关0
#define GATE_WAY1_FLASH_ADDR		((uint32_t)0x0807FFEE)	//网关1
#define GATE_WAY2_FLASH_ADDR		((uint32_t)0x0807FFED)	//网关2
#define GATE_WAY3_FLASH_ADDR		((uint32_t)0x0807FFEC)	//网关3

#define REMOTE_IP0_FLASH_ADDR		((uint32_t)0x0807FFEB)	//远端IP0
#define REMOTE_IP1_FLASH_ADDR		((uint32_t)0x0807FFEA)	//远端IP1
#define REMOTE_IP2_FLASH_ADDR		((uint32_t)0x0807FFE9)	//远端IP2
#define REMOTE_IP3_FLASH_ADDR		((uint32_t)0x0807FFE8)	//远端IP3

#define	REMOTE_PORT_FLASH_ADDR	((uint32_t)0x0807FFE6)	//远端端口号	2个字节	

void read_data_from_flash(void);
uint8_t flash_read_byte(uint32_t Address);
uint16_t flash_read_halfword(uint32_t Address);
void flash_write(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

#endif /* __FLASH_H */

