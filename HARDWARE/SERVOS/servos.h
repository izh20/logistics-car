#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

typedef struct
{
	uint8_t fram_id;//舵机id号，对应于pca9685引脚
	uint16_t duty;//占空比
	uint16_t init_position;//舵机初始位置
} SERVOS;

int degree2duty(float degree);
void servos_param_init(void);
void servos_control(void);
#endif 

