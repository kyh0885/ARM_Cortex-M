/******************************************************************************
* Basic timers TIM6

  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
        Array FND����� SA_A ~ SA_H�� �����Ѵ�. (SA_A�� PC0�� ����Ǿ� �Ѵ�.)
    2)  ARM Cortex-M4 ����� ��ƮC (PC8 ~ PC11)�� 4�� ���̺� �̿��ؼ�
        Array FND����� C0 ~ C3�� �����Ѵ�. (C0�� PC8�� ����Ǿ� �Ѵ�.)
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"


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

int t_cnt = 0,m_cnt=0;


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

void TIM6_DAC_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    t_cnt++ ;
    if(t_cnt >= 1000)
    {
      t_cnt = 0;
      m_cnt ++ ;
    }
  }
}


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef   NVIC_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
                                GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //system_stm32f4xx.c ����
  //TIM6 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1),
  //since APB1 prescaler is different from 1.
  //TIM6CLK = 2 * PCLK1
  //PCLK1 = HCLK / 4  => TIM6CLK = HCLK / 2 = SystemCoreClock /2
  //TIM_CounterMode�� TIM_CounterMode_Up�� �����ϸ�, ī��Ʈ���� TIMx_ARR����
  //��������, Update �̺�Ʈ�� �߻��Ѵ�.
  //TIM_Prescaler�� 83����, TIM_ClockDivision�� TIM_CKD_DIV1(1����)��
  //�����ϸ�, Ÿ�̸�6 ī��Ʈ Ŭ���� (84MHz/1) /(83+1) = 1MHz (1us)�̴�.
  //TIM_Period�� 999���� ����������(TIMx_ARR), ī���Ͱ� �̰��� ��������,
  //(1us x 1000(999+1) = 1ms)
  TIM_TimeBaseStructure.TIM_Prescaler = 84-1;         //(168Mhz/2)/84 = 1MHz(1us)
  TIM_TimeBaseStructure.TIM_Period = 1000-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
  //Ÿ�̸�6�� ���۽�Ų��.
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM6, ENABLE);

  while(1)
  {
    Segment(m_cnt);
  }
}
