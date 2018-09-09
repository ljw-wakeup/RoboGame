#include "control.h"
#include "pid.h"
#include "pwm.h"
#include "gray.h"
#include "delay.h"
#include "timer.h"
///l///////////////////////////////////////////////////////////////////////////////	 
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
PIDtypedef PIDf, PIDb, PIDr, PIDl;
int last_trueincre = 0;
int valid_increnum = 0;
extern Graycalcudef graycal_0;
extern Graycalcudef graycal_1;
extern Graycalcudef graycal_2;
extern Graycalcudef graycal_3;

extern u8 TIM4_mode;
extern u8 TIM4mode1_count;


void Control_pidInit(){
	
	pidInit(&PIDf);
	pidInit(&PIDb);
	pidInit(&PIDr);
	pidInit(&PIDl);
	
	pidSetPoint(&PIDf, 0.0);
	pidSetPoint(&PIDb, 0.0);
	pidSetPoint(&PIDl, 0.0);
	pidSetPoint(&PIDr, 0.0);
	
	pidSetpara(&PIDf, 1, 0, 0);
	pidSetpara(&PIDb, 1, 0, 0);
	pidSetpara(&PIDl, 1, 0, 0);
	pidSetpara(&PIDr, 1, 0, 0);
	
}

void Control_pwmInit(){
	TIM3_PWM_Init(PWM_ORIGIN,0);
	PWMstraightInit(STRAIGHT_BASIC_SPEED, STRAIGHT_BASIC_SPEED, STRAIGHT_BASIC_SPEED, STRAIGHT_BASIC_SPEED);
	PWMrotateInit(ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED);
}
void Control_grayInit(){
	Gray_Init();
}
u8 Control_Straight(u8 grayrequest,int direction){
	int incre = 0;
	int adjustment[4] = {0,0,0,0};
	u8 gray[4] = {0,0,0,0};
	//forward or backward
	if (direction == 0 || direction == 2){
		//正常直走
		if(grayrequest == 0){
			gray[0] = 1;
			Read_Gray(gray);
			//forward
			if(direction == 0) {
				//有无效信息
				if (graycal_0.repeat){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 160;
					else incre = -3 * 160;
					
				}
				//没有重复信息
				else{
					incre = increpid(&PIDf, graycal_0.center) * 160;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//backward
			else {
				//无效
				if(graycal_0.repeat){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 160;
					else incre = -3 * 160;
					
				}
				else{
					incre = increpid(&PIDf, graycal_0.center) * 160;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			adjustment[0] = -incre;
		  adjustment[1] = incre;
			straight(direction,adjustment);
			if (graycal_0.cross) return 1;
			return 0;
		}
		//双线直走
		else if (grayrequest == 2){	
		  gray[2] = 1;
			gray[3] = 1;
			gray[0] = 1;
			Read_Gray(gray);
			//forward
			if(direction == 0) {
				if(graycal_2.maxlength > 1 || graycal_3.maxlength >1){
					if(graycal_2.repeat && graycal_3.repeat){
						valid_increnum ++;
						if(valid_increnum <= 3) incre = last_trueincre;
						else if (valid_increnum > 3 && last_trueincre > 0 ) incre = 3 * 20;
						else incre = -3 * 20;
						}
					else if(graycal_2.repeat)	incre = increpid(&PIDf, graycal_3.center) * 20;
					else if(graycal_3.repeat) incre = increpid(&PIDf, graycal_2.center) * 20;
					else incre = increpid(&PIDf, (graycal_2.center + graycal_3.center) / 2.0) * 20;
				}
				else{
					//向树莓派索取摄像信息进行PID；
				}
			}
			//backward
			else{
				if(graycal_2.maxlength >1 || graycal_3.maxlength > 1){
					if(graycal_2.repeat && graycal_3.repeat){
						valid_increnum ++;
						if(valid_increnum <= 3) incre = last_trueincre;
						else if (valid_increnum > 3 && last_trueincre > 0 ) incre = 3 * 20;
						else incre = -3 * 20;
						}
					else if(graycal_2.repeat)	incre = increpid(&PIDb, graycal_3.center) * 20;
					else if(graycal_3.repeat) incre = increpid(&PIDb, graycal_2.center) * 20;
					else incre = increpid(&PIDb, (graycal_2.center + graycal_3.center) / 2.0) * 20;
				}
				else{
					//向树莓派索取摄像头信息
				}
			}
			adjustment[0] = -incre;
			adjustment[1] = incre;
			straight(direction, adjustment);
			if(graycal_0.cross) return 1;
			return 0;
		}
	}
	//right or left 
  else{
		if(grayrequest == 1){
			gray[1] = 1;
			Read_Gray(gray);
			//右
			if (direction == 1) {
				if (graycal_1.repeat){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 160;
					else incre = -3 * 20;
				}
				//没有重复信息
				else{
					incre = increpid(&PIDr, graycal_1.center) * 160;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//左
			else {
				if (graycal_1.repeat){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 160;
					else incre = -3 * 20;
				}
				//没有重复信息
				else{
					incre = increpid(&PIDl, graycal_1.center) * 160;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
				
			}
				
			adjustment[3] = incre;
			adjustment[2] = -incre;
			straight(direction,adjustment);
			if(graycal_1.cross) return 1;
			return 0;
		}
	}
	return 0;
}

u8 Control_Rotate(int direction, int angle){
	rotate(direction);
	delay_ms(angle);
	return 0;
}


void Control_changeSpeed(int direction,int Dvalue){
	PWMstraightSet(STRAIGHT_BASIC_SPEED + Dvalue, STRAIGHT_BASIC_SPEED + Dvalue, STRAIGHT_BASIC_SPEED + Dvalue, STRAIGHT_BASIC_SPEED + Dvalue);
}

void Control_to_plot(){
	
}

u8 march_in_line(){
	u8 gray_request = 2;
	int direction = 0;
	//forward
		TIM4_mode = 1;
		TIM4_Int_Init(4999,7199);//500ms
		TIM4mode1_count = 0;
		TIM_Cmd(TIM4,ENABLE);
		//10是调参处,开环控制直走时间
		while(TIM4mode1_count<10){
			if(TIM4mode1_count == 5)
			//5也是调参处，这里代表要降速的时刻
				Control_changeSpeed(direction, STRAIGHT_SPEED_CHANGE);
		Control_Straight(gray_request,direction);
		}
		return 1;
}



void Control_test(){
	int adjustment[4] = {500, 500, 500, 500};
	straight(0,adjustment);
}

void Control_Stop(){
	move_stop();
}
 

void Control_Begin(int direction){
	int i;
	int j;
	int adjustment[4] = {0,0,0,0};
	for(i = 3; i >=0 ; i--){
		for(j = 0; j < 4; j++){
			adjustment[j] = i * (-100);
		}
		straight(direction,adjustment);
		delay_ms(100);
	}
}
