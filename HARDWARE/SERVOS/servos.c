#include "servos.h"

#define OPEN_CLAW 90
#define CLOSE_CLAW 150
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
void servos_put_mid_material()
{
	time_delay(20);
	pca_setpwm1(0,0,degree2duty_270(95));
	pca_setpwm1(4,0,degree2duty_270(5));//2号机械臂
	grab_slowly(8,2,1,20,150);//放3号机械臂
	time_delay(80);
	pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));//张开爪子
	time_delay(80);
	car_status.task_mode=RIGHT_TRANSLATION;
	time_delay(40);
	car_status.task_mode=STOP;
	grab_slowly(8,2,1,130,20);//抬升3号机械臂
	//pca_setpwm1(8,0,degree2duty_270(20));
	//grab_slowly(4,2,1,5,60);
	//servos_init(0);
}

void servos_put_left_material()
{
	//pca_setpwm1(0,0,degree2duty_270(100));
	time_delay(20);
	pca_setpwm1(0,0,degree2duty_270(160));
	pca_setpwm1(4,0,degree2duty_270(5));//2号机械臂
	grab_slowly(8,2,1,20,150);//放3号机械臂
	time_delay(80);
	pca_setpwm1(12,0,degree2duty_270(100));//张开爪子
	time_delay(80);
	car_status.task_mode=RIGHT_TRANSLATION;
	time_delay(40);
	car_status.task_mode=STOP;
	grab_slowly(8,2,1,130,20);//抬升3号机械臂
	//grab_slowly(4,2,1,5,60);
	//servos_init(0);
}

void servos_put_right_material()
{
	
	pca_setpwm1(0,0,degree2duty_270(50));
	pca_setpwm1(4,0,degree2duty_270(5));//2号机械臂
	grab_slowly(8,2,1,20,150);//放3号机械臂
	time_delay(80);
	pca_setpwm1(12,0,degree2duty_270(100));//张开爪子
	time_delay(80);
	car_status.task_mode=RIGHT_TRANSLATION;
	time_delay(40);
	car_status.task_mode=STOP;
	grab_slowly(8,2,1,130,20);//抬升3号机械臂
	//grab_slowly(4,2,1,5,60);
	//servos_init(0);
}

void grab_right_material()
{
			pca_setpwm1(0,0,degree2duty_270(55));
			time_delay(20);
			pca_setpwm1(0,0,degree2duty_270(60));
			pca_setpwm1(8,0,degree2duty_270(60));  //140 - 0  300 - 90			
			time_delay(20);
			pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));
			time_delay(20);
			grab_slowly(4,2,1,35,85);
			//pca_setpwm1(4,0,degree2duty_270(70));
			//time_delay(60);
			
			
}
void grab_left_material()
{
		pca_setpwm1(0,0,degree2duty_270(125));
		time_delay(20);
		pca_setpwm1(0,0,degree2duty_270(125));
		pca_setpwm1(8,0,degree2duty_270(60));  //140 - 0  300 - 90		
		time_delay(20);
		pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));
		time_delay(20);
		grab_slowly(4,2,2,35,87);
		//pca_setpwm1(4,0,degree2duty_270(65));
		//time_delay(40);
		
		
		//time_delay(40);
}

