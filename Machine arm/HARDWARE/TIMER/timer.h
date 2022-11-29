#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

struct _sys_time
{
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	int total_second;
	int total_tick;
};

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
#endif
