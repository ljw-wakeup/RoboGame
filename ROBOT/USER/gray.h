#ifndef __GRAY_H
#define __GRAY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

typedef struct 
{
  int max_number;//区间[min_number,max_number)
	int min_number;
  int maxlength;
	float center;
	int cross;
} Graycalcudef;
void Gray_Init(void);//初始化
void Read_Gray(u8* grayrequest); //读灰度
		 				    
#endif
