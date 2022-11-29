/*****************************************************************************
File name: TDT_Alg\src\my_math.h
Description: 酔堀方僥柴麻
Author: 怕勧
Version: 1.1.1.191112_alpha
Date: 19.11.12
History:
	！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	19.11.16 个咢采-個亟my_deathzoom痕方-盾畳劭陣匂恷寄峙氏弌噐660諒籾
	！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	19.11.12 遍肝頼撹
	！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
*****************************************************************************/
#ifndef __MY_MATH_H
#define __MY_MATH_H
#include "sys.h"
#include "math.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))
#define LIM(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define TAN_MAP_RES 0.003921569f /* (smallest non-zero value in table) */
#define RAD_PER_DEG 0.017453293f
#define TAN_MAP_SIZE 256
#define MY_PPPIII 3.14159f
#define MY_PPPIII_HALF 1.570796f

float fast_atan2(float yy, float xx);
float my_pow(float a);
float my_sqrt(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deathzoom(float xx, float zoom);
float To_180_degrees(float xx);
float Math_Max(float a, float b, float c, float d, float e, float f);

#endif
