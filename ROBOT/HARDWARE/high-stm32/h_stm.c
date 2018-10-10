#include "h_stm.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ�ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��


void Left_put(){
	L_flag=0;
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
	
}

void Right_put(){
		R_flag=0;
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

}
void waite_L(){
	Ld_flag=0;
	while(!Ld_flag){}
	
}

void waite_R(){
	Rd_flag=0;
	while(!Rd_flag){}
	
}
void Left_catch(void){
	L_catch=0;
	while(!L_catch){
		USART_SendData(USART1, 'L');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'c');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	waite_L();
}
void Right_catch(void){
	R_catch=0;
	while(!R_catch){
		USART_SendData(USART1, 'R');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'c');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	waite_R();
}
void waite_P(){
	Pd_flag=0;
	while(!Pd_flag){}
	
}
void waite_F(){
	Fd_flag=0;
	while(!Fd_flag){}
	
}
void Put_falg(){
	P_flag=0;
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
	waite_P();

	
}
void Fly(){
		F_flag=0;
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

	waite_F();

}
void Read_rfid(){
	G_flag=0;
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
	
}
void End(){
	end_flag=0;
	while(!end_flag){
		USART_SendData(USART1, 'e');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'n');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 'd');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
		USART_SendData(USART1, 0x0a);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
	}
	
}



void send_instruction(u8 instruction){
	switch(instruction){
		case 1:
			Left_put();
			break;
		case 2:
			Left_catch();
			break;
		case 3:
			Right_put();
			break;
		case 4:
			Right_catch();
			break;
		case 5:
			Put_falg();
			break;
		case 6:
			Fly();
			break;
		case 7:
			End();
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
