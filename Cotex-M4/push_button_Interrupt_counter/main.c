#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "uart2.h"
#include<stdio.h>
#include "stm32f4xx_tim.h"


// delay �Լ�
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

// 7-���׸�Ʈ ��Ʈ�� �迭�� �����Ѵ�.
unsigned char Font[18] = {0x3F, 0X06, 0X5B, 0X4F, 
                                       0X66, 0X6D, 0X7C, 0X07,
                                       0X7F, 0X67, 0X77, 0X7C, 
                                       0X39, 0X5E, 0X79, 0X71, 
                                       0X08, 0X80};

unsigned int m_cnt;


void Segment ( int N )  // Segment �Լ� ����
{
  int  i ;
  unsigned char N1000, N100, N10, N1 ;
  int Buff ;

  N1000 = N /1000;  // ���׸�Ʈ���� ����ϴ� õ�� �ڸ��� ����
  Buff = N % 1000 ;
  N100 = Buff / 100 ; // ���׸�Ʈ���� ����ϴ� �����ڸ� ����
  Buff = Buff % 100;
  N10 = Buff /10 ;     // ���׸�Ʈ���� ����ϴ� ���� �ڸ� ����
  N1 =  Buff % 10 ;    // ���׸�Ʈ���� ����ϴ� ���� �ڸ� ����      

  for( i = 0 ; i < 30; i++ )
  {
    GPIO_Write(GPIOC, Font[N1000]|0x0e00); // ���� ù ��° ���׸�Ʈ�� ON�ϰ�, õ�� �ڸ�  ���ڸ� ����� �ش� 
    Delay(1);

    GPIO_Write(GPIOC, Font[N100]|0x0d00); // ���� �� ��° ���׸�Ʈ�� ON�ϰ�, ���� �ڸ�  ���ڸ� ����� �ش� 
    Delay(1); 

    GPIO_Write(GPIOC, Font[N10]|0x0b00);  // ���� �� ��° ���׸�Ʈ�� ON�ϰ�, ���� �ڸ�  ���ڸ� ����� �ش� 
    Delay(1);

    GPIO_Write(GPIOC, Font[N1]|0x0700);   // ���� �� ��° ���׸�Ʈ�� ON�ϰ�, ���� �ڸ�  ���ڸ� ����� �ش� 
    Delay(1); 
  }
}


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                          GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
                                          GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;             //TIM1_CH1
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);

  TIM_TIxExternalClockConfig(TIM1,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0x01); //���Ͱ��� �̿��Ͽ� ������ ����

  TIM_Cmd(TIM1,ENABLE);

  while(1)
  {
    m_cnt = TIM_GetCounter(TIM1);
    Segment(m_cnt);
  }
}
