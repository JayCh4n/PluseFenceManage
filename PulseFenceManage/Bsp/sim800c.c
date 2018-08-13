/**
  ******************************************************************************
  * File Name          : sim800c.c
  * Description        : sim800c驱动及功能
  ******************************************************************************
	*
  *
  ******************************************************************************
**/

#include "sim800c.h"

uint8_t uart6_rx_buff;
UART_RX_TypeDef uart6_rx_struct = {0,0,0,0};

extern UART_HandleTypeDef huart6;
/***********************************************************************/
//*******函数名:check_sim800c_cmd()
//*******用途:比较sim800c返回的命令和状态
//*******说明:
//*******参数:orig_cmd：原命令字符串 orig_data:原数据（状态）字符串  comp_cmd：比较命令字符串 comp_data：比较数据字符串
//*******返回值:0：失败，1：成功
/***********************************************************************/
static uint8_t check_sim800c_cmd(uint8_t *orig_cmd, uint8_t *orig_data, uint8_t *comp_cmd, uint8_t *comp_data)
{
	do
	{
		if(*(++orig_cmd) != *(++comp_cmd))
		{
			return 0;
		}
	}while((*comp_cmd) != '\n');
	
	do
	{
		if(*(++orig_data) != *(++comp_data))
		{
			return 0;
		}
	}while((*comp_data) != '\n');
	
	return 1;
}

/***********************************************************************/
//*******函数名:sim800c_pwr()
//*******用途:sim800c开机
//*******说明:开机
//*******参数:wait_time:开机等待超时时间
//*******返回值:0：（开机）失败，1：成功
/***********************************************************************/
static uint8_t sim800c_pwr(int8_t wait_time)
{
	do
	{	
		HAL_GPIO_WritePin(SIM800_PWR_GPIO_Port, SIM800_PWR_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(SIM800_PWR_GPIO_Port, SIM800_PWR_Pin, GPIO_PIN_RESET);
		
		if(--wait_time < 0)
		{
			return 0;		//初始化失败
		}
		HAL_Delay(2500);
	}while(HAL_GPIO_ReadPin(SIM800_STATUS_GPIO_Port, SIM800_STATUS_Pin) == GPIO_PIN_RESET);	//如果
	
	return 1;
}

/***********************************************************************/
//*******函数名:sim800c_sync_uart_baud()
//*******用途:同步波特率
//*******说明:
//*******参数:wait_time:等待超时时间
//*******返回值:失败，1：成功
/***********************************************************************/

static uint8_t sim800c_sync_uart_baud(int8_t wait_time)
{
	do
	{
		if(--wait_time < 0)
		{
			return 0;		//初始化失败
		}
		HAL_UART_Transmit(&huart6, "AT\n", 3, 1000);
		uart6_rx_struct.rx_enable = 1;
		uart6_rx_struct.rx_cnt = 0;
		uart6_rx_struct.rx_end = 0;
		uart6_rx_struct.cmd_data_flag = 0;
		HAL_Delay(1000);
	}while(!uart6_rx_struct.rx_end);	//如果
	
	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT\r\n", "OK\r\n"))
	{
		return 0;
	}
	
	return 1;
}

//检查sim卡是否存在 0：不存在 1：存在
static uint8_t check_sim_exist(void)
{
	do
	{
	HAL_UART_Transmit(&huart6,  "AT+CPIN?\n", 9, 1000);
	uart6_rx_struct.rx_enable = 1;
	uart6_rx_struct.rx_cnt = 0;
	uart6_rx_struct.rx_end = 0;
	uart6_rx_struct.cmd_data_flag = 0;
	HAL_Delay(50);	
	}while(!uart6_rx_struct.rx_end);
	
	if(!check_sim800c_cmd(uart6_rx_struct.rx_cmd, uart6_rx_struct.rx_data, "AT+CPIN?\r\n", "+CPIN: READY\r\n"))
	{
		return 0;
	}
	
	return 1;
}

/***********************************************************************/
//*******函数名:sim800c_init()
//*******用途:sim800c模块初始化
//*******说明:开机
//*******参数:wait_time:初始化等待超时时间
//*******返回值:0：初始化（开机）失败，1：成功
/***********************************************************************/
uint8_t sim800c_init(int8_t pwr_wait_time, int8_t baud_wait_time)
{
	if(!sim800c_pwr(pwr_wait_time))
	{
		return 0;
	}
	HAL_Delay(2500);
	if(!sim800c_sync_uart_baud(baud_wait_time))
	{
		return 0;
	}
	
	if(!check_sim_exist())
	{
		return 0;
	}
	
	return 1;
}




