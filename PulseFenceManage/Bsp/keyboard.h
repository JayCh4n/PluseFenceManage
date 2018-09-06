#ifndef	__KEYBOARD_H
#define __KEYBOARD_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "display.h"
#include "master_manage.h"
#include "lwip.h"

void key_action(uint8_t key_num);
void key_drive(void);
void key_scan(void);

#endif /* __KEYBOARD_H */

