/******************************************************************************
* TIM3 & TIM4

  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
        Array FND����� SA_A ~ SA_H�� �����Ѵ�. (SA_A�� PC0�� ����Ǿ� �Ѵ�.)
    2)  ARM Cortex-M4 ����� ��ƮC (PC8 ~ PC11)�� 4�� ���̺� �̿��ؼ�
        Array FND����� C0 ~ C3�� �����Ѵ�. (C0�� PC8�� ����Ǿ� �Ѵ�.)
    3)  ARM Cortex-M4 ����� ��ƮA�� PA6�� 2�� ���̺� �̿��ؼ� 
        ��ƮB�� PB6�� �����Ѵ�.
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

uint16_t IC1ReadValue1 = 0, IC1ReadValue2 = 0;
volatile uint16_t CaptureNumber = 0;
volatile unsigned int c_value;


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

void TIM4_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET) 
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    if(CaptureNumber == 0)
    {
      IC1ReadValue1 = TIM_GetCapture1(TIM4);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
      IC1ReadValue2 = TIM_GetCapture1(TIM4);

      if (IC1ReadValue2 > IC1ReadValue1)
      {
        c_value = (IC1ReadValue2 - IC1ReadValue1); 
      }
      else
      {
        c_value = ((0xFFFF - IC1ReadValue1) + IC1ReadValue2); 
      }
      CaptureNumber = 0;
    }
  }
}


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef   NVIC_InitStructure; 
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_ICInitTypeDef    TIM_ICInitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
                                GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //PWM �����, TIM3 channel 1 pin (PA.06)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;             // TIM3_CH1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //�Է�ĸ����, TIM4 channel 1 pin (PB.06)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;             // TIM4_CH1
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

  // Ÿ�̸�3 PWM ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;           //(168Mhz/2)/1 = 84MHz
  TIM_TimeBaseStructure.TIM_Period = 1000-1;        //(1000(999+1) /84MHz = 11.904us)
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 500;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //Ÿ�̸� 4 �Է� ĸ�� ����
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // ���ֺ� 1 �̸� �ϰ� ������ �߻��Ҷ� ���� �Է� ĸ�İ� �ȴ�.
  TIM_ICInitStructure.TIM_ICFilter = 0x00;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);

  //Ÿ�̸�4�� 3�� ���۽�Ų��.
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
  TIM_Cmd(TIM3, ENABLE);

  while(1)
  {
    Segment(c_value);
  }
}
