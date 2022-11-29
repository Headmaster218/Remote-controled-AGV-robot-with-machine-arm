/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-11-02 19:29:17
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-11-08 19:08:24
 * @FilePath: \USER\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "stm32f10x.h"
#include "motor.h"
#include "MPU6050.h"
#include "091OLED.h"
#include "delay.h"
#include "usart.h"
#include "GPS.h"
#include "spi.h"
#include "24G.h"
#include "timer.h"
#include "led.h"


char cha;

extern struct GGA_DATA gga_data;
extern struct _Mpu_Data Mpu_Data;
int CPU_frec_tick = 0, CPU_freq = 0;
short recv_time = 0;
int nums = 0;
int main(void)
{
	LED_Init();
	delay_init();
	Soft_IIC1_Init();
//	TIM1_Int_Init(1000, 720);//72,000,000/1000/720
 uart_init(19200);
	Motor_Init();
	OLED_Init();
	//delay_ms(1500);
	while (1)
	{
		OLED_ShowNum(0, 0, nums++,6, 16);
		//delay_ms(10);
		
	}
}


// 100Hz
//定时器3中断服务程序
void TIM1_UP_IRQHandler(void) // TIM3中断
{
	//Car_Move();
	TIM1->SR = 0;
	//LED0 = !LED0;

	recv_time -= recv_time > 0 ? 1 : 0;
	if (recv_time == 0)
	{
		Motor_Set_Output(0, 'A');
		Motor_Set_Output(0, 'B');
		Motor_Set_Output(0, 'C');
		Motor_Set_Output(0, 'D');
	}

	/*
	MPU_Get_Raw_Data();
	MPU_My_Calculate();
	if (flag_OLED_refresh)
	{
		flag_OLED_refresh = 0;
		OLED_ShowNum(0, 0, Mpu_Data.temp, 4, 12);

		OLED_ShowNum(76, 0, gga_data.time[0], 2, 12);
		OLED_ShowNum(98, 0, gga_data.time[1], 2, 12);
		OLED_ShowNum(116, 0, gga_data.time[2], 2, 12);

		OLED_ShowNum(11, 1, gga_data.height, 5, 12);
		OLED_ShowNum(90, 1, gga_data.num, 2, 12);

		OLED_ShowFloat(0, 2, gga_data.lon_f, 5, 5, 12);
		OLED_ShowFloat(0, 3, gga_data.lat_f, 5, 5, 12);

		OLED_ShowNum(90, 2, gga_data.speed, 3, 12);

		OLED_ShowNum(90, 3, CPU_freq, 6, 12);
	}*/
}
