/******************************************************************************
* Basic timers TIM6

  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트C (PC0 ~ PC7)를 8핀 케이블를 이용해서
        Array FND모듈의 SA_A ~ SA_H에 연결한다. (SA_A가 PC0로 연결되야 한다.)
    2)  ARM Cortex-M4 모듈의 포트C (PC8 ~ PC11)를 4핀 케이블를 이용해서
        Array FND모듈의 C0 ~ C3에 연결한다. (C0가 PC8로 연결되야 한다.)
******************************************************************************/
// stm32f4xx의 각 레지스터들을 정의한 헤더파일
#include "stm32f4xx.h"

int t_cnt = 0,m_cnt=0;


void Segment ( int N )  // Segment 함수 선언
{
  int  i ;
  unsigned char N1000, N100, N10, N1 ;
  int Buff ;

  N1000 = N /1000;  // 세그먼트에서 사용하는 천의 자리를 추출
  Buff = N % 1000 ;
  N100 = Buff / 100 ; // 세그먼트에서 사용하는 백의자리 추출
  Buff = Buff % 100;
  N10 = Buff /10 ;     // 세그먼트에서 사용하는 십의 자리 추출
  N1 =  Buff % 10 ;    // 세그먼트에서 사용하는 일의 자리 추출      

  for( i = 0 ; i < 30; i++ )
  {
    GPIO_Write(GPIOC, Font[N1000]|0x0e00); // 왼쪽 첫 번째 세그먼트를 ON하고, 천의 자리  숫자를 출력해 준다 
    Delay(1);

    GPIO_Write(GPIOC, Font[N100]|0x0d00); // 왼쪽 두 번째 세그먼트를 ON하고, 백의 자리  숫자를 출력해 준다 
    Delay(1); 

    GPIO_Write(GPIOC, Font[N10]|0x0b00);  // 왼쪽 세 번째 세그먼트를 ON하고, 십의 자리  숫자를 출력해 준다 
    Delay(1);

    GPIO_Write(GPIOC, Font[N1]|0x0700);   // 왼쪽 네 번째 세그먼트를 ON하고, 일의 자리  숫자를 출력해 준다 
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

  //인터럽트 enable 및 Priority 설정.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //system_stm32f4xx.c 참조
  //TIM6 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1),
  //since APB1 prescaler is different from 1.
  //TIM6CLK = 2 * PCLK1
  //PCLK1 = HCLK / 4  => TIM6CLK = HCLK / 2 = SystemCoreClock /2
  //TIM_CounterMode를 TIM_CounterMode_Up로 설정하면, 카운트값이 TIMx_ARR값과
  //같아질때, Update 이벤트가 발생한다.
  //TIM_Prescaler를 83으로, TIM_ClockDivision를 TIM_CKD_DIV1(1분주)로
  //설정하면, 타이머6 카운트 클럭은 (84MHz/1) /(83+1) = 1MHz (1us)이다.
  //TIM_Period를 999으로 설정했으니(TIMx_ARR), 카운터가 이값과 같아지면,
  //(1us x 1000(999+1) = 1ms)
  TIM_TimeBaseStructure.TIM_Prescaler = 84-1;         //(168Mhz/2)/84 = 1MHz(1us)
  TIM_TimeBaseStructure.TIM_Period = 1000-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
  //타이머6을 동작시킨다.
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM6, ENABLE);

  while(1)
  {
    Segment(m_cnt);
  }
}
