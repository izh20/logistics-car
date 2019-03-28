#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define HIGH_SPEED 1350
#define MID_SPEED  1300
#define LOW_SPEED  1750

typedef struct
{
    uint8_t  line_patrol_mode;//巡线模式 		有X轴巡线，Y轴巡线，停止巡线
		uint8_t	 running_mode;		//小车运行模式	停止，前进，后退，左平移，右平移模式  其中x轴巡线对应于前进后退，y轴巡线对应于左右平移
    uint8_t	 task_mode;				//小车任务模式
} CAR_STATUS_t;


enum
{		
    STOP_LINE_PATROL,  //停止巡线
		X_LINE_PATROL,		 //X轴巡线
		Y_LINE_PATROL		 //Y轴巡线
};

enum
{		
    STOP,  						 //停止
		FORWARD,		 			 //前进
		BACK,		 					 //后退
		RIGHT_TRANSLATION, //右平移
		LEFT_TRANSLATION	 //左平移
};

extern CAR_STATUS_t	car_status;
extern int CAR_SPEED;    //小车速度
#define chassis_RB_1 PBout(9) // PA4  右后轮
#define chassis_RB_2 PBout(8) // PA4
#define chassis_LB_1 PBout(7) // PA4	左后轮
#define chassis_LB_2 PBout(6) // PA4
#define chassis_RF_1 PCout(15) // PA4	右前轮
#define chassis_RF_2 PAout(0) // PA4
#define chassis_LF_1 PCout(14) // PA4	左前轮
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
