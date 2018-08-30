#ifndef __PWM_H
#define __PWM_H	 

#define M1_GROUP GPIOG
#define M2_GROUP GPIOG
#define M3_GROUP GPIOA
#define M4_GROUP GPIOG
#define M1_PIN GPIO_Pin_7
#define M2_PIN GPIO_Pin_8
#define M3_PIN GPIO_Pin_8
#define M4_PIN GPIO_Pin_6

#include "sys.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void PWMstraightInit(int k1, int k2, int k3, int k4);
void PWMrotateInit(int k1, int k2, int k3, int k4);
void PWMrotateSet( int k1, int k2, int k3, int k4);
void PWMstraightSet(int k1, int k2, int k3, int k4);
void straight(int direction, int adjustment[4]);
void rotate(int direction);
void move_stop(void);
#endif
