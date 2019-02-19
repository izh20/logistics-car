#ifndef __VCAN_H
#define __VCAN_H
#include "sys.h"
extern int16_t wave_form_data[6];
void shanwai_send_wave_form(void);
void send_data(uint8_t date);
void shanwai_send(void);
#endif
