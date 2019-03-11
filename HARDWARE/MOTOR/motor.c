#include "motor.h"
//PB6	PB7	PB8	PB9	PC13	PC14	PC15	PA0

//#define CAR_SPEED 600//小车速度
#define CAR_STOP  0   //小车停止
int CAR_SPEED=1000;    //小车速度
 /**************************************************************************
作者：周恒
GitHub：
**************************************************************************/
/**************************************************************************
函数功能：底盘gpio初始化
入口参数：无
返回  值：无 
**************************************************************************/

CAR_STATUS_t	car_status;

void chassis_init()
{
	//gpio初始化
	RCC->APB2ENR|=5<<2; //使能 PORTA,B,C 时钟  
	GPIOA->CRL&=0XFFFFFFF0;
	GPIOA->CRL|=0X00000003;//PA0 推挽输出
	//GPIOA->ODR|=0<<0; //PA0 输出低
	GPIOB->CRL&=0X00FFFFFF;
	GPIOB->CRL|=0X33000000;//PB6 PB7 推挽输出
	//GPIOB->ODR|=3<<6; //PB6 PB7 输出
	GPIOB->CRH&=0XFFFFFF00;
	GPIOB->CRH|=0X00000033;//PB8 PB9 推挽输出
	//GPIOB->ODR|=3<<8; //PB8 PB9 输出
	GPIOC->CRH&=0X000FFFFF;
	GPIOC->CRH|=0X33300000;//PC13 PC14 PC15
	//GPIOC->ODR|=7<<13;
	
	
}
/**************************************************************************
函数功能：前进
入口参数：无
返回  值：无 
**************************************************************************/

void chassis_forwrd()
{
	chassis_RB_2=1;
	chassis_RB_1=0;
	chassis_LF_2=1;
	chassis_LF_1=0;
	chassis_LB_1=1;
	chassis_LB_2=0;
	chassis_RF_1=1;
	chassis_RF_2=0;
}
/**************************************************************************
函数功能：右转
入口参数：无
返回  值：无 
**************************************************************************/
void chassis_turn_right()
{
	chassis_RB_1=1;
	chassis_RB_2=0;
	chassis_LF_2=1;
	chassis_LF_1=0;
	chassis_LB_1=1;
	chassis_LB_2=0;
	chassis_RF_2=1;
	chassis_RF_1=0;
}
/**************************************************************************
函数功能：左转
入口参数：无
返回  值：无 
**************************************************************************/
void chassis_turn_left()
{
	chassis_RB_2=1;
	chassis_RB_1=0;
	chassis_LF_1=1;
	chassis_LF_2=0;
	chassis_LB_2=1;
	chassis_LB_1=0;
	chassis_RF_1=1;
	chassis_RF_2=0;
}
/**************************************************************************
函数功能：后退
入口参数：无
返回  值：无 
**************************************************************************/
void chassis_back()
{
	chassis_RB_1=1;
	chassis_RB_2=0;
	chassis_LF_1=1;
	chassis_LF_2=0;
	chassis_LB_2=1;
	chassis_LB_1=0;
	chassis_RF_2=1;
	chassis_RF_1=0;
}
/**************************************************************************
函数功能：左平移
入口参数：无
返回  值：无 
**************************************************************************/
void chassis_left_translation()
{
	chassis_RB_1=1;
	chassis_RB_2=0;
	chassis_LF_1=1;
	chassis_LF_2=0;
	chassis_LB_1=1;
	chassis_LB_2=0;
	chassis_RF_1=1;
	chassis_RF_2=0;
}
/**************************************************************************
函数功能：右平移
入口参数：无
返回  值：无 
**************************************************************************/
void chassis_right_translation()
{
	chassis_RB_2=1;
	chassis_RB_1=0;
	chassis_LF_2=1;
	chassis_LF_1=0;
	chassis_LB_2=1;
	chassis_LB_1=0;
	chassis_RF_2=1;
	chassis_RF_1=0;
}
/**************************************************************************
函数功能：底盘走直线 陀螺仪角度闭环
入口参数：无
返回  值：无 
**************************************************************************/
void chassis_angle_control()
{
	if(car_status.running_mode==FORWARD)//前进
	{
		chassis_forwrd();
		set_motor_pwm(CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT);
	}
	if(car_status.running_mode==BACK)//后退
	{
		chassis_back();
		set_motor_pwm(CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT);
	}
	if(car_status.running_mode==RIGHT_TRANSLATION)//右平移
	{
		chassis_right_translation();
		set_motor_pwm(CAR_SPEED-PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT);
	}
	if(car_status.running_mode==LEFT_TRANSLATION)//左平移
	{
		chassis_left_translation();
		set_motor_pwm(CAR_SPEED+PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT);
	}
	if(car_status.running_mode==STOP)//停止
	{
		set_motor_pwm(CAR_STOP,CAR_STOP,CAR_STOP,CAR_STOP);
	}
}

void mix()
{
	
}

