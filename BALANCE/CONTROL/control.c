#include "control.h"	
#include "filter.h"	
#include "sys.h"
  /**************************************************************************
���ߣ��ܺ�
**************************************************************************/
/*																																		����ϵ
			*			*			*			*			*																								*
y	**********************************																			Y  ***
	**********************************					Y���Ǽ�ⵥ��										*	
			*			*			*			*			*								X���Ǽ��˫��										*
			*			*			*			*			*																								*					*
y	**********************************																					************	X
	**********************************		                                								*
			*			*			*			*			*
			*			*			*			*			*
			*			*			*			*			*
			x			x			x			x			x


*/
//#define TEST_MODE 
#define TASK_MODE	
Location QR_Location,Material_Location;
u8 count=0,locate_delay=0,x_axis=0,y_axis=0;//xΪx�����꣬yΪy����
u8 time_flag=0;//����⵽����ʱ����ʱ��ʼ
u8 X_target=0,Y_target=0;
u8 begin_flag=0;
float Straight_KP=0;
float Straight_KI=0;
float Yaw_target=0,yaw_original;
int PWM_OUTPUT=0;

int cnt=0;
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms��ʱ�ж�
	{   
		cnt++;
		  TIM1->SR&=~(1<<0);                                       //===�����ʱ��1�жϱ�־λ		 
		
		if(cnt%2==0)//ʮ�����һ��
			get_tick();//��ȡʱ��
			//Get_Angle(Way_Angle);                                    //===������̬	
			Read_DMP();
		if(cnt==200)
		{
			Yaw_target=Yaw;
			begin_flag=1;
			servos_init(0);//�����ʼ��
			//servos_ready_grab();
		}
		if(begin_flag==1)//3���Ӻ����
		{
			//servos_ready_grab();
			car_mode_select();//С��xѲ�ߣ�yѲ��ģʽѡ��
			PWM_OUTPUT=straight_control(Yaw,Yaw_target);//�Ƕȱջ���������ֱ��
			chassis_angle_control();//����ǰ�������ˣ���ƽ�ƣ���ƽ�Ƶĳ����нǶȱջ�
			infr_control();//Ѳ�߿��Ƴ�������x��y��ѭ��
			car_locate();//С�����߼������������ڶ�λ
			//car_locate_control(X_target,Y_target);	
			//car_locate_control(3,1);
			//task_planning();
			//vision_control();//�Ӿ�����		
			//grab_task();		
			
			//vision_status_detection();	
			
		}			 
	}       	
	  
} 
extern long tick;
void time_delay(int nms)//1��Ӧ��10ms
{
	long current_time;
	current_time=get_tick();
	while(tick-current_time<nms)
	{
		;
	}
}

extern uint16_t qr_unpack;//��ά���������
char qr_line=0;
char forward_flag=0;
char qr_location_arrive_flag=0;
char qr_right_translation_flag=0;//��ƽ�Ʊ�־λ
char qr_task_finish=0;//qr����ʶ���־λ
char qr_back_flag=0;
void go_to_scan_QR()
{
	if(forward_flag==0)
	{
		car_status.task_mode=LEFT_TRANSLATION;
	}
	if(qr_line>2&&forward_flag==0)
	{
		forward_flag=1;
		car_status.task_mode=RIGHT_TRANSLATION;
		delay_ms(100);
		car_status.task_mode=STOP;
		delay_ms(100);
	}
	if(forward_flag==1)
	{
			car_status.task_mode=FORWARD;
		forward_flag=2;
	}
//	if(x_axis>5&&qr_location_arrive_flag==0)
//	{
//		CAR_SPEED=1200;//����
//	}
		if(x_axis==6&&qr_location_arrive_flag==0)//�����ά������
		{
			
			car_status.task_mode=BACK;
			
			
		}
	
	
		if(qr_right_translation_flag==1)
			{
				
				car_status.task_mode=FORWARD;//ɲ��
				delay_ms(200);
				car_status.task_mode=STOP;
				delay_ms(200);
				qr_location_arrive_flag=1;	
				qr_right_translation_flag=2;
			}
		if(qr_location_arrive_flag==1)//�ڶ�ά�����������߶�
		{
			if(qr_back_flag==0)//��������ĳ���һֱ����
			{
				CAR_SPEED=700;
				Yaw_target=180;
				car_status.task_mode=RIGHT_TRANSLATION;//��ƽ��
			}
			if(qr_back_flag==1)
			{
				//CAR_SPEED=800;
				car_status.task_mode=LEFT_TRANSLATION;//��ƽ��
				delay_ms(100);
				qr_back_flag=2;
			}
			if(qr_back_flag==2)
			{
				
				Yaw_target=180;
				car_status.task_mode=BACK;
				if(x_axis<5)
					CAR_SPEED=1200;//�뿪��ά����������
				qr_task_finish=1;
				qr_location_arrive_flag=3;
			}
		}
		
}

