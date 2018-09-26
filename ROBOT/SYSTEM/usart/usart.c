#include "sys.h"
#include "usart.h"
#include "raspi.h"
u16 num;
u8 flag,RFID;
u8 L_flag=0,R_flag=0,P_flag=0,F_flag,G_flag=0,B_flag=0,Ld_flag=0,Rd_flag=0;//?? ?? ?? ?? ??rfid ???????
extern u16 usart1_len,usart2_len;//??????
u8 b_flag=0,s_flag=0;
char information_all[50];

u16 usart1_len,usart2_len;

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ??	  
#endif
#if 1
#pragma import(__use_no_semihosting)             
//??????????                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//??_sys_exit()??????????    
_sys_exit(int x) 
{ 
	x = x; 
} 
//???fputc?? 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//????,??????   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*??microLib???*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //???????
//??1??????
//??,??USARTx->SR??????????   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //????,??USART_REC_LEN???.

//????
//bit15,	??????
//bit14,	???0x0d
//bit13~0,	??????????
u16 USART1_RX_STA=0;       //USART1??????	  

void Usart_SendString(USART_TypeDef* USARTx,char *str){
	while(*str){
		while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));
		USART_SendData(USARTx,*str);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); 
		str++;           
  }
}

void uart1_init(u32 bound){//???????
  //GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//??USART1,GPIOA??
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.9
   
  //USART1_RX	  GPIOA.10???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.10  

  //Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
   //USART ?????

	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

  USART_Init(USART1, &USART_InitStructure); //?????1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//????????
  USART_Cmd(USART1, ENABLE);                    //????1 

}

void uart2_init(u32 bound){//??rfid??
  //GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//??USART2,GPIOA??
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//USART1_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.2
   
  //USART1_RX	  GPIOA.10???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.3  

  //Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
   //USART ?????

	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

  USART_Init(USART2, &USART_InitStructure); //?????2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//????????
  USART_Cmd(USART2, ENABLE);                    //????2 

}

void USART1_IRQHandler(void)                	//??1??????
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//??SYSTEM_SUPPORT_OS??,?????OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
		{
		Res =USART_ReceiveData(USART1);	//????????
		
		if((USART1_RX_STA&0x8000)==0)//?????
			{
			if(USART1_RX_STA&0x4000)//????0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//????,????
				else USART1_RX_STA|=0x8000;	//????? 
				}
			else //????0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//??????,??????	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//??SYSTEM_SUPPORT_OS??,?????OS.
	OSIntExit();  											 
#endif
		 if((USART1_RX_STA&0x8000)==0x8000){
			 get_motor();
		 }
		
} 
#endif	


#if EN_USART2_RX
u16 USART2_RX_STA=0;       //USART2??????	 
u8 USART2_RX_BUF[USART2_REC_LEN];     //????,??USART_REC_LEN???.
extern u16 usart2_len;													//???????????????




void USART2_IRQHandler(void)                	//??1??????
	{
u8 Res;
#if SYSTEM_SUPPORT_OS 		//??SYSTEM_SUPPORT_OS??,?????OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
		{
		Res =USART_ReceiveData(USART2);	//????????
		
		if((USART2_RX_STA&0x8000)==0)//?????
			{
			if(USART2_RX_STA&0x4000)//????0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//????,????
				else USART2_RX_STA|=0x8000;	//????? 
				}
			else //????0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//??????,??????	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//??SYSTEM_SUPPORT_OS??,?????OS.
	OSIntExit();  											 
#endif
		 if (USART2_RX_STA&0x8000){
			 connect_raspi();
		 }
			 
} 

void uart_send_mydata(u8 *data,u8 len)
{
	int i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART2,data[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//??????
	}
}

void uart_receive_mydata(u8 *data,u8 *len)
{
	u8 i=0;
	*len = (USART2_RX_STA&0xFF);
	for(i=0;i<*len;i++)
	data[i]=USART2_RX_BUF[i];
	
}

void get_motor(void)
{
			
			usart1_len = USART1_RX_STA&0x3fff;//????????????	
			
			switch (USART1_RX_BUF[0]) {
							
				case 'L':
					if(USART1_RX_BUF[1]=='d'){
						if(!Ld_flag)
							Ld_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='L'&&USART1_RX_BUF[1]=='d'){		
						USART_SendData(USART1, 'L');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART_SendData(USART1, 'd');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART1_RX_STA=0;
						}
					}
					else if(USART1_RX_BUF[1]=='r'){
						if(!L_flag)
							L_flag=1;
						
						USART1_RX_STA=0;
					}
				
						
					
					break;
				case 'R':
					if(USART1_RX_BUF[1]=='d'){
						if(!Rd_flag)
							Rd_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='R'&&USART1_RX_BUF[1]=='d'){		
						USART_SendData(USART1, 'R');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART_SendData(USART1, 'd');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART1_RX_STA=0;
						}
					}
					else if(USART1_RX_BUF[1]=='r'){
						if(!R_flag)
							R_flag=1;
						USART1_RX_STA=0;
						
					}
					break;
				case 'P': 
					if(USART1_RX_BUF[1]=='r'){
						if(!P_flag)
							P_flag=1;
						USART1_RX_STA=0;
						
					}
					break;
				case 'F':
					if(USART1_RX_BUF[1]=='r'){
						F_flag=1;
						USART1_RX_STA=0;
						
					}
					break;
				case 'G':
					if(!USART1_RX_BUF[1]){
						RFID=USART1_RX_BUF[1];
					}
					else if(USART1_RX_BUF[1]=='r'){
						G_flag=1;
						USART1_RX_STA=0;
						
					}
					break;
				case 'a': //ask ??????		
				sprintf(information_all,"%d",1);
				Usart_SendString(USART1,information_all);
				USART_SendData(USART1, 0x0d0a);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
					break;
				
				case 'B':
					if(USART1_RX_BUF[1]=='r'){
						if(!B_flag)
							B_flag=1;
						USART1_RX_STA=0;
						
					}
					break;
					case 's'://??
					{
						s_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='s'){		
						USART_SendData(USART1, 's');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART_SendData(USART1, 0x0d0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART1_RX_STA=0;
						}
					}
					break;
						case 'c':
							if(USART1_RX_BUF[1]=='l'&&USART1_RX_BUF[2]=='c'){
							F_flag=0;
							L_flag=0;
							R_flag=0;
							B_flag=0;
							b_flag=0;
							P_flag=0;
							G_flag=0;
							s_flag=0;
							USART_SendData(USART1, 'c');
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
								USART_SendData(USART1, 'l');
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
								USART_SendData(USART1, 'c');
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
								USART_SendData(USART1, 0x0d0a);
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
							}
							break;
						default:
						break;
					}
			
					
				
		
			USART1_RX_STA=0;
					
			
	}
#endif
