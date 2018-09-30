#include "raspi.h"
#define  TRY_TIM 5
u8 raspi_status;
u8 color_list[9];
u8 cam;
extern u8 area;
extern int colorlist[9];

void push_raspi(u8 a){

USART_SendData(USART2, a);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);


}
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2,ENABLE);  //ʧ��TIMx					 
}


//��ʱ��4�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
	///zpush_raspi(area);
		}
		
}



void connect_raspi(void )	
{
	u16 len; 
	u8 i; 
	u8 j;
if(USART2_RX_STA&0x8000)//
{
			len = USART2_RX_STA&0x3fff;//	
					if(len==9) {
						raspi_status='d';
					for(i=0;i<len;i++){
					color_list[i]=USART2_RX_BUF[i];
					}
					/*
				  for(j = 0; j < 9; j++){
						if(colorlist[j] != -1) colorlist[j] = color_list[j] - '0';
					}
					*/
					
					
				} 
			USART2_RX_STA=0;	
		  
}
}

