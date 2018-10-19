#include "control.h"
#include "pid.h"
#include "pwm.h"
#include "gray.h"
#include "delay.h"
#include "timer.h"
///l///////////////////////////////////////////////////////////////////////////////	 
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
	
	pidSetpara(&PIDf, 0.65, 0.0, 0.1);//��Ӧ����250  ������150 400 ��2.5�� ��Ӧ����500 �������� 200-700 
	pidSetpara(&PIDb,0.6, 0.0, 0.1);
	//11.8V(0.2 0.0 0.25)
//	pidSetpara(&PIDl, 2.6, 0.0, 0.5);
//	pidSetpara(&PIDr, 2.6, 0.0, 0.5);
	pidSetpara(&PIDl, 0.5, 0.0, 0.8);//��Ӧ����250  ������150 400 ��2.5�� ��Ӧ����500 �������� 200-700 
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM4,DISABLE);  //ʧ��TIMx					 
}


//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
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
		//����ֱ��
		if(grayrequest == 0){
			gray[0] = 1;
			Read_Gray(gray);
			//forward
			if(direction == 0) {
				//����Ч��Ϣ
				if (graycal_0.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDf, PIDf.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * PIDf.proportion * 90;
					else incre = -5 * 90 * PIDf.proportion;
					
				}
				//û����Ϣ
				else{
					incre = increpid(&PIDf, graycal_0.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//backward
			else {
				//��Ч
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
		//˫��ֱ��
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
					//����ݮ����ȡ������Ϣ����PID��
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
					//����ݮ����ȡ����ͷ��Ϣ
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
			//��
			if (direction == 1) {
				if (graycal_1.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3)  incre = increpid(&PIDr, PIDr.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * 90 * PIDr.proportion;
					else incre = -5 * 90 * PIDr.proportion;
				}
				//û���ظ���Ϣ
				else{
					incre = increpid(&PIDr, graycal_1.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//��
			else {
				if (graycal_1.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = increpid(&PIDl, PIDl.last_error) * 90;
					else if(valid_increnum>3 && last_trueincre >0) incre = 5 * 90 * PIDl.proportion;
					else incre = -5 * 90 * PIDl.proportion;
				}
				//û���ظ���Ϣ
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
		//����ֱ��
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
				//��Ч
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
			//��
			if (direction == 1) {
				if (graycal_1_beta.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
				}
				//û���ظ���Ϣ
				else{
					incre = increpid(&PIDr, graycal_1_beta.center) * 90;	
					valid_increnum = 0;
					last_trueincre = incre;
				}
			}
			//��
			else {
				if (graycal_1_beta.invalid){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
				}
				//û���ظ���Ϣ
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
		//����ֱ��
		if(grayrequest == 0){
			gray[0] = 1;
			Read_Gray(gray);
			//forward
		
			//backward
			if (graycal_0.cross) return 1;
			return 0;
		}
		//˫��ֱ��
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