char start_task=0;//��ʼ����
void go_to_scan_QR_1()
{
if(start_task==0)
{
	if(forward_flag==0)
	{
		car_status.task_mode=LEFT_TRANSLATION;
	}
	if(qr_line>2&&forward_flag==0)
	{
		forward_flag=1;
		car_status.task_mode=RIGHT_TRANSLATION;
		time_delay(20);
		car_status.task_mode=STOP;
		time_delay(20);
	}
	if(forward_flag==1)
	{
		car_status.task_mode=FORWARD;
		forward_flag=2;
	}
	if(x_axis==2)//����������������
	{
		start_openmv();//����openmv	
		
	}
	if(x_axis==3)
	{
		CAR_SPEED=700;
                                                                                         
	}
	if(x_axis==4&&qr_location_arrive_flag==0)//�����ά������
	{
		car_status.task_mode=STOP;//ͣ����ɨ��ά��		
	}
	if(scan_qr_success_flag==1)//��ά��ɨ��ɹ�
	{
		qr_location_arrive_flag=1;	
		scan_qr_success_flag=0;
	}
	if(qr_location_arrive_flag==1)//ɨ���ά��ɹ������
	{	
			Yaw_target=INIT_ANGLE;
			car_status.task_mode=RIGHT_TRANSLATION;
			CAR_SPEED=LOW_SPEED;//�뿪��ά����������
			qr_task_finish=1;
			qr_location_arrive_flag=3;
			start_task=1;
	}		
}
}
extern uint16_t block_unpack;//ɫ��˳��  ��1��2��3

