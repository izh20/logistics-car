#include "control.h"	
#include "filter.h"	
#include "sys.h"
  /**************************************************************************
作者：周恒
**************************************************************************/
/*																																		坐标系
			*			*			*			*			*																								*
y	**********************************																			Y  ***
	**********************************					Y轴是检测单线										*	
			*			*			*			*			*								X轴是检测双线										*
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
u8 count=0,locate_delay=0,x_axis=0,y_axis=0;//x为x轴坐标，y为y坐标
u8 time_flag=0;//当检测到黑线时，计时开始
u8 X_target=0,Y_target=0;
u8 begin_flag=0;
float Straight_KP=0;
float Straight_KI=0;
float Yaw_target=0,yaw_original;
int PWM_OUTPUT=0;

int cnt=0;
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		cnt++;
		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
		
		if(cnt%5==0)//十毫秒计一次
		get_tick();//
		
			//Get_Angle(Way_Angle);                                    //===更新姿态	
			Read_DMP();
		if(cnt==200)
		{
			Yaw_target=Yaw;
			begin_flag=1;
		}
		
		if(begin_flag==1)//3秒钟后进入
		{
			car_mode_select();//小车x巡线，y巡线模式选择
			PWM_OUTPUT=straight_control(Yaw,Yaw_target);//角度闭环，用于走直线
			chassis_angle_control();//用于前进，后退，左平移，右平移的程序，有角度闭环
			infr_control();//巡线控制程序，用于x，y轴循迹
			car_locate();//小车黑线检测程序，用于室内定位
			//car_status.task_mode=FORWARD;
			//car_locate_control(X_target,Y_target);	
			//car_locate_control(3,1);
			//task_planning();
			//vision_control();//视觉控制
			
			
			
			vision_status_detection();
			
			
		}			 
	}       	
	  
} 
extern uint16_t qr_unpack;//二维码解码数据
char qr_line=0;
char forward_flag=0;
char qr_location_arrive_flag=0;
char qr_right_translation_flag=0;//右平移标志位
char qr_task_finish=0;//qr任务识别标志位
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
//		CAR_SPEED=1200;//降速
//	}
		if(x_axis==6&&qr_location_arrive_flag==0)//到达二维码区域
		{
			
			car_status.task_mode=BACK;	
			
		}
	
	
		if(qr_right_translation_flag==1)
			{
				
				car_status.task_mode=FORWARD;//刹车
				delay_ms(200);
				car_status.task_mode=STOP;
				delay_ms(200);
				qr_location_arrive_flag=1;	
				qr_right_translation_flag=2;
			}
		if(qr_location_arrive_flag==1)//在二维码区域来回走动
		{
			if(qr_back_flag==0)//避免里面的程序一直运行
			{
				CAR_SPEED=600;
				Yaw_target=180;
				car_status.task_mode=RIGHT_TRANSLATION;//右平移
			}
			if(qr_back_flag==1)
			{
				//CAR_SPEED=800;
				car_status.task_mode=LEFT_TRANSLATION;//左平移
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
void grab_task()//抓取任务
{
	if(qr_task_finish==1)
	{
		if(x_axis<=4&&grab_flag==0)
		{
			CAR_SPEED=700;//即将接近目标，降速
			if(material_arrive_flag==1)
			{
				car_status.task_mode=FORWARD;
				delay_ms(100);
				grab_flag=1;
			}
		}
		if(grab_flag==1)//在这里进行抓取任务 待写
		{
//			Yaw_target=180;//右移不循线  用陀螺仪走直线
//			car_status.task_mode=RIGHT_TRANSLATION;
				car_status.task_mode=STOP;	
		}
	}
}



/**************************************************************************
函数功能：小车室内定位处理函数
入口参数：
返回  值：无
**************************************************************************/
u8 location_arrive_finish_flag=0;//最终位置到达标志位
u8 QR_code_location=0;
u8 material_location=0;
void car_locate_control(u8 x_target,u8 y_target)
{
	if((x_target>x_axis)&&(x_location_arrive_flag==0))//当目标x轴大于小车所处于的x轴坐标时，让小车前进
	{
		car_status.task_mode=FORWARD;
	}
	if((x_location_arrive_flag==1)&&(car_status.task_mode==FORWARD))//如果到达预定位置
		{
			if((y_target>y_axis)&&(y_location_arrive_flag==0))//当目标y轴大于小车所处于的y轴坐标时，让小车左平移
				{
					car_status.task_mode=LEFT_TRANSLATION;
				}
			
		}
		if((y_axis==y_target))//当小车即将到达y轴目标位置时，需要使能小车前后平移的红外传感器的外部中断，进行小车y轴位置的精确控制
				{
					x_location_arrive_flag=0;
				}	
		if((y_location_arrive_flag==1)&&(car_status.task_mode==LEFT_TRANSLATION))//控制前后的红外传感器到达目标
		{
			car_status.task_mode=STOP;
			location_arrive_finish_flag=1;
			y_location_arrive_flag=0;
		}
//		if((location_arrive_finish_flag==1)&&(car_status.task_mode==STOP))
//		{
//			if((x_target==5)&&(y_target==0))//二维码区域
//				QR_code_location=1;
//		}
		
		
		if((x_target<x_axis)&&(x_location_arrive_flag==0))//当目标x轴小于小车所处于的x轴坐标时，让小车后退
		{
			car_status.task_mode=BACK;
		}
		if((x_location_arrive_flag==1)&&(car_status.task_mode==BACK))//如果到达预定位置
		{
			if((y_target<y_axis)&&(y_location_arrive_flag==0))//当目标y轴小于小车所处于的y轴坐标时，让小车右平移
				{
					car_status.task_mode=RIGHT_TRANSLATION;
				}
			
		}
		if((y_axis==y_target))//当小车即将到达y轴目标位置时，需要使能小车前后平移的红外传感器的外部中断，进行小车y轴位置的精确控制
				{
					x_location_arrive_flag=0;
				}	
		if((y_location_arrive_flag==1)&&(car_status.task_mode==RIGHT_TRANSLATION))//控制前后的红外传感器到达目标
		{
			car_status.task_mode=STOP;
			location_arrive_finish_flag=1;
			y_location_arrive_flag=0;
		}
		
		
		
		
		
		if((location_arrive_finish_flag==1)&&(car_status.task_mode==STOP))//目标位置到达，判断自己到达哪里
		{
			if((x_axis==QR_Location.target_x)&&(y_axis==QR_Location.target_y))//二维码区域
			{
				QR_code_location=1;
			}
			if((x_axis==Material_Location.target_x)&&(y_axis==Material_Location.target_y))//物料提取区
			{
				material_location=1;
			}
		}
	
		
}

