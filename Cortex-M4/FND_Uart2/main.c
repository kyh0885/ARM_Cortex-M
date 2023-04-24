/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
        Array FND����� SA_A ~ SA_H�� �����Ѵ�. (SA_A�� PC0�� ����Ǿ� �Ѵ�.)
    2)  ARM Cortex-M4 ����� ��ƮA (PA0 ~ PA3)�� 4�� ���̺� �̿��ؼ�
        Array FND����� C0 ~ C3�� �����Ѵ�. (C0�� PA0�� ����Ǿ� �Ѵ�.)
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
      GPIO_Write(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);  // ���� ù ��° ���׸�Ʈ ON
      GPIO_Write(GPIOC, Font[N1000]);                                   // õ�� �ڸ� ���ڸ� ����� �ش� 
      Delay(1);            
     
      GPIO_Write(GPIOA,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3); // ���� �� ��° ���׸�Ʈ ON
      GPIO_Write(GPIOC, Font[N100]);                                    // ���� �ڸ� ���ڸ� ����� �ش�      
      Delay(1);  
       
      GPIO_Write(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3); // ���� �� ��° ���׸�Ʈ ON
      GPIO_Write(GPIOC, Font[N10]);                                     // ���� �ڸ� ���ڸ� ����� �ش� 
      Delay(1);
  
      GPIO_Write(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2); // ���� �� ��° ���׸�Ʈ ON
      GPIO_Write(GPIOC, Font[N1]);                                      // ���� �ڸ� ���ڸ� ����� �ش�
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
