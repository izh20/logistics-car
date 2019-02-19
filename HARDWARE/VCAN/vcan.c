#include "vcan.h"
#include "usart.h"
#include "usart2.h"
#include "sys.h"
//��.c�ļ���Ҫ�����ɽ��������ַ��Ͳ���
int16_t  wave_form_data[6] = {0};
void send_data(uint8_t date)
{
				USART1->DR=date;
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���

}
void shanwai_send_wave_form(void)
{
	uint8_t i;
	
	send_data(0x03);
	send_data(0xfc);
	for(i = 0;i<6;i++)
	{
	  send_data((wave_form_data[i]&0xff)); //�ȷ��͵����ڷ��͸�λ
	  send_data((wave_form_data[i]>>8));
		
	  
	}

	send_data(0xfc);
	send_data(0x03);
}	

void shanwai_send(void)
{
//	wave_form_data[0] =(short)Angle_Balance;      
//	wave_form_data[1] =(short)Pitch;
//	wave_form_data[2] =(short)Roll;
//	wave_form_data[3] =(short)Yaw;
//	wave_form_data[4] =(short)0;
//	wave_form_data[5] =(short)0;
	wave_form_data[0] =(short)qr_code;      
	wave_form_data[1] =(short)qr_code_flag;
	wave_form_data[2] =(short)target_x_err;
	wave_form_data[3] =(short)USART2_Date[3];
	wave_form_data[4] =(short)USART2_Date[4];
	wave_form_data[5] =(short)USART2_Date[5];
	shanwai_send_wave_form();   //�����ݴ��䵽������λ�������Կ���ʵʱ����
}

