#ifndef __UDP_CLIENT_H
#define __UDP_CLIENT_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "lwip.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
//#include "malloc.h"
#include "stdio.h"
#include "string.h"
#include "master_manage.h"
#include "max485.h"
#include "display.h"
#include "flash.h"
#include "alarm.h"

#define UDP_MAX_RX_DATA 100
#define UDP_MAX_TX_DATA 100

extern uint8_t udp_remote_ip[4];
extern uint16_t udp_port_num;
extern uint8_t udp_demo_flag;

void udp_monitor_conf(uint8_t *remote_ipaddr, uint16_t port_num);
void udp_monitor_reconf(uint8_t *remote_ipaddr, uint16_t port_num);
void udp_recevice_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port);
void udp_send_data(struct udp_pcb *upcb, uint8_t *p, uint16_t len);
void udp_connection_close(struct udp_pcb *upcb);
void udp_rx_processing(void);
// void udp_txd_group_data(uint8_t *p, uint16_t len);

#endif /* __UDP_CLIENT_H */
