/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮA�� 9~10������(PA9~PA10) 2�� ���̺��
        UART����� RXD, TXD �ɿ� �����Ѵ�.
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"
#include <uart4.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_SIZE 50
#define ARR_CNT 5  



void Serial_Event();
void Serial_Send(unsigned char t);
void Serial_Send_String(char* s); //���ڿ��� �����ϴ� �Լ�
int putchar(int ch); //printf�� ���� �Լ�
void print_2d1(double number); //�������ڸ� �Ҽ� ���ڸ��� ���� �Լ�����
void UART4_init();

extern volatile unsigned char rx1Flag; //���ͷ�Ʈ������ �������� ���� �ֳĸ� ���ͷ�Ʈ�� �����Լ����� ȣ�� �� �� ���� �����̴�.
extern char rx1Data[50];

int main()
{ 
  GPIO_InitTypeDef   GPIO_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //����� �ϱ�����  OUT ���� ����
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push,pull ������� 0 �ƴϸ� 1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100M �� ���������, ������ �߻��� �� ����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //�ܺο� ������ ���ٸ�  ������ ������,,
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                            GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  //������ʹ� �ΰ���� (UART ��ġ�� �̿�)
  // UART�� GPIO �� �ƴ����� �ܺ��� ���� �̿��ϱ� ���ؼ��� �� ������ �ʿ���
 

  Serial_Send_String("Start Main()\n\r"); //������
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
  printf("test :%s\r\n",12.3);
  print_2d1(12.3);
  pToken = strtok(recvBuf,"[@]"); //��� �и��ߴ� �ּҰ��� �����Ѵ�

  while(pToken != NULL) //pArra[0] �� [KSH_ARM]
  {
    pArray[i] =  pToken; 
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]"); //��� ���ڿ��� �и��ϰڴ�
  }
       
  if(!strcmp(pArray[1],"LED"))
  {
    if(pArray[3] != NULL) // ���ڿ��� ���ڷ� �ٲ���             
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

