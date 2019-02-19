#include "infr.h"
#include "sys.h"
//PB12 PB13 PB14 PB15
#define X_INFR_RIGHT PBin(12)   //����С������ѭ��
#define X_INFR_LEFT  PBin(13)
#define Y_INFR_RIGHT PBin(14)   //����С��ǰ��ѭ��
#define Y_INFR_LEFT	 PBin(15)
#define INFR_DETECTION PAin(1)

#define YAW_DELTA    0.02

//ƫ��������
char single_flag=0,double_flag=0;
char single_count=0,double_count=0;
char model=2;	//С��ģʽ 1��Y��ѭ�� 2��X��ѭ�� 0��ֹͣ
/**************************************************************************
�������ܣ����⴫������ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void infr_Init()
{
	RCC->APB2ENR|=1<<3;               //ʹ��PORTBʱ��	   	 
	RCC->APB2ENR|=1<<2;   					 //ʹ��PORTAʱ��	  
	
	GPIOA->CRL&=0XFFFFFF0F; 
	GPIOA->CRL|=0X00000080;//PA1��������
  GPIOA->ODR|=1<<1;     //PA1����	
	
	GPIOB->CRL&=0X0000FFFF;						//PB12,13,14,15 
	GPIOB->CRL|=0X88880000;
	GPIOB->ODR|=15<<12; //����
	Ex_NVIC_Config(GPIO_A,1,RTIR);		//�����ش���
	Ex_NVIC_Config(GPIO_B,12,3);//�����ش���
	Ex_NVIC_Config(GPIO_B,13,3);//�����ش���
	Ex_NVIC_Config(GPIO_B,14,3);//�����ش���
	Ex_NVIC_Config(GPIO_B,15,3);//�����ش���
	MY_NVIC_Init(0,0,EXTI1_IRQn,2);  	//��ռ0�������ȼ�0����2
	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);  	//��ռ2�������ȼ�1����2	
	
}

/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ�� ���ڼ�����
��ڲ�������
����  ֵ���� 
**************************************************************************/
void EXTI_Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   
	GPIOA->CRH&=0XFFF0FFFF; 
	GPIOA->CRH|=0X00080000;//PA12��������
  GPIOA->ODR|=1<<12;     //PA12����	
	
	GPIOA->CRL&=0XFFFFFF0F; 
	GPIOA->CRL|=0X00000080;//PA1��������
  GPIOA->ODR|=1<<1;     //PA1����	
	
	Ex_NVIC_Config(GPIO_A,12,RTIR);		//�����ش���
	Ex_NVIC_Config(GPIO_A,1,RTIR);		//�����ش���
	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);  	//��ռ2�������ȼ�1����2
	MY_NVIC_Init(0,0,EXTI1_IRQn,2);  	//��ռ0�������ȼ�0����2
	
}

u8 x_location_arrive_flag=0;//Ԥ��λ�õ����־λ
u8 y_location_arrive_flag=0;
void  EXTI15_10_IRQHandler() 
{    
		if((EXTI->PR & EXTI_Line12) != (uint32_t)RESET)//�ⲿ�ж���12
		{
			if((x_axis==X_target)&&(car_status.task_mode==FORWARD))//��С����������x��Ŀ��λ��ʱ����Ҫʹ�ܿ���С������ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��x��λ�õľ�ȷ����
			{
				x_location_arrive_flag=1;
				//car_status.task_mode=LEFT_TRANSLATION;
			}
			
			EXTI->PR=1<<12;   
		}
		
		if((EXTI->PR & EXTI_Line13) != (uint32_t)RESET)//�ⲿ�ж���13
		{
			if((x_axis==X_target)&&(car_status.task_mode==BACK))//��С����������x��Ŀ��λ��ʱ����Ҫʹ�ܿ���С������ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��x��λ�õľ�ȷ����
			{
				x_location_arrive_flag=1;
				//car_status.task_mode=RIGHT_TRANSLATION;
			}
			EXTI->PR=1<<13;
		}
		
		if((EXTI->PR & EXTI_Line14) != (uint32_t)RESET)//�ⲿ�ж���14
		{
			if((y_axis==Y_target)&&(x_axis==X_target)&&(car_status.task_mode==LEFT_TRANSLATION))//��С����������y��Ŀ��λ��ʱ����Ҫʹ��С��ǰ��ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��y��λ�õľ�ȷ����
			{
				y_location_arrive_flag=1;
				//car_status.task_mode=STOP;
			}
			EXTI->PR=1<<14;
		}
		
		if((EXTI->PR & EXTI_Line15) != (uint32_t)RESET)//�ⲿ�ж���15
		{
			if((y_axis==Y_target)&&(x_axis==X_target)&&(car_status.task_mode==RIGHT_TRANSLATION))//��С����������y��Ŀ��λ��ʱ����Ҫʹ��С��ǰ��ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��y��λ�õľ�ȷ����
			{
				y_location_arrive_flag=1;
				//car_status.task_mode=STOP;
			}
			EXTI->PR=1<<15; 
		}
}

int black_line=0,black_flag=0;//��Ϊ1ʱ����ʾ��⵽���ߣ�Ϊ2ʱ��ʾ��⵽˫��
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//����
	if(INFR_DETECTION==1)
	{
		black_line++;
		OLED_ShowNumber(90,0,black_line,2,12); 
		black_flag=1;
	}
		EXTI->PR=1<<1;   
}

/**************************************************************************
�������ܣ����⴫���������������������ǵ�Ŀ��yaw����Ѳ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void infr_control()
{
	if(car_status.line_patrol_mode==Y_LINE_PATROL) //Y��Ѳ��ģʽ  ����ǰ��  mode=2
	{
		if(Y_INFR_RIGHT==0&&Y_INFR_LEFT==0)									//infrared������������͵�ƽ
		{
			
			//chassis_back();
		}
		if(Y_INFR_LEFT==1&&Y_INFR_RIGHT==1)
		{
			//chassis_back();
			//chassis_forwrd();
		}
		if(Y_INFR_LEFT==0&&Y_INFR_RIGHT==1)
		{
			Yaw_target-=YAW_DELTA;
			//chassis_turn_left();
			//chassis_turn_left();
		}
		if(Y_INFR_LEFT==1&&Y_INFR_RIGHT==0)
		{
			Yaw_target+=YAW_DELTA;
			//chassis_turn_right();
			//chassis_turn_left();
			
		}
	}
	if(car_status.line_patrol_mode==X_LINE_PATROL) //X��Ѳ��ģʽ  mode=1
	{
		if(X_INFR_RIGHT==0&&X_INFR_LEFT==0)									//infrared������������͵�ƽ
		{
			
		}
		if(X_INFR_LEFT==1&&X_INFR_RIGHT==1)
		{
			
		}
		if(X_INFR_LEFT==0&&X_INFR_RIGHT==1)
		{
			Yaw_target-=YAW_DELTA;
		}
		if(X_INFR_LEFT==1&&X_INFR_RIGHT==0)
		{
			Yaw_target+=YAW_DELTA;
		}
	}
	if(car_status.line_patrol_mode==STOP_LINE_PATROL) //ֹͣģʽ
	{
		set_motor_pwm(0,0,0,0);
	}
}


