#include "udp_client.h"

//UDP 测试全局状态标记变量
//bit7:没有用到
//bit6:0,没有收到数据;1,收到数据了.
//bit5:0,没有连接上;1,连接上了.
//bit4~0:保留

uint8_t udp_remote_ip[4] = {192, 168, 19, 117};
uint16_t udp_port_num = 9000;
uint8_t udp_demo_flag = 0;

uint8_t udp_recvbuf[UDP_MAX_RX_DATA] = {0}; //UDP接收数据缓冲区
uint8_t udp_sendbuf[UDP_MAX_TX_DATA] = {0}; //UDP发送数组
uint16_t udp_rx_lenth = 0;                  //UDP接收数据长度

struct udp_pcb *udppcb_temp; //定义一个TCP服务器控制块

//UDP监听
void udp_monitor_conf(uint8_t *remote_ipaddr, uint16_t port_num)
{
	uint8_t i;
	struct udp_pcb *udppcb;
	ip_addr_t ipaddr;
	err_t err;
	
	for(i=0; i<4; i++)
	{
		ipaddr.addr |= (uint32_t)(remote_ipaddr[i] << (i*8));
	}
	
	udppcb = udp_new();
	if(udppcb)
	{
		err = udp_bind(udppcb, IP_ADDR_ANY, port_num);
	}
	
	if(err == ERR_OK)
	{
		udp_recv(udppcb, udp_recevice_callback, NULL);
	}
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
        // upcb->remote_ip = *addr;                                   //记录远程主机的IP地址
        // upcb->remote_port = port;                                  //记录远程主机的端口号
        // lwipdev.remoteip[0] = upcb->remote_ip.addr & 0xff;         //IADDR4
        // lwipdev.remoteip[1] = (upcb->remote_ip.addr >> 8) & 0xff;  //IADDR3
        // lwipdev.remoteip[2] = (upcb->remote_ip.addr >> 16) & 0xff; //IADDR2
        // lwipdev.remoteip[3] = (upcb->remote_ip.addr >> 24) & 0xff; //IADDR1
        udp_demo_flag |= 1 << 6;                                   //标记接收到数据了
        pbuf_free(p);                                              //释放内存
    }
    else
    {
        udp_disconnect(upcb);
    }
}

//UDP服务器发送数据
void udp_send_data(struct udp_pcb *upcb, uint8_t *p, uint16_t len)
{
    struct pbuf *ptr;

    ptr = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL); //申请内存
    if (ptr)
    {
        ptr->payload = (void *)p;
        udp_send(upcb, ptr); //udp发送数据
        pbuf_free(ptr);      //释放内存
    }
}

//关闭UDP连接
void udp_connection_close(struct udp_pcb *upcb)
{
    udp_disconnect(upcb);
    udp_remove(upcb); //断开UDP连接
}
