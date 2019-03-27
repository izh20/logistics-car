#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 
extern uint16_t qr_codes;
extern uint8_t scan_qr_success_flag;
extern int qr_first,qr_second,qr_third;//第一次抓取物块的颜色
void get_qr_code(void);
void uart_init(u32 pclk2,u32 bound);
#endif	   
















