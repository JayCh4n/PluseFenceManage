#include "keyboard.h"

uint8_t key_sta[5] = {1, 1, 1, 1, 1};					//����״̬ 1��̧�� 0������
uint8_t long_press_sta[5] = {0, 0, 0, 0, 0};	//��������״̬	0��δ���� 1������
uint8_t pre_long_press_sta[5] = {0, 0, 0, 0, 0};

static void long_press_key2_action(void)
{
	value_continuous_add_flag = 1; //�������ӱ�־λ  dynamic_lcd_process(void)��ÿ������ʱ��10
}

static void long_press_key4_action(void)
{
	value_continuous_sub_flag = 1; //����������־λ  dynamic_lcd_process(void)��ÿ������ʱ��10
}

/***********************************************************************/
//*******������:long_press_key5_action()
//*******��;:��������5����
//*******˵��:
//*******����:
//*******����ֵ:��
/***********************************************************************/

static void long_press_key5_action(void)
{
	if(page_sta == IN_MAIN_PAGE)	/*������������沢���ڳ���״̬�³�������5�������˵�*/
	{
		if(zone_struct.arm_sta == 0)
		{
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ
		}
	}
}

static void short_press_key1_action(void)
{
	if(page_sta == IN_MAIN_PAGE)
	{
		set_ctrl_unit(AMING_DISARM, (zone_struct.arm_sta ^ 0x01));
	}
	else if(page_sta == IN_MENU_PAGE)
	{
		lcd_show_main_page();
		page_sta = IN_MAIN_PAGE;
	}
	else if(page_sta == IN_AUTO_DETECT_PAGE)
	{
		if((auto_detect_page_cursor_sta == AT_COMFIRM) || (auto_detect_page_cursor_sta == AT_AUTO_DETECT_COMPLATED))
		{
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ
		}
	}
}

