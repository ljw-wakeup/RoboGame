#include "led.h"
#include "pwm.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTE��ӢSTM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

int rotate_basic[4] = {0, 0, 0, 0};
int straight_basic[4] = {0, 0, 0, 0};
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		LED1=!LED1;
		}
}



//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO  | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = M1_1_PIN | M2_1_PIN | M4_2_PIN | M2_2_PIN | M3_2_PIN| M1_2_PIN; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin =  M3_1_PIN |M3_2_PIN; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin =  M4_1_PIN ; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5|GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_4; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
 	
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}


void straight(int direction, int adjustment[4]) {
	
	if(direction % 2 == 0) {
		if (direction < 1) {
			GPIO_SetBits(M1_1_GROUP, M1_1_PIN);
			GPIO_ResetBits(M1_2_GROUP, M1_2_PIN);
			GPIO_SetBits(M2_1_GROUP, M2_1_PIN);
			GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);
		  GPIO_SetBits(M3_1_GROUP, M3_1_PIN);	
			GPIO_SetBits(M3_2_GROUP, M3_2_PIN);	
			if(adjustment[1] >= 87){
			  GPIO_ResetBits(M4_1_GROUP, M4_1_PIN);
				GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
			  TIM_SetCompare4(TIM3, PWM_ORIGIN-120);
			}
			else if(adjustment[1] <= -87 ){
			  GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
				GPIO_ResetBits(M4_2_GROUP, M4_2_PIN);
			  TIM_SetCompare4(TIM3, PWM_ORIGIN-120);
			}
			else {
				 TIM_SetCompare4(TIM3, PWM_ORIGIN+100);
    		GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
    		GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
			}
			adjust(adjustment,direction);
			TIM_SetCompare1(TIM3, PWM_ORIGIN - (straight_basic[0] + adjustment[0]));
      TIM_SetCompare2(TIM3, PWM_ORIGIN-(straight_basic[1] + adjustment[1]));
		} 
		else {
			GPIO_ResetBits(M1_1_GROUP, M1_1_PIN);
			GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
			GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);
			GPIO_SetBits(M2_2_GROUP, M2_2_PIN);
		  GPIO_SetBits(M4_1_GROUP, M4_1_PIN);	
			GPIO_SetBits(M4_2_GROUP, M4_2_PIN);	
			if(adjustment[1] > 87){
				GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
  		  GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
				TIM_SetCompare3(TIM3, PWM_ORIGIN - (120));
			}
			else if(adjustment[1] < -87){
				GPIO_SetBits(M3_1_GROUP, M3_1_PIN);
  		  GPIO_ResetBits(M3_2_GROUP, M3_2_PIN);
			  TIM_SetCompare3(TIM3, PWM_ORIGIN - (120));
			}
      else{
				TIM_SetCompare3(TIM3, PWM_ORIGIN + (115));
				GPIO_SetBits(M3_1_GROUP, M3_1_PIN);
    		GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
			}
			adjust(adjustment,direction);
			TIM_SetCompare1(TIM3, PWM_ORIGIN-(straight_basic[0] + adjustment[0]));
      TIM_SetCompare2(TIM3, PWM_ORIGIN- (straight_basic[1] + adjustment[1]));
		}		
	} 
	else {
		if (direction < 2) {
			GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
			GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
			GPIO_ResetBits(M4_1_GROUP, M4_1_PIN);
			GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
		  GPIO_SetBits(M2_1_GROUP, M2_1_PIN);	
			GPIO_SetBits(M2_2_GROUP, M2_2_PIN);	
      
			if(adjustment[3] > 60){//200
				GPIO_ResetBits(M1_1_GROUP, M1_1_PIN);
				GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
			  TIM_SetCompare1(TIM3, PWM_ORIGIN - (115));

			}
			else if(adjustment[3] < -60){//200
				GPIO_SetBits(M1_1_GROUP, M1_1_PIN);
				GPIO_ResetBits(M1_2_GROUP, M1_2_PIN);
			  TIM_SetCompare1(TIM3, PWM_ORIGIN - (115));
			}
			else{
				
				TIM_SetCompare1(TIM3, PWM_ORIGIN + (115));
				 GPIO_SetBits(M1_1_GROUP, M1_1_PIN);	
			   GPIO_SetBits(M1_2_GROUP, M1_2_PIN);	
			}
      adjust(adjustment,direction);			
			TIM_SetCompare3(TIM3, PWM_ORIGIN - (straight_basic[2] + adjustment[2]));
			TIM_SetCompare4(TIM3, PWM_ORIGIN - (straight_basic[3] + adjustment[3]));
			
		} 
		else {			
			GPIO_SetBits(M3_1_GROUP, M3_1_PIN);
			GPIO_ResetBits(M3_2_GROUP, M3_2_PIN);
			GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
			GPIO_ResetBits(M4_2_GROUP, M4_2_PIN);
			
		  GPIO_SetBits(M1_1_GROUP, M1_1_PIN);	
			GPIO_SetBits(M1_2_GROUP, M1_2_PIN);	
      
			if(adjustment[3] > 60){//200
			  GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);
				GPIO_SetBits(M2_2_GROUP, M2_2_PIN);
			  TIM_SetCompare2(TIM3, PWM_ORIGIN - (115));
			}
			else if(adjustment[3] < -60){
			  GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);
				GPIO_SetBits(M2_1_GROUP, M2_1_PIN); 
			  TIM_SetCompare2(TIM3, PWM_ORIGIN - (115));
			}
			else{
			  TIM_SetCompare2(TIM3, PWM_ORIGIN + (115));
				 GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);	
			   GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);	
			}
			adjust(adjustment,direction);
			TIM_SetCompare3(TIM3, PWM_ORIGIN - (straight_basic[2] + adjustment[2]));
			TIM_SetCompare4(TIM3, PWM_ORIGIN - (straight_basic[3] + adjustment[3]));
			
		}
	}
}

