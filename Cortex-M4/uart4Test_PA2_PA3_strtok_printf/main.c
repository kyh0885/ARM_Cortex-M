/******************************************************************************
  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트A의 9~10번핀을(PA9~PA10) 2핀 케이블로
        UART모듈의 RXD, TXD 핀에 연결한다.
******************************************************************************/
// stm32f4xx의 각 레지스터들을 정의한 헤더파일
#include "stm32f4xx.h"
#include <uart4.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_SIZE 50
#define ARR_CNT 5  



void Serial_Event();
void Serial_Send(unsigned char t);
void Serial_Send_String(char* s); //문자열을 전송하는 함수
int putchar(int ch); //printf를 위함 함수
void print_2d1(double number); //정수두자리 소수 한자리를 위한 함수선언
void UART4_init();

extern volatile unsigned char rx1Flag; //인터럽트를위한 전역변수 선언 왜냐면 인터럽트는 메인함수에서 호출 될 수 없기 때문이다.
extern char rx1Data[50];

int main()
{ 
  GPIO_InitTypeDef   GPIO_InitStructure; 

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
  printf("test :%s\r\n",12.3);
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

