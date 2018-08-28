#include "gray.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

u8 gray_working_mode;  //���幤��ģʽ��1��2��3
Graycalcudef graycal_1;
Graycalcudef graycal_2;
Graycalcudef graycal_3;
Graycalcudef graycal_4;



void Gray1_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 |  GPIO_Pin_9 |  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 
}
void Gray_Init(void){
	Gray1_Init();
}

void Read_Gray1(u8* grayx_list)
{
	u8 i;
	for(i = 0; i < 8 ; i++){
		grayx_list[i] = PEin((i+7));
	}
}

void CalculFromGray(Graycalcudef* graycal_x, u8* grayx_list){
	  int repeat = 0;
    int maxlength = 0;
    int curlength = 0;
    int min = 0;
    int max = 0;
    int i;
    int j;
    i = 0;
    while(i < 8){
        j = i + 1;
        if(grayx_list[i] == 1){
            while(j < 8)
            {
                if(grayx_list[j] == 1){
                    j++;
                }
                else{
                    break;
                }
            }
            curlength = j - i;
            if(curlength > maxlength){
                min = i;
                max = j-1 ;
                maxlength = curlength;
                repeat = 0;
            }
            else if (curlength == maxlength){
                repeat = 1;
            }
        }
        i = j;
    }
		graycal_x->max_number = max;
		graycal_x->min_number = min;
		graycal_x->maxlength = maxlength;
		graycal_x->center = (float)(max+min)/2.0-3.5;
		if(graycal_x->maxlength >= 5){
			graycal_x->cross = 1;
			if(graycal_x->center >= 0.5){
				graycal_x->center = graycal_x->min_number+1;
			}
			else if(graycal_x->center <= -1){
				graycal_x->center = graycal_x->max_number-1;
			}
		}
		else{
			graycal_x->cross = 0;
		}
}

void Read_Gray(u8* grayrequest){
	u8 gray1_list[8] = {0};
	u8 gray2_list[8] = {0};
	u8 gray3_list[8] = {0};
	u8 gray4_list[8] = {0};
	if(grayrequest [0] == 1 ){
		Read_Gray1(gray1_list);
		CalculFromGray(&graycal_1, gray1_list);	
	}
}



