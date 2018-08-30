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
#define STRAIGHT_BASIC_SPEED 800
#define ROTATE_BASIC_SPEED 400

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


		 				    
#endif
