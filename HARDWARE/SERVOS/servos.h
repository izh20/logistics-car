#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

typedef struct
{
	uint8_t fram_id;//���id�ţ���Ӧ��pca9685����
	uint16_t duty;//ռ�ձ�
	uint16_t init_position;//�����ʼλ��
} SERVOS;
int degree2duty_180(int degree);
int degree2duty_270(int degree);
void servos_param_init(void);
void servos_control(void);
void grab_right_material(void);
void grab_left_material(void);
void grab_mid_material(void);
void servos_init(char mode);//�����ʼ��λ��
void servos_ready_grab(int qr);//�����ʼ��λ��
void grab_slowly(char count,char mode ,int time,int current_angle,int target_angle);//����ץȡ
void grab(int qr);
void servos_put_mid_material(void);
void servos_put_left_material(void);
void servos_put_right_material(void);
#endif 

