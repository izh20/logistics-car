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
void servos_init()//舵机初始化位置
{
	//grab_slowly(0,2,5,175,60);
	
		pca_setpwm1(0,0,degree2duty_270(175));    
		pca_setpwm1(1,0,degree2duty_180(150));
		pca_setpwm1(2,0,degree2duty_180(160));  //140 - 0  300 - 90			
		pca_setpwm1(3,0,degree2duty_180(80));
}

void servos_ready_grab()//舵机初始化位置
{	
		grab_slowly(0,2,20,90,175);
		pca_setpwm1(0,0,degree2duty_270(175));
		pca_setpwm1(1,0,degree2duty_180(80));
		pca_setpwm1(2,0,degree2duty_180(0));  //140 - 0  300 - 90			
		pca_setpwm1(3,0,degree2duty_180(80));
}

void grab_firmly()//抓紧抬升
{
	pca_setpwm1(3,0,degree2duty_180(104));//抓紧
	delay_ms(100);
	pca_setpwm1(2,0,degree2duty_180(0));//抬升
}

/**************************************************************************
函数功能：舵机缓慢抓取函数
入口参数：count 第几号舵机
					mode  舵机型号 1为180  2为270
					time  延迟参数 一般为5-10  代表转一度要50-100ms
					current_angle 舵机此时角度
					target_angle  舵机目标角度
返回  值：
**************************************************************************/
int err,i_count,current_time,last_time,servos_finish_flag=0;
void grab_slowly(char count,char mode ,int time,int current_angle,int target_angle)//缓慢抓取
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