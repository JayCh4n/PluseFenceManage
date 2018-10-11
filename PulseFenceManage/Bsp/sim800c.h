#ifndef __SIM800C_H
#define __SIM800C_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "string.h"

#define	CHINA_MOBILE	1
#define	CHINA_UNICOM	2

typedef struct
{
	uint8_t rx_enable;
	uint8_t rx_end;
	uint8_t rx_cmd[80];
	uint8_t rx_data[80];
	uint8_t rx_cnt;
	uint8_t cmd_data_flag;
}UART_RX_TypeDef;

extern uint8_t process_step;
extern uint8_t sim800c_init_flag;
extern uint8_t sim800c_link_flag;
extern uint8_t sim800c_try_connect_mask;

extern uint8_t uart6_rx_start;
extern uint8_t uart6_rx_time_cnt;
extern uint8_t wait_tcp_contect_flag;
extern uint16_t wait_tcp_contect_cnt;
extern uint8_t sim800c_heart_mask;

uint8_t sim800c_init(int8_t pwr_wait_time, int8_t baud_wait_time);
void sim800c_process(void);
void transparent_data_processing(void);
#endif /* __SIM800C_H */

