/******************************************************************************
  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트C (PC0 ~ PC7)를 8핀 케이블를 이용해서
        Array FND모듈의 SA_A ~ SA_H에 연결한다. (SA_A가 PC0로 연결되야 한다.)
    2)  ARM Cortex-M4 모듈의 포트A (PA0 ~ PA3)를 4핀 케이블를 이용해서
        Array FND모듈의 C0 ~ C3에 연결한다. (C0가 PA0로 연결되야 한다.)
******************************************************************************/
// stm32f4xx의 각 레지스터들을 정의한 헤더파일
#include "stm32f4xx.h"

// delay 함수
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

// 7-세그먼트 폰트를 배열로 지정한다.
unsigned char Font[18] = {0x3F, 0X06, 0X5B, 0X4F, 
                                         0X66, 0X6D, 0X7C, 0X07,
                                         0X7F, 0X67, 0X77, 0X7C, 
                                         0X39, 0X5E, 0X79, 0X71, 
                                         0X08, 0X80};


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
      GPIO_Write(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);  // 왼쪽 첫 번째 세그먼트 ON
      GPIO_Write(GPIOC, Font[N1000]);                                   // 천의 자리 숫자를 출력해 준다 
      Delay(1);            
     
      GPIO_Write(GPIOA,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3); // 왼쪽 두 번째 세그먼트 ON
      GPIO_Write(GPIOC, Font[N100]);                                    // 백의 자리 숫자를 출력해 준다      
      Delay(1);  
       
      GPIO_Write(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3); // 왼쪽 세 번째 세그먼트 ON
      GPIO_Write(GPIOC, Font[N10]);                                     // 십의 자리 숫자를 출력해 준다 
      Delay(1);
  
      GPIO_Write(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2); // 왼쪽 네 번째 세그먼트 ON
      GPIO_Write(GPIOC, Font[N1]);                                      // 일의 자리 숫자를 출력해 준다
      Delay(1);     
  }
}


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  int count = 0;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                             GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  while(1)
  {
    Segment(count); 
    count = count + 1;
    if(count==9999) count = 0;
  }
}
