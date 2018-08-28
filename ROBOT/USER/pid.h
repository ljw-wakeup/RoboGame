#ifndef __PID_H
#define __PID_H	 
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
  float setpoint;//设定目标	
  float proportion ;//比例常数
  float integral ;//积分常数
  float derivative;//微分常数
	float last_error;//上一次输入
	float lastlast_error;//上上次输入
	float last_output;//上次输出
} PIDtypedef;

void pidInit(PIDtypedef* PIDx);
void pidSetPoint(PIDtypedef* PIDx, float goal);
void pidSetpara(PIDtypedef* PIDx, float pp, float ii, float dd);
float increpid(PIDtypedef* PIDx, float cur_error);
void resetPID(PIDtypedef* PIDx);

		 				    
#endif
