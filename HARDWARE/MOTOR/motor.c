#include "motor.h"
//PB6	PB7	PB8	PB9	PC13	PC14	PC15	PA0

//#define CAR_SPEED 600//С���ٶ�
#define CAR_STOP  0   //С��ֹͣ
int CAR_SPEED=1000;    //С���ٶ�
 /**************************************************************************
���ߣ��ܺ�
GitHub��
**************************************************************************/
/**************************************************************************
�������ܣ�����gpio��ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/

CAR_STATUS_t	car_status;

void chassis_init()
{
	//gpio��ʼ��
	RCC->APB2ENR|=5<<2; //ʹ�� PORTA,B,C ʱ��  
	GPIOA->CRL&=0XFFFFFFF0;
	GPIOA->CRL|=0X00000003;//PA0 �������
	//GPIOA->ODR|=0<<0; //PA0 �����
	GPIOB->CRL&=0X00FFFFFF;
	GPIOB->CRL|=0X33000000;//PB6 PB7 �������
	//GPIOB->ODR|=3<<6; //PB6 PB7 ���
	GPIOB->CRH&=0XFFFFFF00;
	GPIOB->CRH|=0X00000033;//PB8 PB9 �������
	//GPIOB->ODR|=3<<8; //PB8 PB9 ���
	GPIOC->CRH&=0X000FFFFF;
	GPIOC->CRH|=0X33300000;//PC13 PC14 PC15
	//GPIOC->ODR|=7<<13;
	
	
}
/**************************************************************************
�������ܣ�ǰ��
��ڲ�������
����  ֵ���� 
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
�������ܣ���ת
��ڲ�������
����  ֵ���� 
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
�������ܣ���ת
��ڲ�������
����  ֵ���� 
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
�������ܣ�����
��ڲ�������
����  ֵ���� 
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
�������ܣ���ƽ��
��ڲ�������
����  ֵ���� 
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
�������ܣ���ƽ��
��ڲ�������
����  ֵ���� 
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
�������ܣ�������ֱ�� �����ǽǶȱջ�
��ڲ�������
����  ֵ���� 
**************************************************************************/
void chassis_angle_control()
{
	if(car_status.running_mode==FORWARD)//ǰ��
	{
		chassis_forwrd();
		set_motor_pwm(CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT);
	}
	if(car_status.running_mode==BACK)//����
	{
		chassis_back();
		set_motor_pwm(CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT);
	}
	if(car_status.running_mode==RIGHT_TRANSLATION)//��ƽ��
	{
		chassis_right_translation();
		set_motor_pwm(CAR_SPEED-PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT);
	}
	if(car_status.running_mode==LEFT_TRANSLATION)//��ƽ��
	{
		chassis_left_translation();
		set_motor_pwm(CAR_SPEED+PWM_OUTPUT,CAR_SPEED+PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT,CAR_SPEED-PWM_OUTPUT);
	}
	if(car_status.running_mode==STOP)//ֹͣ
	{
		set_motor_pwm(CAR_STOP,CAR_STOP,CAR_STOP,CAR_STOP);
	}
}

void mix()
{
	
}

