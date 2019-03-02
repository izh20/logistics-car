#include "servos.h"

SERVOS servos_yaw,servos_pitch1,servos_pitch2,servos_claw;

/**************************************************************************
函数功能：舵机结构体初始化
入口参数：
返回  值：无
**************************************************************************/
void servos_param_init()
{
	
}
/**************************************************************************
函数功能：舵机控制函数，根据openmv反馈数据进行物块抓取
入口参数：
返回  值：无
**************************************************************************/
void servos_control()
{
	
}
/*
off=a*degree+b   (0,55),(180,250)
a=77/512   b=103
*/
#define a 1.514815f//1.083333f
#define b 102
int degree2duty_270(float degree)
{
	return (a*degree+b);
}
#define a_180 2.272222//1.083333f
#define b_180 102
int degree2duty_180(float degree)
{
	return (a_180*degree+b_180);
}
///*
//off=a*degree+b   (0,140),(90,300)
//a=77/512   b=103
//*/
//#define a 1.083333f
//#define b 55

