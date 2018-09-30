#include "led.h"
#include "pwm.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTE精英STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

int rotate_basic[4] = {400, 400, 400, 400};
int straight_basic[4] = {800, 800, 800, 800};
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		LED1=!LED1;
		}
}



//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO

	GPIO_InitStructure.GPIO_Pin = M1_PIN | M2_PIN | M3_PIN | M4_PIN; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(M1_GROUP, &GPIO_InitStructure);//初始化GPIO
 	GPIO_Init(M2_GROUP, &GPIO_InitStructure);
 	GPIO_Init(M3_GROUP, &GPIO_InitStructure);
 	GPIO_Init(M4_GROUP, &GPIO_InitStructure);
 	
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

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
