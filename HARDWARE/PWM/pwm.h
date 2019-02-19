#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define PWM_LF   TIM3->CCR1  
#define PWM_RF   TIM3->CCR2 

#define PWM_LB   TIM3->CCR3  
#define PWM_RB   TIM3->CCR4

void TIM3_PWM_init(int arr,int psc);
void set_motor_pwm(int LF,int RF,int LB,int RB);
#endif
