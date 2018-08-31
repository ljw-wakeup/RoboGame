#include "timer.h"
#include "led.h"
#include "usart.h"
#include "ultrasonic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTE精英STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

u16 motor1=65534,motor2=0,motor3=0,motor4=0;//控制步进电
u8 speed1=2,speed2=2,speed3=2,speed4=2; 
extern u8 looptime,delaytime,Test;
u8 speed1flag,speed2flag,speed3flag,speed4flag; 
extern u16 mg1,mg2,mg3,mg4;//控制最上方的舵机
extern u8 adapter1[2],adapter2[2];//步进电机的转动时间
u16 num;
u8 flag;
u8 L_flag=0,R_flag=0,P_flag=0,F_flag,G_flag=0,B_flag=0;//左手 右手 放下 脱机 读取rfid 左右臂舵机回转
extern u16 usart1_len,usart2_len;//串口数据长度
u8 b_flag=0,s_flag=0;
char information_all[50];
extern u16 ultrasonic1;//超声波返回的定时器计数值
void Usart_SendString(USART_TypeDef* USARTx,char *str){
	while(*str){
		while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));
		USART_SendData(USARTx,*str);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); 
		str++;           
  }
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}
//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断 步进电机的PWM
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		speed1flag++;speed2flag++;speed3flag++;speed4flag++;
		if(speed1flag>=speed1){
				speed1flag=0;	
		if(motor1>0){
			if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6)){
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				motor1--;
			}
			else {
				GPIO_SetBits(GPIOB,GPIO_Pin_6);
		}
		
		}
	
		}
		if(speed2flag>=speed2){
			speed2flag=0;
		if(motor2>0){
			if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7)){
				
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
				motor2--;
					
			}
			else {
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
			}
		}
	
		}
		if(speed3flag>=speed3){
				speed3flag=0;
			if(motor3>0){
				if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5)){
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
					motor3--;
				}
			else {
				GPIO_SetBits(GPIOB,GPIO_Pin_5);
			}
		}
			
		}
	if(speed4flag>=speed4){
				speed4flag=0;
		if(motor4>0){
			if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9)){
				
				GPIO_ResetBits(GPIOB,GPIO_Pin_9);
				motor4--;
			}
			else {
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			
			}
		}
		
		}
	}
}

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3 4时钟
	
 

   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
   //初始化TIM3
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
    //设置该引脚为复用输出功能,输出TIM3 CH3的PWM脉冲波形	GPIOB.0
   //初始化TIM3
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
	
   //初始化TIM3
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}


