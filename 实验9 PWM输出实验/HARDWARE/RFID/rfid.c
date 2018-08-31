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
		t=RFID_USART_Init();//有开天线
		delay_ms(1000);
		if(!t)
			return 0;
		
		t=GET_UID(uid);//获取UID
		delay_ms(1000);
	if(!t)
			return 0;
		
		t=RFID_READ(uid,block);//读取rfid第一块数据
		delay_ms(1000);
	if(!t)
			return 0;
	
		t=RFID_Disableantenna();//关闭天线，减少发热 
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
	u16 RSC = 0;//数据长度
	u8 num;
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	//设置协议
	SPI2_ReadWriteByte(0x81);//写地址
	SPI2_ReadWriteByte(0x2a);//ISO15693协议 发送波特率424K 接受波特率424K
	
	num=SPI2_ReadWriteByte(0x01);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	//设置CRC 与 CRCSEL
	SPI2_ReadWriteByte(0x84);//写地址
	SPI2_ReadWriteByte(0xc0);//设置收发自动CRC与不允许CRCSEL
	
	num=SPI2_ReadWriteByte(0x04);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	//设置接受定时器TMR
	SPI2_ReadWriteByte(0x8c);//写地址
	SPI2_ReadWriteByte(0x81);
	
	num=SPI2_ReadWriteByte(0x0c);//
	
	SPI2_ReadWriteByte(0x8d);//写地址
	SPI2_ReadWriteByte(0x59);//302us*0x8159 = 10s
	
	num=SPI2_ReadWriteByte(0x0d);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	//开启载波
	SPI2_ReadWriteByte(0x86);//写地址
	SPI2_ReadWriteByte(0x05);//开启载波 清空收发缓存区
	
	num=SPI2_ReadWriteByte(0x06);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	SPI2_ReadWriteByte(0x86);//写地址
	SPI2_ReadWriteByte(0x00);//收发缓存区工作正常 
	
	num=SPI2_ReadWriteByte(0x06);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	//好像这一节是没用的东西
	/*
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	SPI2_WriteByte(0x85);//写DATA地址
	SPI2_WriteByte();//写地址
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	*/
	
	
	//启动发送
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	SPI2_ReadWriteByte(0x86);//写地址
	SPI2_ReadWriteByte(0x02);//启动发送
	
	num=SPI2_ReadWriteByte(0x06);//
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	//等待接受结束
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
	status = SPI2_ReadWriteByte(0x05);//接受状态
	while((status&0x80)==0x00){
		status = SPI2_ReadWriteByte(0x05);//接受状态
	}
	//接受完成
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	
	if(status==0x81){//无错误
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
		status = SPI2_ReadWriteByte(0x08);//写RSCH地址
		RSC=status*256;
		
		status = SPI2_ReadWriteByte(0x09);//写RSCL地址
		RSC+=status;//获取数据长度 单位是字节
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	}
	else 
		return 0;
	if(RSC){
		int i=0;
	//开始读出数据
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//使能
		
		while(i<RSC){
			RFID_BUFFER[i]=SPI2_ReadWriteByte(0x00);//写DATA地址
			i++;
		}
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//失能
	}
	else
		return 0;
	return 1;
}


#endif
