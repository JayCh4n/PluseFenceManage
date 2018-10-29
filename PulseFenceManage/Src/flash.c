#include "flash.h"

flash_data_typedef flash_data_struct;
uint8_t write_flash_flag = 0;

uint16_t write_flash_time_cnt = 0;
uint8_t write_flash_time_mask = 0;

void write_flash_process(void)
{
	if(write_flash_time_mask)
	{
		write_flash_time_mask = 0;
		if(write_flash_flag)
		{
			write_flash_flag = 0;
			STMFLASH_Write(FLASH_SAVE_ADDR, (uint32_t *)&flash_data_struct, FLASH_DATA_SIZE);
		}
	}
}

void read_data_from_flash(void)
{
	uint8_t i;
	
	if(flash_read_byte(FLASH_SAVE_ADDR) != 0x01)
	{
		flash_data_struct.flash_first_start = 0x01;
		flash_data_struct.flash_zone_type = (uint8_t)zone_struct.zone_type;
		flash_data_struct.flash_zone1_id = zone_struct.zone1_id;
		flash_data_struct.flash_zone2_id = zone_struct.zone2_id;
		flash_data_struct.flash_zone_mode = (uint8_t)zone_struct.zone_mode;
		flash_data_struct.flash_voltage_level = (uint8_t)zone_struct.zone_voltage_level;
		flash_data_struct.flash_zone1_sensitivity = (uint8_t)zone_struct.zone1_sensitivity;
		flash_data_struct.flash_zone2_sensitivity = (uint8_t)zone_struct.zone2_sensitivity;
		flash_data_struct.flash_targe_delay_time = targe_delay_time;
		flash_data_struct.flash_zone1_alarm_reset_time = zone1_alarm_reset_time;
		flash_data_struct.flash_zone2_alarm_reset_time = zone2_alarm_reset_time;	
		flash_data_struct.flash_demolition_alarm_reset_time = demolition_alarm_reset_time;
#if	USE_TCP
		flash_data_struct.flash_remote_port = tcp_port_num;
#endif	/* USE_TCP */
#if	USE_UDP
		flash_data_struct.flash_remote_port = udp_port_num;
#endif	/* USE_UDP */
		
		for(i=0; i<4; i++)
		{
			flash_data_struct.flash_local_ip[i] = IP_ADDRESS[i];
			flash_data_struct.flash_netmask[i] = NETMASK_ADDRESS[i];
			flash_data_struct.flash_gate_way[i] = GATEWAY_ADDRESS[i];
#if	USE_TCP
			flash_data_struct.flash_remote_ip[i] = tcp_remoteip[i];
#endif	/* USE_TCP */
#if	USE_UDP
			flash_data_struct.flash_remote_ip[i] = udp_remote_ip[i];
#endif	/* USE_UDP */
		}
		HAL_Delay(200);
//		flash_write(FLASH_SECTOR_11, (uint8_t *)&flash_data_struct, sizeof(flash_data_struct));
		STMFLASH_Write(FLASH_SAVE_ADDR, (uint32_t *)&flash_data_struct, FLASH_DATA_SIZE);
	}
	else
	{
//		flash_read(FLASH_SECTOR_11, (uint8_t *)&flash_data_struct, sizeof(flash_data_struct));
		STMFLASH_Read(FLASH_SAVE_ADDR, (uint32_t *)&flash_data_struct, FLASH_DATA_SIZE);
		
		zone_struct.zone_type = (zone_type_def)flash_data_struct.flash_zone_type;
		zone_struct.zone1_id = flash_data_struct.flash_zone1_id;
		zone_struct.zone2_id = flash_data_struct.flash_zone2_id;
		zone_struct.zone_mode = (zone_mode_def)flash_data_struct.flash_zone_mode;
		zone_struct.zone_voltage_level = (zone_voltage_level_def)flash_data_struct.flash_voltage_level;
		zone_struct.zone1_sensitivity = (zone_sensitivity_def)flash_data_struct.flash_zone1_sensitivity;
		zone_struct.zone2_sensitivity = (zone_sensitivity_def)flash_data_struct.flash_zone2_sensitivity;
		zone1_alarm_reset_time = flash_data_struct.flash_zone1_alarm_reset_time;
		zone2_alarm_reset_time = flash_data_struct.flash_zone2_alarm_reset_time;
		demolition_alarm_reset_time = flash_data_struct.flash_demolition_alarm_reset_time;
		
		tcp_port_num = flash_data_struct.flash_remote_port;

		zone_struct_set_buff.zone_type = zone_struct.zone_type;
		zone_struct_set_buff.zone1_id = zone_struct.zone1_id;
		zone_struct_set_buff.zone2_id = zone_struct.zone2_id;
		zone_struct_set_buff.zone_mode = zone_struct.zone_mode;
		zone_struct_set_buff.zone_voltage_level = zone_struct.zone_voltage_level;
		zone_struct_set_buff.zone1_sensitivity = zone_struct.zone1_sensitivity;
		zone_struct_set_buff.zone2_sensitivity = zone_struct.zone2_sensitivity;
		targe_delay_time = flash_data_struct.flash_targe_delay_time;
		
		for(i=0; i<4; i++)
		{
			IP_ADDRESS[i] = flash_data_struct.flash_local_ip[i];
			NETMASK_ADDRESS[i] = flash_data_struct.flash_netmask[i];
			GATEWAY_ADDRESS[i] = flash_data_struct.flash_gate_way[i];
			local_address_set_buff[i] = IP_ADDRESS[i];
			subnet_mask_set_buff[i] = NETMASK_ADDRESS[i];
			gateway_address_set_buff[i] = GATEWAY_ADDRESS[i];
#if	USE_TCP
			tcp_remoteip[i] = flash_data_struct.flash_remote_ip[i];	
			remote_address_set_buff[i] = tcp_remoteip[i];
#endif	/* USE_TCP */
#if	USE_UDP
			udp_remote_ip[i] = flash_data_struct.flash_remote_ip[i];	
			remote_address_set_buff[i] = udp_remote_ip[i];
#endif	/* USE_UDP */			
		}
		
#if	USE_TCP		
		remote_port_set_buff = tcp_port_num;
#endif	/* USE_TCP */	
#if	USE_UDP		
		remote_port_set_buff = udp_port_num;
#endif	/* USE_UDP */			
	}
}

