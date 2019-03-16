#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265


typedef struct
{
  uint8_t target_x;//Ŀ��λ�õ�x������
	uint8_t target_y;//Ŀ��λ�õ�y������
	 
		
} Location;

extern float Straight_KP;
extern float Straight_KI;
extern float yaw_target;
extern int PWM_OUTPUT;
extern float Yaw_target;
extern char qr_line;
extern char material_arrive_flag;
extern char qr_right_translation_flag;//��ƽ�Ʊ�־λ
extern char qr_back_flag;
extern u8 x_axis,y_axis;
extern u8 X_target,Y_target;
extern u8 QR_code_location;
extern u8 material_location;
extern u8 location_arrive_finish_flag;//����λ�õ����־λ
extern char place_flag;//���ñ�־λ
extern char home_arrive_flag;//���ұ�־λ
extern char grab_flag;//ץȡ��־λ
void TIM1_UP_IRQHandler(void);  
void Get_Angle(u8 way);
void param_init(void);
int straight_control(float yaw,float yaw_target);
void car_mode_select(void);
void car_locate(void);
void car_locate_control(u8 x_target,u8 y_target);
void task_planning(void);
void go_to_scan_QR(void);
void go_to_scan_QR_1(void);//�ڶ������

#endif