/**************************************************************************
函数功能：任务规划函数
入口参数：
返回  值：无
**************************************************************************/

void task_planning()
{
	if(QR_code_location==0)
	{
		X_target=QR_Location.target_x;//目标坐标点
		Y_target=QR_Location.target_y;
		car_locate_control(QR_Location.target_x,QR_Location.target_y);//二维码坐标位置
	}
	if(QR_code_location==1)//二维码坐标位置已到达
	{
		X_target=Material_Location.target_x;//目标坐标点
		Y_target=Material_Location.target_y;
		car_locate_control(Material_Location.target_x,Material_Location.target_y);//物料提取区
	}
	if(material_location==1)//物料提取区位置已到达
	{
		car_status.task_mode=STOP;
	}
}

/**************************************************************************
函数功能：小车室内定位处理函数
入口参数：
返回  值：无
**************************************************************************/
void car_locate()
{
	if(black_flag==1)  //当检测到黑线时，启动黑线数量检测  black_flag
				 {	
					 
					 time_flag=1;
					 if(locate_delay==100) //延时0.5秒  小车在1秒内必须通过双线或单线   ///这里有问题
					 {
						 locate_delay=0;
						 black_flag=0;
					//y轴正负方向的判断	 begin
						 if(black_line==1&&(car_status.running_mode==FORWARD))
						 {
							 x_axis++;//x轴坐标
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(0,25,x_axis,1,12);
						 }
						 if(black_line==1&&(car_status.running_mode==BACK))
						 {
							 x_axis--;//x轴坐标
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(0,25,x_axis,1,12);
						 }
					//y轴正负方向的判断	 end
						 
					//x轴正负方向的判断 begin	 
						 if(black_line==2&&(car_status.running_mode==LEFT_TRANSLATION))
						 {
							 y_axis++;//y轴坐标
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(30,25,y_axis,1,12); 
						 }
						 if(black_line==2&&(car_status.running_mode==RIGHT_TRANSLATION))
						 {
							 y_axis--;//y轴坐标
							 black_line=0;
							 time_flag=0;
							 //OLED_ShowNumber(30,25,y_axis,1,12); 
						 }
					//x轴正负方向的判断  end	 
						 else
						 {
							 black_line=0;
							 time_flag=0;
						 }
						 
					 }
					 //OLED_ShowNumber(60,25,black_flag,1,12);	 	
			
		

				 }
				 if(time_flag==1) //黑线检测标志位触发，开始计时，在0.75秒内检测走过的黑线数量
				 {
					 locate_delay++;
					 
				 }
}