void straight_only(int direction, int adjustment[4]) {
  adjust(adjustment,direction);
	if(direction % 2 == 0) {
		if (direction < 1) {
			GPIO_SetBits(M1_1_GROUP, M1_1_PIN);
			GPIO_ResetBits(M1_2_GROUP, M1_2_PIN);
			GPIO_SetBits(M2_1_GROUP, M2_1_PIN);
			GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);
			TIM_SetCompare1(TIM3, PWM_ORIGIN - (straight_basic[0] + adjustment[0]));
      TIM_SetCompare2(TIM3, PWM_ORIGIN - (straight_basic[1] + adjustment[1]));
		  GPIO_SetBits(M3_1_GROUP, M3_1_PIN);	
			GPIO_SetBits(M3_2_GROUP, M3_2_PIN);	
			GPIO_SetBits(M4_1_GROUP, M4_1_PIN);	
			GPIO_SetBits(M4_2_GROUP, M4_2_PIN);	
		} 
		else {
			GPIO_ResetBits(M1_1_GROUP, M1_1_PIN);
			GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
			GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);
			GPIO_SetBits(M2_2_GROUP, M2_2_PIN);
			TIM_SetCompare1(TIM3, PWM_ORIGIN - (straight_basic[0] + adjustment[0]));
      TIM_SetCompare2(TIM3, PWM_ORIGIN - (straight_basic[1] + adjustment[1]));
			GPIO_SetBits(M3_1_GROUP, M3_1_PIN);	
			GPIO_SetBits(M3_2_GROUP, M3_2_PIN);	
		  GPIO_SetBits(M4_1_GROUP, M4_1_PIN);	
			GPIO_SetBits(M4_2_GROUP, M4_2_PIN);	
	  }
	}		
		else {
		if (direction < 2) {
			GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
			GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
			GPIO_ResetBits(M4_1_GROUP, M4_1_PIN);
			GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
			TIM_SetCompare3(TIM3, PWM_ORIGIN - (straight_basic[2] + adjustment[2]));
			TIM_SetCompare4(TIM3, PWM_ORIGIN - (straight_basic[3] + adjustment[3]));
		  GPIO_SetBits(M2_1_GROUP, M2_1_PIN);	
			GPIO_SetBits(M2_2_GROUP, M2_2_PIN);	
			GPIO_SetBits(M1_1_GROUP, M1_1_PIN);	
			GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
		} 
		else {
			GPIO_SetBits(M3_1_GROUP, M3_1_PIN);
			GPIO_ResetBits(M3_2_GROUP, M3_2_PIN);
			GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
			GPIO_ResetBits(M4_2_GROUP, M4_2_PIN);
			TIM_SetCompare3(TIM3, PWM_ORIGIN - (straight_basic[2] + adjustment[2]));
			TIM_SetCompare4(TIM3, PWM_ORIGIN - (straight_basic[3] + adjustment[3]));
		  GPIO_SetBits(M1_1_GROUP, M1_1_PIN);	
			GPIO_SetBits(M1_2_GROUP, M1_2_PIN);	
			GPIO_SetBits(M2_1_GROUP, M2_1_PIN);	
			GPIO_SetBits(M2_2_GROUP, M2_2_PIN);	
		}
	}
}

