#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

enum _Arm_name
{
	TURN = 0, BIG, MEDIUM, SMALL, WRIST, PAW
};

struct _Arm
{
	enum _Arm_name Arm_name;
	u16 goal_pos[6];
	u16 now_pos[6];
	u8 step[6];
	
};

void Arm_Control(void);
void Arm_Init(void);
void Arm_Pos_Set(enum _Arm_name Arm, float angle);


#endif
