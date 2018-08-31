#include "h_stm.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化


void Left_catch(){
	while(!L_flag){
		USART_SendData(USART1, 'L');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'q');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	L_flag=0;
}

void Right_catch(){
	while(!R_flag){
		USART_SendData(USART1, 'R');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'q');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	R_flag=0;
}
void Put_falg(){
	while(!P_flag){
		USART_SendData(USART1, 'P');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'q');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	P_flag=0;
}
void Fly(){
	while(!F_flag){
		USART_SendData(USART1, 'F');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'q');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	F_flag=0;
}
void Read_rfid(){
	while(!G_flag){
		USART_SendData(USART1, 'G');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'q');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	G_flag=0;
}
void Back(){
	while(!B_flag){
		USART_SendData(USART1, 'B');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'q');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	B_flag=0;
}

void send_instruction(u8 instruction){
	switch(instruction){
		case 1:
			Left_catch();
			break;
		case 2:
			Right_catch();
			break;
		case 3:
			Put_falg();
			break;
		case 4:
			Fly();
			break;
		case 5:
			Read_rfid();
			break;
		case 6:
			Back();
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		default:
			break;
	}
}
