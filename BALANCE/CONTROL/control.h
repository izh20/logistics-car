#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#define PI 3.14159265

extern char RED_AREA; //红
extern char GREEN_AREA; //绿
extern char BLUE_AREA; //蓝
extern char FRONT_AREA;
extern char MID_AREA;//
extern char BACK_AREA;
typedef struct
{
  uint8_t target_x;//目标位置的x轴坐标
	uint8_t target_y;//目标位置的y轴坐标 	
} Location;

extern float Straight_KP;
extern float Straight_KI;
extern float yaw_target;
extern int PWM_OUTPUT;
extern float Yaw_target;
extern char qr_line;
extern char material_arrive_flag;
extern char qr_right_translation_flag;//右平移标志位
extern char qr_back_flag;
extern u8 x_axis,y_axis;
extern u8 X_target,Y_target;
extern u8 QR_code_location;
extern u8 material_location;
extern u8 location_arrive_finish_flag;//最终位置到达标志位
extern char place_flag;//放置标志位
extern char home_arrive_flag;//到家标志位
extern char grab_flag;//抓取标志位
extern char circle_count;//循环次数  三个物块共三个循环
void TIM1_UP_IRQHandler(void);  
void Get_Angle(u8 way);
void param_init(void);
int straight_control(float yaw,float yaw_target);
void car_mode_select(void);
void car_locate(void);
void car_locate_control(u8 x_target,u8 y_target);
void task_planning(void);
void go_to_scan_QR(void);
void go_to_scan_QR_1(void);//第二版程序
void grab_task();//抓取任务
void time_delay(int nms);//延时函数
#endif
