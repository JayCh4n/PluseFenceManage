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

#define FLASH_SAVE_ADDR	((uint32_t)0x08060000)	//�Ƿ�Ϊ��һ���ϵ�������־ 	һ���ֽ�

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

#define TEXT_LENTH sizeof(flash_data_struct)	 		  	//���鳤��
#define FLASH_DATA_SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  

extern flash_data_typedef flash_data_struct;
extern uint8_t write_flash_flag;

extern uint16_t write_flash_time_cnt;
extern uint8_t write_flash_time_mask;

void read_data_from_flash(void);
void write_flash_process(void);

void STMFLASH_Unlock(void);					//FLASH����
void STMFLASH_Lock(void);				 	//FLASH����
uint8_t STMFLASH_GetStatus(void);				//���״̬
uint8_t STMFLASH_WaitDone(uint32_t time);				//�ȴ���������
uint8_t STMFLASH_ErasePage(uint32_t sectoraddr);	 	//����ҳ
uint8_t STMFLASH_WriteWord(uint32_t faddr, uint32_t dat);	//д����
uint32_t STMFLASH_ReadWord(uint32_t faddr);		  	//������  
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
uint8_t flash_read_byte(uint32_t Address);
#endif /* __FLASH_H */

