#include "MG.h"
#define TIME 100
#define internal 0x0008

void MG_GPIO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	

	//舵机与步进电机的IO线
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0 1 4 5 6 7 8 9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0 1 4 5 6 7 8 9

}
void move_mg1(u16 a,u16 b){
	if(a>b){
		u16 Old=a,New=b;
		while(Old>New){
		TIM_SetCompare1(TIM3,Old);
			delay_ms(TIME);
			Old-=0x0002;
		}
	}
	else if(a<b){
		u16 Old=a,New=b;
		while(Old<New){
		TIM_SetCompare1(TIM3,Old);
			delay_ms(TIME);
			Old+=0x0002;
		}
	}
	TIM_SetCompare1(TIM3,b);
}

void move_mg2(u16 a,u16 b){
	if(a>b){
		u16 Old=a,New=b;
		while(Old>New){
		TIM_SetCompare2(TIM3,Old);
			delay_ms(TIME);
			Old-=0x0010;
		}
	}
	else if(a<b){
		u16 Old=a,New=b;
		while(Old<New){
		TIM_SetCompare2(TIM3,Old);
			delay_ms(TIME);
			Old+=0x0010;
		}
	}
	TIM_SetCompare2(TIM3,b);
}

void move_mg3(u16 a,u16 b){
	if(a>b){
		u16 Old=a,New=b;
		while(Old>New){
		TIM_SetCompare3(TIM3,Old);
			delay_ms(TIME);
			Old-=0x0010;
		}
	}
	else if(a<b){
		u16 Old=a,New=b;
		while(Old<New){
		TIM_SetCompare3(TIM3,Old);
			delay_ms(TIME);
			Old+=0x0010;
		}
	}
	TIM_SetCompare3(TIM3,b);
}

void move_mg4(u16 a,u16 b){
	if(a>b){
		u16 Old=a,New=b;
		while(Old>New){
		TIM_SetCompare4(TIM3,Old);
			delay_ms(TIME);
			Old-=0x0010;
		}
	}
	else if(a<b){
		u16 Old=a,New=b;
		while(Old<New){
		TIM_SetCompare4(TIM3,Old);
			delay_ms(TIME);
			Old+=0x0010;
		}
	}
	TIM_SetCompare4(TIM3,b);
}


