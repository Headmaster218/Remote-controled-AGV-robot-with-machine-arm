/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-11-02 21:09:25
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-11-08 18:42:52
 * @FilePath: \USERd:\STM32\My Project\Balance_Car\HARDWARE\motor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef __LED_LINE_H
#define __LED_LINE_H

#define ABS(a) ((a) > 0 ? (a) : -(a))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define LIM(a, min, max) (MAX(MIN(a, max), min))



void LED_PWM_Init(unsigned short arr, unsigned short psc);


#endif
