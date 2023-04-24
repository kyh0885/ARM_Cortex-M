/******************************************************************************
  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트A의 9~10번핀을(PA9~PA10) 2핀 케이블로
        UART모듈의 RXD, TXD 핀에 연결한다.
******************************************************************************/
// stm32f4xx의 각 레지스터들을 정의한 헤더파일
#include "stm32f4xx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_SIZE 50
#define ARR_CNT 5  


volatile unsigned char rx1Flag = 0; //인터럽트를위한 전역변수 선언 왜냐면 인터럽트는 메인함수에서 호출 될 수 없기 때문이다.
char rx1Data[50];
void Serial_Event();
void Serial_Send(unsigned char t);
void Serial_Send_String(char* s); //문자열을 전송하는 함수
int putchar(int ch); //printf를 위함 함수
void print_2d1(double number); //정수두자리 소수 한자리를 위한 함수선언

int main()
{ 
  GPIO_InitTypeDef   GPIO_InitStructure; // 구조체 멤버를 초기화하지 않으면 쓰레기 값이 들어가게 됨 
                                                          // 구조체 멤버의 변수를 써서 초기화했다면 아래에서 이 기능들에대해서 다시한번 언급해줘야 한다.
  USART_InitTypeDef USART_InitStructure;//USART 구조체 초기화
  NVIC_InitTypeDef   NVIC_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //출력을 하기위해  OUT 으로 변경
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push,pull 방식으로 0 아니면 1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100M 는 고속이지만, 노이즈 발생할 수 있음
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //외부에 저항이 없다면  내부의 저항을,,
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                            GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  //여기부터는 부가기능 (UART 장치를 이용)
  // UART는 GPIO 는 아니지만 외부의 핀을 이용하기 위해서는 이 과정이 필요함
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //여기서는 9,10번을 썻지만 만약 이미 이번호들을 다른기능(예. timmer)을 위하여 쓰고있다면 
  //Datasheet 를 통하여서 같은기능이 있는 핀번호를 찾아서 쓰기
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);     //USART1_TX
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);    //USART1_RX

  //인터럽트 enable 및 Priority 설정.
  // 인터럽트 컨드롤러 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
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
  USART_Init(USART3, &USART_InitStructure);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // USART1 Interrupt enable
  USART_Cmd(USART3, ENABLE);

  Serial_Send_String("Start Main()\n\r"); //포인터
  while(1)
  {
    if(rx1Flag)  // '\r' 까지 입력되면
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

  rx1Flag = 0; // 다시 Rflag 를 0으로 놓는다.    
  Serial_Send_String(recvBuf);
  Serial_Send_String("\n\r");
  printf("rx : %s\r\n",recvBuf);
  print_2d1(12.3);
  pToken = strtok(recvBuf,"[@]"); //얘기 분리했던 주소값을 리턴한다

  while(pToken != NULL) //pArra[0] 은 [KSH_ARM]
  {
    pArray[i] =  pToken; 
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]"); //계속 문자열을 분리하겠다
  }
       
  if(!strcmp(pArray[1],"LED"))
  {
    if(pArray[3] != NULL) // 문자열을 숫자로 바꾸자             
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
  USART_SendData(USART3, t);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
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
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3,ch);
	return ch;
}

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
	static int i=0;
	rx1Data[i] = USART_ReceiveData(USART3);
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