#include "rfid.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"
#if USART

u8 RFID_init_data[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x08,0x01,0x31,0x38};
u8 Get_uid_data[9] = {0xAA,0xBB,0x05,0x00,0x00,0x00,0x01,0x10,0x11};
u8 RFID_READ_data[18]={0xAA,0xBB,0x10,0x00,0x00,0x00,0x05,0x10,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00};
u8 Disable_antenna_data[10]={0xAA,0xBB,0x06,0x00,0x00,0x00,0x0C,0x01,0x00,0x0D};
//u8 data[100];
u8 uid[8]={0};
u8 block[2]={0};
	
	
	
u8 RFID_USART_Init(void)
{
	u8 len=0;
	u8 i=0;
	u8 data[10]={0};
	uart_send_mydata(RFID_init_data,10);
	
	while(i<50)
		
		{
			if(USART2_RX_STA&0x8000)
			{
				uart_receive_mydata(data,&len);
				break;
			}
			i++;
			delay_ms(10);
		}
			
		
	USART2_RX_STA=0;
		
	if(len==10&&data[8]==0)
	{
		return 1;
	}
	else 
	{
		return 0;		
	}
	
}


u8 GET_UID(u8 *uid)
{
	
	u8 len;
	u8 i;
	u8 data[19]={0};
	uart_send_mydata(Get_uid_data,9);
	
	i=0;
	while(i<50)
		
		{
			if(USART2_RX_STA&0x8000)
			{
				uart_receive_mydata(data,&len);
				break;
			}
			i++;
			delay_ms(10);
		}
			
		
	USART2_RX_STA=0;
	if(len==19)
	{
		for(i=0;i<8;i++)
		uid[i]=data[i+10];
	 
		return 1;
	}
	else
	{
		return 0;
	}
			
	
}

u8 RFID_READ(u8 *uid,u8 *block)
{
	u8 i=0;
	u8 len=0;
	u8 data[14]={0};
	for(i=0;i<8;i++)
		RFID_READ_data[i+9]=uid[i];
	RFID_READ_data[17]=0;
	for(i=4;i<17;i++)
		RFID_READ_data[17]=RFID_READ_data[i]^RFID_READ_data[17];
	uart_send_mydata(RFID_READ_data,18);
	
	i=0;
	while(i<50)
		
		{
			if(USART2_RX_STA&0x8000)
			{
				uart_receive_mydata(data,&len);
				break;
			}
			i++;
			delay_ms(10);
		}
			
		
	USART2_RX_STA=0;
		
	if(len==14)
	{
		for(i=0;i<2;i++)
			block[i]=data[i+9];
		return 1;
	}
	else 
		return 0;
	
}

u8  RFID_Disableantenna(void)
{
	u8 len=0;
	u8 i=0;
	u8 data[10]={0};
	uart_send_mydata(Disable_antenna_data,10);
	
	while(i<50)
	
	{
		if(USART2_RX_STA&0x8000)
		{
			uart_receive_mydata(data,&len);
			break;
		}
		i++;
		delay_ms(10);
	}
	
		USART2_RX_STA=0;
	
	if(len==10&&data[8]==0)
	{
		
		return 1;
	}
	else 
	{
		return 0;		
	}
}

u8 Read_flag(){
		u8 t=0;
		t=RFID_USART_Init();//�п�����
		delay_ms(1000);
		if(!t)
			return 0;
		
		t=GET_UID(uid);//��ȡUID
		delay_ms(1000);
	if(!t)
			return 0;
		
		t=RFID_READ(uid,block);//��ȡrfid��һ������
		delay_ms(1000);
	if(!t)
			return 0;
	
		t=RFID_Disableantenna();//�ر����ߣ����ٷ��� 
		delay_ms(1000);
	if(!t)
			return 0;
	if(!block[1]){
		return block[0];
	}
	return 0;
}
#endif

#if SPI
u8 RFID_BUFFER[3];
u8 RFID_SPI_READ(void){
	u8 status;
	u16 RSC = 0;//���ݳ���
	u8 num;
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	//����Э��
	SPI2_ReadWriteByte(0x81);//д��ַ
	SPI2_ReadWriteByte(0x2a);//ISO15693Э�� ���Ͳ�����424K ���ܲ�����424K
	
	num=SPI2_ReadWriteByte(0x01);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	//����CRC �� CRCSEL
	SPI2_ReadWriteByte(0x84);//д��ַ
	SPI2_ReadWriteByte(0xc0);//�����շ��Զ�CRC�벻����CRCSEL
	
	num=SPI2_ReadWriteByte(0x04);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	//���ý��ܶ�ʱ��TMR
	SPI2_ReadWriteByte(0x8c);//д��ַ
	SPI2_ReadWriteByte(0x81);
	
	num=SPI2_ReadWriteByte(0x0c);//
	
	SPI2_ReadWriteByte(0x8d);//д��ַ
	SPI2_ReadWriteByte(0x59);//302us*0x8159 = 10s
	
	num=SPI2_ReadWriteByte(0x0d);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	//�����ز�
	SPI2_ReadWriteByte(0x86);//д��ַ
	SPI2_ReadWriteByte(0x05);//�����ز� ����շ�������
	
	num=SPI2_ReadWriteByte(0x06);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	SPI2_ReadWriteByte(0x86);//д��ַ
	SPI2_ReadWriteByte(0x00);//�շ��������������� 
	
	num=SPI2_ReadWriteByte(0x06);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	//������һ����û�õĶ���
	/*
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	SPI2_WriteByte(0x85);//дDATA��ַ
	SPI2_WriteByte();//д��ַ
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	*/
	
	
	//��������
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	SPI2_ReadWriteByte(0x86);//д��ַ
	SPI2_ReadWriteByte(0x02);//��������
	
	num=SPI2_ReadWriteByte(0x06);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	//�ȴ����ܽ���
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
	status = SPI2_ReadWriteByte(0x05);//����״̬
	while((status&0x80)==0x00){
		status = SPI2_ReadWriteByte(0x05);//����״̬
	}
	//�������
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	
	if(status==0x81){//�޴���
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
		status = SPI2_ReadWriteByte(0x08);//дRSCH��ַ
		RSC=status*256;
		
		status = SPI2_ReadWriteByte(0x09);//дRSCL��ַ
		RSC+=status;//��ȡ���ݳ��� ��λ���ֽ�
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	}
	else 
		return 0;
	if(RSC){
		int i=0;
	//��ʼ��������
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//ʹ��
		
		while(i<RSC){
			RFID_BUFFER[i]=SPI2_ReadWriteByte(0x00);//дDATA��ַ
			i++;
		}
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//ʧ��
	}
	else
		return 0;
	return 1;
}


#endif
