#include "adapter.h"

void stop(void)
{
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_ResetBits(GPIOE,GPIO_Pin_13);
	GPIO_ResetBits(GPIOE,GPIO_Pin_14);

}

void adapter_GPIO_init(){
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		//电缸的io线
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.0 1 4 5 6 7 8 11
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.0 1 4 5 6 7 8 11
	

	
}
void forward(int n){
	switch(n){
		case 1:
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			break;
		case 2:
			GPIO_SetBits(GPIOE,GPIO_Pin_13);
			GPIO_ResetBits(GPIOE,GPIO_Pin_14);
			break;
		default:
			break;
	}
	
}

void backward(int n){
	switch(n){
		case 1:
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			break;
		case 2:
			GPIO_ResetBits(GPIOE,GPIO_Pin_13);
			GPIO_SetBits(GPIOE,GPIO_Pin_14);
			break;
		default:
			break;
	}
	
}

