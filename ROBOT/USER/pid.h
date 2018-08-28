#ifndef __PID_H
#define __PID_H	 
#include "sys.h"
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
typedef struct 
{
  float setpoint;//�趨Ŀ��	
  float proportion ;//��������
  float integral ;//���ֳ���
  float derivative;//΢�ֳ���
	float last_error;//��һ������
	float lastlast_error;//���ϴ�����
	float last_output;//�ϴ����
} PIDtypedef;

void pidInit(PIDtypedef* PIDx);
void pidSetPoint(PIDtypedef* PIDx, float goal);
void pidSetpara(PIDtypedef* PIDx, float pp, float ii, float dd);
float increpid(PIDtypedef* PIDx, float cur_error);
void resetPID(PIDtypedef* PIDx);

		 				    
#endif
