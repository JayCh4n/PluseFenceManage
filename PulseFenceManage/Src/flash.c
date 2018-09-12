#include "flash.h"

void read_data_from_flash(void)
{
	if(flash_read_byte(FIRST_START_FLASH_ADDR) != 0x01)
	{
		if(HAL_FLASH_Unlock() != HAL_OK)
		{
			return;
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ZONE_TYPE_FLASH_ADDR, (uint8_t)zone_struct.zone_type);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ZONE1_ID_FLASH_ADDR, zone_struct.zone1_id);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ZONE2_ID_FLASH_ADDR, zone_struct.zone2_id);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ZONE_MODE_FLASH_ADDR, (uint8_t)zone_struct.zone_mode);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, VOLTAGE_LAVEL_FLASH_ADDR, (uint8_t)zone_struct.zone_voltage_level);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ZONE1_SENSITIVITY_FLASH_ADDR, (uint8_t)zone_struct.zone1_sensitivity);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ZONE2_SENSITIVITY_FLASH_ADDR, (uint8_t)zone_struct.zone2_sensitivity);
		
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, LOCAL_IP0_FLASH_ADDR, IP_ADDRESS[0]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, LOCAL_IP1_FLASH_ADDR, IP_ADDRESS[1]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, LOCAL_IP2_FLASH_ADDR, IP_ADDRESS[2]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, LOCAL_IP3_FLASH_ADDR, IP_ADDRESS[3]);
		
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, SUBNET_MASK0_FLASH_ADDR, NETMASK_ADDRESS[0]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, SUBNET_MASK1_FLASH_ADDR, NETMASK_ADDRESS[1]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, SUBNET_MASK2_FLASH_ADDR, NETMASK_ADDRESS[2]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, SUBNET_MASK3_FLASH_ADDR, NETMASK_ADDRESS[3]);
		
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, GATE_WAY0_FLASH_ADDR, GATEWAY_ADDRESS[0]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, GATE_WAY1_FLASH_ADDR, GATEWAY_ADDRESS[1]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, GATE_WAY2_FLASH_ADDR, GATEWAY_ADDRESS[2]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, GATE_WAY3_FLASH_ADDR, GATEWAY_ADDRESS[3]);
		
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, REMOTE_IP0_FLASH_ADDR, tcp_remoteip[0]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, REMOTE_IP1_FLASH_ADDR, tcp_remoteip[1]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, REMOTE_IP2_FLASH_ADDR, tcp_remoteip[2]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, REMOTE_IP3_FLASH_ADDR, tcp_remoteip[3]);
		
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, REMOTE_PORT_FLASH_ADDR, tcp_port_num);
		
		HAL_Delay(1000);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FIRST_START_FLASH_ADDR, 0x01);
		HAL_FLASH_Lock();
	}
	else
	{
		zone_struct.zone_type = (zone_type_def)flash_read_byte(ZONE_TYPE_FLASH_ADDR);	zone_struct_set_buff.zone_type = zone_struct.zone_type;
		zone_struct.zone1_id = flash_read_byte(ZONE1_ID_FLASH_ADDR);	zone_struct_set_buff.zone1_id = zone_struct.zone1_id;
		zone_struct.zone2_id = flash_read_byte(ZONE2_ID_FLASH_ADDR);	zone_struct_set_buff.zone2_id = zone_struct.zone2_id;
		zone_struct.zone_mode = (zone_mode_def)flash_read_byte(ZONE_MODE_FLASH_ADDR);	zone_struct_set_buff.zone_mode = zone_struct.zone_mode;
		zone_struct.zone_voltage_level = (zone_voltage_level_def)flash_read_byte(VOLTAGE_LAVEL_FLASH_ADDR);	zone_struct_set_buff.zone_voltage_level = zone_struct.zone_voltage_level;
		zone_struct.zone1_sensitivity = (zone_sensitivity_def)flash_read_byte(ZONE1_SENSITIVITY_FLASH_ADDR);	zone_struct_set_buff.zone1_sensitivity = zone_struct.zone1_sensitivity;
		zone_struct.zone2_sensitivity = (zone_sensitivity_def)flash_read_byte(ZONE2_SENSITIVITY_FLASH_ADDR);	zone_struct_set_buff.zone2_sensitivity = zone_struct.zone2_sensitivity;
		
		IP_ADDRESS[0] = flash_read_byte(LOCAL_IP0_FLASH_ADDR);
		IP_ADDRESS[1] = flash_read_byte(LOCAL_IP1_FLASH_ADDR);
		IP_ADDRESS[2] = flash_read_byte(LOCAL_IP2_FLASH_ADDR);
		IP_ADDRESS[3] = flash_read_byte(LOCAL_IP3_FLASH_ADDR);
		
		NETMASK_ADDRESS[0] = flash_read_byte(SUBNET_MASK0_FLASH_ADDR);
		NETMASK_ADDRESS[1] = flash_read_byte(SUBNET_MASK1_FLASH_ADDR);
		NETMASK_ADDRESS[2] = flash_read_byte(SUBNET_MASK2_FLASH_ADDR);
		NETMASK_ADDRESS[3] = flash_read_byte(SUBNET_MASK3_FLASH_ADDR);
		
		GATEWAY_ADDRESS[0] = flash_read_byte(GATE_WAY0_FLASH_ADDR);
		GATEWAY_ADDRESS[1] = flash_read_byte(GATE_WAY1_FLASH_ADDR);
		GATEWAY_ADDRESS[2] = flash_read_byte(GATE_WAY2_FLASH_ADDR);
		GATEWAY_ADDRESS[3] = flash_read_byte(GATE_WAY3_FLASH_ADDR);		
		
		tcp_remoteip[0] = flash_read_byte(REMOTE_IP0_FLASH_ADDR);
		tcp_remoteip[1] = flash_read_byte(REMOTE_IP1_FLASH_ADDR);
		tcp_remoteip[2] = flash_read_byte(REMOTE_IP2_FLASH_ADDR);
		tcp_remoteip[3] = flash_read_byte(REMOTE_IP3_FLASH_ADDR);			
		
		tcp_port_num = flash_read_halfword(REMOTE_PORT_FLASH_ADDR);
	}
}

uint8_t flash_read_byte(uint32_t Address)
{
	uint8_t value;
	value = *(__IO uint8_t*)(Address);
	return value;
}

uint16_t flash_read_halfword(uint32_t Address)
{
	uint16_t value;
	value = *(__IO uint16_t*)(Address);
	return value;
}

void flash_write(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
	if(HAL_FLASH_Unlock() != HAL_OK)
	{
		return;
	}
	
	HAL_FLASH_Program(TypeProgram, Address, Data);
	
	HAL_FLASH_Lock();
}