void grab_mid_material()
{
		pca_setpwm1(0,0,degree2duty_270(95));
		time_delay(20);
		pca_setpwm1(0,0,degree2duty_270(95));
		pca_setpwm1(8,0,degree2duty_270(50));  //140 - 0  300 - 90
		time_delay(20);
		pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));
		time_delay(20);
		grab_slowly(4,2,1,35,73);
		//pca_setpwm1(4,0,degree2duty_270(65));
		//time_delay(60);
}
void servos_init(char mode)//舵机初始化位置
{
		
		if(mode==0)//开机状态初始化
		{
			pca_setpwm1(0,0,degree2duty_270(20));   
			pca_setpwm1(4,0,degree2duty_270(10));
	  	pca_setpwm1(8,0,degree2duty_270(25));  //140 - 0  300 - 90	
			pca_setpwm1(12,0,degree2duty_270(80));
		}
		if(mode==1)//中间物块抓取初始化
		{
			//grab_slowly(0,2,1,100,30);
			pca_setpwm1(0,0,degree2duty_270(60));  
			grab_slowly(0,2,1,60,20);
			grab_slowly(4,2,1,85,10);
			grab_slowly(8,2,1,65,25);
		}
		if(mode==2)//左边物块抓取初始化
		{
			pca_setpwm1(0,0,degree2duty_270(80));
			
			grab_slowly(0,2,1,80,20);
			grab_slowly(4,2,1,105,10);
			grab_slowly(8,2,1,80,25);
		}
		if(mode==3)//右边物块抓取初始化
		{
			pca_setpwm1(0,0,degree2duty_270(20));
			//grab_slowly(0,2,1,67,30);
			grab_slowly(4,2,1,100,10);
			grab_slowly(8,2,1,75,25);
		}
		if(mode==4)//回家舵机初始化
		{
			
		}
}
extern uint16_t block_unpack;//色块顺序  红1绿2蓝3
void servos_ready_grab(int qr)//舵机初始化位置
{	
	if(qr==block_unpack/100)//准备抓左边
	{
		pca_setpwm1(0,0,degree2duty_270(125));
		time_delay(40);
		pca_setpwm1(8,0,degree2duty_270(30));  //140 - 0  300 - 90		
		time_delay(40);
		//grab_slowly(4,2,2,35,96);
		pca_setpwm1(4,0,degree2duty_270(30));
		time_delay(40);
		pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));
		time_delay(40);
	}
	if(qr==block_unpack/10%10)//准备抓中间
	{
		pca_setpwm1(0,0,degree2duty_270(95));
		time_delay(40);
		pca_setpwm1(8,0,degree2duty_270(30));  //140 - 0  300 - 90		
		time_delay(40);
		//grab_slowly(4,2,2,35,96);
		pca_setpwm1(4,0,degree2duty_270(30));
		time_delay(40);
		pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));
		time_delay(40);
	}
	if(qr==block_unpack/10%10)//准备抓右边
	{
		pca_setpwm1(0,0,degree2duty_270(67));
		time_delay(40);
		pca_setpwm1(8,0,degree2duty_270(30));  //140 - 0  300 - 90		
		time_delay(40);
		//grab_slowly(4,2,2,35,96);
		pca_setpwm1(4,0,degree2duty_270(30));
		time_delay(40);
		pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));
		time_delay(40);
	}
	
	
//		pca_setpwm1(4,0,degree2duty_270(30));
//		time_delay(20);
//		pca_setpwm1(8,0,degree2duty_270(30));  //260
//		time_delay(20);
//		pca_setpwm1(0,0,degree2duty_270(100));
//		time_delay(20);
//		pca_setpwm1(12,0,degree2duty_270(110));
//	//grab_slowly(0,2,1,65,100);
//		time_delay(10);
}

void grab_firmly()//抓紧抬升 并回中
{
	time_delay(150);
	pca_setpwm1(12,0,degree2duty_270(CLOSE_CLAW));//抓紧
	time_delay(150);
//	pca_setpwm1(12,0,degree2duty_270(CLOSE_CLAW));//抓紧
//	time_delay(50);
	grab_slowly(4,2,1,60,30);
	time_delay(50);
	pca_setpwm1(8,0,degree2duty_270(30));
	//grab_slowly(8,2,1,80,110);
	//pca_setpwm1(8,0,degree2duty_270(105));//抬升 200
	
	
	//pca_setpwm1(0,0,degree2duty_270(100));//yaw轴回中
//  grab_slowly(4,2,1,85,20);
//	time_delay(40);
//	grab_slowly(8,2,1,60,30);
		
}
/**************************************************************************
函数功能：抓取物块任务
入口参数：
返回  值：
**************************************************************************/
void grab(int qr)
{
	//servos_ready_grab();
	time_delay(20);
	grab_unpack(qr);//根据二维码和物块坐标，来抓取色块
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
extern long tick;//系统时间

void grab_slowly(char count,char mode ,int time,int current_angle,int target_angle)//缓慢抓取
{
	int err,i_count=0,servos_finish_flag=0;
	long current_time,last_time;
	//int err,i,current_time,servos_finish_flag=0;
	err=(target_angle-current_angle);
	current_time=get_tick();
	while((err!=i_count))
	{
		if(((tick-current_time)>time)&&(mode==1))
		{
			pca_setpwm1(count,0,degree2duty_180(current_angle+i_count));
			if(err>0)
				i_count++;
			else
				i_count--;
			current_time=tick;
		}
		if(((tick-current_time)>time)&&(mode==2))
		{
			OLED_ShowNumber(00,40,(int)i_count,3,12);
			pca_setpwm1(count,0,degree2duty_270(current_angle+i_count));
			//pca_setpwm1(0,0,degree2duty_270(90));  
			if(err>0)
				i_count++;
			else
				i_count--;
			current_time=tick;
		}
	}
}