//����STM32��FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR=FLASH_KEY1;	//д���������.
	FLASH->KEYR=FLASH_KEY2; 
}
//flash����
void STMFLASH_Lock(void)
{
	FLASH->CR|=(uint32_t)1<<31;//����
}
//�õ�FLASH״̬
//����ֵ:
//0,�������
//1,æ 
//2,�����쳣 
uint8_t STMFLASH_GetStatus(void)
{	
	 uint32_t res=0;		
	res=FLASH->SR;  
	if(res&(1<<16))return 1;   		//æ
	else if(res&(1<<4))return 2;	//�����쳣 
	else if(res&(1<<5))return 2;	//�����쳣 
	else if(res&(1<<6))return 2;	//�����쳣 
	else if(res&(1<<7))return 2;	//�����쳣 
	return 0;						//û���κ�״̬/�������.
} 
//�ȴ��������
//time:Ҫ��ʱ�ĳ���(��λ:10ms)
//����ֵ:
//0,���
//2,�����쳣
//0XFF,��ʱ       
uint8_t STMFLASH_WaitDone(uint32_t time)
{
	uint8_t res = 0;
	do
	{
		res = STMFLASH_GetStatus();
		if(res!=1) break;
		HAL_Delay(1);
	}while(--time);
	if(!time)
	{
		return (uint8_t)0xFF;
	}
	else
	{
		return res;
	}
}
//��������
//sectoraddr:������ַ,��Χ��:0~11.
//0~3,16K����;4,64K����;5~11,128K����.
//����ֵ:ִ�����
uint8_t STMFLASH_EraseSector(uint32_t sectoraddr)
{
	uint8_t res=0;
	res=STMFLASH_WaitDone(2000);//�ȴ��ϴβ�������,���2s    
	if(res==0)
	{ 
		FLASH->CR&=~(3<<8);	//���PSIZEԭ��������
		FLASH->CR|=2<<8;	//����Ϊ32bit��,ȷ��VCC=2.7~3.6V֮��!!
		FLASH->CR&=~(0X1F<<3);//���ԭ��������
		FLASH->CR|=sectoraddr<<3;//����Ҫ���������� 
		FLASH->CR|=1<<1;	//�������� 
		FLASH->CR|=1<<16;	//��ʼ����		  
		res=STMFLASH_WaitDone(2000);//�ȴ���������,���2s  
		if(res!=1)			//��æ
		{
			FLASH->CR&=~(1<<1);//�������������־.
		}
	}
	return res;
}
//��FLASHָ����ַдһ����
//faddr:ָ����ַ(�˵�ַ����Ϊ4�ı���!!)
//dat:Ҫд�������
//����ֵ:0,д��ɹ�
//    ����,д��ʧ��
uint8_t STMFLASH_WriteWord(uint32_t faddr, uint32_t dat)
{
	uint8_t res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR&=~(3<<8);	//���PSIZEԭ��������
		FLASH->CR|=2<<8;	//����Ϊ32bit��,ȷ��VCC=2.7~3.6V֮��!!
 		FLASH->CR|=1<<0;	//���ʹ��
		*(__IO uint32_t*)faddr=dat;	//д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������,һ���ֱ��,���100us.
		if(res!=1)//�����ɹ�
		{
			FLASH->CR&=~(1<<0);//���PGλ.
		}
	} 
	return res;
} 
//��ȡָ����ַ��һ����(32λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(__IO uint32_t*)faddr; 
}  
//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return 0;
	else if(addr<ADDR_FLASH_SECTOR_2)return 1;
	else if(addr<ADDR_FLASH_SECTOR_3)return 2;
	else if(addr<ADDR_FLASH_SECTOR_4)return 3;
	else if(addr<ADDR_FLASH_SECTOR_5)return 4;
	else if(addr<ADDR_FLASH_SECTOR_6)return 5;
	else if(addr<ADDR_FLASH_SECTOR_7)return 6;
	else if(addr<ADDR_FLASH_SECTOR_8)return 7;
	else if(addr<ADDR_FLASH_SECTOR_9)return 8;
	else if(addr<ADDR_FLASH_SECTOR_10)return 9;
	else if(addr<ADDR_FLASH_SECTOR_11)return 10; 
	return 11;	
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F(ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.) 
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{ 
	uint8_t status=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
  	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
	STMFLASH_Unlock();									//���� 
 	FLASH->ACR&=~(1<<10);			//FLASH�����ڼ�,�����ֹ���ݻ���!!!�����������ϲŷ����������!
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
	if(addrx<0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
	{
		while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
				status=STMFLASH_EraseSector(STMFLASH_GetFlashSector(addrx));
				if(status)break;	//����������
			}else addrx+=4;
		} 
	}
	if(status==0)
	{
		while(WriteAddr<endaddr)//д����
		{
			if(STMFLASH_WriteWord(WriteAddr,*pBuffer))//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	FLASH->ACR|=1<<10;		//FLASH��������,��������fetch
	STMFLASH_Lock();//����
} 

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}

uint8_t flash_read_byte(uint32_t Address)
{
	uint8_t value;
	value = *(__IO uint8_t*)(Address);
	return value;
}
