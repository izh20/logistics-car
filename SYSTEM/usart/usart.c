#include "usart.h"	  


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
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
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬����
		  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,0,USART1_IRQn,2);//��2��������ȼ� 
}

/**************************************************************************
�������ܣ�����1�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
uint8_t USART1_Date[4]={0};//����2���յ�������
uint8_t USART1_RX_BUF=0;
uint16_t qr_codes=0;
//u16 USART_RX_STA=0;       //����״̬���
u8 i=0;
int USART1_IRQHandler(void)
{	
	if(USART1->SR&(1<<5))//���յ�����
	{	      
				u8 temp;
				temp=USART1->DR;
		//send_data(temp);
//		 if((USART_RX_STA&0x8000)==0)//����δ���
//		 {
//			 if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				USART_RX_STA|=0x8000;	//��������� 
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
uint8_t scan_qr_success_flag=0;//ɨ���ά��ɹ���־λ
int qr_first,qr_second,qr_third;//��һ��ץȡ������ɫ
void get_qr_code()
{
	if(USART1_RX_BUF==1)
	{
		USART1_RX_BUF=0;
		qr_codes=(USART1_Date[0]&0x0f)*100+(USART1_Date[1]&0x0f)*10+(USART1_Date[2]&0x0f);
		OLED_ShowNumber(45,25,(int)qr_codes,3,12);
		qr_first=qr_codes/100;//��һ��ץȡ������ɫ
		qr_second=qr_codes/10%10;//�ڶ���ץȡ������ɫ
		qr_third=qr_codes%10;//������ץȡ������ɫ
		
		scan_qr_success_flag=1;
	}
}
