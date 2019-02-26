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
		
		if(cnt%5==0)//ʮ�����һ��
		get_tick();//
		
			//Get_Angle(Way_Angle);                                    //===������̬	
			Read_DMP();
		if(cnt==200)
		{
			Yaw_target=Yaw;
			begin_flag=1;
		}
		
		if(begin_flag==1)//3���Ӻ����
		{
			car_mode_select();//С��xѲ�ߣ�yѲ��ģʽѡ��
			PWM_OUTPUT=straight_control(Yaw,Yaw_target);//�Ƕȱջ���������ֱ��
			chassis_angle_control();//����ǰ�������ˣ���ƽ�ƣ���ƽ�Ƶĳ����нǶȱջ�
			infr_control();//Ѳ�߿��Ƴ�������x��y��ѭ��
			car_locate();//С�����߼������������ڶ�λ
			//car_status.task_mode=FORWARD;
			//car_locate_control(X_target,Y_target);	
			//car_locate_control(3,1);
			//task_planning();
			//vision_control();//�Ӿ�����
			
			
			
			vision_status_detection();
			
			
		}			 
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
				CAR_SPEED=600;
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
				CAR_SPEED=1200;
				Yaw_target=180;
				car_status.task_mode=BACK;
				qr_task_finish=1;
				qr_location_arrive_flag=3;
			}
		}
		
}
char grab_flag=0;
char material_arrive_flag=0;
void grab_task()//ץȡ����
{
	if(qr_task_finish==1)
	{
		if(x_axis<=4&&grab_flag==0)
		{
			CAR_SPEED=700;//�����ӽ�Ŀ�꣬����
			if(material_arrive_flag==1)
			{
				car_status.task_mode=FORWARD;
				delay_ms(100);
				grab_flag=1;
			}
		}
		if(grab_flag==1)//���������ץȡ���� ��д
		{
//			Yaw_target=180;//���Ʋ�ѭ��  ����������ֱ��
//			car_status.task_mode=RIGHT_TRANSLATION;
				car_status.task_mode=STOP;	
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
					 if(locate_delay==100) //��ʱ0.5��  С����1���ڱ���ͨ��˫�߻���   ///����������
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
	
  Straight_KP=45;
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
	
	if(Bias>25)
		Bias=25;
	if(Bias<-25)
		Bias=-25;
	 Pwm_output+=Straight_KP*(Bias-Last_bias)+Straight_KI*Bias;   //����ʽPI������
//	if(Bias>0)													//��С������תʱ������С�����ֿ���
//		Pwm_output=0;
	 Last_bias=Bias;		//������һ��ƫ��
	 
	
	
	 return Pwm_output;
}
