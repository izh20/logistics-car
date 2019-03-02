#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

typedef struct
{
    uint8_t  block_recognition_status;//色块识别状态
} VISION_STATUS;


enum
{		
    UNIDENTIFICATION,  //色块未识别
		RECOGNITION		 //色块识别	
};
extern uint8_t  USART2_RX_BUF;
extern uint32_t vision_time;//用于检测使视觉识别是否离线
extern VISION_STATUS vision_status;
extern uint16_t qr_unpack;//二维码解码数据
extern uint16_t block_unpack;//色块顺序  红1绿2蓝3
extern uint8_t qr_code;//二维码的数据
extern uint8_t qr_code_flag;//识别标志位 1为未识别，0为识别成功
extern uint16_t target_x_err;//色块x轴误差
//extern uint8_t USART2_Date[4];//串口2接收到的数据
void get_vision_data(void);
int Data_Transform(uint8_t code);
long get_tick(void);
void vision_status_detection(void);

void vision_control(void);
int vision_pid_control(void);
void grab_unpack(void);
#endif

