#include "sys.h"
#include "usart.h"
#include "raspi.h"
u16 num;
u8 flag;
u8 L_flag=0,R_flag=0,P_flag=0,F_flag,G_flag=0,B_flag=0;//?? ?? ?? ?? ??rfid ???????
extern u16 usart1_len,usart2_len;//??????
u8 b_flag=0,s_flag=0;
char information_all[50];

u16 usart1_len,usart2_len;
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //USART1接收状态标记	  

void Usart_SendString(USART_TypeDef* USARTx,char *str){
	while(*str){
		while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));
		USART_SendData(USARTx,*str);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); 
		str++;           
  }
}

void uart1_init(u32 bound){//用于上下机通信
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void uart2_init(u32 bound){//用于rfid读取
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//USART1_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART1_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
		 if((USART1_RX_STA&0x8000)==0x8000){
			 get_motor();
		 }
		
} 
#endif	


#if EN_USART2_RX
u16 USART2_RX_STA=0;       //USART2接收状态标记	 
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 usart2_len;													//根据第三位判断本次接受的数据量




void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
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
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
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
					if(USART1_RX_BUF[1]=='r'){
						if(!L_flag)
							L_flag=1;
						GPIO_ResetBits(GPIOE,GPIO_Pin_5);
						delay_ms(500);
						GPIO_SetBits(GPIOE,GPIO_Pin_5);
						USART_SendData(USART1, 'O');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
			USART_SendData(USART1, 'K');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
			USART_SendData(USART1, 0x0d0a);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART1_RX_STA=0;
					}
					
					else if(USART1_RX_BUF[1]=='q'){
						USART_SendData(USART1, 'L');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
			USART_SendData(USART1, 'r');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
			USART_SendData(USART1, 0x0d0a);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//??????
						USART1_RX_STA=0;
						
					}
					break;
				case 'R':
					if(USART1_RX_BUF[1]=='r'){
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
					if(USART1_RX_BUF[1]=='r'){
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
