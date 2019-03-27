#include "servos.h"

#define OPEN_CLAW 90
#define CLOSE_CLAW 150
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
void servos_put_mid_material()
{
	time_delay(20);
	pca_setpwm1(0,0,degree2duty_270(95));
	pca_setpwm1(4,0,degree2duty_270(5));//2�Ż�е��
	grab_slowly(8,2,1,20,150);//��3�Ż�е��
	time_delay(80);
	pca_setpwm1(12,0,degree2duty_270(OPEN_CLAW));//�ſ�צ��
	time_delay(80);
	car_status.task_mode=RIGHT_TRANSLATION;
	time_delay(40);
	car_status.task_mode=STOP;
	grab_slowly(8,2,1,130,20);//̧��3�Ż�е��
	//pca_setpwm1(8,0,degree2duty_270(20));
	//grab_slowly(4,2,1,5,60);
	//servos_init(0);
}

void servos_put_left_material()
{
	//pca_setpwm1(0,0,degree2duty_270(100));
	time_delay(20);
	pca_setpwm1(0,0,degree2duty_270(160));
	pca_setpwm1(4,0,degree2duty_270(5));//2�Ż�е��
	grab_slowly(8,2,1,20,150);//��3�Ż�е��
	time_delay(80);
	pca_setpwm1(12,0,degree2duty_270(100));//�ſ�צ��
	time_delay(80);
	car_status.task_mode=RIGHT_TRANSLATION;
	time_delay(40);
	car_status.task_mode=STOP;
	grab_slowly(8,2,1,130,20);//̧��3�Ż�е��
	//grab_slowly(4,2,1,5,60);
	//servos_init(0);
}

void servos_put_right_material()
{
	
	pca_setpwm1(0,0,degree2duty_270(50));
	pca_setpwm1(4,0,degree2duty_270(5));//2�Ż�е��
	grab_slowly(8,2,1,20,150);//��3�Ż�е��
	time_delay(80);
	pca_setpwm1(12,0,degree2duty_270(100));//�ſ�צ��
	time_delay(80);
	car_status.task_mode=RIGHT_TRANSLATION;
	time_delay(40);
	car_status.task_mode=STOP;
	grab_slowly(8,2,1,130,20);//̧��3�Ż�е��
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
void servos_init(char mode)//�����ʼ��λ��
{
		
		if(mode==0)//����״̬��ʼ��
		{
			pca_setpwm1(0,0,degree2duty_270(20));   
			pca_setpwm1(4,0,degree2duty_270(10));
	  	pca_setpwm1(8,0,degree2duty_270(25));  //140 - 0  300 - 90	
			pca_setpwm1(12,0,degree2duty_270(80));
		}
		if(mode==1)//�м����ץȡ��ʼ��
		{
			//grab_slowly(0,2,1,100,30);
			pca_setpwm1(0,0,degree2duty_270(60));  
			grab_slowly(0,2,1,60,20);
			grab_slowly(4,2,1,85,10);
			grab_slowly(8,2,1,65,25);
		}
		if(mode==2)//������ץȡ��ʼ��
		{
			pca_setpwm1(0,0,degree2duty_270(80));
			
			grab_slowly(0,2,1,80,20);
			grab_slowly(4,2,1,105,10);
			grab_slowly(8,2,1,80,25);
		}
		if(mode==3)//�ұ����ץȡ��ʼ��
		{
			pca_setpwm1(0,0,degree2duty_270(20));
			//grab_slowly(0,2,1,67,30);
			grab_slowly(4,2,1,100,10);
			grab_slowly(8,2,1,75,25);
		}
		if(mode==4)//�ؼҶ����ʼ��
		{
			
		}
}
extern uint16_t block_unpack;//ɫ��˳��  ��1��2��3
void servos_ready_grab(int qr)//�����ʼ��λ��
{	
	if(qr==block_unpack/100)//׼��ץ���
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
	if(qr==block_unpack/10%10)//׼��ץ�м�
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
	if(qr==block_unpack/10%10)//׼��ץ�ұ�
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

void grab_firmly()//ץ��̧�� ������
{
	time_delay(150);
	pca_setpwm1(12,0,degree2duty_270(CLOSE_CLAW));//ץ��
	time_delay(150);
//	pca_setpwm1(12,0,degree2duty_270(CLOSE_CLAW));//ץ��
//	time_delay(50);
	grab_slowly(4,2,1,60,30);
	time_delay(50);
	pca_setpwm1(8,0,degree2duty_270(30));
	//grab_slowly(8,2,1,80,110);
	//pca_setpwm1(8,0,degree2duty_270(105));//̧�� 200
	
	
	//pca_setpwm1(0,0,degree2duty_270(100));//yaw�����
//  grab_slowly(4,2,1,85,20);
//	time_delay(40);
//	grab_slowly(8,2,1,60,30);
		
}
/**************************************************************************
�������ܣ�ץȡ�������
��ڲ�����
����  ֵ��
**************************************************************************/
void grab(int qr)
{
	//servos_ready_grab();
	time_delay(20);
	grab_unpack(qr);//���ݶ�ά���������꣬��ץȡɫ��
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
extern long tick;//ϵͳʱ��

void grab_slowly(char count,char mode ,int time,int current_angle,int target_angle)//����ץȡ
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

