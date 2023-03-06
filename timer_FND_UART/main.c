/******************************************************************************
* Basic timers TIM7

  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
        Array FND����� SA_A ~ SA_H�� �����Ѵ�. (SA_A�� PC0�� ����Ǿ� �Ѵ�.)
    2)  ARM Cortex-M4 ����� ��ƮC (PC8 ~ PC11)�� 4�� ���̺� �̿��ؼ�
        Array FND����� C0 ~ C3�� �����Ѵ�. (C0�� PC8�� ����Ǿ� �Ѵ�.)
******************************************************************************/

#include "stm32f4xx.h"
#include "uart2.h"
#include <stdio.h>
#include <stdlib.h>
#include "tim7_fnd.h"


extern volatile int fndNumber;
extern volatile unsigned char rx2Flag;
extern char rx2Data[50];
int main()
{

  UART2_init();
  TIM7_Fnd_Init();
  fndNumber = 1234;
  printf("start main()\r\n");
  while(1)
  {
      if(rx2Flag)
      {
          fndNumber = atoi(rx2Data);
          rx2Flag = 0;
      }
  }
}
