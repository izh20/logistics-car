#include "sys.h"

/*
���ߣ��ܺ�
ʱ�䣺2019/1/1
*/
/**************************************************************************

**************************************************************************/

/*******************************����С�����Ŷ��� begin************************/
/*
1.mpu6050 PCA9685����				
	SDA  PA8
	SCL  PA11
2.OLED
	RST			PB3
	DC			PA15
	SCL			PB5
	SDA			PB4
3.USART1	openmvͨѶ
	TX:			PA10
	RX:			PA9
4.USART2 ���ߴ���ͨѶ
	TX			PA2			
	RX			PA3
5.USART3
	TX			PB10
	RX			PB11
4.SWD
  SWDIO:  PA13
	SWCLK:	PA14
5.���⴫����
					PB12
					PB13
					PB14
					PB15
		�ⲿ�ж� ���⴫����
					PA1
					PA12
6.key			PA5			
7.led			PA4
8.�������
					PC13
					PC14
					PC15
					PA0
	���PWM����	TIM3 CH1-CH4
					PB0
					PB1
					PA6
					PA7				
9.


*/
/******************************����С�����Ŷ��� end************************/



u8 Way_Angle=2;    
float Angle_Balance; 
int main(void)
{ 
	Stm32_Clock_Init(9);            //ϵͳʱ������
	delay_init(72);                 //��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	uart_init(72,115200);           //��ʼ������1
	
	TIM3_PWM_init(1999,71);					//����PWM���ٳ�ʼ��
	IIC_Init();                     //ģ��IIC��ʼ��
	
	/*************PCA��ʼ�� begin****************/
//	pca_reset1();
//	pca_setfreq1(50);
	
	/*************PCA��ʼ�� end****************/
  MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //��ʼ��DMP   
	LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	KEY_Init();                     //=====������ʼ��
	infr_Init();										//���⴫�����ⲿ�жϣ����ڶ�λ��ѭ��
	OLED_Init();                    //=====OLED��ʼ��
	
	param_init();										//����PID������ʼ��
	chassis_init();										//��ʼ���������
	
//EXTI_Init();											//���⴫�����ⲿ�жϣ����ڶ�λ
 uart2_init(36,57600);					//��ʼ������2
 Timer1_Init(2999,71);           //=====2MS��һ���жϷ�����
	
	while(1)
		{
			OLED_ShowNumber(0,10,(int)Yaw,3,12);
			OLED_ShowNumber(30,10,(int)Yaw_target,3,12);
			OLED_ShowNumber(60,10,(int)wheel_flag,3,12);
			OLED_ShowNumber(0,25,x_axis,1,12);
			OLED_ShowNumber(15,25,y_axis,1,12); 
			OLED_ShowNumber(30,25,black_flag,1,12);	
			
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
			/***************��� 0�ȶ�Ӧռ�ձ�55/4096 180�ȶ�Ӧռ�ձ�250/4096 ********************/
			/**** joint1 0�ȶ�Ӧ�ڶ��138�ȣ� joint1 -135�ȶ�Ӧ�ڶ��30��          **/
			/**** joint2 0�ȶ�Ӧ�ڶ��90�ȣ� joint2 90�ȶ�Ӧ�ڶ��18��          **/
//			pca_setpwm1(0,0,200);    
				//pca_setpwm1(1,0,degree2duty(30));
//	  	pca_setpwm1(2,0,degree2duty(18));  //140 - 0  300 - 90
//			
			//pca_setpwm1(3,0,degree2duty(80));
			shanwai_send();
				//printf("�� �� �� �� �� �� �� Pitch:  %f\r\n  ",Angle_Balance);  //y 
		
		} 
}