#ifndef __GRAY_H
#define __GRAY_H	 
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
  int max_number;//����[min_number,max_number)
	int min_number;
  int maxlength;
	float center;
	int cross;
} Graycalcudef;
void Gray_Init(void);//��ʼ��
void Read_Gray(u8* grayrequest); //���Ҷ�
		 				    
#endif
