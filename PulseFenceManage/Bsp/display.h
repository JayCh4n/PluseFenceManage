#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "master_manage.h"
#include "tcp_client.h"
#include "lwip.h"
#include "alarm.h"

#define LED_ON		1
#define LED_OFF 	0

//LCD����
#define EXT_CMD1					0x30	//��չ����1
#define EXT_CMD2					0x31	//��չ����2
#define EXT_CMD3					0x38	//��չ����3
#define EXT_CMD4					0x39	//��չ����4

#define SLEEP_OUT					0x94	//�˳�˯��ģʽ
#define SLEEP_IN					0x95	//����˯��ģʽ

#define GBK_B740 0  //��
#define GBK_C7F8 1  //��
#define GBK_D5FD 2  //��
#define GBK_CFDF 3  //��
#define GBK_B5E7 4  //��
#define GBK_D1B9 5  //ѹ
#define GBK_B8BA 6  //��
#define GBK_D7B4 7  //״
#define GBK_CCAC 8  //̬
#define GBK_C4A3 9  //ģ
#define GBK_CABD 10 //ʽ
#define GBK_B6CF 11 //��
#define GBK_C2F6 12 //��
#define GBK_B3E5 13 //��
#define GBK_C1E9 14 //��
#define GBK_C3F4 15 //��
#define GBK_B6C8 16 //��
#define GBK_B4A5 17 //��
#define GBK_CDF8 18 //��
#define GBK_D6F7 19 //��
#define GBK_BBFA 20 //��
#define GBK_C0E0 21 //��
#define GBK_D0CD 22 //��
#define GBK_C9E8 23 //��
#define GBK_D6C3 24 //��
#define GBK_B7B5 25 //��
#define GBK_BBD8 26 //��
#define GBK_C8B7 27 //ȷ
#define GBK_B6A8 28 //��
#define GBK_A973 29 //��
#define GBK_CBAB 30 //˫
#define GBK_B5A5 31 //��
#define GBK_B1BE 32 //��
#define GBK_B5D8 33 //��
#define GBK_D6B7 34 //ַ
#define GBK_D7D3 35 //��
#define GBK_D1CA 36 //��
#define GBK_C2EB 37 //��
#define GBK_B9D8 38 //��
#define GBK_D4B6 39 //Զ
#define GBK_B6CB 40 //��
#define GBK_BFDA 41 //��
#define GBK_B6CC 42 //��
#define GBK_C2B7 43 //·
#define GBK_B3C7 44 //��
#define GBK_B2CC 45 //��
#define GBK_C5D4 46 //��
#define GBK_D7D4 47 //��
#define GBK_BCEC 48 //��
#define GBK_D0A3 49 //У
#define GBK_D7BC 50 //׼
#define GBK_C7EB 51 //��
#define GBK_B1A3 52 //��
#define GBK_CEA7 53 //Χ
#define GBK_C0B8 54 //��
#define GBK_BDD3 55 //��
#define GBK_BAC3 56 //��
#define GBK_BAF3 57 //��
#define GBK_B0B4 58 //��
#define GBK_BCFC 59 //��
#define GBK_BCCC 60 //��
#define GBK_D0F8 61 //��
#define GBK_D4DA 62 //��
#define GBK_BDF8 63 //��
#define GBK_D0D0 64 //��
#define GBK_C9D4 65 //��
#define GBK_BAEE 66 //��
#define GBK_D2D1 67 //��
#define GBK_CDEA 68 //��
#define GBK_B3C9 69 //��

typedef enum{
	IN_MAIN_PAGE = 1,
	IN_MENU_PAGE,
	IN_MASTER_TYPE_SET_PAGE,
	IN_LOCAL_IP_SET_PAGE,
	IN_REMOTE_IP_SET_PAGE,
	IN_AUTO_DETECT_PAGE
}lcd_page_state;

typedef enum{
	AT_MASTER_TYPE_SET = 1,
	AT_LOCAL_IP_SET,
	AT_REMOTE_IP_SET,
	AT_AUTO_DETECT
}lcd_menu_page_cursor_state;

typedef enum{
	AT_ZONE_TYPE = 1,
	AT_ZONE1_ID,
	AT_ZONG2_ID,
	AT_BACK_MASTER_TYPE_SET_PAGE,
	AT_OK_MASTER_TYPE_SET_PAGE,
	TOOGLE_ZONE_TYPE,
	TOOGLE_ZONE1_ID,
	TOOGLE_ZONE2_ID
}lcd_master_type_set_page_cursor_state;

typedef enum{
	AT_LOCAL_ADDRESS = 1,
	AT_SUBNET_MASK,
	AT_GATEWAY_ADDRESS,
	AT_BACK_LOCAL_IP_SET_PAGE,
	AT_OK_LOCAL_IP_SET_PAGE,
	TOOGLE_LOCAL_ADDRESS1,
	TOOGLE_LOCAL_ADDRESS2,
	TOOGLE_LOCAL_ADDRESS3,
	TOOGLE_LOCAL_ADDRESS4,
	TOOGLE_SUBNET_MASK1,
	TOOGLE_SUBNET_MASK2,
	TOOGLE_SUBNET_MASK3,
	TOOGLE_SUBNET_MASK4,
	TOOGLE_GATEWAY_ADDRESS1,
	TOOGLE_GATEWAY_ADDRESS2,
	TOOGLE_GATEWAY_ADDRESS3,
	TOOGLE_GATEWAY_ADDRESS4,
}lcd_local_ip_set_page_cursor_state;