char home_arrive_flag=0;//���ұ�־λ
char circle_count=1;//ѭ������  ������鹲����ѭ��
char grab_flag=0;//ץȡ��־λ
char place_flag=0;//���ñ�־λ
char material_arrive_flag=0;
char place_order=0;//����˳��
char near_arear1=0,near_arear2=0,near_arear3=0;//�ӽ�����������������
void grab_task()//ץȡ����
{
if(circle_count==1)//��һ��ѭ��
{
	if(qr_task_finish==1)
	{
//			if(material_arrive_flag==1)//���꣨4��1��
//			{
//				
//				car_status.task_mode=FORWARD;//ͣ��
//				time_delay(40);
//				car_status.task_mode=STOP;	
//				time_delay(40);
//				material_arrive_flag=2;
//			}
//		if(material_arrive_flag==2)//����������������
//		{		
//				material_arrive_flag=0;
//				CAR_SPEED=LOW_SPEED;//����
//				Yaw_target=INIT_ANGLE;//���Ʋ�ѭ��  ����������ֱ��
//				car_status.task_mode=RIGHT_TRANSLATION;
//		}
		if(grab_flag==1&&material_arrive_flag==0)//ץȡ��־λ
		{	
			car_status.task_mode=STOP;
			grab(qr_first);//�ö�������ץȡ׼����ץȡ��ץȡ��ɶ���
			grab_flag=0;
			material_arrive_flag=3;
			
		}
		if(material_arrive_flag==3)//���ץȡ���
		{
//			CAR_SPEED=MID_SPEED;//����
//			time_delay(150);
//			car_status.task_mode=LEFT_TRANSLATION;
			material_arrive_flag=4;	
		}
		if(material_arrive_flag==4)
		{
			switch (qr_first)
			{
				case 1: place_order=1;break;
				case 2: place_order=2;break;
				case 3:	place_order=3;break;	
			}
			material_arrive_flag=5;
		}
			put_material_task(place_order);	//��ɴ˺���ʱ���ڣ�4��0��
	}
}
		if(circle_count==2)//�ڶ���ץȡѭ��
		{
			if(material_arrive_flag==0)//ץȡ��־λ
			{	
				car_status.task_mode=STOP;
				grab(qr_second);//�ö�������ץȡ׼����ץȡ��ץȡ��ɶ���
				//grab_flag=0;
				material_arrive_flag=3;	
			}
		if(material_arrive_flag==3)//���ץȡ���
		{
			material_arrive_flag=4;	
		}
		if(material_arrive_flag==4)
		{
			switch (qr_second)
			{
				case 1: place_order=1;break;
				case 2: place_order=2;break;
				case 3:	place_order=3;break;	
			}
			material_arrive_flag=5;
		}
			
			put_material_task(place_order);	//��ɴ˺���ʱ���ڣ�4��0��
			
			
			
			
//			if(material_arrive_flag==0)//���������
//			{
//				Yaw_target=TARGET_ANGLE;//���Ʋ�ѭ��  ����������ֱ��
//				car_status.task_mode=RIGHT_TRANSLATION;	
//				CAR_SPEED=LOW_SPEED;//����
//				
//			}
//			if(grab_flag==1)//ץȡ��־λ
//			{
//				car_status.task_mode=STOP;
//				servos_ready_grab();//���������ƶ�ʱ��ץȡ������ʼ׼��
//				time_delay(50);
//				grab_unpack(qr_second);//���ݶ�ά���������꣬��ץȡ��2��ɫ��
//				grab_firmly();//̧��
//				time_delay(50);
//				
//				grab_flag=0;
//				material_arrive_flag=1;
//			}
//			if(material_arrive_flag==1)//���ץȡ���
//			{
//				CAR_SPEED=MID_SPEED;//����
//				car_status.task_mode=LEFT_TRANSLATION;	
//				servos_init(1);//��������̫��
//			}
//			if(place_flag==1)//�����ñ�־λ
//			{
//				car_status.task_mode=STOP;
//				//delay_ms(600);
//				put_material(qr_second);
//				//delay_ms(1000);
//				place_flag=0;
//				circle_count++;//ѭ������	
//				material_arrive_flag=0;//���ں����־λ
//			}
		}
		if(circle_count==3)//��3��ץȡѭ��
		{
			if(material_arrive_flag==0)//ץȡ��־λ
		{	
			//OLED_Init();//���⻨��
			car_status.task_mode=STOP;
			grab(qr_third);//�ö�������ץȡ׼����ץȡ��ץȡ��ɶ���
			//grab_flag=0;
			material_arrive_flag=3;	
		}
		if(material_arrive_flag==3)//���ץȡ���
		{
			material_arrive_flag=4;	
		}
		if(material_arrive_flag==4)
		{
			switch (qr_third)
			{
				case 1: place_order=1;break;
				case 2: place_order=2;break;
				case 3:	place_order=3;break;	
			}
			material_arrive_flag=5;
		}
			
			put_material_task(place_order);	//��ɴ˺���ʱ���ڣ�4��0��
			
			
			
//			
//			if(material_arrive_flag==0)//���������
//			{
//				Yaw_target=TARGET_ANGLE;//���Ʋ�ѭ��  ����������ֱ��
//				car_status.task_mode=RIGHT_TRANSLATION;	
//				CAR_SPEED=LOW_SPEED;//����
//				
//			}
//			if(grab_flag==1)//ץȡ��־λ
//			{
//				car_status.task_mode=STOP;
//				servos_ready_grab();//���������ƶ�ʱ��ץȡ������ʼ׼��
//				time_delay(60);
//				grab_unpack(qr_third);//���ݶ�ά���������꣬��ץȡ��3��ɫ��
//				grab_firmly();//̧��
//				time_delay(60);
//				grab_flag=0;
//				material_arrive_flag=1;
//			}
//			if(material_arrive_flag==1)//���ץȡ���
//			{
//				CAR_SPEED=MID_SPEED;//����
//				car_status.task_mode=LEFT_TRANSLATION;	
//				servos_init(1);//��������̫��
//			}
//			if(place_flag==1)//�����ñ�־λ
//			{
//				car_status.task_mode=STOP;
//				time_delay(60);
//				put_material(qr_third);
//				time_delay(60);
//				place_flag=0;
//				circle_count++;//ѭ������	
//				material_arrive_flag=0;//���ں����־λ
//			}
		}
		if(circle_count==4)//��4��ѭ��,�ؼ�
		{
			if(material_arrive_flag==0)//���������
			{
				Yaw_target=TARGET_ANGLE;//���Ʋ�ѭ��  ����������ֱ��
				car_status.task_mode=RIGHT_TRANSLATION;	
				CAR_SPEED=LOW_SPEED;//����
				material_arrive_flag=1;
			}
			if(grab_flag==1)//������������
			{
//				car_status.task_mode=STOP;
//				delay_ms(600);
				Yaw_target=186;//���˲�ѭ��  ����������ֱ��
				car_status.task_mode=BACK;
				CAR_SPEED=1100;
				grab_flag=0;
			}
			if(x_axis==1&&car_status.task_mode==BACK)
			{
				CAR_SPEED=700;
				
			}
			if(home_arrive_flag==1)//��������HOME
			{
				car_status.task_mode=STOP;
				time_delay(50);
				Yaw_target=TARGET_ANGLE;
				car_status.task_mode=RIGHT_TRANSLATION;//��ƽ��
				CAR_SPEED=HIGH_SPEED;
				time_delay(300);
				home_arrive_flag=2;
			}
			if(home_arrive_flag==2)//����HOME
			{
				car_status.task_mode=STOP;
			}
		}
		
	}

