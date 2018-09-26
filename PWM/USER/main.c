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
extern u8 valid;
extern u8 RFID_init_data[10];
extern u16 motor1,motor2,motor3,motor4;//控制步进电机
extern u8 speed1, speed3;
u8 adapter1[2]={30,100},adapter2[2]={30,100};//步进电机的转动时间
u16 mg1=1900,mg2=0x073a,mg3=0x0717,mg4=0x077f,mg5=0x0780,mg6=0x0780;//控制最上方的舵机
u16 usart1_len,usart2_len;//串口数据长度
u8 looptime=30,delaytime=100;
extern u8 b_flag,s_flag;//电杠回缩,停止
extern u8 L_flag,R_flag,P_flag,F_flag,G_flag,B_flag,Ld_flag,Rd_flag,rfid_send;//左手 右手 放下 脱机 读取rfid 左右臂舵机回转
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
	 
 	uart2_init(19200);	 //串口初始化为115200 负责读取rfid
delay_ms(255);
	
	MG_GPIO_Init();			     //LED端口初始化
delay_ms(255);
	
 	TIM3_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/720/2000=50hz
	delay_ms(255);
	TIM2_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/720/2000=50hz
	 motor_init();
	 delay_ms(255);
	 UP();
	 DIS_motor();
	 adapter_GPIO_init();
	 delay_ms(255);
	 
	TIM4_Int_Init(40,17);	 //分频2。PWM频率=72000000
	
	delay_ms(255);
	
	 //超声波配置
	TIM5_Configuration(19999,71);
delay_ms(255);

	ultrasonic_GPIO_init();
	delay_ms(255);
	
	ultrasonic_IRQ_init();
	 delay_ms(255);
	 GPIO_SetBits(GPIOE,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	
	TIM_SetCompare1(TIM3,mg1);
	TIM_SetCompare2(TIM3,mg2);
	TIM_SetCompare3(TIM3,mg3);
	TIM_SetCompare4(TIM3,mg4);
	TIM_SetCompare1(TIM2,mg5);
	TIM_SetCompare2(TIM2,mg6);
	//test();
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	//test_GPIO_out_init();
while(1)
		{
			
			while(0){
				TIM_SetCompare1(TIM3,mg1);
				TIM_SetCompare2(TIM3,mg2);
				TIM_SetCompare4(TIM3,mg4);
				TIM_SetCompare3(TIM3,mg3);
				TIM_SetCompare1(TIM2,mg5);
				TIM_SetCompare2(TIM2,mg6);
				delay_ms(1000);
			}
			
		if(L_flag==1){
			L_flag=2;
			move_mg3(mg3,0x077a);
			mg3=0x077a;
			move_mg5(mg5,0x0710);
			mg5=0x0710;
			move_mg1(mg1,0x06f0);
			mg1=0x06f0;
			move_mg5(mg3,0x0780);
			mg5=0x0780;
			move_mg3(mg3,0x0717);
			mg3=0x0717;
			while(!Ld_flag){
				USART_SendData(USART1, 'L');
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 'd');
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0d);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			
			}		
			Ld_flag=0;
		}
		
		if(R_flag==1){
			R_flag=2;
			move_mg4(mg4,0x0719);
			mg4=0x0719;
			move_mg6(mg6,0x0718);
			mg6=0x0718;
			move_mg2(mg2,0x0705);
			mg2=0x0705;
			move_mg6(mg6,0x0770);
			mg6=0x0770;
			move_mg4(mg4,0x077f);
			mg4=0x077f;
			while(!Rd_flag){
				USART_SendData(USART1, 'R');
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 'd');
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0d);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			}
			Rd_flag=0;
		}

		if(P_flag==1){//放下 第一次到达 或者开关有效
			P_flag=2;//我方好了！
			L_flag=0;
			R_flag=0;

			move_mg3(mg3,0x0717);//左边
			mg3=0x0717;
			move_mg4(mg4,0x077f);//右边
			mg4=0x077f;
			delay_ms(500);
			delay_ms(500);
			
			move_mg5(mg5,0x0720);//左边
			mg3=0x0720;
			move_mg6(mg6,0x0728);//右边
			mg4=0x0728;
			delay_ms(500);
			delay_ms(500);
			
			move_mg1(mg1,0x073a);
			mg1=0x073a;
			move_mg2(mg2,0x780);
			mg2=0x780;
			
			
		}	
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)){
			//stop_motor();
		L_flag=0;
			R_flag=0;
			move_mg3(mg3,0x0717);
			mg3=0x0717;
			move_mg4(mg4,0x077f);
			mg4=0x077f;
			delay_ms(500);
			delay_ms(500);
			
			move_mg5(mg5,0x0720);//左边
			mg3=0x0720;
			move_mg6(mg6,0x0728);//右边
			mg4=0x0728;
			delay_ms(500);
			delay_ms(500);
			
			move_mg1(mg1,0x073a);
			mg1=0x073a;
			move_mg2(mg2,0x780);
			mg2=0x780;
			
			DOWN();
		
		}
		if(G_flag==1){
				u16 t;
			G_flag=2;
		
			#if USART
				t=Read_flag();//读取旗子信息
			#endif
				
			#if SPI
				RFID_SPI_READ();
				t=RFID_BUFFER[0]*256+RFID_BUFFER[1];//！！！！！！！！！！！       ！！！！！！！！！！！！！可能会反向！！！！！！！！！！！！！！！！
				//t=RFID_BUFFER[0]+RFID_BUFFER[1]*256;
			#endif
			while(!rfid_send){
				USART_SendData(USART2,'G');
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				
				USART_SendData(USART2,t);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0d);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
				USART_SendData(USART1, 0x0a);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//是否发送完成
			}
			rfid_send=0;
		}
		
		if(B_flag==1){//到达拐角点 下面发送信号转会左右臂舵机
		/*	move_mg4(mg4,0x0730);
			mg4=0x0730;
			move_mg3(mg3,0x0760);
			mg3=0x0760;
			*/
			
		B_flag=2;
		}
		
		if(F_flag==1){
			
			int i;	
				
			F_flag=2;
			b_flag=0;
			//抓住  电缸前进后退
		
			backward(1);//上面
			for(i=0;i<adapter1[0];i++)
			delay_ms(adapter1[1]);
			stop();
			
			backward(2);//下面
			for(i=0;i<adapter2[0];i++)
			delay_ms(adapter2[1]);
			stop();
			
		}
		if(b_flag==1){
			//抓住
			int i;
			DIS_motor();
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
		if(ultrasonic1>=5700){
			//F_flag=4;
		}
		if(F_flag==2){//脱机上升
			TIM_Cmd(TIM4,ENABLE);
				EN_motor();
				motor1=motornum;
				motor2=motornum;
				motor3=motornum;
				motor4=motornum;
				
				F_flag=3;
		}
		if(motor1==0){
			stop_motor();
		}
		if(F_flag==3){
			delay_ms(60);
			trig_ultrasonic();
			getultrasonic();
		}
			if(F_flag==4) {//缓慢上升
				speed1=120;
				//speed3=120;		
			}
			
			
	}
	
 }
