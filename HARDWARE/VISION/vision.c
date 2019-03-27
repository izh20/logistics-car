#include "vision.h"

#define vision_target_x 160 //视觉x轴目标
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
uint16_t qr_unpack=0;//二维码解码数据
uint16_t block_unpack=0;//色块顺序  红1绿2蓝3
uint8_t qr_code=0;//二维码的数据
uint8_t qr_code_flag=1;//识别标志位 1为未识别，0为识别成功
uint16_t target_x_err;//色块x轴误差
//uint16_t sum=0;//校验和
uint8_t block_code;
char scan_block_success_flag=0;
//int qr_first,qr_second,qr_third;//第一次抓取物块的颜色
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
		scan_block_success_flag=1;//色块识别成功
//		qr_first=qr_unpack/100;//第一次抓取物块的颜色
//		qr_second=qr_unpack/10%10;//第二次抓取物块的颜色
//		qr_third=qr_unpack%10;//第三次抓取物块的颜色
		
	}
	
}
/**********************************************************************************************************
*函 数 名: grab_unpack
*功能说明: 对抓取顺序解码 
*形    参:
*返 回 值: 
**********************************************************************************************************/

void grab_unpack(int qr)
{
	if(qr==block_unpack/100)//抓左边
	{
		OLED_ShowString(0,55,"l");
		grab_left_material();
		time_delay(40);
		grab_firmly();//抬升
	}
	if(qr==block_unpack/10%10)//抓中间
	{
		OLED_ShowString(0,55,"m");
		grab_mid_material();
		time_delay(40);
		grab_firmly();//抬升
	}
	if(qr==block_unpack%10)//抓右边
	{
		OLED_ShowString(0,55,"r");
		grab_right_material();
		time_delay(40);
		grab_firmly();//抬升
	}
}
/**********************************************************************************************************
*函 数 名: put_material
*功能说明: 放物料顺序  1是红，2是绿，3是蓝
*形    参:
*返 回 值: 
**********************************************************************************************************/
void put_material(int qr)
{
	if(qr==RIGHT_AREA)//放右边
	{
		servos_put_right_material();
	}
	if(qr==MID_AREA)//放中间
	{
		servos_put_mid_material();
	}
	if(qr==LEFT_AREA)//放左边
	{
		servos_put_left_material();
	}
	
}
/**********************************************************************************************************
*函 数 名: Data_Transform
*功能说明: 对数据解码
*形    参:
*返 回 值: 
**********************************************************************************************************/

int Data_Transform(uint8_t code)
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
/**********************************************************************************************************
*函 数 名: vision_status_detection
*功能说明: 视觉信号状态机检测
*形    参:
*返 回 值: 
**********************************************************************************************************/
void vision_status_detection()
{
	if(get_tick()-vision_time>80)
			{
				vision_status.block_recognition_status=UNIDENTIFICATION;//有视觉信息，色块识失败
				OLED_ShowString(90,10,"N");//在显示屏上显示
			}
}

void vision_control()
{
	
	if(vision_status.block_recognition_status==RECOGNITION)//如果识别到色块
	{
		CAR_SPEED=vision_pid_control();//当识别到色块时进行PID计算
		if(target_x_err>vision_target_x)
			car_status.task_mode=BACK;
		else if(target_x_err<vision_target_x)
			car_status.task_mode=FORWARD;
		else
			car_status.task_mode=STOP;
	}
	else
	{
		CAR_SPEED=800;//恢复初始速度
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
	RCC->APB2ENR|=1<<3; //使能 PORTB 时钟  
GPIOB->CRL&=0XFFFFF0FF;
GPIOB->CRL|=0X00000300;//PA4 推挽输出
GPIOB->ODR|=0<<2; //PA4 输出低
}

void start_openmv()
{
	OPENMV_START=1;
}
