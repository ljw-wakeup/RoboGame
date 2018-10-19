#ifndef __PWM_H
#define __PWM_H	 

#define M1_1_GROUP GPIOG
#define M1_2_GROUP GPIOG
#define M2_1_GROUP GPIOG
#define M2_2_GROUP GPIOG
#define M3_1_GROUP GPIOA
#define M3_2_GROUP GPIOA
#define M4_1_GROUP GPIOD 
#define M4_2_GROUP GPIOG 
#define M1_2_PIN GPIO_Pin_7
#define M1_1_PIN GPIO_Pin_5
#define M2_2_PIN GPIO_Pin_8
#define M2_1_PIN GPIO_Pin_4
#define M3_2_PIN GPIO_Pin_8
#define M3_1_PIN GPIO_Pin_11
#define M4_2_PIN GPIO_Pin_6
#define M4_1_PIN GPIO_Pin_14
#define PWM_ORIGIN 1000

#include "sys.h"


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void PWMstraightInit(int k1, int k2, int k3, int k4);
void PWMrotateInit(int k1, int k2, int k3, int k4);
void PWMrotateSet( int k1, int k2, int k3, int k4);
void PWMstraightSet(int k1, int k2, int k3, int k4);
void straight(int direction, int adjustment[4]);
void straight_only(int direction, int adjustment[4]);
void rotate(int direction);
void rotate_beta(int direction);
void move_stop(void);
void adjust(int* adjustment,int direction);

#endif
