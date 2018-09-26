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
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
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
		
