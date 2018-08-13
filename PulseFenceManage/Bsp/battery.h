#ifndef __BATTERY_H
#define __BATTERY_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define RELAY_ON	1
#define RELAY_OFF	0

#define AC_CONNECTED			1
#define AC_DISCONNECTED		0

void relay_battery(uint8_t cmd);
uint8_t ac_detect(void);
float get_battery_voltage(void);

#endif /* __BATTERY_H */











