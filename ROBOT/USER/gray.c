#include "gray.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

u8 gray_working_mode;  //定义工作模式，1，2，3
Graycalcudef graycal_1;
Graycalcudef graycal_2;
Graycalcudef graycal_3;
Graycalcudef graycal_4;



void Gray1_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 |  GPIO_Pin_9 |  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 
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



