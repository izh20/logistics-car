#include "sys.h"

/*
作者：周恒
时间：2019/1/1
*/
/**************************************************************************

**************************************************************************/

/*******************************物流小车引脚定义 begin************************/
/*
1.mpu6050 PCA9685驱动				
	SDA  PA8
	SCL  PA11
2.OLED
	RST			PB3
	DC			PA15
	SCL			PB5
	SDA			PB4
3.USART1	openmv通讯
	TX:			PA10
	RX:			PA9
4.USART2 无线串口通讯
	TX			PA2			
	RX			PA3
	
5.USART3
	TX			PB10
	RX			PB11
4.SWD
  SWDIO:  PA13
	SWCLK:	PA14
5.红外传感器
					PB12
					PB13
					PB14
					PB15
		外部中断 红外传感器
					PA1
					PA12
6.key			PA5			
7.led			PA4
8.电机驱动
					PC13
					PC14
					PC15
					PA0
	电机PWM调速	TIM3 CH1-CH4
					PB0
					PB1
					PA6
					PA7				
9.


*/
/******************************物流小车引脚定义 end************************/



u8 Way_Angle=2;    
float Angle_Balance; 
int main(void)
{ 
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
	uart_init(72,115200);           //初始化串口1
	
	TIM3_PWM_init(1999,71);					//底盘PWM调速初始化
	IIC_Init();                     //模拟IIC初始化
	
	/*************PCA初始化 begin****************/
//	pca_reset1();
//	pca_setfreq1(50);
	
	/*************PCA初始化 end****************/
  MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //初始化DMP   
	LED_Init();                     //=====初始化与 LED 连接的硬件接口
	KEY_Init();                     //=====按键初始化
	infr_Init();										//红外传感器外部中断，用于定位和循迹
	OLED_Init();                    //=====OLED初始化
	
	param_init();										//麦轮PID参数初始化
	chassis_init();										//初始化电机引脚
	
//EXTI_Init();											//红外传感器外部中断，用于定位
 uart2_init(36,57600);					//初始化串口2
 Timer1_Init(4999,71);           //=====2MS进一次中断服务函数
	
	while(1)
		{
			OLED_ShowNumber(0,10,(int)Yaw,3,12);
			OLED_ShowNumber(30,10,(int)Yaw_target,3,12);
			OLED_ShowNumber(45,10,(int)wheel_flag,2,12);
			OLED_ShowNumber(60,10,(int)qr_line,2,12);
			OLED_ShowNumber(0,25,x_axis,1,12);
			OLED_ShowNumber(15,25,y_axis,1,12); 
			//OLED_ShowNumber(30,25,black_flag,1,12);	
			
			//OLED_ShowNumber(60,25,target_x_err,3,12);
			//OLED_ShowNumber(75,25,qr_code_flag,1,12);
			
			OLED_ShowNumber(0,45,(int)QR_code_location,1,12);
			OLED_ShowNumber(15,45,(int)material_location,1,12);
			OLED_ShowNumber(30,45,(int)location_arrive_finish_flag,1,12);
			OLED_ShowNumber(45,45,(int)x_location_arrive_flag,1,12);
			OLED_ShowNumber(60,45,(int)y_location_arrive_flag,1,12);
			OLED_ShowNumber(80,45,(int)X_target,1,12);
			OLED_ShowNumber(95,45,(int)Y_target,1,12);
			//OLED_ShowString(00,40,"VOLTAGE");
			OLED_Refresh_Gram();
			/***************舵机 0度对应占空比55/4096 180度对应占空比250/4096 ********************/
			/**** joint1 0度对应于舵机138度； joint1 -135度对应于舵机30度          **/
			/**** joint2 0度对应于舵机90度； joint2 90度对应于舵机18度          **/
//			pca_setpwm1(0,0,200);    
				//pca_setpwm1(1,0,degree2duty(30));
//	  	pca_setpwm1(2,0,degree2duty(18));  //140 - 0  300 - 90
//			
			//pca_setpwm1(3,0,degree2duty(80));
			//shanwai_send();
				//printf("卡 尔 曼 滤 波 输 出 Pitch:  %f\r\n  ",Angle_Balance);  //y 
			go_to_scan_QR();
			grab_task();
			//car_status.task_mode=BACK;
			
		} 
}
