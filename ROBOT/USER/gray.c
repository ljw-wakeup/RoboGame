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
Graycalcudef graycal_0;
Graycalcudef graycal_1;
Graycalcudef graycal_2;
Graycalcudef graycal_3;
Graycalcudef_beta  graycal_0_beta;
Graycalcudef_beta  graycal_1_beta;



void Gray_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_8 |  GPIO_Pin_9 |  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
	
 
}

void Read_Gray0(u8* grayx_list)
{
	u8 i, temp;
	//(xiabiao)2:2, 1:1,
	for(i = 0; i < 8; i++){
		grayx_list[i] = PEin((14-i));
	}
	 temp = grayx_list[4];
	 grayx_list[4] = grayx_list[5];
	 grayx_list[5] = temp;
	 temp = grayx_list[6];
	 grayx_list[6] = grayx_list[7];
	 grayx_list[7] = temp;
}

void Read_Gray1(u8* grayx_list){
	u8 i;
	for(i = 0; i < 8 ; i++){
		grayx_list[i] = PDin((7 -i ));
	}
	grayx_list[1] = PEin((0));
	grayx_list[3] = PEin((11));
}

void Read_Gray2(u8* grayx_list){
	u8 i;
	for(i = 0; i < 3 ; i++){
		grayx_list[i] = PDin((8+i));
	}
}

void Read_Gray3(u8* grayx_list){
	u8 i;
	for(i = 0; i<4 ; i++){
		grayx_list[i]  = PDin((12+i));
	}
}

