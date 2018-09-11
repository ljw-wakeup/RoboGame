#ifndef __CONTROL_H
#define __CONTROL_H	 
#include "sys.h"
#define QUARTER0 1000
#define QUARTER1 1000
#define TWO_QUARTER0 1000
#define TWO_QUARTER1 1000
#define THREE_QUARTER0 1000
#define THREE_QUARTER1 1000
#define ROUND0 1000
#define ROUND1 1000
#define MARCH_LINES 10000
#define STRAIGHT_BASIC_SPEED 700
#define ROTATE_BASIC_SPEED 400
#define STRAIGHT_SPEED_CHANGE -400
#define ULTRASONIC_DISTANCE 1000
#define ULTRASONIC_SLOWDOWN 2000


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

void Control_pidInit(void);
void Control_pwmInit(void);
void Control_grayInit(void);
u8 Control_Straight(u8 grayrequest,int direction);
u8 Control_Rotate(int direction,int angle);
void Control_changeSpeed(int direction,int Dvalue);
void Control_to_plot(void);
u8 march_in_line(void);
void Control_test(void);
void Control_Stop(void);
void Control_Begin(int direction);

		 				    
#endif
