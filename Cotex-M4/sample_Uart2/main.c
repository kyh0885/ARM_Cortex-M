#include "stm32f4xx.h"
#include "uart2.h"
#include<stdio.h>

static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}


int main()
{
  
  GPIO_InitTypeDef   GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                             GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; // C ��Ʈ ���� 8��Ʈ�� ������� �����Ѵ�.
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  UART2_init();
  
  printf("start main() \n\r");
  printf("float :%f \n\r",3.14);
  
//  GPIO_Write(GPIOC, 0x55);
  while(1)
  {
     GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_SET);
     Delay(500);
     GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_RESET);
     Delay(500);
/*
    GPIO_ToggleBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                   GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); // C ��Ʈ ���� 8��Ʈ�� ��high'�� ����Ѵ�
    Delay(500); // 500 ms ����
*/
/*
    GPIO_SetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                   GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); // C ��Ʈ ���� 8��Ʈ�� ��high'�� ����Ѵ�
    Delay(500); // 500 ms ����

    GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                      GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); // C ��Ʈ ���� 8��Ʈ�� ��low'�� ����Ѵ�
    Delay(500); // 500 ms ����
  }
*/
/*
    GPIO_Write(GPIOC, GPIO_Pin_0); // LED 0�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_1); // LED 1�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_2); // LED 2�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_3); // LED 3�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_4); // LED 4�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_5); // LED 5�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_6); // LED 6�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_7); // LED 7�� ON ��Ų��. 
    Delay(500); // 0.5�� ������
*/
/*
    int ledVal = 0x01;
    for(int i = 0 ; i<8 ; i++)
    {
      GPIO_Write(GPIOC, ledVal << i); // LED 7�� ON ��Ų��. 
      Delay(100); // 0.5�� ������
    }
    GPIO_Write(GPIOC,0x00);
    Delay(100); // 0.5�� ������
    ledVal = 0x80;
    for(int i = 0 ; i<8 ; i++)
    {
      GPIO_Write(GPIOC, ledVal >> i); // LED 7�� ON ��Ų��. 
      Delay(100); // 0.5�� ������
    }   
    GPIO_Write(GPIOC,0x00);
    Delay(100); // 0.5�� ������
  }
*/

  }
}
