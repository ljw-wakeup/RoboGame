#include "pid.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

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



