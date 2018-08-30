#include "ultrasonic.h"


u16 ultrasonic1=0;
u8 valid=0;
void TIM5_Configuration(u16 arr,u16 psc){
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


TIM_TimeBaseStructure.TIM_Prescaler = psc; //72M/72=1M 
TIM_TimeBaseStructure.TIM_Period = arr; //20ms overflow
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
/*load */
//TIM_ARRPreloadConfig(TIM1, ENABLE);
//clear IT bit
//TIM_ClearITPendingBit(TIM1,TIM_IT_Update); 
/* overflow interrupt*/
//TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
/* allow*/
TIM_Cmd(TIM5, ENABLE);
	
}

void ultrasonic_GPIO_init(void){//io口的配置
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

/* 
PE9 CH1 DJ // PE10 CH2 MD // PE11 CH6 adjust */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M
GPIO_Init(GPIOE, &GPIO_InitStructure);

GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource9);



EXTI_InitStructure.EXTI_Line = EXTI_Line9 ;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);



RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	//舵机与步进电机的IO线
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0 1 4 5 6 7 8 9

}

void ultrasonic_IRQ_init(void){//io口中断配置
	NVIC_InitTypeDef NVIC_InitStructure;

/* Configure one bit for preemption priority */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* PB6 PE5 interrupt */
NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //highest priority
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);


}

void EXTI9_5_IRQHandler(void){
	
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		
	EXTI_ClearITPendingBit(EXTI_Line9);
	
	//Delay(1);

	if(!(GPIOE->IDR & GPIO_Pin_9)&&valid){	// PE6=0 falling 
		u16 temp =TIM_GetCounter(TIM5);
		valid=0;
		if(ultrasonic1>temp)
		ultrasonic1=temp-ultrasonic1+20000;
		
		else 
			ultrasonic1=temp-ultrasonic1;
	}
	else{ //Rising
		valid=1;
 ultrasonic1=TIM_GetCounter(TIM5);
	}
	
	
	} 
}




	
void getultrasonic(void){

}
void trig_ultrasonic(void){
	/*
	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
	delay_us(20);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	*/
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);

	delay_us(60);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

}
