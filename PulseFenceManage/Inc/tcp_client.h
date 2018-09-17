#ifndef __TCP_CLIENT_H
#define __TCP_CLIENT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lwip.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
 
//#define TCP_CLIENT_RX_BUFSIZE	200		//����tcp client���������ݳ���

//#define TCP_CLIENT_TX_BUFSIZE	200		//����tcp client��������ݳ���

extern uint8_t tcp_remoteip[4];
extern uint16_t tcp_port_num;
extern uint8_t connect_to_server_mask;
//extern uint32_t TcpRX_Len;	 
//extern uint8_t tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	
//extern uint16_t TcpTX_Len;
//extern uint8_t tcp_txd_buff[TCP_CLIENT_TX_BUFSIZE];	
//extern uint8_t tcp_client_flag;
//extern struct tcp_pcb *tcppcb_temp;
////extern struct tcp_client_struct *es_temp;


////tcp����������״̬
//enum tcp_client_states
//{
//	ES_TCPCLIENT_NONE = 0,		//û������
//	ES_TCPCLIENT_CONNECTED,		//���ӵ��������� 
//	ES_TCPCLIENT_CLOSING,		//�ر�����
//}; 
////LWIP�ص�����ʹ�õĽṹ��
//struct tcp_client_struct
//{
//	uint8_t state;               //��ǰ����״
//	struct tcp_pcb *pcb;    //ָ��ǰ��pcb
//	struct pbuf *p;         //ָ�����/�����pbuf
//};  

//void remote_network_set(uint8_t *ipaddr, uint16_t port_num);
//void tcp_rx_processing(void);	//TCP���ܴ���
//err_t tcp_client_connect(uint8_t *ipaddr, uint16_t port_num);
//err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
//err_t tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err);
//void tcp_client_error(void *arg,err_t err);
//err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
//err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, uint16_t len);
//void tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es);
//void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es );
//void tcp_send_data(uint8_t *data, uint16_t lenth);
#endif /*__TCP_CLIENT_H*/























