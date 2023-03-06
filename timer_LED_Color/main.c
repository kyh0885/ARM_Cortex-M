/******************************************************************************
* General-purpose timers TIM5

  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮA (PA0 ~ PA2)�� 4�� ���̺� �̿��ؼ�
        Full Color LED����� R, G, B�� �����Ѵ�. (R�� PA0�� ����Ǿ� �Ѵ�.)
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"


// delay �Լ�
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

unsigned RGB_Table[5][3] = {
  //  Red    Green  Blue
  {  163,   191,   64  }, // Green Yellow
  {  255,    69,    0  }, // Orange Red

  {   34,   139,   34  }, // Forest Green
  {  135,   206,  235  }, // Sky Blue
  {  128,     0,  128  }, // Purple
}; 

int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  int i;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;  // TIM5_CH1,TIM5_CH2,TIM5_CH3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);

  //system_stm32f4xx.c ����
  //TIM5 input clock (TIM5CLK) is set to 2 * APB1 clock (PCLK1),
  //since APB1 prescaler is different from 1.
  //TIM5CLK = 2 * PCLK1
  //PCLK1 = HCLK / 4  => TIM5CLK = HCLK / 2 = SystemCoreClock /2
  //TIM_CounterMode�� TIM_CounterMode_Up�� �����ϸ�, ī��Ʈ���� TIMx_ARR����
  //��������, Update �̺�Ʈ�� �߻��Ѵ�.
  //TIM_Prescaler�� 83����, TIM_ClockDivision�� TIM_CKD_DIV1(1����)��
  //�����ϸ�, Ÿ�̸�5 ī��Ʈ Ŭ���� (84MHz/1) /(83+1) = 1MHz (1us)�̴�.
  //TIM_Period�� 254�� ����������(TIMx_ARR), ī���Ͱ� �̰��� ��������,
  //(1us x 255(254+1) = 255us)
  TIM_TimeBaseStructure.TIM_Prescaler = 84-1;         //(168Mhz/2)/84 = 1MHz(1us)
  TIM_TimeBaseStructure.TIM_Period = 255-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     // ���� ���
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;        //�ʱ� Duty �� 100 ���� ����('0' �κκ� ����)
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);     // Red
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);     // Green
  TIM_OC3Init(TIM5, &TIM_OCInitStructure);     // Blue

  //Ÿ�̸�5�� ���۽�Ų��.
  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM5, ENABLE);

  TIM_Cmd(TIM5, ENABLE);

  while(1)
  {
    for(i = 0 ; i < 5 ; i++ )
    {
      TIM_Cmd(TIM5, DISABLE);
      TIM_SetCompare1(TIM5,RGB_Table[i][0]);  // PA0 ��
      TIM_SetCompare2(TIM5,RGB_Table[i][1]);  // PA1 ��
      TIM_SetCompare3(TIM5,RGB_Table[i][2]);  // PA2 ��
      TIM_Cmd(TIM5, ENABLE);
      Delay(1000);
    }
  }
}
