#include "udp_client.h"

//UDP 		测试全局状态标记变量
//bit7:		没有用到
//bit6:0,	没有收到数据;1,收到数据了.
//bit5:0,	没有连接上;1,连接上了.
//bit4~0:	保留

extern uint8_t uart1_rx_buff;
extern UART_HandleTypeDef huart1;

uint8_t udp_remote_ip[4] = REMOTE_IPADDR;
uint16_t udp_port_num = REMOTE_PORT;
uint8_t udp_demo_flag = 0;

uint8_t udp_recvbuf[UDP_MAX_RX_DATA] = {0}; 	//UDP接收数据缓冲区
uint8_t udp_sendbuf[UDP_MAX_TX_DATA] = {0}; 	//UDP发送数组
uint16_t udp_rx_lenth = 0;                  	//UDP接收数据长度

ip_addr_t udp_remote_addr;

struct udp_pcb *udppcb; 										//定义一个TCP服务器控制块

//UDP重新修改监听端口
void udp_monitor_reconf(uint8_t *remote_ipaddr, uint16_t port_num)
{
	err_t err;
	
	udp_connection_close(udppcb);
	
	IP4_ADDR(&udp_remote_addr, udp_remote_ip[0], udp_remote_ip[1], udp_remote_ip[2], udp_remote_ip[3]);
	
	udppcb = udp_new();
	if(udppcb)
	{
		err = udp_bind(udppcb, IP_ADDR_ANY, port_num);
	}
	
	if(err == ERR_OK)
	{
		udp_recv(udppcb, udp_recevice_callback, NULL);
	}
	
	udp_connect(udppcb, &udp_remote_addr, port_num);	
}

//UDP监听
void udp_monitor_conf(uint8_t *remote_ipaddr, uint16_t port_num)
{
	err_t err;
	
	IP4_ADDR(&udp_remote_addr, udp_remote_ip[0], udp_remote_ip[1], udp_remote_ip[2], udp_remote_ip[3]);
	
	udppcb = udp_new();
	
	if(udppcb)
	{
		err = udp_bind(udppcb, IP_ADDR_ANY, port_num);
	}
	
	if(err == ERR_OK)
	{
		udp_recv(udppcb, udp_recevice_callback, NULL);
	}
	
	udp_connect(udppcb, &udp_remote_addr, port_num);
}

// //UDP群发数据
// void udp_txd_group_data(uint8_t *p, uint16_t len)
// {
//     if (udppcb->remote_ip.addr == 0)
//     {
//         udppcb->remote_ip.addr = (uint32_t)lwipdev.ip[0] +
//                                  (((uint32_t)lwipdev.ip[1]) << 8) +
//                                  (((uint32_t)lwipdev.ip[2]) << 16) +
//                                  (((uint32_t)lwipdev.ip[3]) << 24);
//         udppcb->remote_port = udp_port_num; //	端口号
//     }
//     udppcb->remote_ip.addr |= 0xFF000000; //群发
//     udp_send_data(udppcb, p, len);
//     //	udppcb->remote_ip.addr = rmtipaddr1;

//     lwip_periodic_handle();
// }

//UDP服务器回调函数
void udp_recevice_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p,const ip_addr_t *addr, uint16_t port)
{
    uint32_t data_len = 0;
    struct pbuf *q;
    if (p != NULL) //接收到不为空的数据时
    {
        memset(udp_recvbuf, 0, UDP_MAX_RX_DATA); //数据接收缓冲区清零
        for (q = p; q != NULL; q = q->next)      //遍历完整个pbuf链表
        {
            //判断要拷贝到UDP_MAX_RX_DATA中的数据是否大于UDP_MAX_RX_DATA的剩余空间，如果大于
            //的话就只拷贝UDP_MAX_RX_DATA中剩余长度的数据，否则的话就拷贝所有的数据
            if (q->len > (UDP_MAX_RX_DATA - data_len))
                memcpy(udp_recvbuf + data_len, q->payload, (UDP_MAX_RX_DATA - data_len)); //拷贝数据
            else
                memcpy(udp_recvbuf + data_len, q->payload, q->len);
            data_len += q->len;
            if (data_len > UDP_MAX_RX_DATA)
                break; //超出TCP客户端接收数组,跳出
        }
        udp_rx_lenth = data_len;                                   //记录接收数据长度
        udp_demo_flag |= 1 << 6;                                   //标记接收到数据了
        pbuf_free(p);                                              //释放内存
    }
    else
    {
        udp_disconnect(upcb);
    }
}

////UDP服务器发送数据
//void udp_send_data(struct udp_pcb *upcb, uint8_t *p, uint16_t len)
//{
//    struct pbuf *ptr;

