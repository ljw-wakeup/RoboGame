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
u8 gray_request;
int direction;
extern u8 color_list[8]; //the same as colorlist
extern u8 cam;
u8 colorlist[8] = {0, 0, 1, 2, 3, 0, 4, 0};
u8 area = 0xFF;
int circle_status = 0; //��һ��ȡ��Ϊ0���ڶ���ȡ��Ϊ1
int cross_count = 0;
u8 iscross;
int leftCross_count = 0;
u8 fetch_flag = 0;
u8 put_flag = 0;

extern u8 L_flag;
extern u32 ultrasonic1;




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
	
	 TIM5_Configuration(19999,71);
	 gray_request = 0;
	 direction = 0;
	
/*
	while(1)
	{	
		
		//�ڳ�ʼ��
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
		
		
		//����Ҫ��㴦���ڵ�һ��ȡ��
		else if(area == 21 && circle_status == 0){
			//�������
			if(colorlist[area] == 1 || colorlist[area] == 2){
				int pass_cross = 0;
				gray_request = 0;
				direction = 0;
				while(1){
					Control_Straight(gray_request,direction);
					//Ѱ����������Ϣ���õ���ֹͣ���ȴ�ȡ�������Ϣ
				}
				while(1){
					//Ѱ������ֹͣ��Ϣ
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
				gray_request = 1;
				direction = 3;
				while(1){
					Control_Straight(gray_request, direction);
					//������ͷ��ȡ��Ϣ�����м�λ��֮��ʼֱ�ߡ�
				}
				march_in_line();
				//���ͷ�����Ϣ
				//�ȴ��ź���Ϣ
				gray_request = 2;
				direction = 2;
				while(1){
					iscross = Control_Straight(gray_request, direction);
					if(iscross){
						cross_count ++;
						if(cross_count > 3){
							cross_count = 0;
							break;
						}
					}
					else cross_count = 0;
				}
				gray_request = 1;
				direction = 3;
				while(1){
					iscross = Control_Straight(gray_request, direction);
					if(iscross){
						cross_count ++ ;
						if(cross_count > 3){
							cross_count = 0;
							break;
						}
					}
					else cross_count = 0;
				}
				
				
			
		}

		
		
		//�ڽ�����ڵ�һ��ȡ��
		else if(area % 3 == 0 && circle_status == 0){
			request_colorList(area);//����colorlist
      //###�ж��Ƿ�ʶ��ɹ�
			//��Ŀ���������ӻ�û������
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
			//û�����ӻ��������Ѿ�������
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
		
		
		
		//�ڽ�����ڵڶ���ȡ��
		else if(area % 3 == 0 && circle_status == 1){
			request_colorList(area);//����colorlist
      //###�ж��Ƿ�ʶ��ɹ�
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
		
		
		//��ȡ����֮�����ڵ�һ��ȡ��
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
		
		//��ȡ����֮�����ڵڶ���ȡ��
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
		
		
		
		//��ȡ����
		else if(area % 3 == 1){
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
		
			
 }
}





*/
	
	#define ABC
	#ifdef ABC
	trig_ultrasonic();
	while(1){
		gray_request = 2;
		direction = 0;
		while(1){
		if(ultrasonic1 < ULTRASONIC_DISTANCE) {
			Control_Stop();
			while(1){
				if(ultrasonic1 > 1200) break;
			}
		}
		iscross = Control_Straight(gray_request,direction);
		if(iscross){
			cross_count++;
			if(cross_count > 5){
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
		while(1){
		iscross =  Control_Straight(gray_request,direction);
		if(!iscross){
			leftCross_count++;
			if(leftCross_count++ > 10){
				leftCross_count = 0;
				break;
			}
		}
		else leftCross_count = 0;
	}
}
	
	#else
		trig_ultrasonic();
		Control_test();
		while(1){
			if(ultrasonic1 < 1000){
				Control_Stop();
				while(ultrasonic1 < 1100){
					;
				}
				Control_test();
			}
		}
			

	#endif
}
