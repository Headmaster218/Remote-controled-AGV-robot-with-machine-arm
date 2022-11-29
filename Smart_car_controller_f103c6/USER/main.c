#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "LED_line.h"

union _usart_receive_data
{
    u8 data[6];
    struct 
    {
				u8 dir;	
        u8 check;
        short distance;
        short angle;
    }data_struct;
} motor;
u8 fwlr = 0;
int i;            
 u16 a;
 int main(void)
 {	
	delay_init();	    //延时函数初始化	  
	uart_init(19200);
	LED_Init();		  	//初始化与LED连接的硬件接口
	KEY_Init();
 	//LED_PWM_Init(3601, 2);//72,000,000
	motor.data_struct.dir = 'F';
	motor.data_struct.distance = 0;
	motor.data_struct.angle =  0;
	motor.data_struct.check = motor.data_struct.dir+motor.data_struct.distance+motor.data_struct.angle;
	while(1)
	{
		//delay_ms(10);
		motor.data_struct.angle+=((u8)((GPIOA->IDR & GPIO_Pin_0)>>0) - (u8)((GPIOA->IDR & GPIO_Pin_1)>>1))*2;
		if(fwlr)
		{
		motor.data_struct.distance += (((GPIOA->IDR & GPIO_Pin_2)>>2) - ((GPIOA->IDR & GPIO_Pin_3)>>3))*7;
		motor.data_struct.dir = 'F';
		}
		else
		{
		motor.data_struct.distance += (((GPIOA->IDR & GPIO_Pin_4)>>4) - ((GPIOA->IDR & GPIO_Pin_5)>>5))*4;
		motor.data_struct.dir = 'L';
		}
		fwlr=!fwlr;
		if(motor.data_struct.distance==motor.data_struct.angle)
			continue;
		
		motor.data_struct.check = (u8)(motor.data_struct.dir+motor.data_struct.distance+motor.data_struct.angle);
		USART_SendData(USART1,0xbd);
		for(i = 0;i<6;i++)
			{
				while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
				USART_SendData(USART1,motor.data[i]);
				motor.data[i] = 0;	
			}
		
		delay_ms(7);
 }
	}
