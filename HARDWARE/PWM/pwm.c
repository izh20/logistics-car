#include "pwm.h"

void TIM3_PWM_init(int arr,int psc)
{
	//TIM3初始化 输出pwm
	RCC->APB1ENR|=1<<1;//TIM3时钟使能    
	RCC->APB2ENR|=3<<2;       //PORTA,B时钟使能 
	GPIOA->CRL&=0X00FFFFFF; //PA6 PA7 清除之前的设置  
	GPIOA->CRL|=0XBB000000; //复用功能输出 
	GPIOB->CRL&=0XFFFFFF00;
	GPIOB->CRL|=0X000000BB;//PB0 PB1 推挽输出
	TIM3->ARR=arr;    //设定计数器自动重装值   
	TIM3->PSC=psc;    //预分频器设置
  
	TIM3->CCMR1|=6<<4;	//CH1 PWM1模式	
	TIM3->CCMR1|=6<<12; //CH2 PWM1模式	
	TIM3->CCMR2|=6<<4;	//CH3 PWM1模式	
	TIM3->CCMR2|=6<<12; //CH4 PWM1模式	
	
	TIM3->CCMR1|=1<<3; //CH1预装载使能	  
	TIM3->CCMR1|=1<<11;//CH2预装载使能	 
	TIM3->CCMR2|=1<<3; //CH3预装载使能	  
	TIM3->CCMR2|=1<<11;//CH4预装载使能	 
	TIM3->CCER|=1<<0;  //CH1输出使能	
	TIM3->CCER|=1<<4;  //CH2输出使能	   
	TIM3->CCER|=1<<8;  //CH3输出使能	
	TIM3->CCER|=1<<12; //CH4输出使能	   
	TIM3->CR1=0x80;  //ARPE使能 
	TIM3->CR1|=0x01;   //使能定时器 	
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
