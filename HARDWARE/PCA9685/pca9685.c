#include "pca9685.h"
#include "ioi2c.h"
#include "delay.h"
#include "math.h"
void pca_write1(u8 adr,u8 data)
{ 
	 IIC_Start();
	
	 IIC_Send_Byte(pca_adr1);
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(adr);
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(data);
	 IIC_Wait_Ack();
	
   IIC_Stop();
}
u8 pca_read1(u8 adr)
{
  u8 data;
	IIC_Start();
	
	IIC_Send_Byte(pca_adr1);
  IIC_Wait_Ack();
	
	IIC_Send_Byte(adr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
  IIC_Send_Byte(pca_adr1|0x01);
  IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
  IIC_Stop();
	
	return data;

}

void pca_reset1(void)
{
	//IIC_Init();
  pca_write1(pca_mode1,0x0);

}

void pca_setfreq1(float freq) 
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
		freq *= 0.92; 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = pca_read1(pca_mode1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		pca_write1(pca_mode1, newmode); // go to sleep
	
		pca_write1(pca_pre, prescale); // set the prescaler
	
		pca_write1(pca_mode1, oldmode);
		delay_ms(2);
	
		pca_write1(pca_mode1, oldmode | 0xa1); 
}

void pca_setpwm1(u8 num, u32 on, u32 off) 
{
			pca_write1(LED0_ON_L+4*num,on);
			pca_write1(LED0_ON_H+4*num,on>>8);
			pca_write1(LED0_OFF_L+4*num,off);
			pca_write1(LED0_OFF_H+4*num,off>>8);
}




void pca_write2(u8 adr,u8 data)
{ 
	 IIC_Start();
	
	 IIC_Send_Byte(pca_adr2);
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(adr);
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(data);
	 IIC_Wait_Ack();
	
   IIC_Stop();
}
u8 pca_read2(u8 adr)
{
  u8 data;
	IIC_Start();
	
	IIC_Send_Byte(pca_adr2);
  IIC_Wait_Ack();
	
	IIC_Send_Byte(adr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
  IIC_Send_Byte(pca_adr2|0x01);
  IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
  IIC_Stop();
	
	return data;

}

void pca_reset2(void)
{
	IIC_Init();
  pca_write2(pca_mode1,0x0);

}

void pca_setfreq2(float freq) 
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
		freq *= 0.92; 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = pca_read2(pca_mode1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		pca_write2(pca_mode1, newmode); // go to sleep
	
		pca_write2(pca_pre, prescale); // set the prescaler
	
		pca_write2(pca_mode1, oldmode);
		delay_ms(2);
	
		pca_write2(pca_mode1, oldmode | 0xa1); 
}

void pca_setpwm2(u8 num, u32 on, u32 off) 
{
			pca_write2(LED0_ON_L+4*num,on);
			pca_write2(LED0_ON_H+4*num,on>>8);
			pca_write2(LED0_OFF_L+4*num,off);
			pca_write2(LED0_OFF_H+4*num,off>>8);
}

