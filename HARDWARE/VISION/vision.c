#include "vision.h"

VISION_STATUS vision_status;
///**************************************************************************
//�������ܣ�����2�����ж�
//��ڲ�������
//����  ֵ����
//**************************************************************************/
uint32_t  Latest_Vision_Control_Pack_Time = 0;
uint32_t vision_time=0;//���ڼ��ʹ�Ӿ�ʶ���Ƿ�����
uint8_t counter;
uint8_t USART2_Date[4]={0};//����2���յ�������
uint8_t  USART2_RX_BUF=0;
void USART2_IRQHandler(void)
{	
		u8 Res;
	if(USART2->SR&(1<<5)) //���յ�����  �����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res=USART2->DR;	//��ȡ���յ�������
		Latest_Vision_Control_Pack_Time = get_tick();
		USART2_Date[counter] =Res;
		if(counter == 0 && USART2_Date[0] != 0x5a)
		{
			counter=0;
			
		}
//		if(counter == 1 && USART2_Date[1] != 0xa5)
//		{
//			counter=0;
//			
//		}
		//printf("%x\n",USART2_Date[1]);
		//usart2_send(Res);
		counter++;
		
		if(counter==4)
		{
			counter=0;
			USART2_RX_BUF=1;
		}
		get_vision_data();
		vision_time=Latest_Vision_Control_Pack_Time;
		vision_status.block_recognition_status=RECOGNITION;//���Ӿ���Ϣ��ɫ��ʶ������
		OLED_ShowString(80,10,"Y");
  } 
}

/**********************************************************************************************************
*�� �� ��: get_vision_data
*����˵��: ��ȡopenmv����������
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/
uint16_t qr_unpack;//��ά���������
uint8_t qr_code=0;//��ά�������
uint8_t qr_code_flag=1;//ʶ���־λ 1Ϊδʶ��0Ϊʶ��ɹ�
uint16_t target_x_err;//ɫ��x�����
uint16_t sum=0;//У���
void get_vision_data()
{
	if(USART2_RX_BUF==1)
	{
		USART2_RX_BUF=0;
		target_x_err=(USART2_Date[1] | USART2_Date[2]<<8);
		qr_code=USART2_Date[3];
		qr_unpack=QR_Data_Transform(qr_code);
		OLED_ShowNumber(45,25,target_x_err,3,12);
		OLED_ShowNumber(70,25,qr_unpack,3,12);
	}
	
}
/**********************************************************************************************************
*�� �� ��: QR_Data_Transform
*����˵��: ��qr����
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/

int QR_Data_Transform(uint8_t code)
{
	uint16_t qr;//qr����
	switch(code)
	{
		case 1:	qr=123;break;
		case 2:	qr=132;break;
		case 3:	qr=213;break;
		case 4:	qr=231;break;
		case 5:	qr=312;break;
		case 6:	qr=321;break;
	}
	return qr;
}
/**********************************************************************************************************
*�� �� ��: get_tick
*����˵��: ��ȡʱ��
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/
long tick=0;
long get_tick()
{
	tick++;
	return tick;
}

void vision_status_detection()
{
	if(get_tick()-vision_time>80)
			{
				vision_status.block_recognition_status=UNIDENTIFICATION;//���Ӿ���Ϣ��ɫ��ʶ������
				OLED_ShowString(80,10,"N");
			}
}
