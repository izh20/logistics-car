#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

typedef struct
{
	uint8_t fram_id;//舵机id号，对应于pca9685引脚
	uint16_t duty;//占空比
	uint16_t init_position;//舵机初始位置
} SERVOS;
int degree2duty_180(int degree);
int degree2duty_270(int degree);
void servos_param_init(void);
void servos_control(void);
void grab_right_material(void);
void grab_left_material(void);
void grab_mid_material(void);
void servos_init(char mode);//舵机初始化位置
void servos_ready_grab(int qr);//舵机初始化位置
void grab_slowly(char count,char mode ,int time,int current_angle,int target_angle);//缓慢抓取
void grab(int qr);
void servos_put_mid_material(void);
void servos_put_left_material(void);
void servos_put_right_material(void);
#endif 

