/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮA�� 9~10������(PA9~PA10) 2�� ���̺��
        UART����� RXD, TXD �ɿ� �����Ѵ�.
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_SIZE 50
#define ARR_CNT 5  


volatile unsigned char rx1Flag = 0;
char rx1Data[50];
void UART1_Init();
void Serial_Event();
void Serial_Send(unsigned char t);
void Serial_Send_String(char* s);
int putchar(int ch);
void print_2d1(double number);

int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);     //USART1_TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);    //USART1_RX

  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // USART1 Interrupt enable
  USART_Cmd(USART1, ENABLE);

  Serial_Send_String("Start Main()\n\r"); 
  while(1)
  {
    if(rx1Flag)  // '\r' ���� �ԷµǸ�
      Serial_Event();
  }
}

void Serial_Event()
{
  int i=0;
  int num = 0;
  char * pToken;
  char * pArray[ARR_CNT]={0};
  char recvBuf[CMD_SIZE]={0};    
  strcpy(recvBuf,rx1Data);
  
  rx1Flag = 0; // �ٽ� Rflag �� 0���� ���´�.    
  Serial_Send_String(recvBuf);
  Serial_Send_String("\n\r");
  printf("rx : %s\r\n",recvBuf);
     
  pToken = strtok(recvBuf,"[@]");

  while(pToken != NULL)
  {
    pArray[i] =  pToken;
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]");
  }
       
  if(!strcmp(pArray[1],"LED"))
  {
    if(pArray != NULL)              
      num = atoi(pArray[3]);
    else
      num = 0;
    if(!strcmp(pArray[2],"ON"))
    {
     GPIO_SetBits(GPIOC, 0x01 << num); 
    }
    else 
    {
      GPIO_ResetBits(GPIOC, 0x01 << num); 
    }
  }  
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

	j = (int)(number*10. + 0.5);
	i = j / 100;					// 10^1
	if(i == 0) Serial_Send(' ');
	else       Serial_Send(i + '0');

	j = j % 100;					// 10^0
	i = j / 10;
	Serial_Send(i + '0');
	Serial_Send('.');

	i = j % 10;					// 10^-1
	Serial_Send(i + '0');
}