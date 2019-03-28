#include "infr.h"
#include "sys.h"
//PB12 PB13 PB14 PB15
#define X_INFR_RIGHT PBin(12)   //控制小车左右循迹
#define X_INFR_LEFT  PBin(13)
#define Y_INFR_RIGHT PBin(14)   //控制小车前后循迹
#define Y_INFR_LEFT	 PBin(15)
#define INFR_DETECTION PAin(1)

#define SPEED 650 //修正速度

#define YAW_DELTA    0.02//0.034
#define X_YAW_DELTA	 0.04
//偏航角增量
char single_flag=0,double_flag=0;
char single_count=0,double_count=0;
char model=2;	//小车模式 1：Y轴循迹 2：X轴循迹 0；停止
/**************************************************************************
函数功能：红外传感器初始化
入口参数：无
返回  值：无
**************************************************************************/
void infr_Init()
{
	RCC->APB2ENR|=1<<3;               //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<2;   					 //使能PORTA时钟	  
	
	GPIOA->CRL&=0XFFFFFF0F; 
	GPIOA->CRL|=0X00000080;//PA1上拉输入
  GPIOA->ODR|=1<<1;     //PA1上拉	
	
	GPIOB->CRL&=0X0000FFFF;						//PB12,13,14,15 
	GPIOB->CRL|=0X88880000;
	GPIOB->ODR|=15<<12; //上拉
	Ex_NVIC_Config(GPIO_A,1,RTIR);		//上升沿触发
	//Ex_NVIC_Config(GPIO_B,12,3);//上升沿触发
	Ex_NVIC_Config(GPIO_B,13,3);//上升沿触发
	Ex_NVIC_Config(GPIO_B,14,3);//上升沿触发
	Ex_NVIC_Config(GPIO_B,15,3);//上升沿触发
	MY_NVIC_Init(0,0,EXTI1_IRQn,2);  	//抢占0，子优先级0，组2
	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);  	//抢占2，子优先级1，组2	
	
}