static void short_press_key2_action(void)
{
	uint8_t num_string[4];
	uint8_t port_string[6];
	
	if(page_sta == IN_MAIN_PAGE)
	{
		if(zone_struct.arm_sta == 1)
		{	
			if(zone_struct.zone1_sensitivity == SENSITIVITY_1)
			{
				set_ctrl_unit(ZONE1_SENSITIVITY, (uint8_t)(zone_struct.zone1_sensitivity ^ 0x03));
			}
			else
			{
				set_ctrl_unit(ZONE1_SENSITIVITY, (uint8_t)(zone_struct.zone1_sensitivity ^ (zone_struct.zone1_sensitivity - 1)));
			}
		}
	}
	
	else if(page_sta == IN_MENU_PAGE)	//����ڲ˵�����
	{
		if(menu_page_coursor_sta == AT_MASTER_TYPE_SET)	//������ָ����������/ID
		{
			clear_screen(5,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(14,1); //�ػ�ʵ��Բ
			menu_page_coursor_sta = AT_AUTO_DETECT;	//���״ָ̬��ȷ��
		}
		else if(menu_page_coursor_sta == AT_AUTO_DETECT)	//������ָ��ȷ��
		{
			clear_screen(14,1,2,16); //���ʵ��Բ
			lcd_show_solid_circle(11, 1); //����ʵ��Բ
			menu_page_coursor_sta = AT_REMOTE_IP_SET;	//���״ָ̬�򷵻�
		}
		else if(menu_page_coursor_sta == AT_REMOTE_IP_SET)	//������ָ��ȷ��
		{
			clear_screen(11,1,2,16); //���ʵ��Բ
			lcd_show_solid_circle(8, 1); //����ʵ��Բ
			menu_page_coursor_sta = AT_LOCAL_IP_SET;	//���״ָ̬�򷵻�
		}
		else if(menu_page_coursor_sta == AT_LOCAL_IP_SET)	//������ָ��ȷ��
		{
			clear_screen(8,1,2,16); //���ʵ��Բ
			lcd_show_solid_circle(5, 1); //����ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET;	//���״ָ̬�򷵻�
		}
	}
	
	else if(page_sta == IN_MASTER_TYPE_SET_PAGE)	//������������ͽ���
	{
		if(master_type_set_page_cursor_sta == AT_ZONE_TYPE)	//������ָ����������
		{
			clear_screen(4,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(17,129); //�ػ�ʵ��Բ
			master_type_set_page_cursor_sta = AT_OK_MASTER_TYPE_SET_PAGE;	//���ָ��ok
		}
		else if(master_type_set_page_cursor_sta == AT_OK_MASTER_TYPE_SET_PAGE)	//������ָ��ok
		{
			lcd_show_hollow_circle(17, 129);	//���ƿ���Բ
			lcd_show_solid_circle(17, 49); //����ʵ��Բ
			master_type_set_page_cursor_sta = AT_BACK_MASTER_TYPE_SET_PAGE;	//���״ָ̬�򷵻�
		}
		else if(master_type_set_page_cursor_sta == AT_BACK_MASTER_TYPE_SET_PAGE)	//������ָ�򷵻�
		{
			lcd_show_hollow_circle(17, 49);	//���ƿ���Բ
			lcd_show_solid_circle(10, 1); //����ʵ��Բ
			master_type_set_page_cursor_sta = AT_ZONG2_ID;	//���ָ�����2ID
		}
		else if(master_type_set_page_cursor_sta == AT_ZONG2_ID)
		{
			clear_screen(10,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(7,1);//����ʵ��Բ
			master_type_set_page_cursor_sta = AT_ZONE1_ID;	//���ָ�����1ID
		}
		else if(master_type_set_page_cursor_sta == AT_ZONE1_ID)
		{
			clear_screen(7,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(4,1);//����ʵ��Բ
			master_type_set_page_cursor_sta = AT_ZONE_TYPE;	//���ָ����������/ID
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE_TYPE)
		{
			zone_struct_set_buff.zone_type ^= 0x03;	//������Ϊ0 ˫����Ϊ1 ���0x01 ��0��1֮��仯
			lcd_show_chs_16x16(4, 121, double_single_gbk_code+(zone_struct_set_buff.zone_type - 1), 1);
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE1_ID)
		{
			if(++zone_struct_set_buff.zone1_id == 0)
			{
				zone_struct_set_buff.zone1_id = 1;			//���ϼ�����id��һ  ������999
			}
			num_to_string(num_string, zone_struct_set_buff.zone1_id);
			lcd_show_str_8x16(7, 105, num_string);
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE2_ID)
		{
			if(++zone_struct_set_buff.zone2_id == 0)
			{
				zone_struct_set_buff.zone2_id = 1;			//���ϼ�����id��һ  ������999
			}
			num_to_string(num_string, zone_struct_set_buff.zone2_id);
			lcd_show_str_8x16(10, 105, num_string);
		}	
	}
	
	else if(page_sta == IN_LOCAL_IP_SET_PAGE)	//�����IP���ý���
	{
		if(local_ip_set_page_cursor_sta == AT_LOCAL_ADDRESS)	//������ָ�򱾵ص�ַ
		{
			clear_screen(5,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(17,129); //�ػ�ʵ��Բ
			local_ip_set_page_cursor_sta = AT_OK_LOCAL_IP_SET_PAGE;	//���ָ��ok
		}
		else if(local_ip_set_page_cursor_sta == AT_OK_LOCAL_IP_SET_PAGE)	//������ָ��ok
		{
			lcd_show_hollow_circle(17, 129);	//���ƿ���Բ
			lcd_show_solid_circle(17, 49); //����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_BACK_LOCAL_IP_SET_PAGE;	//���״ָ̬�򷵻�
		}
		else if(local_ip_set_page_cursor_sta == AT_BACK_LOCAL_IP_SET_PAGE)	//������ָ�򷵻�
		{
			lcd_show_hollow_circle(17, 49);	//���ƿ���Բ
			lcd_show_solid_circle(11, 1); //����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_GATEWAY_ADDRESS;	//���ָ��Զ��IP
		}
		else if(local_ip_set_page_cursor_sta == AT_GATEWAY_ADDRESS)
		{
			clear_screen(11,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(8,1);//����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_SUBNET_MASK;	//���ָ�����ص�ַ
		}
		else if(local_ip_set_page_cursor_sta == AT_SUBNET_MASK)
		{
			clear_screen(8,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(5,1);//����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_LOCAL_ADDRESS;	//���ָ�򱾵ص�ַ
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS1)
		{
			if(++local_address_set_buff[0] > 255)
			{
				local_address_set_buff[0] = 255;
			}
			num_to_string(num_string, local_address_set_buff[0]);
			lcd_show_str_8x16(5, 89, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS2)
		{
			if(++local_address_set_buff[1] > 255)
			{
				local_address_set_buff[1] = 255;
			}
			num_to_string(num_string, local_address_set_buff[1]);
			lcd_show_str_8x16(5, 121, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS3)
		{
			if(++local_address_set_buff[2] > 255)
			{
				local_address_set_buff[2] = 255;
			}
			num_to_string(num_string, local_address_set_buff[2]);
			lcd_show_str_8x16(5, 153, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS4)
		{
			if(++local_address_set_buff[3] > 255)
			{
				local_address_set_buff[3] = 255;
			}
			num_to_string(num_string, local_address_set_buff[3]);
			lcd_show_str_8x16(5, 185, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK1)
		{
			if(++subnet_mask_set_buff[0] > 255)
			{
				subnet_mask_set_buff[0] = 255;
			}				
			num_to_string(num_string, subnet_mask_set_buff[0]);
			lcd_show_str_8x16(8, 89, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK2)
		{
			if(++subnet_mask_set_buff[1] > 255)
			{
				subnet_mask_set_buff[1] = 255;
			}			
			num_to_string(num_string, subnet_mask_set_buff[1]);
			lcd_show_str_8x16(8, 121, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK3)
		{
			if(++subnet_mask_set_buff[2] > 255)
			{
				subnet_mask_set_buff[2] = 255;
			}
			num_to_string(num_string, subnet_mask_set_buff[2]);
			lcd_show_str_8x16(8, 153, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK4)
		{
			if(++subnet_mask_set_buff[3] > 255)
			{
				subnet_mask_set_buff[3] = 255;
			}
			num_to_string(num_string, subnet_mask_set_buff[3]);
			lcd_show_str_8x16(8, 185, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS1)
		{
			if(++gateway_address_set_buff[0] > 255)
			{
				gateway_address_set_buff[0] = 255;
			}
			num_to_string(num_string, gateway_address_set_buff[0]);
			lcd_show_str_8x16(11, 89, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS2)
		{
			if(++gateway_address_set_buff[1] > 255)
			{
				gateway_address_set_buff[1] = 255;
			}		
			num_to_string(num_string, gateway_address_set_buff[1]);
			lcd_show_str_8x16(11, 121, num_string);
		}		
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS3)
		{
			if(++gateway_address_set_buff[2] > 255)
			{
				gateway_address_set_buff[2] = 255;
			}
			num_to_string(num_string, gateway_address_set_buff[2]);
			lcd_show_str_8x16(11, 153, num_string);			
		}	
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS4)
		{
			if(++gateway_address_set_buff[3] > 255)
			{
				gateway_address_set_buff[3] = 255;
			}
			num_to_string(num_string, gateway_address_set_buff[3]);
			lcd_show_str_8x16(11, 185, num_string);			
		}	
	}
	
	else if(page_sta == IN_REMOTE_IP_SET_PAGE)
	{
		if(remote_ip_set_page_cursor_sta == AT_REMOTE_PORT)
		{
			clear_screen(7,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(17,129);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_OK_REMOTE_IP_SET_PAGE;
		}
		else if(remote_ip_set_page_cursor_sta == AT_OK_REMOTE_IP_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 129);	//���ƿ���Բ
			lcd_show_solid_circle(17,49);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_BACK_REMOTE_IP_SET_PAGE;
		}	
		else if(remote_ip_set_page_cursor_sta == AT_BACK_REMOTE_IP_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 49);	//���ƿ���Բ
			lcd_show_solid_circle(10,1);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_REMOTE_ADRESS;
		}
		else if(remote_ip_set_page_cursor_sta == AT_REMOTE_ADRESS)
		{
			clear_screen(10,1,2,16);//���ʵ��Բ
			lcd_show_solid_circle(7,1);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_REMOTE_PORT;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_PORT)
		{
			if(++remote_port_set_buff > 65535)
			{
				remote_port_set_buff = 65535;
			}
			port_to_string(port_string, remote_port_set_buff);
			lcd_show_str_8x16(7, 89, port_string);
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS1)
		{
			if(++remote_address_set_buff[0] > 255)
			{
				remote_port_set_buff = 255;
			}
			num_to_string(num_string, remote_address_set_buff[0]);
			lcd_show_str_8x16(10, 89, num_string);
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS2)
		{
			if(++remote_address_set_buff[1] > 255)
			{
				remote_port_set_buff = 255;
			}
			num_to_string(num_string, remote_address_set_buff[1]);
			lcd_show_str_8x16(10, 121, num_string);
		}	
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS3)
		{
			if(++remote_address_set_buff[2] > 255)
			{
				remote_port_set_buff = 255;
			}
			num_to_string(num_string, remote_address_set_buff[2]);
			lcd_show_str_8x16(10, 153, num_string);
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS4)
		{
			if(++remote_address_set_buff[3] > 255)
			{
				remote_port_set_buff = 255;
			}
			num_to_string(num_string, remote_address_set_buff[3]);
			lcd_show_str_8x16(10, 185, num_string);
		}		
	}
}

static void short_press_key3_action(void)
{
	if(page_sta == IN_MAIN_PAGE)
	{
		if(zone_struct.arm_sta == 1)
		{
			if(zone_struct.zone_type == DOUBLE_ZONE)
			{
				if(zone_struct.zone2_sensitivity == SENSITIVITY_1)
				{
					set_ctrl_unit(ZONE2_SENSITIVITY, (uint8_t)(zone_struct.zone2_sensitivity ^ 0x03));
				}
				else
				{
					set_ctrl_unit(ZONE2_SENSITIVITY, (uint8_t)(zone_struct.zone2_sensitivity ^ (zone_struct.zone2_sensitivity - 1)));
				}
			}
		}
	}
}

static void short_press_key4_action(void)
{
	uint8_t num_string[4];
	uint8_t port_string[6];
	
	if(page_sta == IN_MAIN_PAGE)
	{
		if(zone_struct.arm_sta == 1)
		{
			set_ctrl_unit(TOUCH_NET_MODE, (uint8_t)(zone_struct.zone_mode ^ 0x01));
		}
	}
	else if(page_sta == IN_MENU_PAGE)	//����ڲ˵�����
	{
		if(menu_page_coursor_sta == AT_MASTER_TYPE_SET)	//������ָ����������/ID
		{
			clear_screen(5,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(8,1); //�ػ�ʵ��Բ
			menu_page_coursor_sta = AT_LOCAL_IP_SET;	//���״ָ̬��IP�趨
		}
		else if(menu_page_coursor_sta == AT_LOCAL_IP_SET)	//������ָ��IP�趨
		{
			clear_screen(8,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(11, 1); //����ʵ��Բ
			menu_page_coursor_sta = AT_REMOTE_IP_SET;	//���״ָ̬�򷵻�
		}
		else if(menu_page_coursor_sta == AT_REMOTE_IP_SET)	//������ָ��IP�趨
		{
			clear_screen(11,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(14, 1); //����ʵ��Բ
			menu_page_coursor_sta = AT_AUTO_DETECT;	//���״ָ̬�򷵻�
		}
		else if(menu_page_coursor_sta == AT_AUTO_DETECT)	//������ָ��IP�趨
		{
			clear_screen(14,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(5, 1); //����ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET;	//���״ָ̬�򷵻�
		}		
	}
	else if(page_sta == IN_MASTER_TYPE_SET_PAGE)	//������������ͽ���
	{
		if(master_type_set_page_cursor_sta == AT_ZONE_TYPE)	//������ָ����������
		{
			clear_screen(4,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(7,1); //�ػ�ʵ��Բ
			master_type_set_page_cursor_sta = AT_ZONE1_ID;	//���ָ�����1ID
		}
		else if(master_type_set_page_cursor_sta == AT_ZONE1_ID)	//������ָ�����1ID
		{
			clear_screen(7,1,2,16);	 //���ʵ��Բ
			lcd_show_solid_circle(10, 1); //����ʵ��Բ
			master_type_set_page_cursor_sta = AT_ZONG2_ID;	//���״ָ̬�����2ID
		}
		else if(master_type_set_page_cursor_sta == AT_ZONG2_ID)	//������ָ�����2ID
		{
			clear_screen(10,1,2,16);	 //���ʵ��Բ
			lcd_show_solid_circle(17, 49); //����ʵ��Բ
			master_type_set_page_cursor_sta = AT_BACK_MASTER_TYPE_SET_PAGE;	//���ָ�򷵻�
		}
		else if(master_type_set_page_cursor_sta == AT_BACK_MASTER_TYPE_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 49);	//���ƿ���Բ
			lcd_show_solid_circle(17,129);//����ʵ��Բ
			master_type_set_page_cursor_sta = AT_OK_MASTER_TYPE_SET_PAGE;	//���ָ��OK
		}
		else if(master_type_set_page_cursor_sta == AT_OK_MASTER_TYPE_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 129);	//���ƿ���Բ
			lcd_show_solid_circle(4,1);//����ʵ��Բ
			master_type_set_page_cursor_sta = AT_ZONE_TYPE;	//���ָ���������
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE_TYPE)
		{
			zone_struct_set_buff.zone_type ^= 0x03;	//������Ϊ0 ˫����Ϊ1 ���0x01 ��0��1֮��仯
			lcd_show_chs_16x16(4, 121, double_single_gbk_code+(zone_struct_set_buff.zone_type - 1), 1);
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE1_ID)
		{
			if(--zone_struct_set_buff.zone1_id < 1)
			{
				zone_struct_set_buff.zone1_id = 1;			//���ϼ�����id��һ  ��С����1
			}
			num_to_string(num_string, zone_struct_set_buff.zone1_id);
			lcd_show_str_8x16(7, 105, num_string);
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE2_ID)
		{
			if(--zone_struct_set_buff.zone2_id < 1)
			{
				zone_struct_set_buff.zone2_id = 1;			//���ϼ�����id��һ  ��С����1
			}
			num_to_string(num_string, zone_struct_set_buff.zone2_id);
			lcd_show_str_8x16(10, 105, num_string);
		}			
	}
	else if(page_sta == IN_LOCAL_IP_SET_PAGE)	//������������ͽ���
	{
		if(local_ip_set_page_cursor_sta == AT_LOCAL_ADDRESS)	//������ָ�򱾵ص�ַ
		{
			clear_screen(5,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(8,1); //�ػ�ʵ��Բ
			local_ip_set_page_cursor_sta = AT_SUBNET_MASK;	//���ָ����������
		}
		else if(local_ip_set_page_cursor_sta == AT_SUBNET_MASK)	//������ָ����������
		{
			clear_screen(8,1,2,16);	 //���ʵ��Բ
			lcd_show_solid_circle(11, 1); //����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_GATEWAY_ADDRESS;	//���״ָ̬�����ص�ַ
		}
		else if(local_ip_set_page_cursor_sta == AT_GATEWAY_ADDRESS)	//������ָ�����ص�ַ
		{
			clear_screen(11,1,2,16);	 //���ʵ��Բ
			lcd_show_solid_circle(17, 49); //����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_BACK_LOCAL_IP_SET_PAGE;	//���ָ��Զ�˵�ַ
		}
		else if(local_ip_set_page_cursor_sta == AT_BACK_LOCAL_IP_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 49);	//���ƿ���Բ
			lcd_show_solid_circle(17, 129);//����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_OK_LOCAL_IP_SET_PAGE;	//���ָ��OK
		}		
		else if(local_ip_set_page_cursor_sta == AT_OK_LOCAL_IP_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 129);	//���ƿ���Բ
			lcd_show_solid_circle(5, 1);//����ʵ��Բ
			local_ip_set_page_cursor_sta = AT_LOCAL_ADDRESS;	//���ָ��OK
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS1)
		{
			if(--local_address_set_buff[0] < 1)
			{
				local_address_set_buff[0] = 0;
			}
			num_to_string(num_string, local_address_set_buff[0]);
			lcd_show_str_8x16(5, 89, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS2)
		{
			if(--local_address_set_buff[1] < 1)
			{
				local_address_set_buff[1] = 0;
			}
			num_to_string(num_string, local_address_set_buff[1]);
			lcd_show_str_8x16(5, 121, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS3)
		{
			if(--local_address_set_buff[2] < 1)
			{
				local_address_set_buff[2] = 0;
			}
			num_to_string(num_string, local_address_set_buff[2]);
			lcd_show_str_8x16(5, 153, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS4)
		{
			if(--local_address_set_buff[3] < 1)
			{
				local_address_set_buff[3] = 0;
			}
			num_to_string(num_string, local_address_set_buff[3]);
			lcd_show_str_8x16(5, 185, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK1)
		{
			if(--subnet_mask_set_buff[0] < 1)
			{
				subnet_mask_set_buff[0] = 0;
			}				
			num_to_string(num_string, subnet_mask_set_buff[0]);
			lcd_show_str_8x16(8, 89, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK2)
		{
			if(--subnet_mask_set_buff[1] < 1)
			{
				subnet_mask_set_buff[1] = 0;
			}			
			num_to_string(num_string, subnet_mask_set_buff[1]);
			lcd_show_str_8x16(8, 121, num_string);
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK3)
		{
			if(--subnet_mask_set_buff[2] < 1)
			{
				subnet_mask_set_buff[2] = 0;
			}
			num_to_string(num_string, subnet_mask_set_buff[2]);
			lcd_show_str_8x16(8, 153, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK4)
		{
			if(--subnet_mask_set_buff[3] < 1)
			{
				subnet_mask_set_buff[3] = 0;
			}
			num_to_string(num_string, subnet_mask_set_buff[3]);
			lcd_show_str_8x16(8, 185, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS1)
		{
			if(--gateway_address_set_buff[0] < 1)
			{
				gateway_address_set_buff[0] = 0;
			}
			num_to_string(num_string, gateway_address_set_buff[0]);
			lcd_show_str_8x16(11, 89, num_string);			
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS2)
		{
			if(--gateway_address_set_buff[1] < 1)
			{
				gateway_address_set_buff[1] = 0;
			}		
			num_to_string(num_string, gateway_address_set_buff[1]);
			lcd_show_str_8x16(11, 121, num_string);
		}		
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS3)
		{
			if(--gateway_address_set_buff[2] < 1)
			{
				gateway_address_set_buff[2] = 0;
			}
			num_to_string(num_string, gateway_address_set_buff[2]);
			lcd_show_str_8x16(11, 153, num_string);			
		}	
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS4)
		{
			if(--gateway_address_set_buff[3] < 1)
			{
				gateway_address_set_buff[3] = 0;
			}
			num_to_string(num_string, gateway_address_set_buff[3]);
			lcd_show_str_8x16(11, 185, num_string);			
		}
	}
	
	else if(page_sta == IN_REMOTE_IP_SET_PAGE)
	{
		if(remote_ip_set_page_cursor_sta == AT_REMOTE_PORT)
		{
			clear_screen(7,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(10,1);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_REMOTE_ADRESS;
		}
		else if(remote_ip_set_page_cursor_sta == AT_REMOTE_ADRESS)
		{
			clear_screen(10,1,2,16);	//���ʵ��Բ
			lcd_show_solid_circle(17,49);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_BACK_REMOTE_IP_SET_PAGE;
		}	
		else if(remote_ip_set_page_cursor_sta == AT_BACK_REMOTE_IP_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 49);	//���ƿ���Բ
			lcd_show_solid_circle(17,129);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_OK_REMOTE_IP_SET_PAGE;
		}
		else if(remote_ip_set_page_cursor_sta == AT_OK_REMOTE_IP_SET_PAGE)
		{
			lcd_show_hollow_circle(17, 129);	//���ƿ���Բ
			lcd_show_solid_circle(7,1);//����ʵ��Բ
			remote_ip_set_page_cursor_sta = AT_REMOTE_PORT;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_PORT)
		{
			if(--remote_port_set_buff < 1)
			{
				remote_port_set_buff = 0;
			}
			port_to_string(port_string, remote_port_set_buff);
			lcd_show_str_8x16(7, 89, port_string);
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS1)
		{
			if(--remote_address_set_buff[0] < 1)
			{
				remote_port_set_buff = 0;
			}
			num_to_string(num_string, remote_address_set_buff[0]);
			lcd_show_str_8x16(10, 89, num_string);
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS2)
		{
			if(--remote_address_set_buff[1] < 1)
			{
				remote_port_set_buff = 0;
			}
			num_to_string(num_string, remote_address_set_buff[1]);
			lcd_show_str_8x16(10, 121, num_string);
		}	
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS3)
		{
			if(--remote_address_set_buff[2] < 1)
			{
				remote_port_set_buff = 0;
			}
			num_to_string(num_string, remote_address_set_buff[2]);
			lcd_show_str_8x16(10, 153, num_string);
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS4)
		{
			if(--remote_address_set_buff[3] < 1)
			{
				remote_port_set_buff = 0;
			}
			num_to_string(num_string, remote_address_set_buff[3]);
			lcd_show_str_8x16(10, 185, num_string);
		}		
	}
}

static void short_press_key5_action(void)
{
	uint8_t num_string[4];
	uint8_t port_string[6];
	uint8_t i;
	
	if(page_sta == IN_MAIN_PAGE)
	{
		if(zone_struct.arm_sta == 1)
		{
			set_ctrl_unit(HIGH_LOW_VOLTAGE, (uint8_t)(zone_struct.zone_voltage_level ^ 0x01));
		}
	}
	else if(page_sta == IN_MENU_PAGE)
	{
		if(menu_page_coursor_sta == AT_MASTER_TYPE_SET)
		{
			lcd_show_master_type_set_page();
			lcd_show_solid_circle(4,1);
			master_type_set_page_cursor_sta = AT_ZONE_TYPE;
			page_sta = IN_MASTER_TYPE_SET_PAGE;
		}
		else if(menu_page_coursor_sta == AT_LOCAL_IP_SET)
		{
			lcd_show_local_ip_set_page();
			lcd_show_solid_circle(5,1);
			local_ip_set_page_cursor_sta = AT_LOCAL_ADDRESS;
			page_sta = IN_LOCAL_IP_SET_PAGE;
		}
		else if(menu_page_coursor_sta == AT_REMOTE_IP_SET)
		{
			lcd_show_remote_ip_set_page();
			lcd_show_solid_circle(7,1);
			remote_ip_set_page_cursor_sta = AT_REMOTE_PORT;
			page_sta = IN_REMOTE_IP_SET_PAGE;
		}
		else if(menu_page_coursor_sta == AT_AUTO_DETECT)
		{
			page_sta = IN_AUTO_DETECT_PAGE;
			auto_detect_page_cursor_sta = AT_COMFIRM;
			lcd_show_auto_detect_comfirm_page();
		}
	}
	else if(page_sta == IN_MASTER_TYPE_SET_PAGE)
	{
		if(master_type_set_page_cursor_sta == AT_ZONE_TYPE)
		{
			master_type_set_page_cursor_sta = TOOGLE_ZONE_TYPE;
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE_TYPE)
		{
			lcd_show_chs_16x16(4, 121, double_single_gbk_code+(zone_struct_set_buff.zone_type - 1), 1);
			master_type_set_page_cursor_sta = AT_ZONE_TYPE;
		}
		else if(master_type_set_page_cursor_sta == AT_ZONE1_ID)
		{
			master_type_set_page_cursor_sta = TOOGLE_ZONE1_ID;
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE1_ID)
		{
			num_to_string(num_string, zone_struct_set_buff.zone1_id);
			lcd_show_str_8x16(7, 105, num_string);
			master_type_set_page_cursor_sta = AT_ZONE1_ID;
		}
		else if(master_type_set_page_cursor_sta == AT_ZONG2_ID)
		{
			master_type_set_page_cursor_sta = TOOGLE_ZONE2_ID;
		}
		else if(master_type_set_page_cursor_sta == TOOGLE_ZONE2_ID)
		{
			num_to_string(num_string, zone_struct_set_buff.zone2_id);
			lcd_show_str_8x16(10, 105, num_string);
			master_type_set_page_cursor_sta = AT_ZONG2_ID;
		}
		else if(master_type_set_page_cursor_sta == AT_BACK_MASTER_TYPE_SET_PAGE)
		{
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ
			
			zone_struct_set_buff.zone_type = zone_struct.zone_type;
			zone_struct_set_buff.zone1_id = zone_struct.zone1_id;
			zone_struct_set_buff.zone2_id = zone_struct.zone2_id;			//���ز������趨����
		}
		else if(master_type_set_page_cursor_sta == AT_OK_MASTER_TYPE_SET_PAGE)
		{
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ
			
//			zone_struct.zone_type = zone_struct_set_buff.zone_type;
			set_ctrl_unit(SINGLE_DOUBLE_ZONE, (uint8_t)zone_struct_set_buff.zone_type);
			zone_struct.zone1_id = zone_struct_set_buff.zone1_id;
			zone_struct.zone2_id = zone_struct_set_buff.zone2_id;			//���ز������趨����
			flash_data_struct.flash_zone1_id = zone_struct_set_buff.zone1_id;
			flash_data_struct.flash_zone2_id = zone_struct_set_buff.zone2_id;
			write_flash_flag = 1;
		}
	}
	
	else if(page_sta == IN_LOCAL_IP_SET_PAGE)
	{
		if(local_ip_set_page_cursor_sta == AT_LOCAL_ADDRESS)
		{
			local_ip_set_page_cursor_sta = TOOGLE_LOCAL_ADDRESS1;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS1)
		{
			num_to_string(num_string, local_address_set_buff[0]);
			lcd_show_str_8x16(5, 89, num_string);
			local_ip_set_page_cursor_sta = TOOGLE_LOCAL_ADDRESS2;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS2)
		{
			num_to_string(num_string, local_address_set_buff[1]);
			lcd_show_str_8x16(5, 121, num_string);
			local_ip_set_page_cursor_sta = TOOGLE_LOCAL_ADDRESS3;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS3)
		{
			num_to_string(num_string, local_address_set_buff[2]);
			lcd_show_str_8x16(5, 153, num_string);			
			local_ip_set_page_cursor_sta = TOOGLE_LOCAL_ADDRESS4;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_LOCAL_ADDRESS4)
		{
			num_to_string(num_string, local_address_set_buff[3]);
			lcd_show_str_8x16(5, 185, num_string);			
			local_ip_set_page_cursor_sta = AT_LOCAL_ADDRESS;
		}
		
		else if(local_ip_set_page_cursor_sta == AT_SUBNET_MASK)
		{
			local_ip_set_page_cursor_sta = TOOGLE_SUBNET_MASK1;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK1)
		{
			num_to_string(num_string, subnet_mask_set_buff[0]);
			lcd_show_str_8x16(8, 89, num_string);			
			local_ip_set_page_cursor_sta = TOOGLE_SUBNET_MASK2;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK2)
		{
			num_to_string(num_string, subnet_mask_set_buff[1]);
			lcd_show_str_8x16(8, 121, num_string);			
			local_ip_set_page_cursor_sta = TOOGLE_SUBNET_MASK3;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK3)
		{
			num_to_string(num_string, subnet_mask_set_buff[2]);
			lcd_show_str_8x16(8, 153, num_string);			
			local_ip_set_page_cursor_sta = TOOGLE_SUBNET_MASK4;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_SUBNET_MASK4)
		{
			num_to_string(num_string, subnet_mask_set_buff[3]);
			lcd_show_str_8x16(8, 185, num_string);			
			local_ip_set_page_cursor_sta = AT_SUBNET_MASK;
		}
		
		else if(local_ip_set_page_cursor_sta == AT_GATEWAY_ADDRESS)
		{
			local_ip_set_page_cursor_sta = TOOGLE_GATEWAY_ADDRESS1;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS1)
		{
			num_to_string(num_string, gateway_address_set_buff[0]);
			lcd_show_str_8x16(11, 89, num_string);			
			local_ip_set_page_cursor_sta = TOOGLE_GATEWAY_ADDRESS2;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS2)
		{
			num_to_string(num_string, gateway_address_set_buff[1]);
			lcd_show_str_8x16(11, 121, num_string);			
			local_ip_set_page_cursor_sta = TOOGLE_GATEWAY_ADDRESS3;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS3)
		{
			num_to_string(num_string, gateway_address_set_buff[2]);
			lcd_show_str_8x16(11, 153, num_string);
			local_ip_set_page_cursor_sta = TOOGLE_GATEWAY_ADDRESS4;
		}
		else if(local_ip_set_page_cursor_sta == TOOGLE_GATEWAY_ADDRESS4)
		{
			num_to_string(num_string, gateway_address_set_buff[3]);
			lcd_show_str_8x16(11, 185, num_string);
			local_ip_set_page_cursor_sta = AT_GATEWAY_ADDRESS;
		}
		
		else if(local_ip_set_page_cursor_sta == AT_BACK_LOCAL_IP_SET_PAGE)
		{
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ
			
			for(i=0; i<4; i++)
			{
				local_address_set_buff[i] = IP_ADDRESS[i];
				subnet_mask_set_buff[i] = NETMASK_ADDRESS[i];
				gateway_address_set_buff[i] = GATEWAY_ADDRESS[i];
			}
			
		}
		else if(local_ip_set_page_cursor_sta == AT_OK_LOCAL_IP_SET_PAGE)
		{
			for(i=0; i<4; i++)
			{
				flash_data_struct.flash_local_ip[i] = IP_ADDRESS[i] = local_address_set_buff[i];
				flash_data_struct.flash_netmask[i] = NETMASK_ADDRESS[i] = subnet_mask_set_buff[i];
				flash_data_struct.flash_gate_way[i] = GATEWAY_ADDRESS[i] = gateway_address_set_buff[i];
				
			}			
			local_network_set(IP_ADDRESS, NETMASK_ADDRESS, GATEWAY_ADDRESS);
			write_flash_flag = 1;
			
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ			
		}
	}
	
	else if(page_sta == IN_REMOTE_IP_SET_PAGE)
	{
		if(remote_ip_set_page_cursor_sta == AT_REMOTE_PORT)
		{
			remote_ip_set_page_cursor_sta = TOOGLE_REMOTE_PORT;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_PORT)
		{
			//��ʾport
			port_to_string(port_string, remote_port_set_buff);
			lcd_show_str_8x16(7, 89, port_string);
			remote_ip_set_page_cursor_sta = AT_REMOTE_PORT;
		}
		else if(remote_ip_set_page_cursor_sta == AT_REMOTE_ADRESS)
		{
			remote_ip_set_page_cursor_sta = TOOGLE_REMOTE_ADDRESS1;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS1)
		{
			//
			num_to_string(num_string, remote_address_set_buff[0]);
			lcd_show_str_8x16(10, 89, num_string);
			remote_ip_set_page_cursor_sta = TOOGLE_REMOTE_ADDRESS2;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS2)
		{
			//
			num_to_string(num_string, remote_address_set_buff[1]);
			lcd_show_str_8x16(10, 121, num_string);
			remote_ip_set_page_cursor_sta = TOOGLE_REMOTE_ADDRESS3;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS3)
		{
			//
			num_to_string(num_string, remote_address_set_buff[2]);
			lcd_show_str_8x16(10, 153, num_string);
			remote_ip_set_page_cursor_sta = TOOGLE_REMOTE_ADDRESS4;
		}
		else if(remote_ip_set_page_cursor_sta == TOOGLE_REMOTE_ADDRESS4)
		{
			//
			num_to_string(num_string, remote_address_set_buff[3]);
			lcd_show_str_8x16(10, 185, num_string);
			remote_ip_set_page_cursor_sta = AT_REMOTE_ADRESS;
		}
		
		else if(remote_ip_set_page_cursor_sta == AT_BACK_REMOTE_IP_SET_PAGE)
		{
			remote_port_set_buff = tcp_port_num;
			for(i=0; i<4; i++)
			{
				remote_address_set_buff[i] = tcp_remoteip[i];
			}
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ
		}
		else if(remote_ip_set_page_cursor_sta == AT_OK_REMOTE_IP_SET_PAGE)
		{
			tcp_port_num = remote_port_set_buff;
			flash_data_struct.flash_remote_port = tcp_port_num;
			for(i=0; i<4; i++)
			{
				flash_data_struct.flash_remote_ip[i] = tcp_remoteip[i] = remote_address_set_buff[i];
			}
			remote_network_set(tcp_remoteip, tcp_port_num);
			write_flash_flag = 1;
			
			lcd_show_menu_page();				//��ʾ�˵�ҳ
			lcd_show_solid_circle(5,1);	//��ʵ��Բ
			menu_page_coursor_sta = AT_MASTER_TYPE_SET; //���״ָ̬����������/ID
			page_sta = IN_MENU_PAGE;	//ҳ��״̬�л�Ϊ�˵�ҳ			
		}		
	}
	else if(page_sta == IN_AUTO_DETECT_PAGE)
	{
		if(auto_detect_page_cursor_sta == AT_COMFIRM)
		{
			set_ctrl_unit(AUTO_DETECT, 0x01);
		}
		else if(auto_detect_page_cursor_sta == AT_AUTO_DETECTING)
		{
			
		}
		else if(auto_detect_page_cursor_sta == AT_AUTO_DETECT_COMPLATED)
		{

		}
	}
}

static void key_action(uint8_t key_num)
{
	switch(key_num)
	{
		case 0:	short_press_key1_action();  break;
		case 1:	short_press_key2_action(); 	break;
		case 2:	short_press_key3_action();	break;
		case 3:	short_press_key4_action();	break;
		case 4:	short_press_key5_action();	break;
		default: break;
	}
}

static void long_press_action(uint8_t key_num)
{
	switch(key_num)
	{
		case 0:		break;
		case 1:	long_press_key2_action(); break;
		case 2:		break;
		case 3:	long_press_key4_action();	break;
		case 4:	long_press_key5_action(); break;
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
					if(pre_long_press_sta[i])				//��ֹ����̧��ʱ�����̰�����
					{
						pre_long_press_sta[i] = 0;
						value_continuous_add_flag = 0;
						value_continuous_sub_flag = 0;
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
	static uint16_t long_press_cnt[5] = {0};										//��������
	static uint16_t key_buff[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	for(i=0; i<5; i++)
	{
		if(i == 0)
		{
			key_buff[i] = (key_buff[i] << 1) | HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);	//KEY1
		}
		else
		{
			key_buff[i] = (key_buff[i] << 1) | ((GPIOE->IDR >> (16-i)) & 0x01);	//��ȡio״̬	KEY 2 - 5
		}

		if(key_buff[i] == 0xFF)				//���8�μ�ⶼΪ�ߵ�ƽ ����Ϊ̧��	��ʱ����ʱÿ1msɨ��һ��
		{
			key_sta[i] = 1;
			long_press_cnt[i] = 0;
		}
		else if(key_buff[i] == 0x00)
		{
			if(!pre_long_press_sta[i])		//��ֹһֱ������������
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
