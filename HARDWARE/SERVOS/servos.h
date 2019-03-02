#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

typedef struct
{
	uint8_t fram_id;//���id�ţ���Ӧ��pca9685����
	uint16_t duty;//ռ�ձ�
	uint16_t init_position;//�����ʼλ��
} SERVOS;
int degree2duty_180(float degree);
int degree2duty_270(float degree);
void servos_param_init(void);
void servos_control(void);
#endif 

