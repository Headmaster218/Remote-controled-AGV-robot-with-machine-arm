#include "sys.h"
#include "usart.h"
#include "motor.h"


void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

extern short recv_time;
u8 recive_buf[10];
u8 recv_pack_cnt = 0;
short spd = 300;
u8 dir = 0;
void USART1_IRQHandler(void) 
{
  u8 res = 0;
  res = USART_ReceiveData(USART1);
	USART_ClearFlag(USART1,USART_FLAG_RXNE|USART_FLAG_TXE);
	recv_time = 10;
    if(res == 0x00)
      recv_pack_cnt=0;
  else
		{
			recive_buf[recv_pack_cnt++] = res;
		}
		if(recv_pack_cnt >= 7)
			{
					//Car_Rotate(usart_receive_data.data_struct.angle);
					//Car_Move_Dir(usart_receive_data.data_struct.distance,usart_receive_data.data_struct.dir);
				recv_pack_cnt =0;
			}
			
			if(recv_pack_cnt >= 4)
			{
				if(recive_buf[0] == 'z')
				{
					dir = recive_buf[1];
					if(dir == 'f')
					{
						Motor_Set_Output(spd,'A');
						Motor_Set_Output(spd,'B');
						Motor_Set_Output(spd,'C');
						Motor_Set_Output(spd,'D');
					}
					else if(dir == 'b')
					{
						Motor_Set_Output(-spd,'A');
						Motor_Set_Output(-spd,'B');
						Motor_Set_Output(-spd,'C');
						Motor_Set_Output(-spd,'D');
					}
					else if(dir == 'l')
					{
						Motor_Set_Output(-spd,'A');
						Motor_Set_Output(spd,'B');
						Motor_Set_Output(spd,'C');
						Motor_Set_Output(-spd,'D');
					}
					else if(dir == 'r')
					{
						Motor_Set_Output(spd,'A');
						Motor_Set_Output(-spd,'B');
						Motor_Set_Output(-spd,'C');
						Motor_Set_Output(spd,'D');
					}
				}
				else if(recive_buf[0] == 's')
				{
					switch (recive_buf[1])
					{
						case '0':
							spd = 0;
							break;
						case '1':
							spd = 300;
							break;
						case '2':
							spd = 600;
							break;
						case '3':
							spd = 900;
							break;
						default:
							;
					}
				}
			}
}

/*
union _usart_receive_data usart_receive_data;
u8 recv_pack_cnt = 0;
void USART1_IRQHandler(void) 
{
  u8 res = 0;
  res = USART_ReceiveData(USART1);
	USART_ClearFlag(USART1,USART_FLAG_RXNE|USART_FLAG_TXE);

    if(res == 0xbd)
      recv_pack_cnt=0;
  else
		{
			usart_receive_data.data[recv_pack_cnt++] = res;
		}
		if(recv_pack_cnt == 6)
			{
				if((u8)(usart_receive_data.data_struct.dir+usart_receive_data.data_struct.distance+usart_receive_data.data_struct.angle) == usart_receive_data.data_struct.check)
				{
					Car_Rotate(usart_receive_data.data_struct.angle);
					Car_Move_Dir(usart_receive_data.data_struct.distance,usart_receive_data.data_struct.dir);
				}
				recv_pack_cnt =0;
			}
}*/