void CalculFromGray_beta(Graycalcudef_beta* graycal_x_beta, u8* grayx_list, u8 graytab, u8 graymode){
	//graymode:0:min 1: double 2:max
	u8 listlength = 8;
	u8 min = listlength + 1;
	u8 max = listlength + 1;
	u8 blacknum = 0;
	int i ;
	int j;
	u8 cross;
	for(i = 0; i < listlength; i++){
		if(grayx_list[i] == 1 && min == listlength + 1){ min = i; blacknum ++ ;}
		else if(grayx_list[i] == 1) blacknum ++ ;
	}
	for(i = listlength - 1; i >= 0; i--){
		if(grayx_list[i] == 1) max = i;
		break;
	}
	graycal_x_beta->min = min;
	graycal_x_beta->max = max;
	graycal_x_beta->blacknum = blacknum ;
	if(blacknum == 0) graycal_x_beta->invalid = 1;
	else graycal_x_beta->invalid = 0;
	if(graymode == 0) graycal_x_beta->center = (float)min + 0.5 - 3.5;
	else if(graymode == 1) graycal_x_beta->center = (float)(max + min) / 2.0 - 3.5;
	else graycal_x_beta->center = (float)(max) - 0.5 - 3.5;
}
/*
void CalculFromGray(Graycalcudef* graycal_x, u8* grayx_list, u8 graytab){
	  u8 repeat = 0;
    u8 maxlength = 0;
    u8 curlength = 0;
    u8 min = 0;
    u8 max = 0;
	  u8 listlength  = 0;
    int i;
    int j;
		u8 cross_judge = 0;  //该值只在1,2号灰度工作时才有意义
	//给listlength赋值
	//给cross_judge 赋值
	  if(graytab == 0){
			listlength = 8;
			cross_judge = CROSS_JUDGE_0;
		}
	  else if(graytab == 1){
			listlength = 8;
			cross_judge = CROSS_JUDGE_1;
		}
	  else if(graytab == 2) listlength = 3;
		else listlength = 4;
    i = 0;
    while(i < listlength){
        j = i + 1;
        if(grayx_list[i] == 1){
            while(j < listlength)
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
		graycal_x->repeat = repeat;   // 是否有重复数据
		graycal_x->max_number = max;
		graycal_x->min_number = min;
		graycal_x->maxlength = maxlength;//maxlength如果是0；那么就是没有黑线在前面
		if(graytab == 1) graycal_x->center = (float)(max + min)/2.0 - 3.5;
		else if(graytab == 2) graycal_x->center = (float)(max + min)/2.0 - 1.5;
		else graycal_x->center = (float)(max+min)/2.0-((float)listlength/2.0-0.5);
		graycal_x->cross = 0;
		//如果是1,2号灰度工作那么拐点进行PID修正，并判断是否到达拐点
		if(graytab == 0 || graytab == 1){
			float revise;
			if(graytab == 1) revise = 1;
			else revise = 0.6;
			if(graycal_x->maxlength >cross_judge|| (graycal_x -> repeat && graycal_x -> maxlength >= 3)){
				graycal_x->cross = 1;
				if(graycal_x->center >= 0.5){
					graycal_x->center = graycal_x->min_number + revise ;
				}
				else if(graycal_x->center <= - 0.5){
					graycal_x->center = graycal_x->max_number - revise;
				}
		  }
			else{
				graycal_x->cross = 0;
			}	
		}
		if( maxlength == 0 || repeat == 1) graycal_x->invalid = 1;
		else graycal_x->invalid = 0;
		
}
*/
void CalculFromGray(Graycalcudef* graycal_x, u8* grayx_list, u8 graytab){
	  u8 repeat = 0;
    u8 maxlength = 0;
    u8 curlength = 0;
    u8 min = 0;
    u8 max = 0;
	  u8 listlength  = 0;
    int i;
    int j;
	  u8 invalid = 0;
		u8 cross_judge = 0;  //该值只在1,2号灰度工作时才有意义
	//给listlength赋值
	//给cross_judge 赋值
	  if(graytab == 0){
			listlength = 8;
			cross_judge = CROSS_JUDGE_0;
		}
	  else if(graytab == 1){
			listlength = 8;
			cross_judge = CROSS_JUDGE_1;
		}
	  else if(graytab == 2) listlength = 3;
		else listlength = 4;
    i = 0;
    while(i < listlength){
        j = i + 1;
        if(grayx_list[i] == 1){
            while(j < listlength)
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
							  if(maxlength != 0) invalid = 1;
                maxlength = curlength;
            }
  
        }
        i = j;
    }
		graycal_x->max_number = max;
		graycal_x->min_number = min;
		graycal_x->maxlength = maxlength;//maxlength如果是0；那么就是没有黑线在前面
		if(graytab == 1) graycal_x->center = (float)(max + min)/2.0 - 3.5;
		else if(graytab == 2) graycal_x->center = (float)(max + min)/2.0 - 1.5;
		else graycal_x->center = (float)(max+min)/2.0-((float)listlength/2.0-0.5);
		graycal_x->cross = 0;
		//如果是1,2号灰度工作那么拐点进行PID修正，并判断是否到达拐点
		if(graytab == 0 || graytab == 1){
			float revise;
			if(graytab == 1) revise = 1;
			else revise = 0.6;
			if(graycal_x->maxlength > cross_judge|| (graycal_x -> repeat && graycal_x -> maxlength >= 3)){
				graycal_x->cross = 1;
				if(graycal_x->center >= 0.5){
					graycal_x->center = graycal_x->min_number + revise - 3.5 ;
				}
				else if(graycal_x->center <= - 0.5){
					graycal_x->center = graycal_x->max_number - revise - 3.5;
				}
		  }
			else{
				graycal_x->cross = 0;
			}	
		}
		if( maxlength == 0 || invalid == 1) graycal_x->invalid = 1;
		else graycal_x->invalid = 0;
		
}
void Read_Gray_beta(u8 grayrequest, u8 graymode){
	u8 gray0_list[8] = {0};
	u8 gray1_list[8] = {0};
	if(grayrequest == 0){
		Read_Gray0(gray0_list);
		CalculFromGray_beta(&graycal_0_beta, gray0_list, 0, graymode);	
	}
	else{
		Read_Gray1(gray1_list);
		CalculFromGray_beta(&graycal_1_beta, gray1_list, 1, graymode);	
	}
	
}
void Read_Gray(u8* grayrequest){
	u8 gray0_list[8] = {0};
	u8 gray1_list[8] = {0};
	u8 gray2_list[3] = {0};
	u8 gray3_list[4] = {0};
	if(grayrequest [0]){
		Read_Gray0(gray0_list);
		CalculFromGray(&graycal_0, gray0_list, 0);	
	}
	if(grayrequest[1]){
		Read_Gray1(gray1_list);
		CalculFromGray(&graycal_1, gray1_list, 1);	
	}
	if(grayrequest[2]){
		Read_Gray2(gray2_list);
		CalculFromGray(&graycal_2, gray2_list, 2);
	}
	if(grayrequest[3]){
		Read_Gray3(gray3_list);
		CalculFromGray(&graycal_3, gray3_list, 3);
	}
}



