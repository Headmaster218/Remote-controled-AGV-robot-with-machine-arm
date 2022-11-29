#include "sys.h"
#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos ʹ��
#endif

void uart_init(u32 bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��USART1��GPIOAʱ��

	// USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��GPIOA.9

	// USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //��ʼ��GPIOA.10

	// Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	// USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;										//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;				//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure);	   //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //�������ڿ����ж�
	USART_Cmd(USART1, ENABLE);					   //ʹ�ܴ���1
}
u8 USART_RX_BUF[50];
void GPS_DMA_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_Initstructure;

	//ʱ��
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  //Ƕ��ͨ��ΪDMA1_Channel4_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�Ϊ 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //��Ӧ���ȼ�Ϊ 7
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ͨ���ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  //Ƕ��ͨ��ΪDMA1_Channel4_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�Ϊ 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //��Ӧ���ȼ�Ϊ 7
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ͨ���ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);

	/*����DMAʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/*DMA����*///���ڽ���dma����
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


	//����DMA���ͷ����ж�
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
