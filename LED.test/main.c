  #include "stm32f4xx.h"

  static void Delay(const uint32_t Count) // 데이터 영역에 잡혀서 초기화하지않고 계속 저장하겠다
                                          // Delay앞에 static은 외부의 delay는 숨겨지고 이 파일의  delay만쓰겠다는
                                          // 의미로 해석이 된다.
                                          // const -> 
                                          // uint32_t ->이거를 쓰면, 다른 아키텍쳐에 가서도 32bit로 쓸 수 있다.
                                          // 타입 변경 불필요..
    {
      __IO uint32_t index = 0; //앞에 __IO 로 쓰면, 범용 레지스터에 잡히게 된다. 지역변수인 stack이 아닌
                               //범용 레지스터에 잡히면 속도가 빨라진다.
                                
      for(index = (16800 * Count); index != 0; index--); //for문을 한번 돌리면 10clock이 필요하다
                                                         // 결국 168MHz
    }


  int main() //set up 부분 -> 한번만 설정하면 됨
    {
      GPIO_InitTypeDef GPIO_InitStructure; //주변장치를 제어하기 위한 구조체 사용
                                          // 모든 주변장치는 클록에 공급하므로써 시작된다
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //port c에 클록을 공급하겠다
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // push, pull down 회로를 사용하겠다 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull up/ pull down
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                              GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //내가 사용하고자하는 pin
      GPIO_Init(GPIOC, &GPIO_InitStructure); //모든 구조체를 설정하겠다(위에있는 설정을)
      //GPIO_Write(GPIOC, 0x55);

    
    while(1) //loop 문 시작
    {
      /*
      GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
                                       GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
      Delay(250);
     
     GPIO_SetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
                                     GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
     Delay(250);
      
     
     GPIO_Write(GPIOC, GPIO_Pin_0); // LED 0을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_1); // LED 1을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_2); // LED 2를 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_3); // LED 3을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_4); // LED 4를 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_5); // LED 5를 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_6); // LED 6을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이 

    GPIO_Write(GPIOC, GPIO_Pin_7); // LED 7을 ON 시킨다. 
    Delay(500); // 0.5초 딜레이

      
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
   