#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "control.h"
#include "raspi.h"
#include "h_stm.h"
#include "ultrasonic.h"
#include "timer.h"
/************************************************
 ALIENTEK��ӢSTM32������ʵ��1
 �����ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u8 left = 2;      //�ñ���������ʶ�ǲ�����ೡ�ء���ԭʼΪ�Ҳೡ��
u8 gray_request;
int direction;
extern u8 color_list[9]; //the same as colorlist
extern u8 cam;
u8 colorlist[8] = {0, 1, 2, 4, 3, 0, 0, 0};
u8 area= 0x7F;
int circle_status = 0; //��һ��ȡ��Ϊ0���ڶ���ȡ��Ϊ1
int cross_count = 0;
u8 iscross;
int leftCross_count = 0;
u8 fetch_flag = 0;
u8 put_flag = 0;

extern u8 L_flag;
extern u32 ultrasonic1;
extern u8 CON_grayrequest;
extern int CON_direction;
extern u8 CON_cross;

/*
//��O������ת��
void o_to_a(void){
	Control_Rotate(0, QUARTER0);
	gray_request = 1;
	direction = 1 + left;
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(!iscross){
			leftCross_count ++;
			if(leftCross_count > 5){
				leftCross_count = 0;
				break;
			}
		}
		else leftCross_count = 0;
	}
	while(1){
		Control_Straight(gray_request, direction);
			//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
	}
}


//��������a��תȦ
void a_to_o(void){
	gray_request = 1;
	direction = 3 - left;
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	Control_Rotate(1, QUARTER1);
}
void a_to_b(void){
	Control_Rotate(1, QUARTER1);
	gray_request = 0;
	direction = 0;
	while(1){
		Control_Straight(gray_request,direction);
		//Ѱ����������Ϣ���õ���ֹͣ���ȴ�ȡ�������Ϣ
	}
}

void b_to_a(void)
{
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
		gray_request = 1;
		direction = 3 - left;
		while(1){
			Control_Straight(gray_request, direction);
			//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
		}
}


void o_to_c(void){
	Control_Rotate(0, QUARTER0);
	gray_request = 1;
	direction = 3 - left;
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	while(1){
		Control_Straight(gray_request, direction);
			//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
	}
}

void o_to_b(void){
	gray_request = 0;
	direction = 0;
	while(1){
			Control_Straight(gray_request,direction);
			//Ѱ����������Ϣ���õ���ֹͣ���ȴ�ȡ�������Ϣ
	}
}


void b_to_o(void){
	gray_request= 0;
	direction = 2;
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(!iscross){
			leftCross_count++;
			if(leftCross_count > 5){
				leftCross_count = 0;
				break;
			}
		}
		else leftCross_count = 0;
	}
	while(1){
		iscross = Control_Straight(gray_request, direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	Control_Rotate(0,	QUARTER0);
}


void area_0xff(void){
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

void area_21_circle_status0(void){
	//�������
			if(colorlist[area] == 1 || colorlist[area] == 2){
				o_to_b();
				//����ץ������
				while(1){
					//Ѱ������ֹͣ��Ϣ
				}
				b_to_a();
			}
			//���û��
			else {
				o_to_a();
			}
			march_in_line();
				//���ͷ�����Ϣ
				//�ȴ��ź���Ϣ
			back_in_line();
			//����ڶ���ȡ��
			circle_status = 1;
			//����ڵڶ���ȡ�������һ��������
			if(colorlist[area] == 3 || colorlist[area] == 4){
				a_to_b();
				//����ץ��������
				while(1){
					//ץ���ӽ�����Ϣ
				}
				b_to_o();
			}
			else{
				a_to_o();
			}
			gray_request = 0;
			direction = 2;
			while(1){
				iscross = Control_Straight(gray_request, direction);
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

void area_21_circle_status1(void){
	o_to_c();
	march_in_line();
	//���ͷ�����Ϣ
	//�ȴ��ź���Ϣ
	
}

void area_atpoint_status0(void){
			request_colorList(area);//����colorlist
      //###�ж��Ƿ�ʶ��ɹ�
			//��Ŀ���������ӻ�û������
			if(colorlist[area%3] == 1 || colorlist[area%3] == 2){
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
			//û�����ӻ��������Ѿ�������
			else{
				gray_request = 1;
				direction = 3 - left;
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


void area_atpoint_status1(void){
			request_colorList(area);//����colorlist
      //###�ж��Ƿ�ʶ��ɹ�
			if(colorlist[area%3] == 3 || colorlist[area%3] == 4){
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
				direction = 1 + left;
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
				direction = 3 - left;
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

void area_betweenpoint_status0(void){
			gray_request = 1 ;
			direction = 3 - left;
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
void area_betweenpoint_status1(void){
		if(fetch_flag < 2) {
				gray_request = 1;
				direction = 1 + left;
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
				direction = 3 - left;
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

void area_fetchline(){
			if(((colorlist[area] == 1 || colorlist[area] == 2 )&& circle_status == 0 )|| ((colorlist[area] == 3 || colorlist[area] == 4 ) && circle_status ==1)){
				gray_request =  0 ;
			  direction = 0 ;
				Control_Straight(gray_request,direction);
      //�������Լ�����ͨ��ʵ��ץ��
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
*/





