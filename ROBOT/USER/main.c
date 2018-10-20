#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "control.h"
#include "raspi.h"
#include "h_stm.h"
#include "ultrasonic.h"
#include "timer.h"
#define ONSTM
/************************************************
 ALIENTEK精英STM32开发板实验1
 跑马灯实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 left = 2; 
//该变量用来标识是不是左侧场地、、原始为右侧场地
u8 gray_request;
int direction;
extern u8 color_list[9]; //the same as colorlist
extern u8 cam;
int colorlist[9] = {7,8,5,6,1,2,3,4,1};
u8 area= 0x7f;
int circle_status = 0; //第一轮取旗为0，第二轮取旗为1
int cross_count = 0;
u8 iscross;
int leftCross_count = 0;
u8 fetch_flag = 0;
u8 put_flag = 0;
int ultra_count = 0;
extern u8 ultra_flag;


extern u8 L_flag;
extern u32 ultrasonic1;
extern u8 CON_grayrequest;
extern int CON_direction;
extern u8 CON_cross;
//return 0: left fetch
void send_first(u8 circle){
	u8 a,b,i;
	if(left == 0 && circle == 0){
		for(i = 0; i < 8; i++){
			if(colorlist[i]==7) a = i;
			else if(colorlist[i] == 8) b = i;
		}
		if(a < b )send_instruction(1);
		else send_instruction(3);
	}
	if(left == 0 && circle == 1){
		for(i = 0; i < 8; i++){
			if(colorlist[i]==5) a = i;
			else if(colorlist[i] == 6) b = i;
		}
		if( a > b )send_instruction(1);
		else send_instruction(3);
	}
	if(left == 2 && circle == 0){
		for(i = 0; i < 8; i++){
			if(colorlist[i]==1) a = i;
			else if(colorlist[i] == 2) b = i;
		}
		if( a < b )send_instruction(1);
		else send_instruction(3);
	}
	if(left == 2 && circle == 1){
		for(i = 0; i < 8; i++){
			if(colorlist[i]==3) a = i;
			else if(colorlist[i] == 4) b = i;
		}
		if( a > b )send_instruction(1);
		else send_instruction(3);
	}
}
void detect_distance(){
	int count = 0;
	while(1){
		if(PCin(3) == 0){
			count ++ ;
			if(count >= 100){
				count = 0;
				return
					;
			}
		}
		else count = 0 ;
	}
}

void b_to_o_beta(void){
	CON_direction = 2;
	CON_grayrequest = 0;
	Control_PID_Begin();
	while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
				  break;
			  }
		  }
		  else cross_count = 0;
	 }
	while(1){
		iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		if(!iscross){
			leftCross_count ++ ;
			if(leftCross_count > 5){
				leftCross_count = 0;
				break;
			}
		}
		else leftCross_count = 0;
	}
  while(1){
		iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		if(iscross){
			cross_count ++;
			if(cross_count > 3){
				cross_count = 0;
				area = 21;
				Control_Stop();
				#ifdef ONSTM
				if(colorlist[(area) / 3] == -2) send_instruction(3);
				else {
					send_instruction(1);
				 } 
				#endif
				break;
			}
		}
		else cross_count = 0;
	}
}


void o_to_c_beta(void){
  int adjustment[4] = {0, 0, 0, 0};
	if(left == 2){
		CON_direction = 2;
		CON_grayrequest = 0;
		Control_Set_PID(2, -1.0);
		Control_Reset_PID();
		Control_PID_Begin();
		delay_ms(700);
		while(1){
			iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			if(!iscross){
				leftCross_count ++ ;
				if(leftCross_count > 3){
					leftCross_count= 0;
					break;
				}
				
			}
			else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
		delay_ms(400);
		while(1){
			iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			if(!iscross){
				leftCross_count ++ ;
				if(leftCross_count > 3){
					leftCross_count= 0;
					break;
				}
				
			}
			else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
					Control_Stop();
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
	}
	else{
		Control_Stop();
	  Control_Rotate(0, QUARTER0);
		Control_Stop_only();
		CON_direction = 0;
		CON_grayrequest = 0;
		Control_PID_Begin();
		delay_ms(600);
		while(1){
			iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			if(!iscross){
				leftCross_count ++ ;
				if(leftCross_count > 10){
					leftCross_count= 0;
					break;
				}
				
			}
			else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
					Control_Stop();
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
	}
	CON_direction = 1;
	CON_grayrequest = 1;
	Control_Set_PID(1, - 1.5);
	 Control_PID_Begin();
	  delay_ms(1000);
	  delay_ms(1000);
	  Control_Stop();
	  Control_rotate_beta(1, 10000);
	  Control_Stop_only();
	 #ifdef ONSTM
	  send_instruction(6);
	#endif
	  Control_Begin_Only(3, adjustment);
	  delay_ms(1000);
    delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
	  Control_Stop();
	  send_instruction(7);
	  while(1){}
}
void o_to_c_beta_high(void){
  int adjustment[4] = {0, 0, 0, 0};
	if(left == 2){
		CON_direction = 2;
		CON_grayrequest = 0;
		Control_Set_PID(2, -1.0);
		Control_Reset_PID();
		Control_PID_Begin();
		delay_ms(700);
		while(1){
			iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			if(!iscross){
				leftCross_count ++ ;
				if(leftCross_count > 3){
					leftCross_count= 0;
					break;
				}
				
			}
			else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
		delay_ms(400);
		while(1){
			iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			if(!iscross){
				leftCross_count ++ ;
				if(leftCross_count > 3){
					leftCross_count= 0;
					break;
				}
				
			}
			else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
	}
	else{
		CON_direction = 0;
		CON_grayrequest = 0;
		Control_PID_Begin();
		delay_ms(400);
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 3){
				  cross_count = 0;
					Control_Stop();
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
	}
	CON_direction = 1;
	CON_grayrequest = 1;
	Control_Set_PID(1, - 0.5);
	 Control_PID_Begin();
	  delay_ms(1000);
	  delay_ms(1000);
	  Control_Stop();
	  Control_rotate_beta(1, 10000);
	  Control_Stop_only();
	  #ifdef ONSTM
	  send_instruction(6);
	#endif
	  Control_Begin_Only(3, adjustment);
	  delay_ms(1000);
    delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
	  Control_Stop();
	  send_instruction(7);
	  while(1){}
}


void low_beta(void){
	  CON_grayrequest = 1;
	  CON_direction = 1;
	  Control_PID_Begin();
    Control_Reset_PID();
	  Control_Set_PID(1, - 2.0);
	  delay_ms(1500);
	  Control_Stop_only();
	  Control_rotate_beta(1, 10000);
	  Control_Stop_only();
	  Control_Set_PID(1, 0.5); 
	  #ifdef ONSTM
	  send_instruction(5);
	  fetch_flag = 0 ;
	  #endif
	  //发送放旗信息
	 //等待放好信息
	  Control_rotate_beta(0,2000);
	  Control_Stop_only();
	  CON_grayrequest = 1;
	  CON_direction = 3;
	  Control_PID_Begin();
	  while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 5){
				  cross_count = 0;
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
}



void low_to_o_beta(void){
	if(left == 0){
		Control_Stop();
		Control_Rotate(1, QUARTER1);
		Control_Stop_only();
		CON_direction = 2;
		CON_grayrequest = 0;
		Control_PID_Begin();
		delay_ms(800);
		while(1){
			  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				leftCross_count ++ ;
			  if(!iscross){
				  if(leftCross_count > 10){
					  leftCross_count = 0;
					  break;
				  }
			  }
			  else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 5){
				  cross_count = 0;
					Control_Stop();
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
	}
}



void o_to_low_beta(void){
  if(left == 0){
		Control_Stop();
		CON_direction = 0;
		CON_grayrequest = 0;
		Control_Set_PID(0, 1.0);
		Control_PID_Begin();
		delay_ms(400);
			while(1){
			  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				leftCross_count ++ ;
			  if(!iscross){
				  if(leftCross_count > 10){
					  leftCross_count = 0;
					  break;
				  }
			  }
			  else leftCross_count = 0;
		}
		while(1){
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		  if(iscross){
			  cross_count++;
			  if(cross_count > 5){
				  cross_count = 0;
				  break;
			  }
		  }
		  else cross_count = 0;
	  }
		Control_Stop();
		Control_Set_PID(0, 0.0);
		Control_Rotate(0, QUARTER0);
		Control_Stop_only();
	}
}


void b_to_low_beta(void)
{
		CON_grayrequest = 0;
		CON_direction = 2;
	  if(left == 0)  Control_Set_PID(2, 2.5);
	  else Control_Set_PID(2, -1.0);
	  Control_PID_Begin();
		while(1){
			iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			cross_count ++ ;
			if(iscross){
				if(cross_count > 3){
					cross_count = 0;
					break;
				}
			}
			else cross_count = 0;
		}
		if(left == 2){
			while(1){
			  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				leftCross_count ++ ;
			  if(!iscross){
				  if(leftCross_count > 5){
					  leftCross_count = 0;
					  break;
				  }
			  }
			  else leftCross_count = 0;
		  }
			while(1){
			  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				cross_count ++ ;
			  if(iscross){
				  if(cross_count > 3){
					  cross_count = 0;
					  break;
				  }
			  }
			  else cross_count = 0;
		  }
		}
		else{
			Control_Stop();
			Control_Rotate(1,	QUARTER1);
			Control_Rotate(1,	QUARTER1);
			Control_Stop_only();
		}
		
}





void o_to_b_beta(void){
	CON_grayrequest = 0;
	CON_direction = 0;
	Control_Reset_PID();
	 if(colorlist[(area ) / 3] == 6 || colorlist[(area ) / 3] == 4 || colorlist[(area ) / 3] == 2  || colorlist[(area ) / 3] == 8){
			Control_Set_PID(0, 0.0);
	 }
	else  Control_Set_PID(0, 0.0); 
  detect_distance();
	Control_Stop();
	#ifdef ONSTM
  if(colorlist[(area) / 3] == 5 - left * 2 || colorlist[(area ) / 3] == 7 - left * 2) {
		send_instruction(2);
		fetch_flag++;
	}
	else{
		send_instruction(4);
		fetch_flag++;
	}
	#endif
	if(colorlist[(area ) / 3] == 6 || colorlist[(area) / 3] == 2|| colorlist[(area ) / 3] == 4 || colorlist[(area ) / 3] == 8){
		colorlist[(area ) / 3] = -1;
   }
	else {
	    colorlist[(area) / 3] = -2;   
	} 
	Control_Set_PID(0, 0.0);
}



void low_to_b_beta(void){
	if(left == 0){
		Control_Stop();
		Control_Rotate(0,	QUARTER0);
	
	}
	Control_Stop_only();
	CON_direction = 0;
	CON_grayrequest = 0;
	 if(colorlist[(area) / 3] == 6 || colorlist[(area ) / 3] == 4 || colorlist[(area) / 3] == 2  || colorlist[(area ) / 3] == 8){
			Control_Set_PID(0, 0.0);
	 }
	else  Control_Set_PID(0, 0.0); 
	Control_PID_Begin();
//	detect_distance();
	 delay_ms(1000);
	 delay_ms(1000);
	Control_Stop();
#	ifdef ONSTM
  if(colorlist[(area) / 3] == 5 - left * 2 || colorlist[(area) / 3] == 7 - left * 2) {
		send_instruction(2);
		fetch_flag ++;
	}
	else{
		send_instruction(4);
		fetch_flag ++;
	}
#else
	fetch_flag ++ ;
# endif
	if(colorlist[(area) / 3] == 6 || colorlist[(area ) / 3] == 2|| colorlist[(area ) / 3] == 4 || colorlist[(area ) / 3] == 8) colorlist[(area ) / 3] = -1;
	else colorlist[(area) / 3] = -2;
	Control_Set_PID(0, 0.0);
	Control_Reset_PID();
}


void area_0x7f(void){
	CON_grayrequest = 0;
	CON_direction = 0;
	while(1){
		iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		if(iscross){
			cross_count ++ ;
			if(cross_count > 3){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	while(1){
		iscross =  Control_Cal_Gray(CON_grayrequest, CON_direction);
		if(!iscross){
			leftCross_count++;
			if(leftCross_count > 10){
				leftCross_count = 0;
				break;
			}
		}
		else leftCross_count = 0;
	}
	while(1){
		iscross =  Control_Cal_Gray(CON_grayrequest, CON_direction);
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

void area_21_circle_status0_beta(void){
	//如果有旗
			if(colorlist[area/3] == 7 - left * 3 || colorlist[area/3] == 8 - left * 3){
				o_to_b_beta();
				b_to_low_beta();
			}
			//如果没旗
			else o_to_low_beta();
			low_beta();
			
			//进入第二轮取旗
			circle_status = 1;
			#ifdef ONSTM
		  send_first(1);
		  #endif
			fetch_flag = 0;
			//如果在第二轮取旗在最后一条有旗子
			if(colorlist[area/3] == 5 - left || colorlist[area/3] == 6- left){
				low_to_b_beta();
				b_to_o_beta();
			}
			else low_to_o_beta();
			CON_grayrequest = 1;
			CON_direction = 1 + left;
			Control_Set_PID(1, 0.5); 
			Control_Set_PID(3, 0.5); 
			Control_PID_Begin();
			delay_ms(300);
			while(1){
				iscross =Control_Cal_Gray(gray_request,direction);
				if(!iscross){
					leftCross_count ++;
					if(leftCross_count > 3){
						area = 20;
						circle_status = 1;
						leftCross_count = 0;
						break;
					}
				}
				else leftCross_count = 0;
			}		
}




void area_21_circle_status1_beta(void){
	o_to_c_beta();
}

void area_atpoint_status0(void){
			if(colorlist[area/3] == 7 -  3*left || colorlist[area/3] == 8 - 3 * left){
				if(area != 0 || (area == 0 && circle_status == 1)){
			   	Control_Stop();
				}
				CON_grayrequest = 0;
				CON_direction = 0;
				Control_Straight_only(0);
				delay_ms(300);
				Control_PID_Begin();
				while(1){
					iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
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
				CON_grayrequest = 1;
				CON_direction = 3 - left;
				delay_ms(500);
				while(1){
					iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
					if(!iscross){
						leftCross_count ++;
						if(leftCross_count > 10){
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


void area_atpoint_status1(void){
			if(colorlist[area/3] == 5 - left || colorlist[area/3] == 6 - left ){
				Control_Stop();
				CON_grayrequest = 0;
				CON_direction = 0;
				Control_Straight_only(0);
				delay_ms(400);
				Control_PID_Begin();
				while(1){
				  iscross = Control_Cal_Gray(CON_grayrequest,CON_direction);
				  if(!iscross){
					  leftCross_count ++;
					  if(leftCross_count > 3){
						  area = area +1;
						  leftCross_count = 0;
							break;
					  }
				  }
				  else leftCross_count = 0;
			    }
				}
			else if(fetch_flag < 2){
				CON_grayrequest = 1;
				CON_direction = 1 + left;
				delay_ms(400);
				while(1){			
					iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				  if(!iscross){
					  leftCross_count ++;
					  if(leftCross_count > 3){
						  area = area - 1;
						  leftCross_count = 0;
							break;
					  }
			 	  }
				  else leftCross_count = 0;
			  }
		  }
			else{
				CON_grayrequest = 1;
				CON_direction = 3 - left;
				delay_ms(400);
				while(1){
				  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				  if(!iscross){
					  leftCross_count ++;
					  if(leftCross_count > 3){
						  area = area + 2;
						  leftCross_count = 0;
							break;
					  }
				  }
			  }
      }
}

void area_betweenpoint_status0(void){
			CON_grayrequest = 1 ;
			CON_direction = 3 - left;
		  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
			if(iscross){
				cross_count ++ ;
				if(cross_count > 3){
					area = area +1;
					cross_count = 0;
				}
			}
			else cross_count = 0;			
}
void area_betweenpoint_status1(void){
		if(fetch_flag < 2) {
				CON_grayrequest = 1;
				CON_direction = 1 + left;
				iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
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
				CON_grayrequest = 1;
				CON_direction = 3 - left;
				iscross = Control_Cal_Gray(CON_grayrequest,CON_direction);
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

void area_fetchline(){

			if(((colorlist[(area - 1) / 3] == 7 - left * 3 || colorlist[(area - 1) / 3] == 8 - left * 3 )&& circle_status == 0 )|| ((colorlist[(area - 1) / 3] == 5 - left || colorlist[(area - 1) / 3] == 6 - left ) && circle_status ==1)){
				
				CON_grayrequest =  0 ;
			  CON_direction = 0 ;
			  
				
			 if(colorlist[(area - 1) / 3] == 6 || colorlist[(area - 1) / 3] == 4 || colorlist[(area - 1) / 3] == 2  || colorlist[(area - 1) / 3] == 8){
					 Control_Reset_PID();
				   Control_Set_PID(0, 0.0);
				}
				else {
						Control_Reset_PID(); 
						Control_Set_PID(0, 0.0); 
				}
				Control_PID_Begin();
			  delay_ms(400);
				delay_ms(600);
				delay_ms(800);
			  //detect_distance();
				Control_Stop();
				#ifdef ONSTM
				if(colorlist[(area -1) / 3] == 5 - left * 2 || colorlist[(area -1) / 3] == 7 - left * 2) {
					send_instruction(2);
					fetch_flag++;
					
				}
				else{
					send_instruction(4);
					fetch_flag++;
				}
				#else 
				fetch_flag ++ ;
				#endif
	
				if(colorlist[(area - 1) / 3] == 6 || colorlist[(area - 1) / 3] == 2|| colorlist[(area - 1) / 3] == 4 || colorlist[(area - 1) / 3] == 8) colorlist[(area - 1) / 3] = -1;
				else {colorlist[(area - 1) / 3] = -2; }
				
			}
			else{
				//Control_Reset_PID();
				Control_Set_PID(0, 0.0);
				if(colorlist[(area - 1) / 3] == -2) Control_Set_PID(2, 0.0);
				else {
					Control_Set_PID(2, 0.0);
				}
				CON_grayrequest = 0;
				CON_direction = 2;
				while(1){
				  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				  if(iscross){
					  cross_count ++ ;
					  if(cross_count > 3){
						  area = area - 1;
						  cross_count = 0;
							Control_Set_PID(2, 0.0);
							Control_Stop();
							#ifdef ONSTM
							if(colorlist[(area) / 3] == -2) {
								send_instruction(3);
							}
							
				      else {
					      send_instruction(1);
				      } 
							#endif
							break;
							
					  }
				  }
				  else cross_count = 0;		
		   }
		 }
}

void change_colorlist(void){
	u8 i, a,b,c,d, temp;
	if(left == 0){
		for(i = 0; i < 8; i++){
			if(colorlist[i] == 5) a = i;
			else if(colorlist[i] == 6) b = i;
			else if(colorlist[i] == 7) c = i;
			else if(colorlist[i] == 8) d = i;
		}
		colorlist[a] = 7;
		colorlist[b]= 8;
		colorlist[c] = 5;
		colorlist[d] = 6;
	}
	else{
		for(i = 0; i < 8; i++){
			if(colorlist[i] == 1) a = i;
			else if(colorlist[i] == 2) b = i;
			else if(colorlist[i] == 3) c = i;
			else if(colorlist[i] == 4) d = i;
		}
		colorlist[a] = 3;
		colorlist[b]= 4;
		colorlist[c] = 1;
		colorlist[d] = 2;
	}
}

int main(void){

	 delay_init();	    //延时函数初始化	  
	 Control_grayInit();
	 Control_pidInit();
	 Control_pwmInit();
	
	 delay_ms(200);
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	 uart1_init(9600);	 //上STM
	 uart2_init(9600);	//树莓派
//	 ultrasonic_IRQ_init();
//   ultrasonic_GPIO_init();
	 TIM5_TEST_GPIO_init();
   TIM4_Int_Init(1999,71);//0.1ms
//	 TIM5_Configuration(19999,71);//20ms
	 TIM2_Int_Init(9999,719);//100ms
//	 trig_ultrasonic();
//	Control_Stop();
//	 TIM2_Int_Init(19999,71);


	while(1){
			if(colorlist[8] != 0) break;
	}
	if(colorlist[8] == 1){
		#ifdef ONSTM
	  send_first(0);
		#endif
	  while(1)
	  {	
		  Control_PID_Begin();
		  //在初始线
			if(area % 3 == 0 && circle_status == 1){
				Control_Stop();
				delay_ms(1000);
				Control_PID_Begin();
			}
		  if(area == 0x7F && circle_status == 0){
			   area_0x7f();
		  }
		  //在重要结点处并在第一轮取旗
		  else if(area == 21 && circle_status == 0){
			  area_21_circle_status0_beta();
		  }
		

		  //在重要结点处并在第二轮取旗
		  else if(area == 21 && circle_status == 1){
			  area_21_circle_status1_beta();
		  }
		
		
		  //在结点且在第一轮取旗
		  else if(area % 3 == 0 && circle_status == 0){
		 	  area_atpoint_status0();
		  }
		  //在结点且在第二轮取旗
		  else if(area % 3 == 0 && circle_status == 1){
			  area_atpoint_status1();
		  }
		  
		  //在取旗线之间且在第一轮取旗
		  else if(area % 3 ==2 && circle_status == 0 ){
			  area_betweenpoint_status0();

		  }
		
		  //在取旗线之间且在第二轮取旗
		  else if(area % 3 == 2 && circle_status == 1){
			  area_betweenpoint_status1();
		  }
		  //在取旗线
		  else if(area % 3 == 1){
			  area_fetchline();		
	    } 	
     }
	 }
	else{
		change_colorlist();
		#ifdef ONSTM
		send_first(0);
		#endif
	  while(1)
	  {	
		  Control_PID_Begin();
		  //在初始线
		  if(area == 0x7F && circle_status == 0){
			   area_0x7f();
		  }
		  //在重要结点处并在第一轮取旗
		  else if(area == 21 && circle_status == 0){
				if(colorlist[area/3] == 7 - left * 3 || colorlist[area/3] == 8 - left * 3){
				   o_to_b_beta();
				   b_to_low_beta();
			  }
				if(left == 0){
		      CON_direction = 0;
		      CON_grayrequest = 0;
		      Control_PID_Begin();
		      delay_ms(300);
		      while(1){
		        iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		        if(iscross){
			        cross_count++;
			        if(cross_count > 5){
				         cross_count = 0;
				         break;
			        }
		        }
		        else cross_count = 0;
	        }
	      }
			 area_21_circle_status0_beta();
		  }
		

		  //在重要结点处并在第二轮取旗
		  else if(area == 21 && circle_status == 1){
			  area_21_circle_status1_beta();
		  }
		
		
		  //在结点且在第一轮取旗
		  else if(area % 3 == 0 && circle_status == 0){
		 	  area_atpoint_status0();
		  }
		  //在结点且在第二轮取旗
		  else if(area % 3 == 0 && circle_status == 1){
			  area_atpoint_status1();
		  }
		  
		  //在取旗线之间且在第一轮取旗
		  else if(area % 3 ==2 && circle_status == 0 ){
			  area_betweenpoint_status0();

		  }
		
		  //在取旗线之间且在第二轮取旗
		  else if(area % 3 == 2 && circle_status == 1){
			  area_betweenpoint_status1();
		  }
		  //在取旗线
		  else if(area % 3 == 1){
			  area_fetchline();		
	    } 	
     }
		
	}


	#define ABC
	#ifdef ABc
	//这里是基值设置在这个函数里，可以看到宏定义，如果两个轮子基准值不同，就自己添加宏定义

 Control_pwmSet(1, 3, 0);
CON_grayrequest = 1;
CON_direction = 3;
Control_PID_Begin();
while(1){}

while(1){}
	
	
	//转弯参数测试：第一个参数是方向。1顺时针0逆时针，第二个参数是转的时间。QUARTER0对应的是方向0的宏定义，QUARTER1对应的是方向1的宏定义，宏定义在control.h里面
	
	//直走参数测试：参数是方向 

	
	
	
	
	#else
send_instruction(1);
delay_ms(1000);
send_instruction(2);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
send_instruction(3);
delay_ms(1000);
send_instruction(4);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
send_instruction(5);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
send_instruction(6);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
send_instruction(7);
delay_ms(1000);
Control_test(2);
delay_ms(1000);
area_fetchline();
while(1){}
/*	#else 
	trig_ultrasonic();
	while(1){
		Control_test();
		if(ultrasonic1 < ULTRASONIC_DISTANCE){
			Control_Stop();
			while(1){
				if(ultrasonic1 >1200) break;
			}
		}
	}
	*/
	#endif
}
