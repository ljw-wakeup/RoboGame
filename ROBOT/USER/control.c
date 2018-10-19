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
extern Graycalcudef_beta graycal_0_beta;
extern Graycalcudef_beta graycal_1_beta;

u8 TIM4_mode;
int TIM4mode1_count;
u8 CON_grayrequest;
int CON_direction;
u8 CON_cross;

void Control_Set_PID(u8 direction , float goal){
	if(direction == 0) pidSetPoint(&PIDf, goal);
	else if (direction == 1) pidSetPoint(&PIDr, goal);
	else if (direction == 2) pidSetPoint(&PIDb, goal);
	else pidSetPoint(&PIDl, goal);
}

void Control_Set_PID_para(u8 direction, float pp, float ii, float dd){
	if(direction == 0) pidSetpara(&PIDf, pp, ii, dd);
	else if (direction == 1)  pidSetpara(&PIDr, pp, ii, dd);
	else if (direction == 2)  pidSetpara(&PIDb, pp, ii, dd);
	else pidSetpara(&PIDl, pp, ii, dd);
}
void Control_Reset_PID(){
	resetPID(&PIDf);
	resetPID(&PIDb);
	resetPID(&PIDl);
	resetPID(&PIDr);
	last_trueincre = 0;
	
}
void Control_pidInit(){
	
	pidInit(&PIDf);
	pidInit(&PIDb);
	pidInit(&PIDr);
	pidInit(&PIDl);
	
	pidSetPoint(&PIDf, 0.0);
	pidSetPoint(&PIDb, 0.0);
	pidSetPoint(&PIDl, 0.5);
	pidSetPoint(&PIDr, 0.5);
	
	pidSetpara(&PIDf, 0.65, 0.0, 0.1);//对应基础250  上下限150 400 （2.5） 对应基础500 上下限制 200-700 
	pidSetpara(&PIDb,0.6, 0.0, 0.1);
	//11.8V(0.2 0.0 0.25)
//	pidSetpara(&PIDl, 2.6, 0.0, 0.5);
//	pidSetpara(&PIDr, 2.6, 0.0, 0.5);
	pidSetpara(&PIDl, 0.5, 0.0, 0.8);//对应基础250  上下限150 400 （2.5） 对应基础500 上下限制 200-700 
	pidSetpara(&PIDr,0.5, 0.0, 0.8);
	
}

void Control_pwmInit(){
	TIM3_PWM_Init(PWM_ORIGIN,35);
//	Control_Stop();
	PWMstraightInit(STRAIGHT_BASIC_SPEED_0_0, STRAIGHT_BASIC_SPEED_0_1, STRAIGHT_BASIC_SPEED_0_2, STRAIGHT_BASIC_SPEED_0_3);
	PWMrotateInit(ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED);
	PBout((5)) = 1;
	PBout((6)) = 1;
	PBout((7)) = 1;
	PBout((4)) = 1;
}

void Control_pwmSet(u8 straightmode, u8 direction, u8 mode){
	//staight
	if(straightmode == 1){
		if(direction == 0)	PWMstraightSet(STRAIGHT_BASIC_SPEED_0_0, STRAIGHT_BASIC_SPEED_0_1, STRAIGHT_BASIC_SPEED_0_2, STRAIGHT_BASIC_SPEED_0_3);
		else if(direction == 1) PWMstraightSet(STRAIGHT_BASIC_SPEED_1_0, STRAIGHT_BASIC_SPEED_1_1, STRAIGHT_BASIC_SPEED_1_2, STRAIGHT_BASIC_SPEED_1_3);
		else if(direction == 2) PWMstraightSet(STRAIGHT_BASIC_SPEED_2_0, STRAIGHT_BASIC_SPEED_2_1, STRAIGHT_BASIC_SPEED_2_2, STRAIGHT_BASIC_SPEED_2_3);	
		else  PWMstraightSet(STRAIGHT_BASIC_SPEED_3_0, STRAIGHT_BASIC_SPEED_3_1, STRAIGHT_BASIC_SPEED_3_2, STRAIGHT_BASIC_SPEED_3_3);	
	}
	else{
		
	}
}
void Control_grayInit(){
	Gray_Init();
}



void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4,DISABLE);  //失能TIMx					 
}


//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			if(TIM4_mode == 1){ 
		TIM4mode1_count++;
	}
	CON_cross = Control_Straight(CON_grayrequest,CON_direction);
		
		}
	
}

