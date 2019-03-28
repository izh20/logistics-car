#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define HIGH_SPEED 1350
#define MID_SPEED  1300
#define LOW_SPEED  1750

typedef struct
{
    uint8_t  line_patrol_mode;//Ѳ��ģʽ 		��X��Ѳ�ߣ�Y��Ѳ�ߣ�ֹͣѲ��
		uint8_t	 running_mode;		//С������ģʽ	ֹͣ��ǰ�������ˣ���ƽ�ƣ���ƽ��ģʽ  ����x��Ѳ�߶�Ӧ��ǰ�����ˣ�y��Ѳ�߶�Ӧ������ƽ��
    uint8_t	 task_mode;				//С������ģʽ
} CAR_STATUS_t;


enum
{		
    STOP_LINE_PATROL,  //ֹͣѲ��
		X_LINE_PATROL,		 //X��Ѳ��
		Y_LINE_PATROL		 //Y��Ѳ��
};

enum
{		
    STOP,  						 //ֹͣ
		FORWARD,		 			 //ǰ��
		BACK,		 					 //����
		RIGHT_TRANSLATION, //��ƽ��
		LEFT_TRANSLATION	 //��ƽ��
};

extern CAR_STATUS_t	car_status;
extern int CAR_SPEED;    //С���ٶ�
#define chassis_RB_1 PBout(9) // PA4  �Һ���
#define chassis_RB_2 PBout(8) // PA4
#define chassis_LB_1 PBout(7) // PA4	�����
#define chassis_LB_2 PBout(6) // PA4
#define chassis_RF_1 PCout(15) // PA4	��ǰ��
#define chassis_RF_2 PAout(0) // PA4
#define chassis_LF_1 PCout(14) // PA4	��ǰ��
#define chassis_LF_2 PCout(13) // PA4
void chassis_turn_left(void);
void chassis_turn_right(void);
void chassis_init(void);
void chassis_right_translation(void);
void chassis_left_translation(void);
void chassis_back(void);
void chassis_forwrd(void);
void chassis_angle_control(void);
#endif
