  #include "stm32f4xx.h"

  static void Delay(const uint32_t Count) // ������ ������ ������ �ʱ�ȭ�����ʰ� ��� �����ϰڴ�
                                          // Delay�տ� static�� �ܺ��� delay�� �������� �� ������  delay�����ڴٴ�
                                          // �ǹ̷� �ؼ��� �ȴ�.
                                          // const -> 
                                          // uint32_t ->�̰Ÿ� ����, �ٸ� ��Ű���Ŀ� ������ 32bit�� �� �� �ִ�.
                                          // Ÿ�� ���� ���ʿ�..
    {
      __IO uint32_t index = 0; //�տ� __IO �� ����, ���� �������Ϳ� ������ �ȴ�. ���������� stack�� �ƴ�
                               //���� �������Ϳ� ������ �ӵ��� ��������.
                                
      for(index = (16800 * Count); index != 0; index--); //for���� �ѹ� ������ 10clock�� �ʿ��ϴ�
                                                         // �ᱹ 168MHz
    }


  int main() //set up �κ� -> �ѹ��� �����ϸ� ��
    {
      GPIO_InitTypeDef GPIO_InitStructure; //�ֺ���ġ�� �����ϱ� ���� ����ü ���
                                          // ��� �ֺ���ġ�� Ŭ�Ͽ� �����ϹǷν� ���۵ȴ�
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //port c�� Ŭ���� �����ϰڴ�
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push, pull down ȸ�θ� ����ϰڴ� 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull up/ pull down
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                              GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //���� ����ϰ����ϴ� pin
      GPIO_Init(GPIOC, &GPIO_InitStructure); //��� ����ü�� �����ϰڴ�(�����ִ� ������)
      //GPIO_Write(GPIOC, 0x55);

    
    while(1) //loop �� ����
    {
      /*
      GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
                                       GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
      Delay(250);
     
     GPIO_SetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
                                     GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
     Delay(250);
      
     
     GPIO_Write(GPIOC, GPIO_Pin_0); // LED 0�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_1); // LED 1�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_2); // LED 2�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_3); // LED 3�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_4); // LED 4�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_5); // LED 5�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_6); // LED 6�� ON ��Ų��. 
    Delay(500); // 0.5�� ������ 

    GPIO_Write(GPIOC, GPIO_Pin_7); // LED 7�� ON ��Ų��. 
    Delay(500); // 0.5�� ������

      
    int ledVal = 0x01;
    for (int i=0; i<8; i++)
    {
      GPIO_Write(GPIOC, ledVal<<i);
      Delay(500); 
    }
      GPIO_Write(GPIOC, 0x00);
      Delay(500); 
      ledVal = 0x80;
    for (int i=0; i<8; i++)
    {
      GPIO_Write(GPIOC, ledVal>>i);
      Delay(500); 
    }
      GPIO_Write(GPIOC, 0x00);
      Delay(500);
      
      GPIO_ToggleBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
                                       GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
      Delay(250);*/
      
      GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
      Delay(500);
      GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET);
      Delay(500);
     
    }
   }
   