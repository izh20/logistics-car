#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

typedef struct
{
    uint8_t  block_recognition_status;//ɫ��ʶ��״̬
} VISION_STATUS;


enum
{		
    UNIDENTIFICATION,  //ɫ��δʶ��
		RECOGNITION		 //ɫ��ʶ��	
};
extern uint8_t  USART2_RX_BUF;
extern uint32_t vision_time;//���ڼ��ʹ�Ӿ�ʶ���Ƿ�����
extern VISION_STATUS vision_status;
extern uint16_t qr_unpack;//��ά���������
extern uint16_t block_unpack;//ɫ��˳��  ��1��2��3
extern uint8_t qr_code;//��ά�������
extern uint8_t qr_code_flag;//ʶ���־λ 1Ϊδʶ��0Ϊʶ��ɹ�
extern uint16_t target_x_err;//ɫ��x�����
//extern uint8_t USART2_Date[4];//����2���յ�������
void get_vision_data(void);
int Data_Transform(uint8_t code);
long get_tick(void);
void vision_status_detection(void);

void vision_control(void);
int vision_pid_control(void);
void grab_unpack(void);
#endif

