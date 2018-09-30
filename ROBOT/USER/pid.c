#include "pid.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

void pidInit(PIDtypedef* PIDx){
	PIDx->proportion = 0;
	PIDx->integral = 0;
	PIDx->derivative = 0;
	PIDx->setpoint = 0;
	PIDx->last_error = 0;
	PIDx->lastlast_error = 0;
	PIDx->last_output = 0;
}

void pidSetPoint(PIDtypedef* PIDx, float goal){
	PIDx->setpoint = goal;
}

void pidSetpara(PIDtypedef* PIDx, float pp, float ii, float dd){
	PIDx->proportion = pp;
	PIDx->integral = ii;
	PIDx->derivative = dd;
}

float increpid(PIDtypedef* PIDx, float cur_error){
	float incre;
	float cur_output;
	cur_error -= PIDx->setpoint;
	incre = PIDx->proportion * (cur_error - PIDx->last_error) + PIDx->integral * PIDx->last_error + PIDx->derivative * (cur_error - 2 * PIDx->last_error + PIDx->lastlast_error );
	cur_output = PIDx->last_output + incre;
	PIDx->lastlast_error = PIDx->last_error;
	PIDx->last_error = cur_error;
	PIDx->last_output = cur_output;
	return cur_output;
}

void resetPID(PIDtypedef* PIDx)
{
    PIDx->last_error=0;
    PIDx->lastlast_error = 0;
		PIDx->last_output = 0;
}