void rotate(int direction) {

	if (direction > 0) {
		GPIO_ResetBits(M1_1_GROUP, M1_1_PIN);
		GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
		GPIO_SetBits(M2_1_GROUP, M2_1_PIN);
		GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);
		GPIO_SetBits(M3_1_GROUP, M3_1_PIN);
		GPIO_ResetBits(M3_2_GROUP, M3_2_PIN);
		GPIO_ResetBits(M4_1_GROUP, M4_1_PIN);
		GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
		TIM_SetCompare1(TIM3, PWM_ORIGIN - rotate_basic[0]);
		TIM_SetCompare2(TIM3, PWM_ORIGIN - rotate_basic[1]);
		TIM_SetCompare3(TIM3, PWM_ORIGIN - rotate_basic[2]);
		TIM_SetCompare4(TIM3, PWM_ORIGIN - rotate_basic[3]);
	} else {
		GPIO_ResetBits(M1_2_GROUP, M1_2_PIN);
		GPIO_SetBits(M1_1_GROUP, M1_1_PIN);
		GPIO_SetBits(M2_2_GROUP, M2_2_PIN);
		GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);
		GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
		GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
		GPIO_ResetBits(M4_2_GROUP, M4_2_PIN);
		GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
		TIM_SetCompare1(TIM3, PWM_ORIGIN - rotate_basic[0]);
		TIM_SetCompare2(TIM3, PWM_ORIGIN - rotate_basic[1]);
		TIM_SetCompare3(TIM3, PWM_ORIGIN - rotate_basic[2]);
		TIM_SetCompare4(TIM3, PWM_ORIGIN - rotate_basic[3]);
	}
}

void rotate_beta(int direction){
	if (direction > 0) {
		GPIO_ResetBits(M1_2_GROUP, M1_2_PIN);
		GPIO_SetBits(M1_1_GROUP, M1_1_PIN);
		GPIO_SetBits(M2_1_GROUP, M2_1_PIN);
		GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);
		GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
		GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
		GPIO_ResetBits(M4_1_GROUP, M4_1_PIN);
		GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
		TIM_SetCompare1(TIM3,PWM_ORIGIN - 250);
		TIM_SetCompare2(TIM3, PWM_ORIGIN - 250);
		TIM_SetCompare3(TIM3, PWM_ORIGIN - 150);
		TIM_SetCompare4(TIM3, PWM_ORIGIN - 250);
	} else {
		GPIO_ResetBits(M1_1_GROUP, M1_1_PIN);
		GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
		GPIO_SetBits(M2_2_GROUP, M2_2_PIN);
		GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);
		GPIO_SetBits(M3_2_GROUP, M3_2_PIN);
		GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
		GPIO_ResetBits(M4_2_GROUP, M4_2_PIN);
		GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
		TIM_SetCompare1(TIM3, PWM_ORIGIN - 150);
		TIM_SetCompare2(TIM3, PWM_ORIGIN - 250);
		TIM_SetCompare3(TIM3, PWM_ORIGIN - 250);
		TIM_SetCompare4(TIM3, PWM_ORIGIN - 250);
	}
}

