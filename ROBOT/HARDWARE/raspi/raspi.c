#include "raspi.h"
#define  TRY_TIM 5
u8 raspi_status;
u8 color_list[8];
u8 cam;
extern u8 area;

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2,ENABLE);  //失能TIMx					 
}


//定时器4中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
	
	request_colorList(area);
		
}



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
		