void put_material_task(char order)//��ʱС��λ���ڣ�4��0�����괦,�˺�������ʱҲ�ڣ�4��0�����괦
{
		if(order==1)//����������1����
		{
			if(material_arrive_flag==5)//�������һֱִ��
			{
				car_status.task_mode=FORWARD;
				material_arrive_flag=6;
			}
			if(near_arear1==1&&material_arrive_flag==6)//�����ڣ�5��0����
			{
				car_status.task_mode=BACK;
				time_delay(30);
				car_status.task_mode=STOP;
				time_delay(30);
				car_status.task_mode=LEFT_TRANSLATION;
				
				near_arear1=0;//����
				material_arrive_flag=7;
			}
			if(material_arrive_flag==7)
			{
				if(x_axis==5&&y_axis==2)
				{
					//time_delay(50);//��ʱһ������ó������˶�һ����ͣ����
					car_status.task_mode=STOP;
					servos_put_material();//�����
					//OLED_Init();
					//time_delay(50);//��ʱһ����ٶ�
					Yaw_target=TARGET_ANGLE;
					car_status.task_mode=RIGHT_TRANSLATION;
					material_arrive_flag=8;
					//OLED_Init();
				}
			}
			if(near_arear1==2&&material_arrive_flag==8)
			{
				car_status.task_mode=LEFT_TRANSLATION;
				time_delay(10);
//				car_status.task_mode=STOP;
//				time_delay(30);
				Yaw_target=TARGET_ANGLE;//Ŀ��Ƕ�Ϊ180��
				car_status.task_mode=BACK;
				near_arear1=0;
				material_arrive_flag=9;
			}
			if(near_arear1=3&&material_arrive_flag==9)
			{
				car_status.task_mode=FORWARD;
				time_delay(30);
				car_status.task_mode=STOP;
				near_arear1=0;
				order=0;//����
				material_arrive_flag=0;
				circle_count++;//��һ��ѭ��
			}
		}
		if(order==2)//����������2��
		{
			if(material_arrive_flag==5)//�������һֱִ��
			{
				car_status.task_mode=LEFT_TRANSLATION;
				material_arrive_flag=6;
			}
			if(material_arrive_flag==6)
			{
				if(x_axis==4&&y_axis==2)
				{
					//time_delay(50);//��ʱһ������ó������˶�һ����ͣ����
					car_status.task_mode=STOP;
					servos_put_material();//�����
					time_delay(50);//��ʱһ����ٶ�
					Yaw_target=TARGET_ANGLE;
					car_status.task_mode=RIGHT_TRANSLATION;
					material_arrive_flag=7;
				}
			}
			if(grab_flag==1&&material_arrive_flag==7)//������������
			{
				material_arrive_flag=0;
				car_status.task_mode=STOP;
				grab_flag=0;
				order=0;//����
				circle_count++;//��һ��ѭ��
			}
			
		}
		if(order==3)//����������3��
		{
			if(material_arrive_flag==5)//�������һֱִ��
			{
				Yaw_target=TARGET_ANGLE;//Ŀ��Ƕ�Ϊ180��
				car_status.task_mode=BACK;
				material_arrive_flag=6;
			}
			if(near_arear3==1&&material_arrive_flag==6)
			{
				car_status.task_mode=FORWARD;
				time_delay(30);
				car_status.task_mode=LEFT_TRANSLATION;
				material_arrive_flag=7;
			}
			if(material_arrive_flag==7)
			{
				if(x_axis==3&&y_axis==2)
				{
					//time_delay(50);//��ʱһ������ó������˶�һ����ͣ����
					car_status.task_mode=STOP;
					servos_put_material();//�����
					time_delay(50);//��ʱһ����ٶ�
					Yaw_target=TARGET_ANGLE;
					car_status.task_mode=RIGHT_TRANSLATION;
					material_arrive_flag=8;
				}
			}
			if(near_arear3==2&&material_arrive_flag==8)//�����꣨3��0��
			{
				car_status.task_mode=LEFT_TRANSLATION;
				time_delay(30);
				car_status.task_mode=STOP;
				time_delay(30);
				Yaw_target=TARGET_ANGLE;//Ŀ��Ƕ�Ϊ180��
				car_status.task_mode=FORWARD;
				near_arear3=0;
				material_arrive_flag=9;
			}
			if(near_arear3==3&&material_arrive_flag==9)//�����꣨4��0��
			{
				car_status.task_mode=BACK;
				time_delay(30);
				car_status.task_mode=STOP;
				near_arear3=0;
				order=0;//����
				material_arrive_flag=0;
				circle_count++;//��һ��ѭ��
			}
			
		}
}

