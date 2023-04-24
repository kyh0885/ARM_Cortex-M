//�� ����
//PC12 ~ PC15 : button0 ~ button3(EXTI 12~15)
#include "stm32f4xx.h"
#include "key.h"

volatile int key = 0;
void Key_Init()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;                                 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
  //�ܺ� ���ͷ�Ʈ1�� ��¿������� �߻��ǰ� �����Ѵ�.
  EXTI_InitStructure.EXTI_Line =  EXTI_Line0 |  EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
  
  //��ƮC�� 12~15������ �ܺ� ���ͷ�Ʈ 12~15 �� �����Ѵ�.
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);   
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2); 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);  
  
  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
    
}
void EXTI15_10_IRQHandler(void)
{
 
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    key = 1; //�˶����� ���
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
  else if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    key = 2; 
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
  else if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    key = 3;
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
  else if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    key = 4;
    EXTI_ClearITPendingBit(EXTI_Line3);
  }  
}