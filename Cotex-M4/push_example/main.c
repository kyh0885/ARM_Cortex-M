#include "stm32f4xx.h"


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //�̹� Ǯ�� Ǯ�ٿ� ������ִ�-> �׷��� �ʿ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; // C ��Ʈ ���� 8��Ʈ�� ������� �����Ѵ�.
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                                            // A ��Ʈ ���� 8��Ʈ�� �Է����� �����Ѵ�.
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  while(1)
  {
    GPIO_Write(GPIOC,GPIO_ReadInputData(GPIOA)&0x00FF);// A ��Ʈ ���� 8��Ʈ�� �Էµ� ���� ��Ʈ C�� ���� 8��Ʈ�� �ٷ� ����Ѵ�. 
  }
}
