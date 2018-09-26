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

extern u8 TIM4_mode;
extern u8 TIM4mode1_count;
u8 CON_grayrequest;
int CON_direction;
u8 CON_cross;


void Control_pidInit(){
	
	pidInit(&PIDf);
	pidInit(&PIDb);
	pidInit(&PIDr);
	pidInit(&PIDl);
	
	pidSetPoint(&PIDf, 0.0);
	pidSetPoint(&PIDb, 0.0);
	pidSetPoint(&PIDl, 1);
	pidSetPoint(&PIDr, 1);
	
	pidSetpara(&PIDf, 4, 0, 0);
	pidSetpara(&PIDb, 2.5, 0, 5);
	pidSetpara(&PIDl, 2.5, 0, 5);
	pidSetpara(&PIDr, 2.5, 0, 5);
	
}

void Control_pwmInit(){
	TIM3_PWM_Init(PWM_ORIGIN,0);
	PWMstraightInit(STRAIGHT_BASIC_SPEED, STRAIGHT_BASIC_SPEED, STRAIGHT_BASIC_SPEED, STRAIGHT_BASIC_SPEED);
	PWMrotateInit(ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED, ROTATE_BASIC_SPEED);
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
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
		}
	CON_cross = Control_Straight(CON_grayrequest,CON_direction);
		
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
					if(valid_increnum <= 3) incre = increpid(&PIDf, PIDf.last_error);
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
					
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
					if(valid_increnum <= 3) incre = increpid(&PIDb, PIDb.last_error);
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
					
				}
				else{
					incre = increpid(&PIDf, graycal_0.center) * 90;	
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
				if(graycal_2.maxlength > 1 || graycal_3.maxlength >1){
					if(graycal_2.invalid && graycal_3.invalid){
						valid_increnum ++;
						if(valid_increnum <= 3) incre = increpid(&PIDf, PIDf.last_error);
						else if (valid_increnum > 3 && last_trueincre > 0 )  incre = 1.5 * 20;
						else  incre = -1.5 * 20; 
						}
					else if(graycal_2.invalid)	{incre = increpid(&PIDf, graycal_3.center) * 20; valid_increnum = 0;}
					else if(graycal_3.invalid) { incre = increpid(&PIDf, graycal_2.center) * 20; valid_increnum = 0;}
					else  { incre = increpid(&PIDf, (graycal_2.center + graycal_3.center) / 2.0) * 20; valid_increnum = 0;}
				}
				else{
					//����ݮ����ȡ������Ϣ����PID��
				}
			}
			//backward
			else{
				if(graycal_2.maxlength >1 || graycal_3.maxlength > 1){
					if(graycal_2.invalid && graycal_3.invalid){
						valid_increnum ++;
						if(valid_increnum <= 3) incre = increpid(&PIDb, PIDb.last_error);
						else if (valid_increnum > 3 && last_trueincre > 0 )  incre = 1.5 * 90; 
						else incre = -1.5 * 20;
						}
					else if(graycal_2.invalid)	{incre = increpid(&PIDb, graycal_3.center) * 90;  valid_increnum = 0; }
					else if(graycal_3.invalid) {incre = increpid(&PIDb, graycal_2.center) * 90;  valid_increnum = 0; }
					else {incre = increpid(&PIDb, (graycal_2.center + graycal_3.center) / 2.0) * 90;  valid_increnum = 0; }
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
				if (graycal_1.repeat){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
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
				if (graycal_1.repeat){
					valid_increnum ++;
					if(valid_increnum <= 3) incre = last_trueincre;
					else if(valid_increnum>3 && last_trueincre >0) incre = 3 * 90;
					else incre = -3 * 90;
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

u8 Control_Rotate(int direction, int angle){
	rotate(direction);
	if(angle >= 1000){
		delay_ms(1000);
		angle -= 1000;
	}
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
		//10�ǵ��δ�,��������ֱ��ʱ��
		while(TIM4mode1_count<10){
			if(TIM4mode1_count == 5)
			//5Ҳ�ǵ��δ����������Ҫ���ٵ�ʱ��
				Control_changeSpeed(direction, STRAIGHT_SPEED_CHANGE);
		Control_Straight(gray_request,direction);
		}
		return 1;
}

void back_in_line(){
	u8 gray_request = 2;
	int direction = 2;
	u8 iscross;
	int cross_count;
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(iscross){
				cross_count ++;
				if(cross_count > 5){
					cross_count = 0;
					break;
				}
		}
		else cross_count = 0;
	}
}


void Control_test(){
	int adjustment[4] = {200, 200, 200, 200};
	straight(1,adjustment);
}

void Control_Stop(){
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
	resetPID(&PIDf);
	resetPID(&PIDb);
	resetPID(&PIDl);
	resetPID(&PIDr);
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
