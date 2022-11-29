#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "control.h"


extern struct _Arm Arm1;
float angle = 0;
u8 name = 1;
u8 time = 0;
 int main(void)
 {		
	LED_Init();
	delay_init();	    	 //延时函数初始化	  
	Arm_Init();
	 
	delay_ms(1000);
	Arm_Pos_Set(TURN,70);
		while(1)
		{
			delay_ms(100);
			time++;
			if(time == 0)
			{
				Arm_Pos_Set(BIG ,30);
			}
			else if(time == 30)
			{
				Arm_Pos_Set(MEDIUM ,-30);
			}
			
			else if(time == 60)
			{
				Arm_Pos_Set(SMALL ,-30);
			}
			
			else if(time == 90)
			{
				Arm_Pos_Set(BIG ,-10);
			}
			else if(time == 120)
			{
				Arm_Pos_Set(MEDIUM ,-10);
			}
			else if(time == 150)
			{
				Arm_Pos_Set(SMALL ,30);
			}
		}
 }

 
 
 
