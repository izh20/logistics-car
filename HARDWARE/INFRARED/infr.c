#include "infr.h"
#include "sys.h"
//PB12 PB13 PB14 PB15
#define X_INFR_RIGHT PBin(12)   //����С������ѭ��
#define X_INFR_LEFT  PBin(13)
#define Y_INFR_RIGHT PBin(14)   //����С��ǰ��ѭ��
#define Y_INFR_LEFT	 PBin(15)
#define INFR_DETECTION PAin(1)

#define SPEED 650 //�����ٶ�

#define YAW_DELTA    0.02//0.034
#define X_YAW_DELTA	 0.04
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
	//Ex_NVIC_Config(GPIO_B,12,3);//�����ش���
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
u8 block_position=0;//���λ��
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
			
			delay_ms(10);   //����			 
			if(KEY1==0)		//����
			{
				LED=~LED;
				block_position++;
				if(block_position==7)
					block_position=0;
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
			//delay_ms(18);//����
			if(x_axis==4&&car_status.task_mode==BACK)//ʶ���ά����жϳ��Ƿ��ߵ�����ǰ��
				material_arrive_flag=1;
//			if(x_axis==5&&X_INFR_LEFT==1&&car_status.task_mode==BACK)//�����λ������Ӧ���꣬�˱�־λΪ�����ƶ�
//				qr_right_translation_flag=1;
			EXTI->PR=1<<13;
		}
		
		if((EXTI->PR & EXTI_Line14) != (uint32_t)RESET)//�ⲿ�ж���14
		{
//			if((y_axis==Y_target)&&(x_axis==X_target)&&(car_status.task_mode==LEFT_TRANSLATION))//��С����������y��Ŀ��λ��ʱ����Ҫʹ��С��ǰ��ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��y��λ�õľ�ȷ����
//			{
//				y_location_arrive_flag=1;
//				//car_status.task_mode=STOP;
//			}
			if(x_axis==4&&y_axis==0&&(car_status.task_mode==RIGHT_TRANSLATION))//ץȡ��־λ1Ϊ��һ��2Ϊ�ڶ���3Ϊ������
			{
				grab_flag=1;
				//car_status.task_mode=STOP;
			}
			
			delay_ms(18);//����
			
			if(Y_INFR_RIGHT==1)
			{
				qr_line++;
			}
			if(x_axis==6&&y_axis==0&&Y_INFR_RIGHT==1)//�ӽ�qr����ʱ����
				qr_back_flag=1;
			if(x_axis==0&&y_axis==0&&(car_status.task_mode==BACK)&&(circle_count==4))//�ؼұ�־λ
			{
				home_arrive_flag=1;
				//car_status.task_mode=STOP;
			}
			EXTI->PR=1<<14;
		}
		
		if((EXTI->PR & EXTI_Line15) != (uint32_t)RESET)//�ⲿ�ж���15
		{
//			if((y_axis==Y_target)&&(x_axis==X_target)&&(car_status.task_mode==RIGHT_TRANSLATION))//��С����������y��Ŀ��λ��ʱ����Ҫʹ��С��ǰ��ƽ�Ƶĺ��⴫�������ⲿ�жϣ�����С��y��λ�õľ�ȷ����
//			{
//				y_location_arrive_flag=1;
//				//car_status.task_mode=STOP;
//			}
			if(x_axis==4&&y_axis==2&&(car_status.task_mode==LEFT_TRANSLATION))//��С�������������Ϸ�����ʱ�������ж�
			{
				place_flag=1;
			}
			
			if(x_axis==4&&y_axis==0&&(car_status.task_mode==RIGHT_TRANSLATION))//ץȡ��־λ1Ϊ��һ��2Ϊ�ڶ���3Ϊ������
			{
				grab_flag=1;
				car_status.task_mode=STOP;
			}
			if(x_axis==1&&y_axis==0&&(car_status.task_mode==BACK)&&(circle_count==4))//�ؼұ�־λ
			{
				home_arrive_flag=1;
				//car_status.task_mode=STOP;
			}
			
			EXTI->PR=1<<15; 
		}
}

int black_line=0,black_flag=0;//��Ϊ1ʱ����ʾ��⵽���ߣ�Ϊ2ʱ��ʾ��⵽˫��
void EXTI1_IRQHandler(void)
{
	delay_ms(18);//����
	if(INFR_DETECTION==1)
	{
		black_line++;
		OLED_ShowNumber(90,0,black_line,2,12); 
		black_flag=1;
	}
//	if(x_axis==0&&y_axis==0&&(car_status.task_mode==BACK)&&(circle_count==4))//�ؼұ�־λ
//	{
//		home_arrive_flag=1;
//		car_status.task_mode=STOP;
//	}
		EXTI->PR=1<<1;   
}

/**************************************************************************
�������ܣ����⴫���������������������ǵ�Ŀ��yaw����Ѳ��
��ڲ�������
����  ֵ���� 
**************************************************************************/

void infr_control()
{
	if(car_status.line_patrol_mode==Y_LINE_PATROL&&car_status.running_mode==FORWARD) //Y��Ѳ��ģʽ  ����ǰ  mode=2
	{
		if(Y_INFR_RIGHT==0&&Y_INFR_LEFT==0)									//infrared������������͵�ƽ
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
//	if(car_status.line_patrol_mode==Y_LINE_PATROL&&car_status.running_mode==BACK) //Y��Ѳ��ģʽ  ���ĺ�  mode=2
//	{
//		if(Y_INFR_RIGHT==0&&Y_INFR_LEFT==0)									//infrared������������͵�ƽ
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
	if(car_status.line_patrol_mode==X_LINE_PATROL&&car_status.running_mode==LEFT_TRANSLATION) //X��Ѳ��ģʽ  mode=1
	{
		if(X_INFR_RIGHT==0&&X_INFR_LEFT==0&&circle_count!=0)									//infrared������������͵�ƽ
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
	if(car_status.line_patrol_mode==X_LINE_PATROL&&car_status.running_mode==RIGHT_TRANSLATION) //X��Ѳ��ģʽ  mode=1
	{
		if(X_INFR_RIGHT==0&&X_INFR_LEFT==0&&circle_count!=0&&circle_count!=4)									//infrared������������͵�ƽ
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
	if(car_status.line_patrol_mode==STOP_LINE_PATROL) //ֹͣģʽ
	{
		set_motor_pwm(0,0,0,0);
	}
}



