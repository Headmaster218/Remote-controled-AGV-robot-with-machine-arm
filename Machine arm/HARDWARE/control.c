#include "control.h"
#include "timer.h"
struct _Arm Arm1;
signed char arm_offset[6] = {-25,20,0,-50,0,0};
short output;
void Arm_Control()
{	
	u8 i = 0;
	while(i < 6)
	{
		if(i>3)
			Arm1.now_pos[i] = Arm1.goal_pos[i];
		else
		{
			if(Arm1.goal_pos[i] == Arm1.now_pos[i]);
				else if(Arm1.goal_pos[i] < Arm1.now_pos[i])
			{
				if(Arm1.now_pos[i]-Arm1.goal_pos[i]<Arm1.step[i]+5)
					Arm1.now_pos[i]--;
				else
					Arm1.now_pos[i]-=Arm1.step[i];
			}
			else//(Arm1.goal_pos[i] > Arm1.now_pos[i])
			{
				if(Arm1.goal_pos[i]-Arm1.now_pos[i]<Arm1.step[i]+5)
					Arm1.now_pos[i]++;
				else
					Arm1.now_pos[i]+=Arm1.step[i];
			}
		}
		i++;
	}
	
	TIM_SetCompare2(TIM3, Arm1.now_pos[TURN]);
	TIM_SetCompare4(TIM4, Arm1.now_pos[BIG]);
	TIM_SetCompare1(TIM3, Arm1.now_pos[MEDIUM]);
	TIM_SetCompare2(TIM4, Arm1.now_pos[SMALL]);
	TIM_SetCompare3(TIM4, Arm1.now_pos[WRIST]);
	TIM_SetCompare1(TIM4, Arm1.now_pos[PAW]);

}

void Arm_Pos_Set(enum _Arm_name Arm, float angle)
{
	Arm1.goal_pos[Arm] = angle*5.333333333333333 + 700 + arm_offset[Arm];
	if(Arm == PAW)
	{
		Arm1.goal_pos[PAW] = MIN(1000,Arm1.goal_pos[Arm]);
		Arm1.goal_pos[PAW] = MAX( 450,Arm1.goal_pos[Arm]);
	}
	else
	{
		Arm1.goal_pos[Arm] = MIN(1200,Arm1.goal_pos[Arm]);
		Arm1.goal_pos[Arm] = MAX( 200,Arm1.goal_pos[Arm]);
	}
}

void Arm_Init()
{
	u8 i = 0;
	Arm1.step[0] = 7;
	Arm1.step[1] = 5;
	Arm1.step[2] = 13;
	Arm1.step[3] = 20;
	TIM3_PWM_Init(10000-1,144);
	while(i < 6)
	{
		Arm1.now_pos[i] = 700;
		Arm_Pos_Set(i, 0);
		i++;
	}
}
