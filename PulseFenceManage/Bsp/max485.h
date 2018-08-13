#ifndef __MAX_485_H
#define __MAX_485_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define MAX485_1		1
#define MAX485_2		2

void max485_send_str(uint8_t device, uint8_t *str, uint16_t lenth);

#endif /*__MAX_485_H */


