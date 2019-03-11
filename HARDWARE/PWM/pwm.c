#include "pwm.h"

void TIM3_PWM_init(int arr,int psc)
{
	//TIM3��ʼ�� ���pwm
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��    
	RCC->APB2ENR|=3<<2;       //PORTA,Bʱ��ʹ�� 
	GPIOA->CRL&=0X00FFFFFF; //PA6 PA7 ���֮ǰ������  
	GPIOA->CRL|=0XBB000000; //���ù������ 
	GPIOB->CRL&=0XFFFFFF00;
	GPIOB->CRL|=0X000000BB;//PB0 PB1 �������
	TIM3->ARR=arr;    //�趨�������Զ���װֵ   
	TIM3->PSC=psc;    //Ԥ��Ƶ������
  
	TIM3->CCMR1|=6<<4;	//CH1 PWM1ģʽ	
	TIM3->CCMR1|=6<<12; //CH2 PWM1ģʽ	
	TIM3->CCMR2|=6<<4;	//CH3 PWM1ģʽ	
	TIM3->CCMR2|=6<<12; //CH4 PWM1ģʽ	
	
	TIM3->CCMR1|=1<<3; //CH1Ԥװ��ʹ��	  
	TIM3->CCMR1|=1<<11;//CH2Ԥװ��ʹ��	 
	TIM3->CCMR2|=1<<3; //CH3Ԥװ��ʹ��	  
	TIM3->CCMR2|=1<<11;//CH4Ԥװ��ʹ��	 
	TIM3->CCER|=1<<0;  //CH1���ʹ��	
	TIM3->CCER|=1<<4;  //CH2���ʹ��	   
	TIM3->CCER|=1<<8;  //CH3���ʹ��	
	TIM3->CCER|=1<<12; //CH4���ʹ��	   
	TIM3->CR1=0x80;  //ARPEʹ�� 
	TIM3->CR1|=0x01;   //ʹ�ܶ�ʱ�� 	
}


void set_motor_pwm(int LF,int RF,int LB,int RB)
{
	if(LF>1800)
		LF=1800;
	if(LF<0)
		LF=0;
	if(RF>1800)
		RF=1800;
	if(RF<0)
		RF=0;
	if(LB>1800)
		LB=1800;
	if(LB<0)
		LB=0;
	if(RB>1800)
		RB=1800;
	if(RB<0)
		RB=0;
	TIM3->CCR1=LB;
	TIM3->CCR2=RB;
	TIM3->CCR3=RF;
	TIM3->CCR4=LF;
}
