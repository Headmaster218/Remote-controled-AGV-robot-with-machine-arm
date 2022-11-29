#include "sys.h"
#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos 使用
#endif

void uart_init(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟

	// USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化GPIOA.9

	// USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //初始化GPIOA.10

	// Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器

	// USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;										//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;				//收发模式

	USART_Init(USART1, &USART_InitStructure);	   //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口空闲中断
	USART_Cmd(USART1, ENABLE);					   //使能串口1
}
u8 USART_RX_BUF[50];
void GPS_DMA_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_Initstructure;

	//时钟
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  //嵌套通道为DMA1_Channel4_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //响应优先级为 7
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //通道中断使能
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  //嵌套通道为DMA1_Channel4_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //响应优先级为 7
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //通道中断使能
	NVIC_Init(&NVIC_InitStructure);

	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/*DMA配置*///串口接收dma配置
	DMA_Initstructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	DMA_Initstructure.DMA_MemoryBaseAddr = (u32)USART_RX_BUF;
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC; // dev to mem
	DMA_Initstructure.DMA_BufferSize = 50;
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;
	DMA_Initstructure.DMA_Priority = DMA_Priority_High;
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_Initstructure);


	//开启DMA发送发成中断
	USART_Cmd(USART1, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_ClearFlag(DMA1_FLAG_HT5);

	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

u8 USART_RX_STA = 0, temppointer = 0, flag_OLED_refresh = 0;


void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))
	{
	PCout(13) = 0;
	delay_ms(100);
	PCout(13) = 1;
	}
	USART1->SR;
	USART1->DR;
	USART_ClearFlag(USART1,USART_FLAG_RXNE|USART_FLAG_TXE);
}
