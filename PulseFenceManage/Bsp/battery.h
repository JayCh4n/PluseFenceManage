#ifndef __BATTERY_H
#define __BATTERY_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "display.h"

#define RELAY_ON	1
#define RELAY_OFF	0

#define AC_CONNECTED			1
#define AC_DISCONNECTED		0

#define CHARGEING		1
#define DISCHARGE		0

extern uint8_t bettery_manage_mask;

void relay_battery(uint8_t cmd);
uint8_t ac_detect(void);
float get_battery_voltage(void);
void bettery_manage_process(void);
	
#endif /* __BATTERY_H */











