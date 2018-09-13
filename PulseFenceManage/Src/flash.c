#include "flash.h"

flash_data_typedef flash_data_struct;
uint8_t write_flash_flag = 0;

void write_flash_process(uint8_t cycle_quantity)
{
	static uint8_t cycle_cnt = 0;
	
	if(++cycle_cnt >= cycle_quantity)
	{
		cycle_cnt = 0;
		if(write_flash_flag)
		{
			write_flash_flag = 0;
			flash_write(FLASH_SECTOR_8, (uint8_t *)&flash_data_struct, sizeof(flash_data_struct));
		}
	}
}

void read_data_from_flash(void)
{
	uint8_t i;
	
	if(flash_read_byte(FIRST_START_FLASH_ADDR) != 0x01)
	{
		flash_data_struct.flash_first_start = 0x01;
		flash_data_struct.flash_zone_type = (uint8_t)zone_struct.zone_type;
		flash_data_struct.flash_zone1_id = zone_struct.zone1_id;
		flash_data_struct.flash_zone2_id = zone_struct.zone2_id;
		flash_data_struct.flash_zone_mode = (uint8_t)zone_struct.zone_mode;
		flash_data_struct.flash_voltage_level = (uint8_t)zone_struct.zone_voltage_level;
		flash_data_struct.flash_zone1_sensitivity = (uint8_t)zone_struct.zone1_sensitivity;
		flash_data_struct.flash_zone2_sensitivity = (uint8_t)zone_struct.zone2_sensitivity;
		flash_data_struct.flash_remote_port = tcp_port_num;
		
		for(i=0; i<4; i++)
		{
			flash_data_struct.flash_local_ip[i] = IP_ADDRESS[i];
			flash_data_struct.flash_netmask[i] = NETMASK_ADDRESS[i];
			flash_data_struct.flash_gate_way[i] = GATEWAY_ADDRESS[i];
			flash_data_struct.flash_remote_ip[i] = tcp_remoteip[i];
		}
		HAL_Delay(1000);
		flash_write(FLASH_SECTOR_8, (uint8_t *)&flash_data_struct, sizeof(flash_data_struct));
	}
	else
	{
		flash_read(FLASH_SECTOR_8, (uint8_t *)&flash_data_struct, sizeof(flash_data_struct));
		
		zone_struct.zone_type = (zone_type_def)flash_data_struct.flash_zone_type;
		zone_struct.zone1_id = flash_data_struct.flash_zone1_id;
		zone_struct.zone2_id = flash_data_struct.flash_zone2_id;
		zone_struct.zone_mode = (zone_mode_def)flash_data_struct.flash_zone_mode;
		zone_struct.zone_voltage_level = (zone_voltage_level_def)flash_data_struct.flash_voltage_level;
		zone_struct.zone1_sensitivity = (zone_sensitivity_def)flash_data_struct.flash_zone1_sensitivity;
		zone_struct.zone2_sensitivity = (zone_sensitivity_def)flash_data_struct.flash_zone2_sensitivity;
		tcp_port_num = flash_data_struct.flash_remote_port;
		
		for(i=0; i<4; i++)
		{
			IP_ADDRESS[i] = flash_data_struct.flash_local_ip[i];
			NETMASK_ADDRESS[i] = flash_data_struct.flash_netmask[i];
			GATEWAY_ADDRESS[i] = flash_data_struct.flash_gate_way[i];
			tcp_remoteip[i] = flash_data_struct.flash_remote_ip[i];			
		}
	}
}

uint8_t flash_read_byte(uint32_t Address)
{
	uint8_t value;
	value = *(__IO uint8_t*)(Address);
	return value;
}

void flash_read(uint32_t Sector, uint8_t *p_data, uint16_t data_lenth)
{
	uint16_t i = 0;
	uint32_t addr;
	
	switch(Sector)
	{
		case FLASH_SECTOR_0: 
			addr = 0x08000000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_1:
			addr = 0x08004000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_2:
			addr = 0x08008000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_3:
			addr = 0x0800C000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_4:
			addr = 0x08010000;
			if(data_lenth > 64000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_5:
			addr = 0x08020000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_6:
			addr = 0x08040000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_7:
			addr = 0x08060000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_8:
			addr = 0x08080000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;	
	}
	do
	{
		*(p_data+i) = *(__IO uint8_t*)(addr+i);
	}while(++i < data_lenth);
	
}

void flash_write(uint32_t Sector, uint8_t *p_data, uint16_t data_lenth)
{
	uint16_t i = 0;
	uint32_t addr;
	uint32_t sector_error;
	FLASH_EraseInitTypeDef EraseInit_Struct;
	
	switch(Sector)
	{
		case FLASH_SECTOR_0: 
			addr = 0x08000000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_1:
			addr = 0x08004000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_2:
			addr = 0x08008000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_3:
			addr = 0x0800C000;
			if(data_lenth > 16000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_4:
			addr = 0x08010000;
			if(data_lenth > 64000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_5:
			addr = 0x08020000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_6:
			addr = 0x08040000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_7:
			addr = 0x08060000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;
		case FLASH_SECTOR_8:
			addr = 0x08080000;
			if(data_lenth > 128000)
			{
				return;
			}
			break;			
	}
	
	if(HAL_FLASH_Unlock() != HAL_OK)
	{
		return;
	}
	
	EraseInit_Struct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInit_Struct.Sector = Sector;
	EraseInit_Struct.NbSectors = 1;
	EraseInit_Struct.VoltageRange =	FLASH_VOLTAGE_RANGE_3;
	
//	FLASH_Erase_Sector(Sector, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASHEx_Erase(&EraseInit_Struct, &sector_error);
	
	do
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, *(p_data+i));
	}while(++i < data_lenth);
	
	HAL_FLASH_Lock();
}



