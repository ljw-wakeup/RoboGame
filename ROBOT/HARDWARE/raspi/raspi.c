#include "raspi.h"
#define  TRY_TIM 5
u8 raspi_status;
u8 color_list[8];
u8 cam;


void push_quest(u8 para){
u8 i;
	i=0;
	while(raspi_status=='C'&&i<TRY_TIM){
	USART_SendData(USART2, 'C');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, 'q');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, para);	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, 0x0d);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	USART_SendData(USART2, 0x0a);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	}
	i=0;
	while(raspi_status=='S'&&i<TRY_TIM){
	USART_SendData(USART2, 'S');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, 'q');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, para);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);	
	USART_SendData(USART2, 0x0d);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	USART_SendData(USART2, 0x0a);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	}
	i=0;
	while(raspi_status=='d'&&i<TRY_TIM){
	USART_SendData(USART2, 'C');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, 'r');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, 0x0d);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	USART_SendData(USART2, 0x0a);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	i++;
	}
	i=0;
	while(raspi_status=='t'&&i<TRY_TIM){
	USART_SendData(USART2, 'S');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	USART_SendData(USART2, 'r');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
USART_SendData(USART2, 0x0d);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		
	USART_SendData(USART2, 0x0a);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);				
	i++;
	}


}

void connect_raspi(void )	
{
	u16 len; 
	u8 i;
if(USART2_RX_STA&0x8000)//
		{
			len = USART2_RX_STA&0x3fff;//	
			switch(raspi_status){
				case 'C':
					if(len==8) {
						raspi_status='d';
					for(i=0;i<len;i++){
					color_list[i]=USART2_RX_BUF[i];
					}
				}
				break;
				
				case 'S':
						if(len==1) {
						raspi_status='t';
					for(i=0;i<len;i++){
					cam=USART2_RX_BUF[i];
					}
				break;
						}
				}///end of switch
			 USART2_RX_STA=0;
}
}

void request_colorList(u8 area){
	raspi_status='C';
	push_quest(area);
	while(raspi_status=='C');
	///?color_list
}

void request_camDis(u8 area){
	raspi_status='S';
	push_quest(area);
	while(raspi_status=='S');
	///?cam
	
}
		
