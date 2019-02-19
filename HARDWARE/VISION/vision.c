#include "vision.h"

VISION_STATUS vision_status;
///**************************************************************************
//函数功能：串口2接收中断
//入口参数：无
//返回  值：无
//**************************************************************************/
uint32_t  Latest_Vision_Control_Pack_Time = 0;
uint32_t vision_time=0;//用于检测使视觉识别是否离线
uint8_t counter;
uint8_t USART2_Date[4]={0};//串口2接收到的数据
uint8_t  USART2_RX_BUF=0;
void USART2_IRQHandler(void)
{	
		u8 Res;
	if(USART2->SR&(1<<5)) //接收到数据  接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res=USART2->DR;	//读取接收到的数据
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
		vision_status.block_recognition_status=RECOGNITION;//有视觉信息，色块识别正常
		OLED_ShowString(80,10,"Y");
  } 
}

/**********************************************************************************************************
*函 数 名: get_vision_data
*功能说明: 获取openmv传来的数据
*形    参:
*返 回 值: 
**********************************************************************************************************/
uint16_t qr_unpack;//二维码解码数据
uint8_t qr_code=0;//二维码的数据
uint8_t qr_code_flag=1;//识别标志位 1为未识别，0为识别成功
uint16_t target_x_err;//色块x轴误差
uint16_t sum=0;//校验和
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
*函 数 名: QR_Data_Transform
*功能说明: 对qr解码
*形    参:
*返 回 值: 
**********************************************************************************************************/

int QR_Data_Transform(uint8_t code)
{
	uint16_t qr;//qr解码
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
*函 数 名: get_tick
*功能说明: 获取时间
*形    参:
*返 回 值: 
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
				vision_status.block_recognition_status=UNIDENTIFICATION;//有视觉信息，色块识别正常
				OLED_ShowString(80,10,"N");
			}
}