void Control_Straight_only(int direction){
	int adjustment[4] = {0,0,0,0};
	straight_only(direction,adjustment);
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
				if (graycal_0.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDf, PIDf.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * PIDf.proportion * 90;
					else incre = -5 * 90 * PIDf.proportion;
					
				}
				//没有信息
				else{
					incre = increpid(&PIDf, graycal_0.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//backward
			else {
				//无效
				if(graycal_0.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDb, PIDb.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * 90 * PIDb.proportion;
					else incre = -5 * 90 * PIDb.proportion;
					
				}
				else{
					incre = increpid(&PIDb, graycal_0.center) * 90;	
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
				u8 cam = 0;
				if(cam == 0){
					if(graycal_2.invalid && graycal_3.invalid){
						valid_increnum ++;
						if(valid_increnum <= 3) incre = increpid(&PIDf, PIDf.last_error) * 95;
						else if (valid_increnum > 3 && last_trueincre > 0 )  incre = 2.5 * 95;
						else  incre = -2.5 * 95; 
						}
					else if(graycal_2.invalid)	{incre = increpid(&PIDf, graycal_3.center) * 95; last_trueincre = incre;  valid_increnum = 0;}
					else if(graycal_3.invalid) { incre = increpid(&PIDf, graycal_2.center) * 95; last_trueincre = incre; valid_increnum = 0;}
					else  { incre = increpid(&PIDf, (graycal_2.center + graycal_3.center) / 2.0) * 95; last_trueincre = incre; valid_increnum = 0;}
				}
				else{
					//向树莓派索取摄像信息进行PID；
				}
			}
			//backward
			else{
				u8 cam = 0;
				if(cam == 0){
					if(graycal_2.invalid && graycal_3.invalid){
						valid_increnum ++;
						if(valid_increnum <= 3) incre = increpid(&PIDb, PIDb.last_error) * 95;
						else if (valid_increnum > 3 && last_trueincre > 0 )  incre = 2.5 * 95; 
						else incre = -2.5 * 95;
						}
					else if(graycal_2.invalid)	{incre = increpid(&PIDb, graycal_3.center) * 95; last_trueincre = incre;  valid_increnum = 0; }
					else if(graycal_3.invalid) {incre = increpid(&PIDb, graycal_2.center) * 95; last_trueincre = incre;  valid_increnum = 0; }
					else {incre = increpid(&PIDb, (graycal_2.center + graycal_3.center) / 2.0) * 95; last_trueincre = incre; valid_increnum = 0; }
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
				if (graycal_1.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3)  incre = increpid(&PIDr, PIDr.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * 90 * PIDr.proportion;
					else incre = -5 * 90 * PIDr.proportion;
				}
				//没有重复信息
				else{
					incre = increpid(&PIDr, graycal_1.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//左
			else {
				if (graycal_1.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDl, PIDl.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * 90 * PIDl.proportion;
					else incre = -5 * 90 * PIDl.proportion;
				}
				//没有重复信息
				else{
					incre = increpid(&PIDl, graycal_1.center) * 90;	
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
u8 Control_Straight_beta(u8 grayrequest,u8 graymode, int direction){
	int incre = 0;
	int adjustment[4] = {0,0,0,0};
	//forward or backward
	if (direction == 0 || direction == 2){
		//正常直走
			Read_Gray_beta(grayrequest, graymode);
			//forward
			if(direction == 0) {
				if (graycal_0_beta.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDf, PIDf.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
					
				}
				//valid
				else{
					incre = increpid(&PIDf, graycal_0_beta.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//backward
			else {
				//无效
				if(graycal_0_beta.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDb, PIDb.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
					
				}
				else{
					incre = increpid(&PIDb, graycal_0_beta.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			adjustment[0] = -incre;
		  adjustment[1] = incre;
			straight(direction,adjustment);
			if (graycal_0_beta.blacknum > 3) return 1;
			return 0;
		}
	//right or left 
  else{
			Read_Gray_beta(grayrequest,graymode);
			//右
			if (direction == 1) {
				if (graycal_1_beta.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
				}
				//没有重复信息
				else{
					incre = increpid(&PIDr, graycal_1_beta.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//左
			else {
				if (graycal_1_beta.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
				}
				//没有重复信息
				else{
					incre = increpid(&PIDl, graycal_1_beta.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
				
			}
				
			adjustment[3] = incre;
			adjustment[2] = -incre;
			straight(direction,adjustment);
			if(graycal_1_beta.blacknum > 3) return 1;
			return 0;
		}
	}

u8 Control_Rotate(int direction, int angle){
	rotate(direction);
	if(angle > 1000){
		delay_ms(1000);
		angle -= 1000;
	}
	delay_ms(angle);
	return 0;
}


void Control_to_plot(){
	
}



void Control_test(int direction){
	int adjustment[4] = {0, 0, 0, 0};
	straight_only(direction,adjustment);
}


void Control_Stop(){
	Control_PID_Stop();
	move_stop();
}

void Control_Stop_only(){
	Control_PID_Stop();
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

void Control_PID_Begin(){
	Control_Reset_PID();
	TIM_Cmd(TIM4,ENABLE);
}

void Control_PID_Stop(){
	TIM_Cmd(TIM4, DISABLE);
}

u8 Control_Cal_Gray(u8 grayrequest,int direction){
	u8 gray[4] = {0,0,0,0};
	//forward or backward
	if (direction == 0 || direction == 2){
		//正常直走
		if(grayrequest == 0){
			gray[0] = 1;
			Read_Gray(gray);
			//forward
		
			//backward
			if (graycal_0.cross) return 1;
			return 0;
		}
		//双线直走
		else if (grayrequest == 2){	
		  gray[2] = 1;
			gray[3] = 1;
			gray[0] = 1;
			Read_Gray(gray);
			if(graycal_0.cross) return 1;
			return 0;
		}
	}
	//right or left 
  else{
		if(grayrequest == 1){
			gray[1] = 1;
			Read_Gray(gray);
			if(graycal_1.cross) return 1;
			return 0;
		}
	}
	return 0;
}
void Control_Begin_Only(u8 direction, int* adjustment){
	straight_only(direction, adjustment);
}

void Control_rotate_beta(int direction, int angle){
	if(direction == 1){
 	  rotate_beta(direction);
	  if(angle >= 1000){
		  delay_ms(1000);
		  angle -= 1000;
	  }
	  delay_ms(angle);
  }
	else{
		int iscross_count = 0;
		while(1){
			rotate_beta(direction);
			Control_Cal_Gray(0, 0);
			if(graycal_0.maxlength >= 2){
				iscross_count ++ ;
				if(iscross_count > 2){
					return;
				}
			}
			else iscross_count = 0;
		}
	}
}
