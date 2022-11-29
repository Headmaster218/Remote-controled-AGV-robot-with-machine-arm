#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

extern u16 USART_RX_STA; //接收状态标记
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

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
};









#endif