/**************************************************************************
函数功能：外部中断初始化 用于检测黑线
入口参数：无
返回  值：无 
**************************************************************************/
void EXTI_Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   
	GPIOA->CRH&=0XFFF0FFFF; 
	GPIOA->CRH|=0X00080000;//PA12上拉输入
  GPIOA->ODR|=1<<12;     //PA12上拉	
	
	GPIOA->CRL&=0XFFFFFF0F; 
	GPIOA->CRL|=0X00000080;//PA1上拉输入
  GPIOA->ODR|=1<<1;     //PA1上拉	
	
	Ex_NVIC_Config(GPIO_A,12,RTIR);		//上升沿触发
	Ex_NVIC_Config(GPIO_A,1,RTIR);		//上升沿触发
	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);  	//抢占2，子优先级1，组2
	MY_NVIC_Init(0,0,EXTI1_IRQn,2);  	//抢占0，子优先级0，组2
	
}
u8 block_position=0;//物块位置
u8 x_location_arrive_flag=0;//预定位置到达标志位
u8 y_location_arrive_flag=0;
void  EXTI15_10_IRQHandler() 
{    
		if((EXTI->PR & EXTI_Line12) != (uint32_t)RESET)//外部中断线12
		{
			if((x_axis==X_target)&&(car_status.task_mode==FORWARD))//当小车即将到达x轴目标位置时，需要使能控制小车左右平移的红外传感器的外部中断，进行小车x轴位置的精确控制
			{
				x_location_arrive_flag=1;
				//car_status.task_mode=LEFT_TRANSLATION;
			}
			
			delay_ms(10);   //消抖			 
			if(KEY1==0)		//按键
			{
				LED=~LED;
				block_position++;
				if(block_position==7)
					block_position=0;
	}
			
			
			EXTI->PR=1<<12;   
		}
		
		if((EXTI->PR & EXTI_Line13) != (uint32_t)RESET)//外部中断线13
		{
			if((x_axis==X_target)&&(car_status.task_mode==BACK))//当小车即将到达x轴目标位置时，需要使能控制小车左右平移的红外传感器的外部中断，进行小车x轴位置的精确控制
			{
				x_location_arrive_flag=1;
				//car_status.task_mode=RIGHT_TRANSLATION;
			}
			//delay_ms(18);//消抖
			if(x_axis==4&&car_status.task_mode==BACK)//识别二维码后，判断车是否走到物料前方
				material_arrive_flag=1;
//			if(x_axis==5&&X_INFR_LEFT==1&&car_status.task_mode==BACK)//到达二位码所对应坐标，此标志位为向右移动
//				qr_right_translation_flag=1;
			EXTI->PR=1<<13;
		}
		
		if((EXTI->PR & EXTI_Line14) != (uint32_t)RESET)//外部中断线14
		{
//			if((y_axis==Y_target)&&(x_axis==X_target)&&(car_status.task_mode==LEFT_TRANSLATION))//当小车即将到达y轴目标位置时，需要使能小车前后平移的红外传感器的外部中断，进行小车y轴位置的精确控制
//			{
//				y_location_arrive_flag=1;
//				//car_status.task_mode=STOP;
//			}
			if(x_axis==4&&y_axis==0&&(car_status.task_mode==RIGHT_TRANSLATION))//抓取标志位1为第一次2为第二次3为第三次
			{
				grab_flag=1;
				//car_status.task_mode=STOP;
			}
			
			delay_ms(18);//消抖
			
			if(Y_INFR_RIGHT==1)
			{
				qr_line++;
			}
			if(x_axis==6&&y_axis==0&&Y_INFR_RIGHT==1)//接近qr坐标时后退
				qr_back_flag=1;
			if(x_axis==0&&y_axis==0&&(car_status.task_mode==BACK)&&(circle_count==4))//回家标志位
			{
				home_arrive_flag=1;
				//car_status.task_mode=STOP;
			}
			EXTI->PR=1<<14;
		}
		
		if((EXTI->PR & EXTI_Line15) != (uint32_t)RESET)//外部中断线15
		{
//			if((y_axis==Y_target)&&(x_axis==X_target)&&(car_status.task_mode==RIGHT_TRANSLATION))//当小车即将到达y轴目标位置时，需要使能小车前后平移的红外传感器的外部中断，进行小车y轴位置的精确控制
//			{
//				y_location_arrive_flag=1;
//				//car_status.task_mode=STOP;
//			}
			if(x_axis==4&&y_axis==2&&(car_status.task_mode==LEFT_TRANSLATION))//当小车即将到达物料放置区时，触发中断
			{
				place_flag=1;
			}
			
			if(x_axis==4&&y_axis==0&&(car_status.task_mode==RIGHT_TRANSLATION))//抓取标志位1为第一次2为第二次3为第三次
			{
				grab_flag=1;
				car_status.task_mode=STOP;
			}
			if(x_axis==1&&y_axis==0&&(car_status.task_mode==BACK)&&(circle_count==4))//回家标志位
			{
				home_arrive_flag=1;
				//car_status.task_mode=STOP;
			}
			
			EXTI->PR=1<<15; 
		}
}

int black_line=0,black_flag=0;//当为1时，表示检测到单线，为2时表示检测到双线
void EXTI1_IRQHandler(void)
{
	delay_ms(18);//消抖
	if(INFR_DETECTION==1)
	{
		black_line++;
		OLED_ShowNumber(90,0,black_line,2,12); 
		black_flag=1;
	}
//	if(x_axis==0&&y_axis==0&&(car_status.task_mode==BACK)&&(circle_count==4))//回家标志位
//	{
//		home_arrive_flag=1;
//		car_status.task_mode=STOP;
//	}
		EXTI->PR=1<<1;   
}

/**************************************************************************
函数功能：红外传感器检测黑线来修正陀螺仪的目标yaw进行巡线
入口参数：无
返回  值：无 
**************************************************************************/

