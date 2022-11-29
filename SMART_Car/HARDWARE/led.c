
#include "led.h"

    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口??
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽?出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度?50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据?定??初始化GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PB.5 ?出高
}
 
