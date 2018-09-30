#include "led.h"
#include "pwm.h"
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

int rotate_basic[4] = {400, 400, 400, 400};
int straight_basic[4] = {800, 800, 800, 800};
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
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
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = M1_PIN | M2_PIN | M3_PIN | M4_PIN; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(M1_GROUP, &GPIO_InitStructure);//��ʼ��GPIO
 	GPIO_Init(M2_GROUP, &GPIO_InitStructure);
 	GPIO_Init(M3_GROUP, &GPIO_InitStructure);
 	GPIO_Init(M4_GROUP, &GPIO_InitStructure);
 	
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
	adjust(adjustment);
	if(direction % 2 == 0) {
		if (direction < 1) {
			GPIO_SetBits(M1_GROUP, M1_PIN);
			GPIO_SetBits(M2_GROUP, M2_PIN);
			TIM_SetCompare1(TIM3, straight_basic[0] + adjustment[0]);
      TIM_SetCompare2(TIM3, straight_basic[1] + adjustment[1]);
			
			if(adjustment[1] > 0){
				GPIO_ResetBits(M3_GROUP, M3_PIN);
			  GPIO_ResetBits(M4_GROUP, M4_PIN);
				TIM_SetCompare3(TIM3, PWM_ORIGIN - 350 - adjustment[1]* 0.5);
			  TIM_SetCompare4(TIM3, PWM_ORIGIN - 350 - adjustment[1] *0.5);
			}
			else if(adjustment[1] < 0){
				GPIO_SetBits(M3_GROUP, M3_PIN);
			  GPIO_SetBits(M4_GROUP, M4_PIN);
			  TIM_SetCompare3(TIM3, 350 - adjustment[1] * 0.5);
			  TIM_SetCompare4(TIM3, 350 - adjustment[1] * 0.5);
			}
			else {
				GPIO_ResetBits(M3_GROUP, M3_PIN);
    		TIM_SetCompare3(TIM3, PWM_ORIGIN+1);
    		GPIO_ResetBits(M4_GROUP, M4_PIN);
    		TIM_SetCompare4(TIM3, PWM_ORIGIN+1);
			}
		} 
		else {
			GPIO_ResetBits(M1_GROUP, M1_PIN);
			GPIO_ResetBits(M2_GROUP, M2_PIN);
			TIM_SetCompare1(TIM3, PWM_ORIGIN- straight_basic[0] - adjustment[0]);
			TIM_SetCompare2(TIM3, PWM_ORIGIN - straight_basic[1] - adjustment[1]);
			TIM_SetCompare4(TIM3, PWM_ORIGIN+1) ;
		  GPIO_ResetBits(M4_GROUP, M4_PIN);	
			if(adjustment[1] > 0){
				GPIO_ResetBits(M3_GROUP, M3_PIN);
//			  GPIO_ResetBits(M4_GROUP, M4_PIN);
				TIM_SetCompare3(TIM3, PWM_ORIGIN - 350 - adjustment[1]* 0.4);
//			  TIM_SetCompare4(TIM3, PWM_ORIGIN - 350 - adjustment[1] *0.5);
			}
			else if(adjustment[1] < 0){
				GPIO_SetBits(M3_GROUP, M3_PIN);
//			  GPIO_SetBits(M4_GROUP, M4_PIN);
			  TIM_SetCompare3(TIM3, 350 - adjustment[1] * 0.4);
//			  TIM_SetCompare4(TIM3, 350 - adjustment[1] * 0.5);
			}
      else{
				GPIO_ResetBits(M3_GROUP, M3_PIN);
    		TIM_SetCompare3(TIM3, PWM_ORIGIN+1);
    		GPIO_ResetBits(M4_GROUP, M4_PIN);
    		TIM_SetCompare4(TIM3, PWM_ORIGIN+1);
			}			
		}		
//		GPIO_ResetBits(M3_GROUP, M3_PIN);
//		TIM_SetCompare3(TIM3, PWM_ORIGIN+1);
//		GPIO_ResetBits(M4_GROUP, M4_PIN);
//		TIM_SetCompare4(TIM3, PWM_ORIGIN+1);
	} else {
		if (direction < 2) {
			GPIO_ResetBits(M3_GROUP, M3_PIN);
			GPIO_ResetBits(M4_GROUP, M4_PIN);
			TIM_SetCompare3(TIM3, PWM_ORIGIN - straight_basic[2] - adjustment[2]);
			TIM_SetCompare4(TIM3, PWM_ORIGIN - straight_basic[3] - adjustment[3]);
		
			TIM_SetCompare2(TIM3, PWM_ORIGIN+1) ;
		  GPIO_ResetBits(M2_GROUP, M2_PIN);	

			if(adjustment[3] > 0){
				GPIO_ResetBits(M1_GROUP, M1_PIN);
//			  GPIO_ResetBits(M2_GROUP, M2_PIN);
			  TIM_SetCompare1(TIM3, PWM_ORIGIN- 750 -  0.5 * adjustment[3]);
//			  TIM_SetCompare2(TIM3, PWM_ORIGIN - 350 -  0.5 * adjustment[3]);
			}
			else if(adjustment[3] < 0){
				GPIO_SetBits(M1_GROUP, M1_PIN);
//			  GPIO_SetBits(M2_GROUP, M2_PIN);
			  TIM_SetCompare1(TIM3, 750 -  0.5 *adjustment[3]);
//			  TIM_SetCompare2(TIM3, 350 -  adjustment[3]);
			}
			else{
				TIM_SetCompare1(TIM3, PWM_ORIGIN+1 );
		    GPIO_ResetBits(M1_GROUP, M1_PIN);
		    TIM_SetCompare2(TIM3, PWM_ORIGIN+1) ;
		    GPIO_ResetBits(M2_GROUP, M2_PIN);	
			}
		} 
		else {
			GPIO_SetBits(M3_GROUP, M3_PIN);
			GPIO_SetBits(M4_GROUP, M4_PIN);
			TIM_SetCompare3(TIM3, straight_basic[2] + adjustment[2]);
			TIM_SetCompare4(TIM3, straight_basic[3] + adjustment[3]);
		  GPIO_ResetBits(M1_GROUP, M1_PIN);
		  TIM_SetCompare1(TIM3, PWM_ORIGIN+1) ;
//			TIM_SetCompare3(TIM3, straight_basic[2]);
//    		TIM_SetCompare4(TIM3, straight_basic[3]);
			if(adjustment[3] > 0){
//				GPIO_ResetBits(M1_GROUP, M1_PIN);
			  GPIO_ResetBits(M2_GROUP, M2_PIN);
//			  TIM_SetCompare1(TIM3, PWM_ORIGIN- 750  -  adjustment[3]);
			  TIM_SetCompare2(TIM3, PWM_ORIGIN - 350 - 0.5 * adjustment[3]);
			}
			else if(adjustment[3] < 0){
//				GPIO_SetBits(M1_GROUP, M1_PIN);
			  GPIO_SetBits(M2_GROUP, M2_PIN);
//			  TIM_SetCompare1(TIM3, 750 - adjustment[3]);
			  TIM_SetCompare2(TIM3, 350 - 0.5 * adjustment[3]);
			}
			else{
				TIM_SetCompare1(TIM3, PWM_ORIGIN+1 );
		    GPIO_ResetBits(M1_GROUP, M1_PIN);
		    TIM_SetCompare2(TIM3, PWM_ORIGIN+1) ;
		    GPIO_ResetBits(M2_GROUP, M2_PIN);	
			}
			
		}
//		TIM_SetCompare1(TIM3, PWM_ORIGIN+1 );
//		GPIO_ResetBits(M1_GROUP, M1_PIN);
//		TIM_SetCompare2(TIM3, PWM_ORIGIN+1) ;
//		GPIO_ResetBits(M2_GROUP, M2_PIN);	
	}
}

