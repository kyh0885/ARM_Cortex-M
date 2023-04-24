#include "stm32f4xx.h"
//delay�Լ�
//static �� �Լ� �տ� ���̸�? �Լ��� �ʱ�ȭ���� �ʰ� ����Ѵ�, ���̺귯�� ���� delay�� �ִ��� �����ϰ� �� ���� ���� �ִ� Delay�Լ��� ����ϰڴ�.
//const. �� �ڵ带 avr�� ���� �� 32��Ʈ�� �ƴ� 16��Ʈ�� �ȴ�.
static void Delay(const uint32_t Count)
{
  //����(��)�� �޸� ���� ���� �������Ϳ� ��� ����.  volitale �� ������ �������� ����ȭ�� ���� �ʴ� ��.
  __IO uint32_t index = 0;   
  // 1�ʸ� ��Ȯ�ϰ� �ؾ��ϴµ� ����ȭ�� �Ǹ� 10Ŭ������ ���� �ڵ带 ����ϰ� �Ǿ� 1�� ���ϰ� ��. ���� delay�Լ��� ���������� �۵��� �� ����
  for(index = (16800*Count); index != 0; index--);
}
int main()
{
  //int delay = 500; ���� �����ϰ� ��� ���ϸ� �����. �� ����ȭ ��. 
  GPIO_InitTypeDef GPIO_InitStructure;  //gpio : ���� ����� ��Ʈ. ����ü , ���ÿ� 20byte ��� �����Ⱚ�� ������
  //��� ������ ������ִ�  gpio���� �˾ƾ���.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); // �ֺ���ġ�� Ŭ���� �����������. [��� ����] �� [��� ��ġ]�� ����ϴ°�
  //���, pushpull, �ӵ�, Ǯ�� Ǯ�ٿ� ���� ����ü�� ������ ���� 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //������ ���� �����������Ƿ� inittypedef gpio �� ���� � ���� ���������� �˾ƺ��� ��.
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //����ü�� �ʱ�ȭ�� , PushPull�� 0�ƴϸ� 1�� ����.
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//���� ����ϴ� �ӵ�
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  
  GPIO_Init(GPIOC, &GPIO_InitStructure);// Base �ּҿ� initStructure �ּҸ� �ѱ�
  
  //GPIO_Write(GPIOC, 0x55);
  while(1)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
    Delay(200);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);
    Delay(200);
  }



  /*
    // C ��Ʈ ���� 8��Ʈ����high'�� ����Ѵ�.
    GPIO_ToggleBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|  //0�� 1�� 1�� 0���� �ٲ۴�.
    GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
  // 500 ms ����
    Delay(500); 
  // C ��Ʈ ����8��Ʈ����low'�� ����Ѵ�.
  */

    /*GPIO_Write(GPIOC, 0x00);
    Delay(500);
    int ledVal = 0x01;
    for(int i = 0; i <8 ; i++)
    {
    GPIO_Write(GPIOC, ledVal << i);
    Delay(500);
    }
    GPIO_Write(GPIOC, 0x00);
    Delay(500);
    ledVal = 0x80;
    for(int i = 0; i <8; i++)
    {
      GPIO_Write(GPIOC, ledVal >> i);
      Delay(500);
    }*/
    
   /*for (int i =0; i <= 7 ; i++)
    {
      int a = 0;
      a = GPIO_Pin_0 << i; 
      GPIO_SetBits(GPIOC, a );
      //500ms ����
      Delay(250);
      //c��Ʈ ���� 8��Ʈ�� 'low'�� ����Ѵ�.
      GPIO_ResetBits(GPIOC, a);
      Delay(250);
      //500ms ����
      //���� �� �ֱ�� 1s�̸�  ���ļ��� 1hz�̴�.
    }
    for (int i =0; i <= 7 ; i++)
    {
      int a = 0;
      a = GPIO_Pin_7 >> i;
      //gpio_setbits(GPIOC, a );
      GPIO_Write(GPIOC,a); //Write�� ��õ� ���� Set , �������� Off ��Ŵ. 
      //500ms ����
      Delay(250);
      //c��Ʈ ���� 8��Ʈ�� 'low'�� ����Ѵ�.
      GPIO_ResetBits(GPIOC, a);
      //500ms ����
      Delay(250);
      //���� �� �ֱ�� 1s�̸�  ���ļ��� 1hz�̴�.
    }*/
  /* int a = GPIO_Pin_0;
  for(int i=-8; i<8; i++){
      GPIO_Write(GPIOC,a);
     if(i<0) a = a<<1;
      else if(i==0) GPIO_ResetBits(GPIOC, a);
     else if(i>0) a=a>>1;
     Delay(200);
   }
  }*/
 
}
