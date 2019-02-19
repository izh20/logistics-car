#ifndef __PCA_H
#define __PCA_H	
#include "stm32f10x.h"

#define pca_adr1 0x80

#define pca_adr2 0x82

#define pca_mode1 0x0
#define pca_pre 0xFE


#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

void pca_write1(u8 adr,u8 data);
u8 pca_read1(u8 adr);
void pca_reset1(void);
void pca_setfreq1(float freq) ;
void pca_setpwm1(u8 num, u32 on, u32 off);

void pca_write2(u8 adr,u8 data);
u8 pca_read2(u8 adr);
void pca_reset2(void);
void pca_setfreq2(float freq) ;
void pca_setpwm2(u8 num, u32 on, u32 off);


#endif