void infr_control()
{
	if(car_status.line_patrol_mode==Y_LINE_PATROL&&car_status.running_mode==FORWARD) //Y轴巡线模式  车的前  mode=2
	{
		if(Y_INFR_RIGHT==0&&Y_INFR_LEFT==0)									//infrared遇到黑线输出低电平
		{
		}
		if(Y_INFR_LEFT==1&&Y_INFR_RIGHT==1)
		{
			
		}
		if(Y_INFR_LEFT==0&&Y_INFR_RIGHT==1)
		{
			Yaw_target-=YAW_DELTA;
			
		}
		if(Y_INFR_LEFT==1&&Y_INFR_RIGHT==0)
		{
			Yaw_target+=YAW_DELTA;
			
		}
	}
//	if(car_status.line_patrol_mode==Y_LINE_PATROL&&car_status.running_mode==BACK) //Y轴巡线模式  车的后  mode=2
//	{
//		if(Y_INFR_RIGHT==0&&Y_INFR_LEFT==0)									//infrared遇到黑线输出低电平
//		{
//		}
//		if(Y_INFR_LEFT==1&&Y_INFR_RIGHT==1)
//		{
//			
//		}
//		if(Y_INFR_LEFT==0&&Y_INFR_RIGHT==1)
//		{
//			Yaw_target+=YAW_DELTA;
//			
//		}
//		if(Y_INFR_LEFT==1&&Y_INFR_RIGHT==0)
//		{
//			Yaw_target-=YAW_DELTA;
//			
//		}
//	}
	if(car_status.line_patrol_mode==X_LINE_PATROL&&car_status.running_mode==LEFT_TRANSLATION) //X轴巡线模式  mode=1
	{
		if(X_INFR_RIGHT==0&&X_INFR_LEFT==0&&circle_count!=0)									//infrared遇到黑线输出低电平
		{
			CAR_SPEED=LOW_SPEED;
			chassis_left_translation();
		}
		if(X_INFR_LEFT==1&&X_INFR_RIGHT==1&&circle_count!=0)
		{
			chassis_left_translation();
		}
		if(X_INFR_LEFT==0&&X_INFR_RIGHT==1&&circle_count!=0)
		{
			CAR_SPEED=SPEED;
			chassis_forwrd();
			//Yaw_target-=YAW_DELTA;
		}
		if(X_INFR_LEFT==1&&X_INFR_RIGHT==0&&circle_count!=0)
		{
			CAR_SPEED=SPEED;
			chassis_back();
			//Yaw_target+=YAW_DELTA;
		}
	}
	if(car_status.line_patrol_mode==X_LINE_PATROL&&car_status.running_mode==RIGHT_TRANSLATION) //X轴巡线模式  mode=1
	{
		if(X_INFR_RIGHT==0&&X_INFR_LEFT==0&&circle_count!=0&&circle_count!=4)									//infrared遇到黑线输出低电平
		{
			CAR_SPEED=LOW_SPEED;
			chassis_right_translation();
		}
		if(X_INFR_LEFT==1&&X_INFR_RIGHT==1&&circle_count!=0&&circle_count!=4)
		{
			CAR_SPEED=LOW_SPEED;
			chassis_right_translation();
		}
		if(X_INFR_LEFT==0&&X_INFR_RIGHT==1&&circle_count!=0&&circle_count!=4)
		{
			CAR_SPEED=SPEED;
			chassis_forwrd();
			//Yaw_target+=YAW_DELTA;
		}
		if(X_INFR_LEFT==1&&X_INFR_RIGHT==0&&circle_count!=0&&circle_count!=4)
		{
			CAR_SPEED=SPEED;
			chassis_back();
			//Yaw_target-=YAW_DELTA;
		}
	}
	if(car_status.line_patrol_mode==STOP_LINE_PATROL) //停止模式
	{
		set_motor_pwm(0,0,0,0);
	}
}



