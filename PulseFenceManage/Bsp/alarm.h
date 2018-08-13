#ifndef __ALARM_H
#define __ALARM_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define RELAY_ON		1
#define RELAY_OFF		0

#define DEMOLITED			1
#define DISDEMOLITED	0

void relay_alarm(uint8_t cmd);
void relay_siren(uint8_t cmd);
void buzz_on(void);
void buzz_off(void);

#endif /* __ALARM_H */

