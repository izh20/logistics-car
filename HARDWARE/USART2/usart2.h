#ifndef __USRAT2_H
#define __USRAT2_H 
#include "sys.h"	  

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���


void usart2_send(u8 data);
void uart2_init(u32 pclk2,u32 bound);
//int USART2_IRQHandler(void);
#endif