void get_motor(void)
{
	if(USART1_RX_STA&0x8000)//是否有接收到东西
		{
			usart1_len = USART1_RX_STA&0x3fff;//得到此次接收到的数据长度	
			
			switch (USART1_RX_BUF[0]) {
				case 'u':
					USART_SendData(USART1,'u');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
					USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				trig_ultrasonic();
					break;
				case 'L':
					if(USART1_RX_BUF[1]=='q'){
						if(!L_flag)
							L_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='L'&&USART1_RX_BUF[1]=='q'){		
						USART_SendData(USART1, 'L');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;
				case 'R':
					if(USART1_RX_BUF[1]=='q'){
						if(!R_flag)
							R_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='R'&&USART1_RX_BUF[1]=='q'){		
						USART_SendData(USART1, 'R');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;
				case 'P': 
					if(USART1_RX_BUF[1]=='q'){
						if(!P_flag)
							P_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='P'&&USART1_RX_BUF[1]=='q'){		
						USART_SendData(USART1, 'P');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;
				case 'F':
					if(USART1_RX_BUF[1]=='q'){
						F_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='F'&&USART1_RX_BUF[1]=='q'){		
						USART_SendData(USART1, 'F');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;
				case 'G':
					if(USART1_RX_BUF[1]=='q'){
						G_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='G'&&USART1_RX_BUF[1]=='q'){		
						USART_SendData(USART1, 'G');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;

				case 'a': //ask 询问所有信息
				
				sprintf(information_all,"motor %d %d %d %d \r\n speed %d %d %d %d \r\n diangang %d %d %d %d \r\n mg %d %d %d %d\r\n mgyanshi looptime%d  delaytime%d\n F_flag=%d\n b_flag=%d\n",motor1,motor2,motor3,motor4,
					speed1,speed2,speed3,speed4,
					adapter1[0],adapter1[1],adapter2[0],adapter2[1]
					,mg1,mg2,mg3,mg4,looptime,delaytime,F_flag,b_flag);
				Usart_SendString(USART1,information_all);
				USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
					break;
				
				case 'b': //爬杆臂松手
					if(USART1_RX_BUF[1]=='q'){
						if(b_flag==0)b_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='b'&&USART1_RX_BUF[1]=='q'){		
							USART_SendData(USART1, 'b');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART1_RX_STA=0;
						}
					}
					break;
				case 'B':
					if(USART1_RX_BUF[1]=='q'){
						if(!B_flag)
							B_flag=1;
						
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='B'&&USART1_RX_BUF[1]=='q'){		
						USART_SendData(USART1, 'B');
							
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 'r');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成		
			USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;
					case 's'://停止
					{
						s_flag=1;
						while(USART1_RX_STA&0x8000&&USART1_RX_BUF[0]=='s'){		
						USART_SendData(USART1, 's');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART1_RX_STA=0;
						}
					}
					break;
				case 't': //设置步进电机目标行程和速度
					switch(USART1_RX_BUF[1]){
						case 1:
							motor1=USART1_RX_BUF[2];
							motor1*=256;
							motor1+=USART1_RX_BUF[3];
							speed1=USART1_RX_BUF[4];	
							speed1flag=0;
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							break;
						case 2:
							motor2=USART1_RX_BUF[2];
							motor2*=256;
							motor2+=USART1_RX_BUF[3];
							speed2=USART1_RX_BUF[4];
							speed2flag=0;
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						break;
						case 3:
							motor3=USART1_RX_BUF[2];
							motor3*=256;
							motor3+=USART1_RX_BUF[3];
							speed3=USART1_RX_BUF[4];
							speed2flag=0;
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						case 4:
							motor4=USART1_RX_BUF[2];
							motor4*=256;
							motor4+=USART1_RX_BUF[3];
							speed4=USART1_RX_BUF[4];	
							speed2flag=0;
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d0a);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							break;
							default:
							break;
					}
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
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
								USART_SendData(USART1, 'l');
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
								USART_SendData(USART1, 'c');
								while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
								USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							}
							break;
						case 'v':
							USART_SendData(USART1, 'v');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							switch(USART1_RX_BUF[1]){
								case 1:
									speed1=USART1_RX_BUF[2];
									break;
								case 2:
									speed2=USART1_RX_BUF[2];
									break;
								case 3:
									speed3=USART1_RX_BUF[2];
									break;
								case 4:
									speed4=USART1_RX_BUF[2];
									break;
								default:
									break;
							}
							break;
						case 'g': //设置步进电机目标行程和速度
					switch(USART1_RX_BUF[1]){
						case 1:
							mg1=USART1_RX_BUF[2];
							mg1*=256;
							mg1+=USART1_RX_BUF[3];
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							break;
						
						case 2:
							mg2=USART1_RX_BUF[2];
							mg2*=256;
							mg2+=USART1_RX_BUF[3];
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
						USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							break;
						case 3:
							mg3=USART1_RX_BUF[2];
							mg3*=256;
							mg3+=USART1_RX_BUF[3];
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							break;
						case 4:
							mg4=USART1_RX_BUF[2];
							mg4*=256;
							mg4+=USART1_RX_BUF[3];
							USART_SendData(USART1, 'T');
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
							break;
						default:
							break;
					}
					break;
					case 'd'://设置电缸行程
					switch(USART1_RX_BUF[1]){
						case 't':
							delaytime = USART1_RX_BUF[2];
							looptime = USART1_RX_BUF[3];
							break;
						case 1:
							adapter1[0]=USART1_RX_BUF[2];
						  adapter1[1]=USART1_RX_BUF[3];
							break;
						case 2:
							adapter2[0]=USART1_RX_BUF[2];
						  adapter2[1]=USART1_RX_BUF[3];
							break;
						default:
							break;
					}
			USART_SendData(USART1, 'O');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			USART_SendData(USART1, 'K');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			USART_SendData(USART1, 0x0d);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
					break;
					default:
						break;
				}
			USART1_RX_STA=0;
					
			
			
	}
}
void RCC_init(void){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5, ENABLE);	//使能定时器3 4时钟
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//Ê¹ÄÜPORTA,PORTEÊ±ÖÓ

}
