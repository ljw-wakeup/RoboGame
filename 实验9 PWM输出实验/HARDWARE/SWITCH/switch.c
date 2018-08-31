#include "timer.h"
#include "led.h"
#include "usart.h"
#include "switch.h"
void switch_GPIO_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
}
