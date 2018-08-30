#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "control.h"
#include "raspi.h"
#include "h_stm.h"
#include "ultrasonic.h"
#include "timer.h"
/************************************************
 ALIENTEK精英STM32开发板实验1
 跑马灯实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 gray_request;
int direction;
extern u8 color_list[8]; //the same as colorlist
extern u8 cam;
u8 colorlist[8] = {0, 0, 1, 2, 3, 0, 4, 0};
u8 area = 0xFF;
int circle_status = 0; //第一轮取旗为0，第二轮取旗为1
int cross_count = 0;
int leftCross_count = 0;
u8 fetch_flag = 0;
u8 put_flag = 0;

extern u8 L_flag;
extern u8 TIM4_mode;
extern u8 mode1_count;
u8 march_in_line(int direction){
	gray_request = 2;
	if(direction == 0){
		TIM4_mode = 1;
		TIM4_Int_Init(4999,7199);//500ms
		mode1_count = 0;
		TIM_Cmd(TIM4,ENABLE);
		//10是调参处
		while(mode1_count <10){
			if(mode1_count == 5)
			//5也是调参处，这里代表要降速的时刻
			Control_changeSpeed();
		Control_Straight(gray_request,direction);
		}
		
	}
	direction = 0;
	//
	while(1){
		
	}
	Control_Straight(gray_request,direction);
	delay_ms(MARCH_LINES);
	
	
}

int main(void){
	 u8 iscross;
	 delay_init();	    //延时函数初始化	  
	 Control_grayInit();
	 Control_pidInit();
	 Control_pwmInit();
	 delay_ms(200);
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	 uart1_init(9600);	 //上STM
	 uart2_init(9600);	//树莓派
	 ultrasonic_IRQ_init();
   ultrasonic_GPIO_init();
	 TIM5_Configuration(19999,71);
	 gray_request = 0;
	 direction = 0;
	

	while(1)
	{	
		
		//在初始线
		if(area == 0xFF && circle_status == 0){
			gray_request = 0;
			direction = 0;
			while(1){
				iscross = Control_Straight(gray_request,direction);
				if(iscross){
					cross_count ++ ;
					if(cross_count > 3){
						area = 0;
						cross_count = 0;
						break;
					}
				}
				else cross_count = 0;
			}
		}
		
		
		//在重要结点处并在第一轮取旗
		else if(area == 21 && circle_status == 0){
			//如果有旗
			if(colorlist[area] == 1 || colorlist[area] == 2){
				int pass_cross = 0;
				gray_request = 0;
				direction = 0;
				while(1){
					Control_Straight(gray_request,direction);
					//寻求超声波的信息，得到则停止，等待取旗完成信息
				}
				while(1){
					//寻求蓝牙停止信息
				}
				gray_request = 0;
				direction = 2;
				while(1){
					iscross = Control_Straight(gray_request, direction);
					if(iscross){
						if(cross_count > 3){
							cross_count = 0;
							break;
						}
					}
					else cross_count = 0;
				}
				Control_Rotate(0, QUARTER0);
				march_in_line(0);
				//发送放旗信息
				//等待放好信息
				while(1){
					
				}
				march_in_line(2);
				
			
		}

		
		
		//在结点且在第一轮取旗
		else if(area % 3 == 0 && circle_status == 0){
			request_colorList(area);//请求colorlist
      //###判断是否识别成功
			//有目标旗且旗子还没被拿走
			if(colorlist[area] == 1 || colorlist[area] == 2){
				gray_request = 0;
				direction = 0;
				while(1){
					iscross = Control_Straight(gray_request,direction);
					if(!iscross){
						leftCross_count ++;
						if(leftCross_count > 3){
							area = area +1;
							leftCross_count = 0;
							break;
						}
					}
					else{
						leftCross_count = 0;
					}	
			  }
			}
			//没有旗子或者旗子已经被拿走
			else{
				gray_request = 1;
				direction = 3;
				while(1){
					iscross = Control_Straight(gray_request,direction);
					if(!iscross){
						leftCross_count ++;
						if(leftCross_count > 3){
							area = area +2;
							leftCross_count = 0;
							break;
						}
					}
					else{
						leftCross_count = 0;
					}
				}					
			}
		}
		
		
		
		//在结点且在第二轮取旗
		else if(area % 3 == 0 && circle_status == 1){
			request_colorList(area);//请求colorlist
      //###判断是否识别成功
			if(colorlist[area] == 3 || colorlist[area] == 4){
				gray_request = 0;
				direction = 0;
				iscross = Control_Straight(gray_request, direction);
				if(!iscross){
					leftCross_count ++;
					if(leftCross_count > 3){
						area = area +1;
						leftCross_count = 0;
					}
				}
				else leftCross_count = 0;
			}
			else if(fetch_flag < 2){
				gray_request = 1;
				direction = 1;
				iscross = Control_Straight(gray_request, direction);
				if(!iscross){
					leftCross_count ++;
					if(leftCross_count > 3){
						area = area - 1;
						leftCross_count = 0;
					}
				}
				else leftCross_count = 0;
			}
			else{
				gray_request = 1;
				direction = 3;
				iscross = Control_Straight(gray_request, direction);
				if(!iscross){
					leftCross_count ++;
					if(leftCross_count > 3){
						area = area + 2;
						leftCross_count = 0;
					}
				}
			}
		}
		
		
		//在取旗线之间且在第一轮取旗
		else if(area % 3 ==2 && circle_status == 0 ){
			gray_request = 1 ;
			direction = 3;
		  iscross = Control_Straight(gray_request,direction);
			if(iscross){
				cross_count ++ ;
				if(cross_count > 3){
					area = area +1;
					cross_count = 0;
				}
			}
			else cross_count = 0;		
		}
		
		//在取旗线之间且在第二轮取旗
		else if(area % 3 == 2 && circle_status == 1){
			if(fetch_flag < 2) {
				gray_request = 1;
				direction = 1;
				iscross = Control_Straight(gray_request,direction);
				if(iscross){
					cross_count++;
					if(cross_count > 3){
						area = area - 2;
						cross_count = 0;
					}
				}
				else cross_count = 0;
			}
			else{
				gray_request = 1;
				direction = 3;
				iscross = Control_Straight(gray_request, direction);
				if(iscross){
					cross_count++;
					if(cross_count > 3){
						area = area + 1;
						cross_count = 0;
					}
				}
				else cross_count = 0;
			}
		}
		
		
		
		//在取旗线
		else if(area % 3 == 1){
			if(((colorlist[area] == 1 || colorlist[area] == 2 )&& circle_status == 0 )|| ((colorlist[area] == 3 || colorlist[area] == 4 ) && circle_status ==1)){
				gray_request =  0 ;
			  direction = 0 ;
				Control_Straight(gray_request,direction);
      //超声波以及蓝牙通信实行抓旗
				colorlist[area] = 0;
			}
			else{
				gray_request = 0;
				direction = 2;
				iscross = Control_Straight(gray_request,direction);
				if(iscross){
					cross_count ++ ;
					if(cross_count > 3){
						area = area - 1;
						cross_count = 0;
					}
				}
				else cross_count = 0;		
		 }			
	 }
		
			
 }
}






	
	#define ABC 
	#ifdef ABC
	while(1){
		gray_request = 0;
		direction = 0;
		iscross = Control_Straight(gray_request,direction);
		if(iscross){
			cross_count++;
		}
	}
	
	#else
	
	#endif
}