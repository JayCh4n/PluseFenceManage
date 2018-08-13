#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define LED_ON		1
#define LED_OFF 	0

//LCD����
#define EXT_CMD1					0x30	//��չ����1
#define EXT_CMD2					0x31	//��չ����2
#define EXT_CMD3					0x38	//��չ����3
#define EXT_CMD4					0x39	//��չ����4

#define SLEEP_OUT					0x94	//�˳�˯��ģʽ
#define SLEEP_IN					0x95	//����˯��ģʽ

#define GBK_B740	0		//��
#define GBK_C7F8	1   //��
#define GBK_D5FD	2   //��
#define GBK_CFDF	3   //��
#define GBK_B5E7	4   //��
#define GBK_D1B9	5   //ѹ
#define GBK_B8BA	6   //��
#define GBK_D7B4	7   //״
#define GBK_CCAC	8   //̬
#define GBK_C4A3	9   //ģ
#define GBK_CABD	10  //ʽ
#define GBK_B6CF	11  //��
#define GBK_C2F6	12  //��
#define GBK_B3E5	13  //��
#define GBK_C1E9	14  //��
#define GBK_C3F4	15  //��
#define GBK_B6C8	16  //��
#define GBK_B4A5  17	//��
#define GBK_CDF8  18	//��
#define GBK_D6F7	19	//��
#define GBK_BBFA	20	//��
#define GBK_C0E0	21	//��
#define GBK_D0CD	22	//��
#define GBK_C9E8	23	//��
#define GBK_D6C3  24	//��
#define GBK_B7B5  25	//��
#define GBK_BBD8  26	//��
#define GBK_C8B7  27	//ȷ
#define GBK_B6A8  28	//��


void led_arming(uint8_t cmd);
void led_zone1_alarm(uint8_t cmd);
void led_zone2_alarm(uint8_t cmd);
void led_dismantle(uint8_t cmd);
void lcd_transfer_command(uint16_t cmd);
void lcd_transfer_data(uint16_t data);
void lcd_init(void);
void lcd_address(uint8_t page_start, uint16_t column_start, uint8_t page_total, uint16_t column_total);
void clear_screen(uint8_t page_start, uint16_t column_start, uint8_t page_total, uint16_t column_total);
void test(void);
void lcd_show_8x16(uint8_t page_start, uint16_t column_start, uint8_t *data);
void lcd_show_str_8x16(uint8_t page_start, uint16_t column_start, uint8_t *str);
//void lcd_show_str_8x16(uint8_t page_start, uint16_t column_start, uint8_t lenth, uint8_t *str);
void lcd_show_16x16(uint8_t page_start, uint16_t column_start, uint8_t *data);
void lcd_show_chs_16x16(uint8_t page_start, uint16_t column_start, uint8_t *gbk_code, uint8_t lenth);
void lcd_show_32x16(uint8_t page_start, uint16_t column_start, uint8_t *data);
void lcd_show_256x160(uint8_t *pic);
void delay_display(void);

void lcd_show_sensitivity_icon(uint8_t page_start, uint16_t column_start, uint8_t level);
void lcd_show_main_page(void);
void lcd_show_menu_page(void);
#endif /* __DISPLAY_H */
