#include "stm32f4xx.h"
#include <uart4.h>

volatile unsigned char rx1Flag = 0; 
char rx1Data[50];




void UART4_init(){
  
  GPIO_InitTypeDef   GPIO_InitStructure; // ����ü ����� �ʱ�ȭ���� ������ ������ ���� ���� �� 
                                                          // ����ü ����� ������ �Ἥ �ʱ�ȭ�ߴٸ� �Ʒ����� �� ��ɵ鿡���ؼ� �ٽ��ѹ� �������� �Ѵ�.
  USART_InitTypeDef USART_InitStructure;//USART ����ü �ʱ�ȭ
  NVIC_InitTypeDef   NVIC_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //���⼭�� 9,10���� ������ ���� �̹� �̹�ȣ���� �ٸ����(��. timmer)�� ���Ͽ� �����ִٸ� 
  //Datasheet �� ���Ͽ��� ��������� �ִ� �ɹ�ȣ�� ã�Ƽ� ����
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);     //USART1_TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);    //USART1_RX

  //���ͷ�Ʈ enable �� Priority ����.
  // ���ͷ�Ʈ ����ѷ� 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ȸ�� enable
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; // char �ڷ���
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

	j = (int)(number*10. + 0.5); //�ݿø��ϱ� ���� ��
	i = j / 100;					// 10^1 -> ���ϱ�
	if(i == 0) Serial_Send(' ');
	else       Serial_Send(i + '0');

	j = j % 100;					// 10^0
	i = j / 10;
	Serial_Send(i + '0');
	Serial_Send('.');

	i = j % 10;					// 10^-1
	Serial_Send(i + '0');
}