void PWMstraightInit(int k1, int k2, int k3, int k4){
	straight_basic[0] = k1;
	straight_basic[1] = k2;
	straight_basic[2] = k3;
	straight_basic[3] = k4;	
}

void PWMrotateInit(int k1, int k2, int k3, int k4){
	rotate_basic[0] = k1;
	rotate_basic[1] = k2;
	rotate_basic[2] = k3;
	rotate_basic[3] = k4;
}

void PWMstraightSet(int k1, int k2, int k3, int k4){
	straight_basic[0] = k1;
	straight_basic[1] = k2;
	straight_basic[2] = k3;
	straight_basic[3] = k4;	
}

void PWMrotateSet( int k1, int k2, int k3, int k4){
	rotate_basic[0] = k1;
	rotate_basic[1] = k2;
	rotate_basic[2] = k3;
	rotate_basic[3] = k4;	
}
void move_stop() {
	  GPIO_ResetBits(M1_1_GROUP, M1_1_PIN);
	  GPIO_ResetBits(M1_2_GROUP, M1_2_PIN);
		GPIO_ResetBits(M2_1_GROUP, M2_1_PIN);
  	GPIO_ResetBits(M2_2_GROUP, M2_2_PIN);
		GPIO_ResetBits(M3_1_GROUP, M3_1_PIN);
	  GPIO_ResetBits(M3_2_GROUP, M3_2_PIN); 
		GPIO_ResetBits(M4_1_GROUP, M4_1_PIN);
  	GPIO_ResetBits(M4_2_GROUP, M4_2_PIN);
	  delay_ms(100);
	  GPIO_SetBits(M1_1_GROUP, M1_1_PIN);
	  GPIO_SetBits(M1_2_GROUP, M1_2_PIN);
		GPIO_SetBits(M2_1_GROUP, M2_1_PIN);
  	GPIO_SetBits(M2_2_GROUP, M2_2_PIN);
		GPIO_SetBits(M3_1_GROUP, M3_1_PIN);
	  GPIO_SetBits(M3_2_GROUP, M3_2_PIN); 
		GPIO_SetBits(M4_1_GROUP, M4_1_PIN);
  	GPIO_SetBits(M4_2_GROUP, M4_2_PIN);
	  
}

void adjust(int* adjustment,int direction){
	int i ;
	if(direction==0||direction==2){
	for(i = 0; i < 4; i++){
		if(adjustment[i] + straight_basic[i] > PWM_ORIGIN - (int)(0.4 * PWM_ORIGIN)) adjustment[i] = PWM_ORIGIN - straight_basic[i] - (int)(0.4* PWM_ORIGIN);
		else if(adjustment[i] + straight_basic[i] < (int)(0.15* PWM_ORIGIN)) adjustment[i] = -straight_basic[i] + (int)(0.15* PWM_ORIGIN);
	}
   }
	else{
		for(i = 0; i < 4; i++){
		if(adjustment[i] + straight_basic[i] > PWM_ORIGIN - (int)(0.4* PWM_ORIGIN)) adjustment[i] = PWM_ORIGIN - straight_basic[i] - (int)(0.4* PWM_ORIGIN);
		else if(adjustment[i] + straight_basic[i] < (int)(0.15* PWM_ORIGIN)) adjustment[i] = -straight_basic[i] + (int)(0.15* PWM_ORIGIN);//0.2
	}
  }
}