//    ptr = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL); //申请内存
//    if (ptr)
//    {
//        ptr->payload = (void *)p;
//        udp_sendto(upcb, ptr, &udp_remote_addr, udp_port_num); //udp发送数据
//        pbuf_free(ptr);      //释放内存
//    }
//}

void udp_send_data(struct udp_pcb *upcb, uint8_t *p, uint16_t len)
{
	struct pbuf *ptr;
	ptr = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL); 				//申请内存
	
	if(ptr)
	{
		pbuf_take(ptr, p, len); 																//将sendbuf中的数据打包进pbuf结构中
		udp_sendto(upcb, ptr, &udp_remote_addr, udp_port_num);
		pbuf_free(ptr);																					//释放内存
	} 
}

//关闭UDP连接
void udp_connection_close(struct udp_pcb *upcb)
{
    udp_disconnect(upcb);
    udp_remove(upcb); 		//断开UDP连接
}


static void udp_return_master_msg(void)
{
	uint16_t crc;
	
	udp_sendbuf[0] = 0xC8;
	udp_sendbuf[1] = 0xD9;
	udp_sendbuf[2] = 0xE7;
	udp_sendbuf[3] = 0xD9;	//数据头
	udp_sendbuf[4] = 0x02;	//设备类型
	udp_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
	udp_sendbuf[6] = 0x01; 	//功能码
	udp_sendbuf[7] = 0x19;	//数据长度
	
	udp_sendbuf[8] = zone_struct.zone_type;	//
	udp_sendbuf[9] = zone_struct.zone1_id;
	udp_sendbuf[10] = zone_struct.zone2_id;
	udp_sendbuf[11] = zone_struct.zone1_sta <= 3 ? 1:zone_struct.zone1_sta - 2;
	udp_sendbuf[12] = zone_struct.zone2_sta <= 3 ? 1:zone_struct.zone2_sta - 2;
	
	udp_sendbuf[13] = demolition_sta;
	udp_sendbuf[14] = zone_struct.zone1_voltage_level;
	udp_sendbuf[15] = zone_struct.zone2_voltage_level;
	udp_sendbuf[16] = zone_struct.zone1_sensitivity;
	udp_sendbuf[17] = zone_struct.zone2_sensitivity;
	udp_sendbuf[18] = 4;										//防区属性
	udp_sendbuf[19] = zone_struct.zone1_mode;
	udp_sendbuf[20] = zone_struct.zone2_mode;
	
	udp_sendbuf[21] = IP_ADDRESS[0];
	udp_sendbuf[22] = IP_ADDRESS[1];
	udp_sendbuf[23] = IP_ADDRESS[2];
	udp_sendbuf[24] = IP_ADDRESS[3];
	
	udp_sendbuf[25] = NETMASK_ADDRESS[0];
	udp_sendbuf[26] = NETMASK_ADDRESS[1];
	udp_sendbuf[27] = NETMASK_ADDRESS[2];
	udp_sendbuf[28] = NETMASK_ADDRESS[3];
	
	udp_sendbuf[29] = GATEWAY_ADDRESS[0];
	udp_sendbuf[30] = GATEWAY_ADDRESS[1];
	udp_sendbuf[31] = GATEWAY_ADDRESS[2];
	udp_sendbuf[32] = GATEWAY_ADDRESS[3];	
	
	crc = CRC16(udp_sendbuf, 33);
	
	udp_sendbuf[33] = crc >> 8;
	udp_sendbuf[34] = crc;
	
	udp_sendbuf[35] = 0xF8;
	udp_sendbuf[36] = 0xC6;
	udp_sendbuf[37] = 0xF8;
	udp_sendbuf[38] = 0xC6;
	
	udp_send_data(udppcb, udp_sendbuf, 39);
}

//static void udp_return_set_ok(uint8_t cmd)
//{
//	uint16_t crc;

//	udp_sendbuf[0] = 0xC8;
//	udp_sendbuf[1] = 0xD9;
//	udp_sendbuf[2] = 0xE7;
//	udp_sendbuf[3] = 0xD9;	//数据头
//	udp_sendbuf[4] = 0x02;	//设备类型
//	udp_sendbuf[5] = zone_struct.zone1_id;	//设备ID（防区1ID）
//	udp_sendbuf[6] = cmd; 	//功能码
//	udp_sendbuf[7] = 1;			//数据长度
//	
//	udp_sendbuf[8] = 0xA9;	//
//	
//	crc = CRC16(max485_1_sendbuf, 9);
//	
//	udp_sendbuf[9] = crc >> 8;
//	udp_sendbuf[10] = crc;
//	
//	udp_sendbuf[11] = 0xF8;
//	udp_sendbuf[12] = 0xC6;
//	udp_sendbuf[13] = 0xF8;
//	udp_sendbuf[14] = 0xC6;
//	
//	udp_send_data(udppcb, udp_sendbuf, 15);
//}