/**************************************************************************
�������ܣ�С�����ڶ�λ������
��ڲ�����
����  ֵ����
**************************************************************************/
u8 location_arrive_finish_flag=0;//����λ�õ����־λ
u8 QR_code_location=0;
u8 material_location=0;
void car_locate_control(u8 x_target,u8 y_target)
{
	if((x_target>x_axis)&&(x_location_arrive_flag==0))//��Ŀ��x�����С�������ڵ�x������ʱ����С��ǰ��
	{
		car_status.task_mode=FORWARD;
	}
	if((x_location_arrive_flag==1)&&(car_status.task_mode==FORWARD))//�������Ԥ��λ��
		{
			if((y_target>y_axis)&&(y_location_arrive_flag==0))//��Ŀ��y�����С�������ڵ�y������ʱ����С����ƽ��
				{
					car_status.task_mode=LEFT_TRANSLATION;
				}
			
		}
		if((y_axis==y_target))//��С����������y��Ŀ��λ��ʱ����Ҫʹ��С��ǰ��ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��y��λ�õľ�ȷ����
				{
					x_location_arrive_flag=0;
				}	
		if((y_location_arrive_flag==1)&&(car_status.task_mode==LEFT_TRANSLATION))//����ǰ��ĺ��⴫��������Ŀ��
		{
			car_status.task_mode=STOP;
			location_arrive_finish_flag=1;
			y_location_arrive_flag=0;
		}
//		if((location_arrive_finish_flag==1)&&(car_status.task_mode==STOP))
//		{
//			if((x_target==5)&&(y_target==0))//��ά������
//				QR_code_location=1;
//		}
		
		
		if((x_target<x_axis)&&(x_location_arrive_flag==0))//��Ŀ��x��С��С�������ڵ�x������ʱ����С������
		{
			car_status.task_mode=BACK;
		}
		if((x_location_arrive_flag==1)&&(car_status.task_mode==BACK))//�������Ԥ��λ��
		{
			if((y_target<y_axis)&&(y_location_arrive_flag==0))//��Ŀ��y��С��С�������ڵ�y������ʱ����С����ƽ��
				{
					car_status.task_mode=RIGHT_TRANSLATION;
				}
			
		}
		if((y_axis==y_target))//��С����������y��Ŀ��λ��ʱ����Ҫʹ��С��ǰ��ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��y��λ�õľ�ȷ����
				{
					x_location_arrive_flag=0;
				}	
		if((y_location_arrive_flag==1)&&(car_status.task_mode==RIGHT_TRANSLATION))//����ǰ��ĺ��⴫��������Ŀ��
		{
			car_status.task_mode=STOP;
			location_arrive_finish_flag=1;
			y_location_arrive_flag=0;
		}
		
		
		
		
		
		if((location_arrive_finish_flag==1)&&(car_status.task_mode==STOP))//Ŀ��λ�õ���ж��Լ���������
		{
			if((x_axis==QR_Location.target_x)&&(y_axis==QR_Location.target_y))//��ά������
			{
				QR_code_location=1;
			}
			if((x_axis==Material_Location.target_x)&&(y_axis==Material_Location.target_y))//������ȡ��
			{
				material_location=1;
			}
		}
	
		
}