/**************************************************************************
函数功能：小车模式选择
入口参数：
返回  值：无
**************************************************************************/
void car_mode_select()
{
	#ifdef TEST_MODE
	/***************按键选择运行模式，用于测试*****************/
	if(wheel_flag==1)//检测按键
		{
			car_status.line_patrol_mode=Y_LINE_PATROL;  //Y轴对应小车的前后
			car_status.running_mode=FORWARD;
		}
	if(wheel_flag==2)//检测按键
		{
			car_status.line_patrol_mode=X_LINE_PATROL;	//X轴对应小车的左右平移
			car_status.running_mode=LEFT_TRANSLATION;
		}
	if(wheel_flag==0)//检测按键
		{
			car_status.line_patrol_mode=STOP_LINE_PATROL;//停止状态
			car_status.running_mode=STOP;
		}
	#endif
		
	#ifdef TASK_MODE
		/***************小车自动进行任务规划*****************/
	if(car_status.task_mode==FORWARD)//前进
	{
		car_status.line_patrol_mode=Y_LINE_PATROL;  //Y轴对应小车的前后
		car_status.running_mode=FORWARD;
	}
	if(car_status.task_mode==BACK)//后退
	{
		car_status.line_patrol_mode=Y_LINE_PATROL;  //Y轴对应小车的前后
		car_status.running_mode=BACK;
	}
	if(car_status.task_mode==RIGHT_TRANSLATION)//右平移
	{
		car_status.line_patrol_mode=X_LINE_PATROL;  //X轴对应小车的左右平移
		car_status.running_mode=RIGHT_TRANSLATION;
	}
	if(car_status.task_mode==LEFT_TRANSLATION)//左平移
	{
		car_status.line_patrol_mode=X_LINE_PATROL;  //X轴对应小车的左右平移
		car_status.running_mode=LEFT_TRANSLATION;
	}
	if(car_status.task_mode==STOP)//停止
	{
		car_status.line_patrol_mode=STOP_LINE_PATROL;  //停止巡线
		car_status.running_mode=STOP;
	}
	#endif
}

/**************************************************************************
函数功能：获取角度
入口参数：获取角度的算法 1：无  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/
void Get_Angle(u8 way)
{ 
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y;
	    if(way==1)                                      //DMP没有涉及到严格的时序问题，在主函数读取
			{	
			}			
      else
      {
			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
			Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L);    //读取Z轴陀螺仪
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度记
	 		if(Gyro_Y>32768)  Gyro_Y-=65536;     //数据类型转换  也可通过short强制类型转换
			if(Accel_Z>32768)  Accel_Z-=65536;     //数据类型转换
	  	if(Accel_X>32768) Accel_X-=65536;    //数据类型转换
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //计算与地面的夹角	
		  Gyro_Y=Gyro_Y/16.4;                                    //陀螺仪量程转换	
      if(Way_Angle==2)		  	Kalman_Filter(Accel_Y,-Gyro_Y);//卡尔曼滤波	
			else if(Way_Angle==3)   Yijielvbo(Accel_Y,-Gyro_Y);    //互补滤波
	    Angle_Balance=angle;                                   //更新平衡倾角
	  	}
}

void param_init(void)
{
	
  Straight_KP=45;
  Straight_KI=0;
	Yaw=0;
	Yaw_target=0;
	car_status.line_patrol_mode=STOP_LINE_PATROL;//停止状态
	car_status.line_patrol_mode=STOP;
	
	QR_Location.target_x=5;
	QR_Location.target_y=1;
	Material_Location.target_x=3;
	Material_Location.target_y=0;
	
	X_target=0;//目标坐标点
	Y_target=0;
}

/**************************************************************************
函数功能：直线控制
入口参数：yaw轴测量值，yaw轴目标值（yaw_target）		
返回  值：无
**************************************************************************/
int straight_control(float yaw,float yaw_target)   //如果车向左偏时，Bias小于0
{
	 static int Bias,Pwm_output,Last_bias;
	 Bias=(int)(yaw_target-yaw);                //计算偏差
	
	if(Bias>25)
		Bias=25;
	if(Bias<-25)
		Bias=-25;
	 Pwm_output+=Straight_KP*(Bias-Last_bias)+Straight_KI*Bias;   //增量式PI控制器
//	if(Bias>0)													//当小车向右转时，屏蔽小车右轮控制
//		Pwm_output=0;
	 Last_bias=Bias;		//保存上一次偏差
	 
	
	
	 return Pwm_output;
}
