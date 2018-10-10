#include "ultrasonic.h"


u32 ultrasonic1=0;
u32 temp1 = 0;
u8 valid=0;
u8 TIM5_mode= 0;//TIM5模式为0：
u8 TIM5_count = 0;
u8 Exti_count = 0;
u8 test = 1;
int testr = 0;
u8 ultra_flag = 0;

void TIM5_NVIC_Configuration(){
	
NVIC_InitTypeDef NVIC_InitStructure;
//中断优先级NVIC设置
NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

}
void TIM5_Configuration(u16 arr,u16 psc){
	
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE); //时钟使能

TIM_TimeBaseStructure.TIM_Prescaler = psc; //72M/72=1M 
TIM_TimeBaseStructure.TIM_Period = arr; //20ms overflow
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
TIM_ARRPreloadConfig(TIM5, ENABLE);     
TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
TIM5->CNT = 0;
//中断优先级NVIC设置


}




//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	//	LED1=!LED1;
		}

	if(	TIM5_mode == 0){
		TIM5_mode = 1;
	  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	  TIM5_Configuration(19999,71);
		TIM5_count =  ULTRASONIC_TRIGTIME;
	}
	else if( TIM5_mode == 1){
		if(TIM5_count == 0){
			TIM5_mode = 0;
			valid = 0;
			TIM5_count = ULTRASONIC_TRIGTIME;
			TIM5_Configuration(60, 71);
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		}
		else{
			TIM5_count --;
		}
	}

}

void TIM5_TEST_GPIO_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}




void ultrasonic_GPIO_init(void){//io口的配置

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);



EXTI_InitStructure.EXTI_Line = EXTI_Line4;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0 1 4 5 6 7 8 9

}

void ultrasonic_IRQ_init(void){//io口中断配置
	NVIC_InitTypeDef NVIC_InitStructure;

/* Configure one bit for preemption priority */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* PB6 PE5 interrupt */
NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //highest priority
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);


}

void EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){
		

	//Delay(1);
	
	if(!PAin(4)){	// PE6=0 falling 
		if(valid){
		u16 temp =TIM_GetCounter(TIM5);
		valid=0;	
		ultrasonic1 = (ULTRASONIC_TRIGTIME - TIM5_count) * 20000 + temp - temp1;
		TIM5_count = 0;
    ultra_flag = 1;
		}
	}
	else{ //Rising
		u16 a;
		a = TIM5->CNT;
		TIM5_count = ULTRASONIC_TRIGTIME; 
//		TIM_Cmd(TIM5, DISABLE);
		TIM5->CNT = 0;
		valid=1;
		temp1=TIM_GetCounter(TIM5);
	}
	
		EXTI_ClearITPendingBit(EXTI_Line4);
	
	} 
}




	
void get_ultrasonic(void){
	

}


void trig_ultrasonic(void){
	ultrasonic1 = 0;
	TIM5_mode = 0;
	TIM5_count = ULTRASONIC_TRIGTIME;
	TIM5_Configuration(60,71);
	TIM5_NVIC_Configuration();
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}

void stop_ultrasonic(void){
	TIM_Cmd(TIM5, DISABLE);
}
