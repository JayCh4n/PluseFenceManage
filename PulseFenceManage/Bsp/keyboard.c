#include "keyboard.h"

uint8_t key_sta[5] = {1, 1, 1, 1, 1};					//按键状态 1：抬起 0：按下
uint8_t long_press_sta[5] = {0, 0, 0, 0, 0};	//按键长按状态	0：未长按 1：长按
uint8_t pre_long_press_sta[5] = {0, 0, 0, 0, 0};

void key_action(uint8_t key_num)
{
	switch(key_num)
	{
		case 0:	led_dismantle(LED_ON);  break;
		case 1:	 	break;
		case 2:		break;
		case 3:		break;
		case 4:		break;
		default: break;
	}
}

void long_press_action(uint8_t key_num)
{
	switch(key_num)
	{
		case 0:	lcd_show_menu_page();  break;
		case 1:	 	break;
		case 2:		break;
		case 3:		break;
		case 4:	  break;
		default:  break;
	}
}

void key_drive(void)
{
	uint8_t i;
	static uint8_t pre_key_sta[5] = {1, 1, 1, 1, 1};
	
	for(i=0; i<5; i++)
	{		
		if(long_press_sta[i])
		{
			long_press_action(i);
			long_press_sta[i] = 0;
			pre_long_press_sta[i] = 1;
		}
		else
		{
			if(key_sta[i] != pre_key_sta[i])
			{
				pre_key_sta[i] = key_sta[i];
				
				if(key_sta[i] == 1)
				{
					if(pre_long_press_sta[i])				//防止长按抬起时触发短按功能
					{
						pre_long_press_sta[i] = 0;		
					}
					else
					{
						key_action(i);					
					}
				}
			}
		}
	}
}

void key_scan(void)
{
	uint8_t i;
	static uint16_t long_press_cnt[5] = {0};										//长按计数
	static uint8_t key_buff[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	for(i=0; i<5; i++)
	{
		key_buff[i] = (key_buff[i] << 1) | ((GPIOE->IDR >> (i+7)) & 0x01);	//读取io状态
		
		if(key_buff[i] == 0xFF)				//如果8次检测都为高电平 则认为抬起	定时器定时每1ms扫描一次
		{
			key_sta[i] = 1;
			long_press_cnt[i] = 0;
		}
		else if(key_buff[i] == 0x00)
		{
			if(!pre_long_press_sta[i])		//防止一直触发长按功能
			{
				key_sta[i] = 0;
				if(++long_press_cnt[i] >= 2000)
				{
					long_press_cnt[i] = 0;
					long_press_sta[i] = 1;
				}
			}
		}
	}
}
