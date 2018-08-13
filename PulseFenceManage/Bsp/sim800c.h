#ifndef __SIM800C_H
#define __SIM800C_H

#include "main.h"
#include "stm32f4xx_hal.h"

typedef struct
{
	uint8_t rx_enable;
	uint8_t rx_end;
	uint8_t rx_cmd[80];
	uint8_t rx_data[80];
	uint8_t rx_cnt;
	uint8_t cmd_data_flag;
}UART_RX_TypeDef;

uint8_t sim800c_init(int8_t pwr_wait_time, int8_t baud_wait_time);
#endif /* __SIM800C_H */

