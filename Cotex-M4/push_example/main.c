#include "stm32f4xx.h"


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //이미 풀업 풀다운 기능이있다-> 그래서 필요없음
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; // C 포트 하위 8비트를 출력으로 선언한다.
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                                            // A 포트 하위 8비트를 입력으로 선언한다.
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  while(1)
  {
    GPIO_Write(GPIOC,GPIO_ReadInputData(GPIOA)&0x00FF);// A 포트 하위 8비트에 입력된 값을 포트 C의 하위 8비트로 바로 출력한다. 
  }
}
