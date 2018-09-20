/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

/* USER CODE BEGIN 0 */

#include "tcp_client.h"
#include "udp_client.h"

/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/
void _Error_Handler(char * file, int line);

/* USER CODE BEGIN 1 */

uint8_t IP_ADDRESS[4] = {192, 168, 0, 10};
uint8_t NETMASK_ADDRESS[4] = {255, 255, 255, 0};
uint8_t GATEWAY_ADDRESS[4] = {192, 168, 0, 1};
uint8_t detect_net_sta_mask = 0;					//轮询连接服务器标志 2S
/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
//uint8_t IP_ADDRESS[4];
//uint8_t NETMASK_ADDRESS[4];
//uint8_t GATEWAY_ADDRESS[4];

/* USER CODE BEGIN 2 */

void local_network_set(uint8_t *ip_addr, uint8_t *net_mask, uint8_t *gate_way)
{
	uint8_t i;
	
	for(i=0; i<4; i++)
	{
		IP_ADDRESS[i] = ip_addr[i];
		NETMASK_ADDRESS[i] = net_mask[i];
		GATEWAY_ADDRESS[i] = gate_way[i];
	}
	
	netif_set_down(&gnetif);
	
	IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
	
	netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);
	
	netif_set_up(&gnetif);
	
//	tcp_client_connection_close(tcppcb_temp, 0);
}

void network_cable_disconnect_handle(void)
{
	HAL_ETH_Stop(&heth);
	netif_set_down(&gnetif);
	netif_remove(&gnetif);
}

void network_cable_reconnect_handle(void)
{
	HAL_ETH_DeInit(&heth);
	/* IP addresses initialization without DHCP (IPv4) */
	IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
	IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
	IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

	/* add the network interface (IPv4/IPv6) without RTOS */
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	netif_set_up(&gnetif);
}

/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
//  IP_ADDRESS[0] = 192;
//  IP_ADDRESS[1] = 168;
//  IP_ADDRESS[2] = 19;
//  IP_ADDRESS[3] = 10;
//  NETMASK_ADDRESS[0] = 255;
//  NETMASK_ADDRESS[1] = 255;
//  NETMASK_ADDRESS[2] = 255;
//  NETMASK_ADDRESS[3] = 0;
//  GATEWAY_ADDRESS[0] = 192;
//  GATEWAY_ADDRESS[1] = 168;
//  GATEWAY_ADDRESS[2] = 19;
//  GATEWAY_ADDRESS[3] = 1;
  
  /* Initilialize the LwIP stack without RTOS */
  lwip_init();

  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4/IPv6) without RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */
}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

/**
 * ----------------------------------------------------------------------
 * Function given to help user to continue LwIP Initialization
 * Up to user to complete or change this function ...
 * Up to user to call this function in main.c in while (1) of main(void) 
 *-----------------------------------------------------------------------
 * Read a received packet from the Ethernet buffers 
 * Send it to the lwIP stack for handling
 * Handle timeouts if LWIP_TIMERS is set and without RTOS
 * Handle the llink status if LWIP_NETIF_LINK_CALLBACK is set and without RTOS 
 */
void MX_LWIP_Process(void)
{
/* USER CODE BEGIN 4_1 */
	static uint8_t pre_link_sta = 2;				//保存上次网线连接状态 0：断开 1：连接 2：上电不知道连接还是未连接
//	static uint8_t first_cnonect_flag = 1;	//第一次连接标志  		 0: 
//	static uint8_t tcp_connflag = 0;		//tcp连接状态标志位
	uint8_t link_sta = 0;								//网线连接状态
	uint32_t regvalue = 0;							//读取phy芯片寄存器值

/* USER CODE END 4_1 */
  ethernetif_input(&gnetif);
  
/* USER CODE BEGIN 4_2 */
	
/* USER CODE END 4_2 */  
  /* Handle timeouts */
  sys_check_timeouts();

/* USER CODE BEGIN 4_3 */
	
	tcp_rx_processing();
	udp_rx_processing();
	
	if(detect_net_sta_mask)															//2s判断一次网线状态 和tcp服务器连接状态	并做相应处理
	{
		detect_net_sta_mask = 0;
		
		HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);
		link_sta = (uint8_t)(regvalue >> 2) & 0x01;						//读取phy BSR状态寄存器并获取网线接连状态
		
		if(pre_link_sta == 2)												//如果是第一次上电 记录上电时网线状态
		{
			pre_link_sta = link_sta;
			if(!link_sta)
			{
				network_cable_disconnect_handle();			//如果第一次上电时没有插入网线  做网线连接断开处理
			}
		}
		
		if(link_sta != pre_link_sta)								//如果网线状态有变化	
		{
			pre_link_sta = link_sta;									//记录当前网络状态  用于判断下次网络变化
			if(link_sta)
			{
				network_cable_reconnect_handle();				//如果网线插上 做网线插入处理
			}
			else
			{
//				tcp_client_connection_close(tcppcb_temp, 0);
				network_cable_disconnect_handle();			//如果网线断开	做网线断开处理
			}
		}
		
		if(link_sta)
		{
			
//			if(tcp_client_flag & 1<<5)
//			{
//				tcp_connflag = 1;
//			}
//			else if(tcp_connflag)
//			{
//				tcp_connflag = 0;
//			}
//			
//			if(tcp_connflag==0 && (tcp_client_flag & 1<<5)==0)		//如果未连接到tcp服务器
//			{
//				if(first_cnonect_flag)															//如果是第一次连接tcp服务器
//				{
//					first_cnonect_flag = 0;		
//					tcp_client_connect(tcp_remoteip, tcp_port_num);		
//				}
//				else																								//如果不是第一次连接
//				{
//					tcp_client_connection_close(tcppcb_temp, 0);
//					tcp_client_connect(tcp_remoteip, tcp_port_num);
//				}
//			}
		}
	}		

/* USER CODE END 4_3 */
}

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */
	
  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */	
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */	
  return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
