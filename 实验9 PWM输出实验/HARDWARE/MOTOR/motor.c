
#include "motor.h"
extern u16 motor1,motor2,motor3,motor4;//??????
void motor_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5|GPIO_Pin_9;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0 1 4 5 6 7 8 9
	
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_0;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0 1 4 5 6 7 8 9
	
	
}

void UP(void){
	GPIO_ResetBits(GPIOE,GPIO_Pin_1);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
	
}



void DOWN(void){
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	TIM_Cmd(TIM4, ENABLE);
}

void EN_motor(void){
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
}

void DIS_motor(void){
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
}

void stop_motor(void){
	TIM_Cmd(TIM4, DISABLE);  //??TIMx??
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
}