/***********************************************************************/
//*******函数名:udp_rx_processing(void)
//*******用途:Tcp接受处理
//*******说明:
//*******参数:无
//*******返回值:无
/***********************************************************************/
void udp_rx_processing(void)
{
	uint32_t i;
//	uint16_t crc;
	uint16_t alarm_delay_s;	//报警延时 单位s
	uint32_t alarm_delay_ms;
	
	master_ctrl_cmd_def cmd;
//	uint8_t zone_num; 								// 0:双防区 1：1防区  2：2防区
	
	if(udp_demo_flag >> 6 & 0x01)
	{
		udp_demo_flag &= ~(0x01 << 6);
	}
	else
	{
		return;
	}
	
	//判断数据头
	if((udp_recvbuf[0] != 0xE7) || (udp_recvbuf[1] != 0xD9) || (udp_recvbuf[2] != 0xE7) || (udp_recvbuf[3] != 0xD9))
	{
		return;
	}
	
	//判断数据类型
	if(udp_recvbuf[4] != 0x02)
	{
		return;
	}
	
	//判断id
	if((udp_recvbuf[5] != zone_struct.zone1_id) && (udp_recvbuf[5] != 0xFF))
	{
		return;
	}
	
	if(udp_rx_lenth != udp_recvbuf[7] + 14)
	{
		return;
	}
	
	//判断数据尾
	if((udp_recvbuf[udp_rx_lenth - 4] != 0xF8) || (udp_recvbuf[udp_rx_lenth - 3] != 0xC6) || (udp_recvbuf[udp_rx_lenth - 2] != 0xF8) || (udp_recvbuf[udp_rx_lenth - 1] != 0xC6))
	{
		return;
	}
	
//	crc = CRC16(udp_recvbuf, udp_rx_lenth - 6);
//	
//	//判断crc
//	if(((uint8_t)(crc>>8) != udp_recvbuf[udp_rx_lenth - 6]) || ((uint8_t)(crc) != udp_recvbuf[udp_rx_lenth - 5]))
//	{
//		return;
//	}
	
	communication_sta = COMMUNICATING;
	cmd = (master_ctrl_cmd_def)udp_recvbuf[6];
		
	switch(cmd)
	{
		case INQUIRE_MSTAER_MAG:
			if(udp_recvbuf[8] == 0xA5)
			{
				udp_return_master_msg();
			}
			break;
		case MODIFY_ZONE_TYPE_ID:
			if(zone_struct.zone1_arm_sta || zone_struct.zone2_arm_sta)		//必须在两个防区都撤防的情况下 才能设定
				break;
			if(udp_recvbuf[8] == 2)		//如果双防区
			{
				temp_zone1_id = udp_recvbuf[9];
				temp_zone2_id = udp_recvbuf[10];
			}
			else if(udp_recvbuf[8] == 1)	//如果单防区
			{
				temp_zone1_id = udp_recvbuf[9];
			}
			set_ctrl_unit(SINGLE_DOUBLE_ZONE, 0xFF, udp_recvbuf[8]);
//			udp_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_LOCAL_IP:
			for(i=0; i<4; i++)
			{
				IP_ADDRESS[i] = udp_recvbuf[8+i];
				NETMASK_ADDRESS[i] = udp_recvbuf[12+i];
				GATEWAY_ADDRESS[i] = udp_recvbuf[16+i];
				local_address_set_buff[i] = IP_ADDRESS[i];
				subnet_mask_set_buff[i] = NETMASK_ADDRESS[i];
				gateway_address_set_buff[i] = GATEWAY_ADDRESS[i];
				flash_data_struct.flash_local_ip[i] = IP_ADDRESS[i];
				flash_data_struct.flash_netmask[i] = NETMASK_ADDRESS[i];
				flash_data_struct.flash_gate_way[i] = GATEWAY_ADDRESS[i];
				write_flash_time_cnt = 0;
				write_flash_flag = 1;
			}
			local_network_set(IP_ADDRESS, NETMASK_ADDRESS, GATEWAY_ADDRESS);
//			udp_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_VOLTAGE_LAVEL:
			if(udp_recvbuf[9] == 0x01)
			{
				set_ctrl_unit(HIGH_LOW_VOLTAGE, 0x01, udp_recvbuf[8]);
			}
			else if(udp_recvbuf[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(HIGH_LOW_VOLTAGE, 0x02, udp_recvbuf[8]);
				else
					break;
			}
			else if(udp_recvbuf[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(HIGH_LOW_VOLTAGE, 0xFF,udp_recvbuf[8]);
				else
					set_ctrl_unit(HIGH_LOW_VOLTAGE, 0x01, udp_recvbuf[8]);
			}
//				udp_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_ZONE_SENSITIVITY:
			if(udp_recvbuf[8] == 0 || udp_recvbuf[8] >= 4)
			{
				break;
			}
			if(udp_recvbuf[9] == 0x01)
			{
					set_ctrl_unit(SENSITIVITY, 0x01, udp_recvbuf[8]);
			}
			else if(udp_recvbuf[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(SENSITIVITY, 0x02, udp_recvbuf[8]);
				else
					break;
			}
			else if(udp_recvbuf[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(SENSITIVITY, 0xFF, udp_recvbuf[8]);
				else
					set_ctrl_unit(SENSITIVITY, 0x01, udp_recvbuf[8]);
			}
//					udp_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_ALARM_DELAY: 
			alarm_delay_s = udp_recvbuf[8];
			alarm_delay_s = (alarm_delay_s << 8) | udp_recvbuf[9];
			alarm_delay_ms = alarm_delay_s * 1000;
		
			if(udp_recvbuf[10] == 0x01)
			{
				zone1_alarm_reset_time = alarm_delay_ms;
			}
			else if(udp_recvbuf[10] == 0x02)
			{
				zone2_alarm_reset_time = alarm_delay_ms;
			}
			else if(udp_recvbuf[10] == 0xFF)
			{
				zone1_alarm_reset_time = zone2_alarm_reset_time = alarm_delay_ms;
			}
			flash_data_struct.flash_zone1_alarm_reset_time = zone1_alarm_reset_time;
			flash_data_struct.flash_zone2_alarm_reset_time = zone2_alarm_reset_time;
//			flash_data_struct.flash_demolition_alarm_reset_time = demolition_alarm_reset_time;
			write_flash_time_cnt = 0;
			write_flash_flag = 1;
//			udp_return_set_ok((uint8_t)cmd);
			break;
		case MODIFY_TRIGGER_DELAY: 
			alarm_delay_s = udp_recvbuf[8];
			alarm_delay_s = (alarm_delay_s << 8) | udp_recvbuf[9];
			alarm_delay_ms = alarm_delay_s * 1000;
		
			if(udp_recvbuf[10] == 0x01)
			{
				zone1_trigger_delay_time = alarm_delay_ms;
			}
			else if(udp_recvbuf[10] == 0x02)
			{
				zone2_trigger_delay_time = alarm_delay_ms;
			}
			else if(udp_recvbuf[10] == 0xFF)
			{
				zone1_trigger_delay_time = zone2_trigger_delay_time = alarm_delay_ms;
			}
			
			flash_data_struct.flash_zone1_trigger_delay_time = zone1_trigger_delay_time;
			flash_data_struct.flash_zone2_trigger_delay_time = zone2_trigger_delay_time;
			write_flash_time_cnt = 0;
			write_flash_flag = 1;
			break;
		case MODIFY_ARM_DISARM:
			//0:撤防  1：布防
			if(udp_recvbuf[9] == 0x01)
			{
				set_ctrl_unit(AMING_DISARM, 0x01, udp_recvbuf[8]);
			}
			else if(udp_recvbuf[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(AMING_DISARM, 0x02, udp_recvbuf[8]);
				else
					break;
			}
			else if(udp_recvbuf[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(AMING_DISARM, 0xFF, udp_recvbuf[8]);
				else
					set_ctrl_unit(AMING_DISARM, 0x01, udp_recvbuf[8]);
			}
//			udp_return_set_ok((uint8_t)cmd);
			break;
		case TMING_CMD:
			break;
		case MODIFY_TOUCH_NET:
			if(udp_recvbuf[9] == 0x01)
			{
				set_ctrl_unit(TOUCH_NET_MODE, 0x01, udp_recvbuf[8]);
			}
			else if(udp_recvbuf[9] == 0x02)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(TOUCH_NET_MODE, 0x02, udp_recvbuf[8]);
				else
					break;
			}
			else if(udp_recvbuf[9] == 0xFF)
			{
				if(zone_struct.zone_type == DOUBLE_ZONE)
					set_ctrl_unit(TOUCH_NET_MODE, 0xFF, udp_recvbuf[8]);
				else
					set_ctrl_unit(TOUCH_NET_MODE, 0x01, udp_recvbuf[8]);
			}
			break;
		default:	break;
	}
}













