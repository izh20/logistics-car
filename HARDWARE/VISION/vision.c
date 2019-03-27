#include "vision.h"

#define vision_target_x 160 //�Ӿ�x��Ŀ��
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
		if(counter == 1 && USART2_Date[1] != 0xa5)
		{
			counter=0;
			
		}
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
uint16_t qr_unpack=0;//��ά���������
uint16_t block_unpack=0;//ɫ��˳��  ��1��2��3
uint8_t qr_code=0;//��ά�������
uint8_t qr_code_flag=1;//ʶ���־λ 1Ϊδʶ��0Ϊʶ��ɹ�
uint16_t target_x_err;//ɫ��x�����
//uint16_t sum=0;//У���
uint8_t block_code;
char scan_block_success_flag=0;
//int qr_first,qr_second,qr_third;//��һ��ץȡ������ɫ
void get_vision_data()
{
	if(USART2_RX_BUF==1)
	{
		USART2_RX_BUF=0;
		//target_x_err=(USART2_Date[1] | USART2_Date[2]<<8);
		qr_code=USART2_Date[2];
		block_code=USART2_Date[3];
		block_unpack=Data_Transform(block_code);
		qr_unpack=Data_Transform(qr_code);
		scan_block_success_flag=1;//ɫ��ʶ��ɹ�
//		qr_first=qr_unpack/100;//��һ��ץȡ������ɫ
//		qr_second=qr_unpack/10%10;//�ڶ���ץȡ������ɫ
//		qr_third=qr_unpack%10;//������ץȡ������ɫ
		
	}
	
}
/**********************************************************************************************************
*�� �� ��: grab_unpack
*����˵��: ��ץȡ˳����� 
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/

void grab_unpack(int qr)
{
	if(qr==block_unpack/100)//ץ���
	{
		OLED_ShowString(0,55,"l");
		grab_left_material();
		time_delay(40);
		grab_firmly();//̧��
	}
	if(qr==block_unpack/10%10)//ץ�м�
	{
		OLED_ShowString(0,55,"m");
		grab_mid_material();
		time_delay(40);
		grab_firmly();//̧��
	}
	if(qr==block_unpack%10)//ץ�ұ�
	{
		OLED_ShowString(0,55,"r");
		grab_right_material();
		time_delay(40);
		grab_firmly();//̧��
	}
}
/**********************************************************************************************************
*�� �� ��: put_material
*����˵��: ������˳��  1�Ǻ죬2���̣�3����
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/
void put_material(int qr)
{
	if(qr==RIGHT_AREA)//���ұ�
	{
		servos_put_right_material();
	}
	if(qr==MID_AREA)//���м�
	{
		servos_put_mid_material();
	}
	if(qr==LEFT_AREA)//�����
	{
		servos_put_left_material();
	}
	
}
/**********************************************************************************************************
*�� �� ��: Data_Transform
*����˵��: �����ݽ���
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/

int Data_Transform(uint8_t code)
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
/**********************************************************************************************************
*�� �� ��: vision_status_detection
*����˵��: �Ӿ��ź�״̬�����
*��    ��:
*�� �� ֵ: 
**********************************************************************************************************/
void vision_status_detection()
{
	if(get_tick()-vision_time>80)
			{
				vision_status.block_recognition_status=UNIDENTIFICATION;//���Ӿ���Ϣ��ɫ��ʶʧ��
				OLED_ShowString(90,10,"N");//����ʾ������ʾ
			}
}

void vision_control()
{
	
	if(vision_status.block_recognition_status==RECOGNITION)//���ʶ��ɫ��
	{
		CAR_SPEED=vision_pid_control();//��ʶ��ɫ��ʱ����PID����
		if(target_x_err>vision_target_x)
			car_status.task_mode=BACK;
		else if(target_x_err<vision_target_x)
			car_status.task_mode=FORWARD;
		else
			car_status.task_mode=STOP;
	}
	else
	{
		CAR_SPEED=800;//�ָ���ʼ�ٶ�
		car_status.task_mode=STOP;
	}
	
}



int vision_kp=6;
int vision_pid_control()
{
	//int vision_target_x=160;
	int vision_output,err;
	err=abs(vision_target_x-target_x_err);
	vision_output=vision_kp*err;
	if(vision_output)
	return vision_output;
}

void openmv_start_init()
{
	RCC->APB2ENR|=1<<3; //ʹ�� PORTB ʱ��  
GPIOB->CRL&=0XFFFFF0FF;
GPIOB->CRL|=0X00000300;//PA4 �������
GPIOB->ODR|=0<<2; //PA4 �����
}

void start_openmv()
{
	OPENMV_START=1;
}
