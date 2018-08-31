#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "rfid.h"
#include "adapter.h"
#include "ultrasonic.h"
#include "switch.h"
#include "MG.h"
#include "spi.h"
#include "motor.h"
#include "test.h"
/************************************************
 ALIENTEK精英STM32开发板实验9
 PWM输出实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
extern u8 RFID_init_data[10];
extern u16 motor1,motor2,motor3,motor4;//控制步进电机
extern u8 speed1, speed2,speed3,speed4; 
u8 adapter1[2]={0,100},adapter2[2]={0,100};//步进电机的转动时间
u16 mg1=1850,mg2=0x073a,mg3=1850,mg4=1850;//控制最上方的舵机
u16 usart1_len,usart2_len;//串口数据长度
u8 looptime=30,delaytime=100;
extern u8 b_flag,s_flag;//电杠回缩,停止
extern u8 L_flag,R_flag,P_flag,F_flag,G_flag,B_flag;//左手 右手 放下 脱机 读取rfid 左右臂舵机回转
extern u16 ultrasonic1;//超声波返回的定时器计数值
extern u8 RFID_BUFFER[3];//rfid的读出的数据

void assert_failed(uint8_t* file, uint32_t line)
{
 printf("Wrong parameters value: file %s on line %d\r\n", file, line);
 while(1);
}
void test(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}
 int main(void)
 {		
 	 delay_init();	    	 //延时函数初始化	

	 RCC_init();
	
	 switch_GPIO_init();
	 
	//KEY_Init(); //初始化与案件链接的硬件接口
	 
	delay_ms(255);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 
	uart1_init(9600);	 //串口初始化为115200 负责蓝牙通信 上下机
	delay_ms(255);
	 
 	uart2_init(9600);	 //串口初始化为115200 负责读取rfid
delay_ms(255);
	
	MG_GPIO_Init();			     //LED端口初始化
delay_ms(255);
	
 	TIM3_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/720/2000=50hz
	delay_ms(255);
	
	 //motor_init();
	 delay_ms(255);
	 
	 adapter_GPIO_init();
	 delay_ms(255);
	 
	TIM4_Int_Init(498,71);	 //分频2。PWM频率=72000000/72/1000=1khz
	delay_ms(255);
	
	 //超声波配置
	TIM5_Configuration(19999,71);
delay_ms(255);

	ultrasonic_GPIO_init();
	delay_ms(255);
	
	ultrasonic_IRQ_init();
	 delay_ms(255);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	 GPIO_SetBits(GPIOE,GPIO_Pin_5);
	// UP();
	 //DIS_motor();
	TIM_SetCompare1(TIM3,mg1);
	TIM_SetCompare2(TIM3,mg2);
	TIM_SetCompare3(TIM3,mg3);
	TIM_SetCompare4(TIM3,mg4);
	//test();
	
	//test_GPIO_out_init();
while(1)
		{
			
			while(0){
				//GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				//TIM_SetCompare1(TIM3,mg1);
	//TIM_SetCompare2(TIM3,mg2);
	//TIM_SetCompare3(TIM3,mg3);
	//TIM_SetCompare4(TIM3,mg4);
				//delay_ms(500);
				test_GPIO_output();
				//u16 t;
				//t=Read_flag();//读取旗子信息
				//trig_ultrasonic();
			//PBout(12)=1;
				//GPIO_SetBits(GPIOB,GPIO_Pin_0);
				//delay_ms(500);
				//delay_ms(500);
			//SPI_I2S_SendData(SPI2, 0x35); //Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
		//USART_SendData(USART2, 0x1998);//Ïò´®¿Ú1·¢ËÍÊý¾Ý
			//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
			//USART_SendData(USART2, 0xf00f);//Ïò´®¿Ú1·¢ËÍÊý¾Ý
			//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
				
			//PBout(12)=0;
				//GPIO_SetBits(GPIOB,GPIO_Pin_0);
				//delay_ms(500);
				//delay_us(60);
				//DOWN();
	 //EN_motor();
				//uart_send_mydata(RFID_init_data,10);
			}
			
		if(L_flag==1){
			L_flag=2;;
			/*int i;
			
			for(i=0;i<looptime;i++){
				TIM_SetCompare1(TIM3,0x06f0);
				delay_ms(40);
				TIM_SetCompare1(TIM3,2000);
				delay_ms(delaytime);
			}
			*/
			move_mg1(mg1,0x06f0);
			mg1=0x06f0;
			
      

		}
		
		if(R_flag==1){
			R_flag=2;
			/*int i;
			
			for(i=0;i<looptime;i++){
				TIM_SetCompare2(TIM3,0x0710);
				delay_ms(40);
				TIM_SetCompare2(TIM3,2000);
				delay_ms(delaytime);
			}
			*/
			move_mg2(mg2,0x0715);
			mg2=0x0715;
			
		
		

		}

		if(P_flag==1){//放下 第一次到达 或者开关有效
			P_flag=2;//我方好了！
			L_flag=0;
			R_flag=0;
			USART_SendData(USART1, 'P');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			USART_SendData(USART1, '\n');
			
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			USART_SendData(USART1, 0x0d0a);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			/*
				for(i=0;i<looptime;i++){
				TIM_SetCompare3(TIM3,0x06f0);
				TIM_SetCompare4(TIM3,0x06f0);
				delay_ms(40);
				
					TIM_SetCompare3(TIM3,2000);
				TIM_SetCompare4(TIM3,2000);
				delay_ms(delaytime);
					
			}
				*/
			move_mg3(mg3,0x06f0);
			mg3=0x06f0;
			move_mg4(mg4,0x06f0);
			mg4=0x06f0;
			delay_ms(500);
			delay_ms(500);
			
			
			/*
			for(i=0;i<looptime;i++){
				TIM_SetCompare1(TIM3,0x073a);
				TIM_SetCompare2(TIM3,0x780);
				delay_ms(40);
				TIM_SetCompare1(TIM3,2000);
				TIM_SetCompare2(TIM3,2000);
				delay_ms(delaytime);
			}
			*/
			move_mg1(mg1,0x073a);
			mg1=0x073a;
			move_mg2(mg2,0x780);
			mg2=0x780;
			
			
		}	
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)){
			//stop_motor();
		L_flag=0;
			R_flag=0;
			USART_SendData(USART1, 'P');
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			USART_SendData(USART1, '\n');
			
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			USART_SendData(USART1, 0x0d0a);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			/*
				for(i=0;i<looptime;i++){
				TIM_SetCompare3(TIM3,0x06f0);
				TIM_SetCompare4(TIM3,0x06f0);
				delay_ms(40);
				
					TIM_SetCompare3(TIM3,2000);
				TIM_SetCompare4(TIM3,2000);
				delay_ms(delaytime);
					
			}
				*/
			move_mg3(mg3,0x06f0);
			mg3=0x06f0;
			move_mg4(mg4,0x06f0);
			mg4=0x06f0;
			delay_ms(500);
			delay_ms(500);
			
			
			/*
			for(i=0;i<looptime;i++){
				TIM_SetCompare1(TIM3,0x073a);
				TIM_SetCompare2(TIM3,0x780);
				delay_ms(40);
				TIM_SetCompare1(TIM3,2000);
				TIM_SetCompare2(TIM3,2000);
				delay_ms(delaytime);
			}
			*/
			move_mg1(mg1,0x073a);
			mg1=0x073a;
			move_mg2(mg2,0x780);
			mg2=0x780;
			
			//DOWN();
		
		}
		if(G_flag==1){
			u16 t;
			#if USART
				t=Read_flag();//读取旗子信息
			#endif
				
			#if SPI
			/*	
			RFID_SPI_READ();
				t=RFID_BUFFER[0]*256+RFID_BUFFER[1];//！！！！！！！！！！！！！！！！！！！！！！！！可能会反向！！！！！！！！！！！！！！！！
				//t=RFID_BUFFER[0]+RFID_BUFFER[1]*256;
			*/
			#endif
			USART_SendData(USART1,t);
		}
		
		if(B_flag==1){//到达拐角点 下面发送信号转会左右臂舵机
			/*int i;
			
			for(i=0;i<looptime;i++){
				TIM_SetCompare3(TIM3,0x0790);
				TIM_SetCompare4(TIM3,0x0790);
				delay_ms(40);
				TIM_SetCompare3(TIM3,2000);
				TIM_SetCompare4(TIM3,2000);
				delay_ms(delaytime);
			}
			*/
			move_mg3(mg3,0x0790);
			mg3=0x0790;
			move_mg4(mg4,0x0790);
			mg4=0x0790;
			
		B_flag=2;
		}
		
		if(F_flag==1){
			int i;	
			F_flag=2;
			b_flag=0;
			//抓住  电缸前进后退
			backward(1);
			for(i=0;i<adapter1[0];i++)
			delay_ms(adapter1[1]);
			stop();
			backward(2);
			for(i=0;i<adapter2[0];i++)
			delay_ms(adapter2[1]);
			stop();
		
		}
		if(b_flag==1){
			//抓住
			int i;
			b_flag=2;
			F_flag=0;
			forward(1);
			for(i=0;i<adapter1[0];i++)
			delay_ms(adapter1[1]);
			stop();
			forward(2);
			for(i=0;i<adapter2[0];i++)
			delay_ms(adapter2[1]);
			stop();
			
		}
		if(s_flag){
			F_flag=0;
			b_flag=0;
			s_flag=0;
			stop();
				}
		if(ultrasonic1>=7200){
			//GPIO_SetBits(GPIOE,GPIO_Pin_5);//步进电机的使能线
			F_flag=4;
		}
		if(F_flag==2){//脱机上升
				//EN_motor();
				motor1=4000;
				motor2=4000;
				motor3=4000;
				motor4=4000;
				
				F_flag=3;
		}
		if(F_flag==3){
			ultrasonic1=0;
			trig_ultrasonic();
			getultrasonic();
		}
			if(F_flag==4) {//缓慢上升
				speed1=120;
				speed2=120;
				speed3=120;
				speed4=120;
				
			}
			
			
	}
	
 }