void rotate(int direction) {

	if (direction > 0) {
		GPIO_ResetBits(M1_GROUP, M1_PIN);
		GPIO_SetBits(M2_GROUP, M2_PIN);
		GPIO_SetBits(M3_GROUP, M3_PIN);
		GPIO_ResetBits(M4_GROUP, M4_PIN);
		TIM_SetCompare1(TIM3, PWM_ORIGIN - rotate_basic[0]);
		TIM_SetCompare2(TIM3, rotate_basic[1]);
		TIM_SetCompare3(TIM3, rotate_basic[2]);
		TIM_SetCompare4(TIM3, PWM_ORIGIN - rotate_basic[3]);
	} else {
		GPIO_SetBits(M1_GROUP, M1_PIN);
		GPIO_ResetBits(M2_GROUP, M2_PIN);
		GPIO_ResetBits(M3_GROUP, M3_PIN);
		GPIO_SetBits(M4_GROUP, M4_PIN);
		TIM_SetCompare1(TIM3, rotate_basic[0]);
		TIM_SetCompare2(TIM3, PWM_ORIGIN - rotate_basic[1]);
		TIM_SetCompare3(TIM3, PWM_ORIGIN - rotate_basic[2]);
		TIM_SetCompare4(TIM3, rotate_basic[3]);
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
	  GPIO_ResetBits(M1_GROUP, M1_PIN);
		GPIO_ResetBits(M2_GROUP, M2_PIN);
		GPIO_ResetBits(M3_GROUP, M3_PIN);
		GPIO_ResetBits(M4_GROUP, M4_PIN);
		TIM_SetCompare1(TIM3, PWM_ORIGIN + 1);
		TIM_SetCompare2(TIM3, PWM_ORIGIN + 1);
		TIM_SetCompare3(TIM3, PWM_ORIGIN + 1);
		TIM_SetCompare4(TIM3, PWM_ORIGIN + 1);
}

void adjust(int* adjustment){
	int i ;
	for(i = 0; i < 4; i++){
		if(adjustment[i]< 0 ) adjustment[i] = 1 * adjustment[i] ;
		if(adjustment[i] + straight_basic[i] > PWM_ORIGIN - (int)(0.05 * PWM_ORIGIN)) adjustment[i] = PWM_ORIGIN - straight_basic[i] - (int)(0.05 * PWM_ORIGIN);
		else if(adjustment[i] + straight_basic[i] < (int)(0.5 * PWM_ORIGIN)) adjustment[i] = -straight_basic[i] + (int)(0.5 * PWM_ORIGIN);
	}
}
