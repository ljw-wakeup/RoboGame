#ifndef __RFID_H
#define __RFID_H	 
#include "sys.h"

#define USART 1

u8 RFID_READ(u8 *uid,u8 *block);//��ȡrfid��һ������
u8 RFID_USART_Init(void);//��ʼ����������ʹ������
u8 RFID_Disableantenna(void);//�ر����ߣ����ٷ���
u8 GET_UID(u8 *uid);//��ȡUID
u8 Read_flag(void);//�ж�ǰ����ʲô����

#define SPI 0

u8 RFID_SPI_READ(void);


#endif
