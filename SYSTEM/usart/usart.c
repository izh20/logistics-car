#include "usart.h"	  


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,0,USART1_IRQn,2);//组2，最低优先级 
}

/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
uint8_t USART1_Date[4]={0};//串口2接收到的数据
uint8_t USART1_RX_BUF=0;
uint16_t qr_codes=0;
//u16 USART_RX_STA=0;       //接收状态标记
u8 i=0;
int USART1_IRQHandler(void)
{	
	if(USART1->SR&(1<<5))//接收到数据
	{	      
				u8 temp;
				temp=USART1->DR;
		//send_data(temp);
//		 if((USART_RX_STA&0x8000)==0)//接收未完成
//		 {
//			 if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				USART_RX_STA|=0x8000;	//接收完成了 
//			}else
//			{
//				if(temp==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//					USART_RX_STA++;
//				}
//			}
		
				USART1_Date[i]=temp;
				
			if(USART1_Date[3]==0x0D)
			{
				USART1_RX_BUF=1;
			}
			i++;
			if(i==4)
			{
					i=0;
			}
			
			get_qr_code();
		 }
   
return 0;	
}
uint8_t scan_qr_success_flag=0;//扫描二维码成功标志位
int qr_first,qr_second,qr_third;//第一次抓取物块的颜色
void get_qr_code()
{
	if(USART1_RX_BUF==1)
	{
		USART1_RX_BUF=0;
		qr_codes=(USART1_Date[0]&0x0f)*100+(USART1_Date[1]&0x0f)*10+(USART1_Date[2]&0x0f);
		OLED_ShowNumber(45,25,(int)qr_codes,3,12);
		qr_first=qr_codes/100;//第一次抓取物块的颜色
		qr_second=qr_codes/10%10;//第二次抓取物块的颜色
		qr_third=qr_codes%10;//第三次抓取物块的颜色
		
		scan_qr_success_flag=1;
	}
}