/**************************************************************************
�������ܣ�����滮����
��ڲ�����
����  ֵ����
**************************************************************************/

void task_planning()
{
	if(QR_code_location==0)
	{
		X_target=QR_Location.target_x;//Ŀ�������
		Y_target=QR_Location.target_y;
		car_locate_control(QR_Location.target_x,QR_Location.target_y);//��ά������λ��
	}
	if(QR_code_location==1)//��ά������λ���ѵ���
	{
		X_target=Material_Location.target_x;//Ŀ�������
		Y_target=Material_Location.target_y;
		car_locate_control(Material_Location.target_x,Material_Location.target_y);//������ȡ��
	}
	if(material_location==1)//������ȡ��λ���ѵ���
	{
		car_status.task_mode=STOP;
	}
}

/**************************************************************************
�������ܣ�С�����ڶ�λ������
��ڲ�����
����  ֵ����
**************************************************************************/
void car_locate()
{
	if(black_flag==1)  //����⵽����ʱ�����������������  black_flag
				 {	
					 
					 time_flag=1;
					 if(locate_delay==23) //��ʱ0.15��  С����1���ڱ���ͨ��˫�߻���   ///����������
					 {
						 locate_delay=0;
						 black_flag=0;
					//y������������ж�	 begin
						 if(black_line==1&&(car_status.running_mode==FORWARD))
						 {
							 x_axis++;//x������
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(0,25,x_axis,1,12);
						 }
						 if(black_line==1&&(car_status.running_mode==BACK))
						 {
							 x_axis--;//x������
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(0,25,x_axis,1,12);
						 }
					//y������������ж�	 end
						 
					//x������������ж� begin	 
						 if(black_line==2&&(car_status.running_mode==LEFT_TRANSLATION))
						 {
							 y_axis++;//y������
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(30,25,y_axis,1,12); 
						 }
						 if(black_line==2&&(car_status.running_mode==RIGHT_TRANSLATION))
						 {
							 y_axis--;//y������
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(30,25,y_axis,1,12); 
						 }
					//x������������ж�  end	 
						 else
						 {
							 black_line=0;
							 time_flag=0;
						 }
						 
					 }
					 //OLED_ShowNumber(60,25,black_flag,1,12);	 	
			
		

				 }
				 if(time_flag==1) //���߼���־λ��������ʼ��ʱ����0.75���ڼ���߹��ĺ�������
				 {
					 locate_delay++;
					 
				 }
}

