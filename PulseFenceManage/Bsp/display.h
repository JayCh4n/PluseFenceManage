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

//LCD命令
#define EXT_CMD1					0x30	//扩展命令1
#define EXT_CMD2					0x31	//扩展命令2
#define EXT_CMD3					0x38	//扩展命令3
#define EXT_CMD4					0x39	//扩展命令4

#define SLEEP_OUT					0x94	//退出睡眠模式
#define SLEEP_IN					0x95	//进入睡眠模式

#define GBK_B740 0  //防
#define GBK_C7F8 1  //区
#define GBK_D5FD 2  //正
#define GBK_CFDF 3  //线
#define GBK_B5E7 4  //电
#define GBK_D1B9 5  //压
#define GBK_B8BA 6  //负
#define GBK_D7B4 7  //状
#define GBK_CCAC 8  //态
#define GBK_C4A3 9  //模
#define GBK_CABD 10 //式
#define GBK_B6CF 11 //断
#define GBK_C2F6 12 //脉
#define GBK_B3E5 13 //冲
#define GBK_C1E9 14 //灵
#define GBK_C3F4 15 //敏
#define GBK_B6C8 16 //度
#define GBK_B4A5 17 //触
#define GBK_CDF8 18 //网
#define GBK_D6F7 19 //主
#define GBK_BBFA 20 //机
#define GBK_C0E0 21 //类
#define GBK_D0CD 22 //型
#define GBK_C9E8 23 //设
#define GBK_D6C3 24 //置
#define GBK_B7B5 25 //返
#define GBK_BBD8 26 //回
#define GBK_C8B7 27 //确
#define GBK_B6A8 28 //定
#define GBK_A973 29 //：
#define GBK_CBAB 30 //双
#define GBK_B5A5 31 //单
#define GBK_B1BE 32 //本
#define GBK_B5D8 33 //地
#define GBK_D6B7 34 //址
#define GBK_D7D3 35 //子
#define GBK_D1CA 36 //掩
#define GBK_C2EB 37 //码
#define GBK_B9D8 38 //关
#define GBK_D4B6 39 //远
#define GBK_B6CB 40 //端
#define GBK_BFDA 41 //口
#define GBK_B6CC 42 //短
#define GBK_C2B7 43 //路
#define GBK_B3C7 44 //撤
#define GBK_B2CC 45 //布
#define GBK_C5D4 46 //旁
#define GBK_D7D4 47 //自
#define GBK_BCEC 48 //检
#define GBK_D0A3 49 //校
#define GBK_D7BC 50 //准
#define GBK_C7EB 51 //请
#define GBK_B1A3 52 //保
#define GBK_CEA7 53 //围
#define GBK_C0B8 54 //栏
#define GBK_BDD3 55 //接
#define GBK_BAC3 56 //好
#define GBK_BAF3 57 //后
#define GBK_B0B4 58 //按
#define GBK_BCFC 59 //键
#define GBK_BCCC 60 //继
#define GBK_D0F8 61 //续
#define GBK_D4DA 62 //在
#define GBK_BDF8 63 //进
#define GBK_D0D0 64 //行
#define GBK_C9D4 65 //稍
#define GBK_BAEE 66 //侯
#define GBK_D2D1 67 //已
#define GBK_CDEA 68 //完
#define GBK_B3C9 69 //成

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