typedef enum{
	AT_REMOTE_PORT = 1,
	AT_REMOTE_ADRESS,
	AT_BACK_REMOTE_IP_SET_PAGE,
	AT_OK_REMOTE_IP_SET_PAGE,
	TOOGLE_REMOTE_PORT,
	TOOGLE_REMOTE_ADDRESS1,
	TOOGLE_REMOTE_ADDRESS2,
	TOOGLE_REMOTE_ADDRESS3,
	TOOGLE_REMOTE_ADDRESS4
}lcd_remote_ip_set_page_cursor_state;

typedef enum{
	AT_COMFIRM = 1,
	AT_AUTO_DETECTING,
	AT_AUTO_DETECT_COMPLATED
}lcd_auto_detect_page_cursor_state;

extern uint8_t dynamic_lcd_mask;
extern lcd_page_state page_sta;
extern lcd_menu_page_cursor_state menu_page_coursor_sta;
extern lcd_master_type_set_page_cursor_state master_type_set_page_cursor_sta;
extern lcd_local_ip_set_page_cursor_state local_ip_set_page_cursor_sta;
extern lcd_remote_ip_set_page_cursor_state remote_ip_set_page_cursor_sta;
extern lcd_auto_detect_page_cursor_state auto_detect_page_cursor_sta;

extern uint8_t zone_gbk_code[2];
extern uint8_t positive_line_voltage_gbk_code[4];
extern uint8_t negative_line_voltage_gbk_code[4];
extern uint8_t status_gbk_code[2];
extern uint8_t mode_gbk_code[2];
extern uint8_t broken_line_gbk_code[2];
extern uint8_t pulse_gbk_code[2];
extern uint8_t sensitivity_gbk_code[3];
extern uint8_t touch_net_gbk_code[2];
extern uint8_t master_type_gbk_code[4];
extern uint8_t set_gbk_code[2];
extern uint8_t back_gbk_code[2];
extern uint8_t determine_gbk_code[2];
extern uint8_t double_single_gbk_code[2];
extern uint8_t local_gbk_code[2];
extern uint8_t address_gbk_code[2];
extern uint8_t subnet_mask_gbk_code[4];
extern uint8_t gateway_gbk_code[2];
extern uint8_t remote_gbk_code[2];
extern uint8_t pore_gbk_code[2];
extern uint8_t short_circuit[2];
extern uint8_t disarm_gbk_code[2];
extern uint8_t arming_gbk_code[2];
extern uint8_t bypass_gbk_code[2];
extern uint8_t self_test_calibration_gbk_code[4];
extern uint8_t make_sure_gbk_code[2];
extern uint8_t fence_gbk_code[2];
extern uint8_t continue_gbk_code[2];
extern uint8_t in_progress_gnk_code[4];
extern uint8_t please_wait_gbk_code[3];
extern uint8_t completed_gbk_code[3];

void led_arming(uint8_t cmd);
void led_zone1_alarm(uint8_t cmd);
void led_zone2_alarm(uint8_t cmd);
void led_alarm(uint8_t zone_num, uint8_t cmd);
void led_dismantle(uint8_t cmd);
void lcd_transfer_command(uint16_t cmd);
void lcd_transfer_data(uint16_t data);
void lcd_init(void);
void lcd_address(uint8_t page_start, uint16_t column_start, uint8_t page_total, uint16_t column_total);
void clear_screen(uint8_t page_start, uint16_t column_start, uint8_t page_total, uint16_t column_total);
void lcd_show_8x16(uint8_t page_start, uint16_t column_start, uint8_t *data);
void lcd_show_str_8x16(uint8_t page_start, uint16_t column_start, uint8_t *str);
//void lcd_show_str_8x16(uint8_t page_start, uint16_t column_start, uint8_t lenth, uint8_t *str);
void lcd_show_16x16(uint8_t page_start, uint16_t column_start, uint8_t *data);
void lcd_show_chs_16x16(uint8_t page_start, uint16_t column_start, uint8_t *gbk_code, uint8_t lenth);
void lcd_show_32x16(uint8_t page_start, uint16_t column_start, uint8_t *data);
void lcd_show_256x160(uint8_t *pic);
void delay_display(void);

void lcd_show_sensitivity_icon(uint8_t page_start, uint16_t column_start, uint8_t level);
void lcd_show_solid_circle(uint8_t page_start, uint16_t column_start);
void lcd_show_hollow_circle(uint8_t page_start, uint16_t column_start);
void lcd_show_main_page(void);
void lcd_show_menu_page(void);
void lcd_show_master_type_set_page(void);
void lcd_show_local_ip_set_page(void);
void lcd_show_remote_ip_set_page(void);
void lcd_show_auto_detect_comfirm_page(void);

void num_to_string(uint8_t *string, uint16_t num);
void port_to_string(uint8_t *string, uint16_t port_num);
void dynamic_lcd_process(void);
void lcd_update_main_page_process(void);
#endif /* __DISPLAY_H */
