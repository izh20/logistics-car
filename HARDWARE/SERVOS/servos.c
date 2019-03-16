#include "servos.h"

SERVOS servos_yaw,servos_pitch1,servos_pitch2,servos_claw;

/**************************************************************************
�������ܣ�����ṹ���ʼ��
��ڲ�����
����  ֵ����
**************************************************************************/
void servos_param_init()
{
	
}
/**************************************************************************
�������ܣ�������ƺ���������openmv�������ݽ������ץȡ
��ڲ�����
����  ֵ����
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
int degree2duty_270(int degree)
{
	return (a*degree+b);
}
#define a_180 2.272222//1.083333f
#define b_180 102
int degree2duty_180(int degree)
{
	return (a_180*degree+b_180);
}
///*
//off=a*degree+b   (0,140),(90,300)
//a=77/512   b=103
//*/
//#define a 1.083333f
//#define b 55
void grab_right_material()
{
			pca_setpwm1(0,0,degree2duty_270(160));
			delay_ms(100);
			pca_setpwm1(1,0,degree2duty_180(80));
			delay_ms(100);
	  	pca_setpwm1(2,0,degree2duty_180(15));  //140 - 0  300 - 90		
			delay_ms(100);
			pca_setpwm1(3,0,degree2duty_180(80));
}
void grab_left_material()
{
			pca_setpwm1(0,0,degree2duty_270(190));   
			delay_ms(100);
			pca_setpwm1(1,0,degree2duty_180(80));
			delay_ms(100);
	  	pca_setpwm1(2,0,degree2duty_180(15));  //140 - 0  300 - 90
			delay_ms(100);
			pca_setpwm1(3,0,degree2duty_180(80));
}

void grab_mid_material()
{
			pca_setpwm1(0,0,degree2duty_270(175));  
			delay_ms(100);  
			pca_setpwm1(1,0,degree2duty_180(80));
			delay_ms(100);
	  	pca_setpwm1(2,0,degree2duty_180(15));  //140 - 0  300 - 90			
			delay_ms(100);
			pca_setpwm1(3,0,degree2duty_180(80));
}
void servos_init()//�����ʼ��λ��
{
	//grab_slowly(0,2,5,175,60);
	
		pca_setpwm1(0,0,degree2duty_270(175));    
		pca_setpwm1(1,0,degree2duty_180(150));
		pca_setpwm1(2,0,degree2duty_180(160));  //140 - 0  300 - 90			
		pca_setpwm1(3,0,degree2duty_180(80));
}

void servos_ready_grab()//�����ʼ��λ��
{	
		grab_slowly(0,2,20,90,175);
		pca_setpwm1(0,0,degree2duty_270(175));
		pca_setpwm1(1,0,degree2duty_180(80));
		pca_setpwm1(2,0,degree2duty_180(0));  //140 - 0  300 - 90			
		pca_setpwm1(3,0,degree2duty_180(80));
}

void grab_firmly()//ץ��̧��
{
	pca_setpwm1(3,0,degree2duty_180(104));//ץ��
	delay_ms(100);
	pca_setpwm1(2,0,degree2duty_180(0));//̧��
}

/**************************************************************************
�������ܣ��������ץȡ����
��ڲ�����count �ڼ��Ŷ��
					mode  ����ͺ� 1Ϊ180  2Ϊ270
					time  �ӳٲ��� һ��Ϊ5-10  ����תһ��Ҫ50-100ms
					current_angle �����ʱ�Ƕ�
					target_angle  ���Ŀ��Ƕ�
����  ֵ��
**************************************************************************/
int err,i_count,current_time,last_time,servos_finish_flag=0;
void grab_slowly(char count,char mode ,int time,int current_angle,int target_angle)//����ץȡ
{
	//int err,i,current_time,servos_finish_flag=0;
	err=(target_angle-current_angle);
	current_time=get_tick();
	if(((get_tick()-last_time)>time)&&(mode==1)&&(err!=i_count))
	{
		//OLED_ShowString(00,40,"SERVOS");
		pca_setpwm1(count,0,degree2duty_180(current_angle+i_count));
		if(err>0)
			i_count++;
		else
			i_count--;
		last_time=current_time;
	}
	else if(((get_tick()-last_time)>time)&&(mode==2)&&(err!=i_count))
	{
		OLED_ShowString(00,40,"SERVOS");
		pca_setpwm1(count,0,degree2duty_270(current_angle+i_count));
		//pca_setpwm1(0,0,degree2duty_270(90));  
		if(err>0)
			i_count++;
		else
			i_count--;
		last_time=current_time;
	}
	
	
	
}