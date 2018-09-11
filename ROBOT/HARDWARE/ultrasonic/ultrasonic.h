#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H 
#include "led.h"
#include "usart.h"
#include "sys.h"
#include "delay.h"


#define ULTRASONIC_TRIGTIME 5

void EXTI15_10_IRQHandler(void);
void EXTI4_IRQHandler(void);
void ultrasonic_IRQ_init(void);
void ultrasonic_GPIO_init(void);
void TIM5_Configuration(u16 arr,u16 psc);
void TIM5_NVIC_Configuration(void);
void trig_ultrasonic(void);
void get_ultrasonic(void);
void stop_ultrasonic(void);
void TIM5_TEST_GPIO_init(void);
#endif
