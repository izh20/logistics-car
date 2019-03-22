#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#define PI 3.14159265

#define TARGET_ANGLE 170;
#define INIT_ANGLE   180
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
extern char circle_count;//ѭ������  ������鹲����ѭ��
extern char place_order;//������˳�� 1Ϊ��ɫ 2Ϊ��ɫ��3Ϊ��ɫ
extern char near_arear1,near_arear2,near_arear3;//�ӽ�����������������
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
void grab_task();//ץȡ����
void time_delay(int nms);//��ʱ����
void put_material_task(char order);//��ʱС��λ���ڣ�4��0�����괦
#endif
