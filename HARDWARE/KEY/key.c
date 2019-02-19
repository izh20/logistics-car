#include "key.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ�������ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;               //ʹ��PORTAʱ��	   	 
	GPIOA->CRL&=0XFF0FFFFF;           
	GPIOA->CRL|=0X00800000;           //PA5 ��������
  GPIOA->ODR|=1<<15;                //PA5 ����	
	Ex_NVIC_Config(GPIO_A,5,FTIR);		//�½��ش���
	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//��ռ2�������ȼ�1����2
} 
/**************************************************************************
�������ܣ��ⲿ�ж϶�ȡ����״̬
��ڲ�������
����  ֵ���� 
**************************************************************************/
char wheel_flag=0;
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(10);   //����			 
    if(KEY==0)		//����
	{
  	LED=~LED;
		wheel_flag++;
		if(wheel_flag==3)
			wheel_flag=0;
	}
 	EXTI->PR=1<<5; //���LINE15�ϵ��жϱ�־λ  
}
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������ 2��˫�� 
**************************************************************************/
u8 click_N_Double (u8 time)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(KEY==0)  Forever_count++;   //������־λδ��1
     else        Forever_count=0;
		if(0==KEY&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2;//˫��ִ�е�ָ��
				}
		}
		if(1==KEY)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//����ִ�е�ָ��
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������ 
**************************************************************************/
u8 click(void)
{
			static u8 flag_key=1;//�������ɿ���־
			if(flag_key&&KEY==0)
			{
				delay_ms(10);	
				if(KEY==0)	
				{	
					flag_key=0;
					return 1;	// ��������
				}
			}
			else if(1==KEY)			flag_key=1;
			return 0;//�ް�������
}
/**************************************************************************
�������ܣ��������
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������2s
**************************************************************************/
u8 Long_Press(void)
{
			static u16 Long_Press_count,Long_Press;
	    if(Long_Press==0&&KEY==0)  Long_Press_count++;   //������־λδ��1
      else                       Long_Press_count=0; 
		  if(Long_Press_count>200)		
			{
				Long_Press=1;	
				Long_Press_count=0;
				return 1;
			}				
			 if(Long_Press==1)     //������־λ��1
			{
				  Long_Press=0;
			}
			return 0;
}