//��O������ת��
void o_to_a(void){
	Control_Stop();
	Control_Rotate(0, QUARTER0);
	Control_Stop();
	gray_request = 1;
	direction = 1 + left;
	Control_PID_Begin();
	delay_ms(100);
	while(1){
	  Control_Cal_Gray(gray_request,direction);
		delay_ms(5000);
		Control_Stop();
		while(1){
			
		}
			//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
	}
}


//��������a��תȦ
void a_to_o(void){
	gray_request = 1;
	direction = 3 - left;
	while(1){
		iscross = Control_Cal_Gray(gray_request,direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	Control_Rotate(1, QUARTER1);
}
void a_to_b(void){
	Control_Rotate(1, QUARTER1);
	gray_request = 0;
	direction = 0;
	while(1){
		Control_Cal_Gray(gray_request,direction);
		//Ѱ����������Ϣ���õ���ֹͣ���ȴ�ȡ�������Ϣ
	}
}

void b_to_a(void)
{
		gray_request = 0;
		direction = 2;
		while(1){
			iscross = Control_Cal_Gray(gray_request,direction);
			if(iscross){
				if(cross_count > 3){
					cross_count = 0;
					break;
				}
			}
			else cross_count = 0;
		}
		Control_Rotate(0, QUARTER0);
		gray_request = 1;
		direction = 3 - left;
		while(1){
			Control_Cal_Gray(gray_request,direction);
			//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
		}
}


void o_to_c(void){
	Control_Rotate(0, QUARTER0);
	gray_request = 1;
	direction = 3 - left;
	while(1){
		iscross = Control_Cal_Gray(gray_request,direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	while(1){
		Control_Cal_Gray(gray_request,direction);
			//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
	}
}

void o_to_b(void){
	gray_request = 0;
	direction = 0;
	while(1){
			Control_Cal_Gray(gray_request,direction);
			//Ѱ����������Ϣ���õ���ֹͣ���ȴ�ȡ�������Ϣ
	}
}


void b_to_o(void){
	gray_request= 0;
	direction = 2;
	while(1){
		iscross = Control_Cal_Gray(gray_request,direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	while(1){
		iscross = Control_Cal_Gray(gray_request,direction);
		if(!iscross){
			leftCross_count++;
			if(leftCross_count > 5){
				leftCross_count = 0;
				break;
			}
		}
		else leftCross_count = 0;
	}
	while(1){
		iscross = Control_Cal_Gray(gray_request,direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
	Control_Rotate(0,	QUARTER0);
}


void area_0x7f(void){
	CON_grayrequest = 0;
	CON_direction = 0;
	while(1){
		iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
		if(iscross){
			cross_count ++ ;
			if(cross_count > 3){
				Control_Stop();
				delay_ms(1000);
				area = 0;
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	}
}

void area_21_circle_status0(void){
	//�������
			if(colorlist[area] == 1 || colorlist[area] == 2){
				o_to_b();
				//����ץ������
				while(1){
					//Ѱ������ֹͣ��Ϣ
				}
				b_to_a();
			}
			//���û��
			else {
				o_to_a();
			}
			march_in_line();
				//���ͷ�����Ϣ
				//�ȴ��ź���Ϣ
			back_in_line();
			//����ڶ���ȡ��
			circle_status = 1;
			//����ڵڶ���ȡ�������һ��������
			if(colorlist[area] == 3 || colorlist[area] == 4){
				a_to_b();
				//����ץ��������
				while(1){
					//ץ���ӽ�����Ϣ
				}
				b_to_o();
			}
			else{
				a_to_o();
			}
			gray_request = 0;
			direction = 2;
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

void area_21_circle_status1(void){
	o_to_c();
	march_in_line();
	//���ͷ�����Ϣ
	//�ȴ��ź���Ϣ
	
}

void area_atpoint_status0(void){
			//request_colorList(area);//����colorlist
      //###�ж��Ƿ�ʶ��ɹ�
			//��Ŀ���������ӻ�û������
	    Control_PID_Begin();
	    while(colorlist[area/3] == 0){}
			if(colorlist[area/3] == 1 || colorlist[area/3] == 2){
				CON_grayrequest = 0;
				CON_direction = 0;
				delay_ms(1000);
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
			//û�����ӻ��������Ѿ�������
			else{
				CON_grayrequest = 1;
				CON_direction = 3 - left;
				delay_ms(60);
				while(1){
					iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
					if(!iscross){
						leftCross_count ++;
						if(leftCross_count > 2){
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
	    Control_PID_Begin();
	    while(colorlist[area/3] == 0){}
			if(colorlist[area%3] == 3 || colorlist[area%3] == 4){
				gray_request = 0;
				direction = 0;
				iscross = Control_Cal_Gray(gray_request,direction);
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
				direction = 1 + left;
				iscross = Control_Cal_Gray(gray_request,direction);
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
				direction = 3 - left;
				iscross = Control_Cal_Gray(gray_request,direction);
				if(!iscross){
					leftCross_count ++;
					if(leftCross_count > 3){
						area = area + 2;
						leftCross_count = 0;
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
					Control_Stop();
					delay_ms(50);
				}
			}
			else cross_count = 0;			
}
void area_betweenpoint_status1(void){
		if(fetch_flag < 2) {
				gray_request = 1;
				direction = 1 + left;
				iscross = Control_Cal_Gray(gray_request,direction);
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
				direction = 3 - left;
				iscross = Control_Cal_Gray(gray_request,direction);
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
			Control_PID_Begin();
			if(((colorlist[(area - 1) / 3] == 1 || colorlist[(area - 1) / 3] == 2 )&& circle_status == 0 )|| ((colorlist[(area - 1) / 3] == 3 || colorlist[(area - 1) / 3] == 4 ) && circle_status ==1)){
				CON_grayrequest =  0 ;
			  CON_direction = 0 ;
			  delay_ms(50);
			  while(1){
			    if(ultrasonic1 < ULTRASONIC_DISTANCE) break;
				}
				Control_Stop();
				delay_ms(500);
				colorlist[(area - 1) / 3] = 0;
			}
				/*
				 //�������Լ�����ͨ��ʵ��ץ 
				Control_Stop();
				if(colorlist[(area -1) / 3] == 1) {
					send_instruction(1);
					send_instruction(7);
				}
				else{
					send_instruction(2);
					send_instruction(8);
				}
				colorlist[(area - 1) / 3] = 0;
			}
				*/
			else{
				CON_grayrequest = 0;
				CON_direction = 2;
				while(1){
				  iscross = Control_Cal_Gray(CON_grayrequest, CON_direction);
				  if(iscross){
					  cross_count ++ ;
					  if(cross_count > 3){
						  area = area - 1;
						  cross_count = 0;
							Control_Stop();
							break;
					  }
				  }
				  else cross_count = 0;		
		   }
				
		 }
}


int main(void){

	 delay_init();	    //��ʱ������ʼ��	  
	 Control_grayInit();
	 Control_pidInit();
	 Control_pwmInit();
	 delay_ms(200);
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	 uart1_init(9600);	 //��STM
	 uart2_init(9600);	//��ݮ��
	 ultrasonic_IRQ_init();
   ultrasonic_GPIO_init();
	 TIM5_TEST_GPIO_init();
   TIM4_Int_Init(4999,71);//5ms
	 TIM5_Configuration(19999,71);//20ms
   trig_ultrasonic();
	 TIM2_Int_Init(9999,719);//100ms
//	 TIM2_Int_Init(19999,71);

	while(1)
	{	
		Control_PID_Begin();
		//�ڳ�ʼ��
		if(area == 0x7F && circle_status == 0){
			 area_0x7f();
		}
		//����Ҫ��㴦���ڵ�һ��ȡ��
		else if(area == 21 && circle_status == 0){
			area_21_circle_status0();
		}
		

		//����Ҫ��㴦���ڵڶ���ȡ��
		else if(area == 21 && circle_status == 1){
			area_21_circle_status1();
		}
		
		
		//�ڽ�����ڵ�һ��ȡ��
		else if(area % 3 == 0 && circle_status == 0){
			area_atpoint_status0();
		}
		//�ڽ�����ڵڶ���ȡ��
		else if(area % 3 == 0 && circle_status == 1){
			area_atpoint_status1();
		}
		
		//��ȡ����֮�����ڵ�һ��ȡ��
		else if(area % 3 ==2 && circle_status == 0 ){
			area_betweenpoint_status0();

		}
		
		//��ȡ����֮�����ڵڶ���ȡ��
		else if(area % 3 == 2 && circle_status == 1){
			area_betweenpoint_status1();
		}
		//��ȡ����
		else if(area % 3 == 1){
			area_fetchline();		
	 }	
   }








	
	#define AB
	#ifdef ABC
//	trig_ultrasonic();
	Control_PID_Begin();
	while(1){
		CON_grayrequest = 0;
		CON_direction = 2;
		while(1){
		iscross = Control_Cal_Gray(gray_request,direction);
		if(iscross){
			cross_count++;
			if(cross_count > 3){
				Control_Stop();
				delay_ms(500);
				delay_ms(500);
				delay_ms(500);
				delay_ms(500);
				delay_ms(500);
				cross_count = 0;
				break;
			}
		}
		else cross_count = 0;
	 }
		Control_Stop();
		while(1){
		
	}
}
	
	#else
	trig_ultrasonic();
  Control_PID_Begin();
  CON_grayrequest = 1;
  CON_direction = 3;
  delay_ms(50);
  while(1){
	  if(ultrasonic1 < ULTRASONIC_DISTANCE)break;
	}
	Control_Stop();
	while(1){
		
	}
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