/**************************************************************************
�������ܣ�С��ģʽѡ��
��ڲ�����
����  ֵ����
**************************************************************************/
void car_mode_select()
{
	#ifdef TEST_MODE
	/***************����ѡ������ģʽ�����ڲ���*****************/
	if(wheel_flag==1)//��ⰴ��
		{
			car_status.line_patrol_mode=Y_LINE_PATROL;  //Y���ӦС����ǰ��
			car_status.running_mode=FORWARD;
		}
	if(wheel_flag==2)//��ⰴ��
		{
			car_status.line_patrol_mode=X_LINE_PATROL;	//X���ӦС��������ƽ��
			car_status.running_mode=LEFT_TRANSLATION;
		}
	if(wheel_flag==0)//��ⰴ��
		{
			car_status.line_patrol_mode=STOP_LINE_PATROL;//ֹͣ״̬
			car_status.running_mode=STOP;
		}
	#endif
		
	#ifdef TASK_MODE
		/***************С���Զ���������滮*****************/
	if(car_status.task_mode==FORWARD)//ǰ��
	{
		car_status.line_patrol_mode=Y_LINE_PATROL;  //Y���ӦС����ǰ��
		car_status.running_mode=FORWARD;
	}
	if(car_status.task_mode==BACK)//����
	{
		car_status.line_patrol_mode=Y_LINE_PATROL;  //Y���ӦС����ǰ��
		car_status.running_mode=BACK;
	}
	if(car_status.task_mode==RIGHT_TRANSLATION)//��ƽ��
	{
		car_status.line_patrol_mode=X_LINE_PATROL;  //X���ӦС��������ƽ��
		car_status.running_mode=RIGHT_TRANSLATION;
	}
	if(car_status.task_mode==LEFT_TRANSLATION)//��ƽ��
	{
		car_status.line_patrol_mode=X_LINE_PATROL;  //X���ӦС��������ƽ��
		car_status.running_mode=LEFT_TRANSLATION;
	}
	if(car_status.task_mode==STOP)//ֹͣ
	{
		car_status.line_patrol_mode=STOP_LINE_PATROL;  //ֹͣѲ��
		car_status.running_mode=STOP;
	}
	#endif
}

/**************************************************************************
�������ܣ���ȡ�Ƕ�
��ڲ�������ȡ�Ƕȵ��㷨 1����  2�������� 3�������˲�
����  ֵ����
**************************************************************************/
void Get_Angle(u8 way)
{ 
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y;
	    if(way==1)                                      //DMPû���漰���ϸ��ʱ�����⣬����������ȡ
			{	
			}			
      else
      {
			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY��������
			Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L);    //��ȡZ��������
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
	 		if(Gyro_Y>32768)  Gyro_Y-=65536;     //��������ת��  Ҳ��ͨ��shortǿ������ת��
			if(Accel_Z>32768)  Accel_Z-=65536;     //��������ת��
	  	if(Accel_X>32768) Accel_X-=65536;    //��������ת��
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //���������ļн�	
		  Gyro_Y=Gyro_Y/16.4;                                    //����������ת��	
      if(Way_Angle==2)		  	Kalman_Filter(Accel_Y,-Gyro_Y);//�������˲�	
			else if(Way_Angle==3)   Yijielvbo(Accel_Y,-Gyro_Y);    //�����˲�
	    Angle_Balance=angle;                                   //����ƽ�����
	  	}
}

void param_init(void)
{
	
  Straight_KP=50;
  Straight_KI=0;
	Yaw=0;
	Yaw_target=0;
	car_status.line_patrol_mode=STOP_LINE_PATROL;//ֹͣ״̬
	car_status.line_patrol_mode=STOP;
	
	QR_Location.target_x=5;
	QR_Location.target_y=1;
	Material_Location.target_x=3;
	Material_Location.target_y=0;
	
	X_target=0;//Ŀ�������
	Y_target=0;
}

/**************************************************************************
�������ܣ�ֱ�߿���
��ڲ�����yaw�����ֵ��yaw��Ŀ��ֵ��yaw_target��		
����  ֵ����
**************************************************************************/
int straight_control(float yaw,float yaw_target)   //���������ƫʱ��BiasС��0
{
	 static int Bias,Pwm_output,Last_bias;
	 Bias=(int)(yaw_target-yaw);                //����ƫ��
	
	if(Bias>35)
		Bias=35;
	if(Bias<-35)
		Bias=-35;
	 Pwm_output+=Straight_KP*(Bias-Last_bias)+Straight_KI*Bias;   //����ʽPI������
//	if(Bias>0)													//��С������תʱ������С�����ֿ���
//		Pwm_output=0;
	 Last_bias=Bias;		//������һ��ƫ��
	 
	
	
	 return Pwm_output;
}
