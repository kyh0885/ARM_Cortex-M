#include "stm32f4xx.h"
#include <uart4.h>

volatile unsigned char rx1Flag = 0; 
char rx1Data[50];




void UART4_init(){
  
  GPIO_InitTypeDef   GPIO_InitStructure; // 구조체 멤버를 초기화하지 않으면 쓰레기 값이 들어가게 됨 
                                                          // 구조체 멤버의 변수를 써서 초기화했다면 아래에서 이 기능들에대해서 다시한번 언급해줘야 한다.
  USART_InitTypeDef USART_InitStructure;//USART 구조체 초기화
  NVIC_InitTypeDef   NVIC_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //여기서는 9,10번을 썻지만 만약 이미 이번호들을 다른기능(예. timmer)을 위하여 쓰고있다면 
  //Datasheet 를 통하여서 같은기능이 있는 핀번호를 찾아서 쓰기
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);     //USART1_TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);    //USART1_RX

  //인터럽트 enable 및 Priority 설정.
  // 인터럽트 컨드롤러 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //회선 enable
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; // char 자료형
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No; 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_Init(USART1, &USART_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // USART1 Interrupt enable
  USART_Cmd(USART1, ENABLE);
}

void Serial_Send(unsigned char t)
{
  USART_SendData(USART1, t);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
}

void Serial_Send_String(char* s)
{
  while( *s != '\0')
  {
   Serial_Send((unsigned char)(*s));
   s++;   //s = s + 1;
  }
}

int putchar(int ch)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,ch);
	return ch;
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
	static int i=0;
	rx1Data[i] = USART_ReceiveData(USART1);
	if(rx1Data[i] == '\r')  
	{
		rx1Data[i] = '\0';
		rx1Flag = 1;
		i = 0;
	}
	else
	{
		i++;
	}
  }
}

void print_2d1(double number)		        /* floating-point number xx.x */
{ 
        unsigned int i, j;

	j = (int)(number*10. + 0.5); //반올림하기 위해 씀
	i = j / 100;					// 10^1 -> 몫구하기
	if(i == 0) Serial_Send(' ');
	else       Serial_Send(i + '0');

	j = j % 100;					// 10^0
	i = j / 10;
	Serial_Send(i + '0');
	Serial_Send('.');

	i = j % 10;					// 10^-1
	Serial_Send(i + '0');
}