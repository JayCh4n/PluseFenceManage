#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "master_manage.h"
#include "lwip.h"
#include "tcp_client.h"
#include "udp_client.h"
#include "display.h"
#include "alarm.h"

typedef struct{
	uint8_t flash_first_start;
	uint8_t flash_zone_type;
	uint8_t flash_zone1_id;
	uint8_t flash_zone2_id;
	uint8_t flash_zone1_mode;
	uint8_t flash_zone2_mode;
	uint8_t flash_zone1_voltage_level;
	uint8_t flash_zone2_voltage_level;
	uint8_t flash_zone1_sensitivity;
	uint8_t flash_zone2_sensitivity;
	uint32_t flash_zone1_trigger_delay_time;					//触发延时  单位ms
	uint32_t flash_zone2_trigger_delay_time;					//触发延时  单位ms
	uint32_t flash_zone1_alarm_reset_time; 	//防区1报警延时 单位ms
	uint32_t flash_zone2_alarm_reset_time; 	//防区2报警延时 单位ms	
	uint32_t flash_demolition_alarm_reset_time; //防拆报警延时 单位ms
	uint8_t flash_local_ip[4];
	uint8_t flash_netmask[4];
	uint8_t flash_gate_way[4];
	uint8_t flash_remote_ip[4];
	uint16_t flash_remote_port;
}flash_data_typedef;

#define FLASH_SAVE_ADDR	((uint32_t)0x08060000)	//是否为第一次上电启动标志 	一个字节

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

#define TEXT_LENTH sizeof(flash_data_struct)	 		  	//数组长度
#define FLASH_DATA_SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//扇区11起始地址,128 Kbytes  

extern flash_data_typedef flash_data_struct;
extern uint8_t write_flash_flag;

extern uint16_t write_flash_time_cnt;
extern uint8_t write_flash_time_mask;

void read_data_from_flash(void);
void write_flash_process(void);

void STMFLASH_Unlock(void);					//FLASH解锁
void STMFLASH_Lock(void);				 	//FLASH上锁
uint8_t STMFLASH_GetStatus(void);				//获得状态
uint8_t STMFLASH_WaitDone(uint32_t time);				//等待操作结束
uint8_t STMFLASH_ErasePage(uint32_t sectoraddr);	 	//擦除页
uint8_t STMFLASH_WriteWord(uint32_t faddr, uint32_t dat);	//写入字
uint32_t STMFLASH_ReadWord(uint32_t faddr);		  	//读出字  
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);   		//从指定地址开始读出指定长度的数据
uint8_t flash_read_byte(uint32_t Address);
#endif /* __FLASH_H */

