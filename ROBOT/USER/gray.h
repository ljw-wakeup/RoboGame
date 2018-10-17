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
#define CROSS_JUDGE_0 4
#define CROSS_JUDGE_1 3
#define CROSS_JUDGE_2 3    //���Ϊ4����Ч
#define CROSS_JUDGE_3 3    //���Ϊ4����Ч
typedef struct 
{
  int max_number;//����[min_number,max_number)
	int min_number;
  int maxlength;
	float center;
	u8 cross;
	u8 repeat;
	u8 invalid;
} Graycalcudef;
typedef struct
{
	u8 min;
	u8 max;
	u8 blacknum;
	u8 invalid;
	float center;
}Graycalcudef_beta;
void Gray_Init(void);//��ʼ��
void Read_Gray(u8* grayrequest); //���Ҷ�
void Read_Gray_beta(u8 grayrequest, u8 graymode);
void CalculFromGray_beta(Graycalcudef_beta* graycal_x_beta, u8* grayx_list, u8 graytab, u8 graymode);
		 				    
#endif
