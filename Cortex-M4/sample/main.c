#include "stm32f4xx.h"
//delay함수
//static 을 함수 앞에 붙이면? 함수를 초기화하지 않고 사용한다, 라이브러리 내에 delay가 있더라도 무시하고 이 파일 내에 있는 Delay함수만 사용하겠다.
//const. 이 코드를 avr로 갔을 때 32비트가 아닌 16비트가 된다.
static void Delay(const uint32_t Count)
{
  //스택(램)에 메모리 잡지 말고 레지스터에 잡기 위함.  volitale 은 컴파일 과정에서 최적화를 하지 않는 것.
  __IO uint32_t index = 0;   
  // 1초를 정확하게 해야하는데 최적화가 되면 10클럭보다 적은 코드를 사용하게 되어 1초 이하가 됨. 따라서 delay함수가 정상적으로 작동할 수 없음
  for(index = (16800*Count); index != 0; index--);
}
int main()
{
  //int delay = 500; 변수 선언하고 사용 안하면 사라짐. 즉 최적화 함. 
  GPIO_InitTypeDef GPIO_InitStructure;  //gpio : 범용 입출력 포트. 구조체 , 스택에 20byte 잡고 쓰레기값이 들어가있음
  //어느 버스에 연결돼있는  gpio인지 알아야함.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); // 주변장치에 클럭을 공급해줘야함. [어디 버스] 의 [어느 장치]를 사용하는가
  //출력, pushpull, 속도, 풀업 풀다운 설정 구조체에 설정값 저장 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //각각의 값이 정해져있으므로 inittypedef gpio 로 들어가서 어떤 값의 열거형인지 알아보면 됨.
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //구조체를 초기화중 , PushPull는 0아니면 1을 보냄.
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//핀이 출력하는 속도
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  
  GPIO_Init(GPIOC, &GPIO_InitStructure);// Base 주소에 initStructure 주소를 넘김
  
  //GPIO_Write(GPIOC, 0x55);
  while(1)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
    Delay(200);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);
    Delay(200);
  }



  /*
    // C 포트 하위 8비트에‘high'를 출력한다.
    GPIO_ToggleBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|  //0을 1로 1을 0으로 바꾼다.
    GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
  // 500 ms 지연
    Delay(500); 
  // C 포트 하위8비트에‘low'를 출력한다.
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
      //500ms 지연
      Delay(250);
      //c포트 하위 8비트에 'low'를 출력한다.
      GPIO_ResetBits(GPIOC, a);
      Delay(250);
      //500ms 지연
      //따라서 한 주기는 1s이며  주파수는 1hz이다.
    }
    for (int i =0; i <= 7 ; i++)
    {
      int a = 0;
      a = GPIO_Pin_7 >> i;
      //gpio_setbits(GPIOC, a );
      GPIO_Write(GPIOC,a); //Write는 명시된 값만 Set , 나머지는 Off 시킴. 
      //500ms 지연
      Delay(250);
      //c포트 하위 8비트에 'low'를 출력한다.
      GPIO_ResetBits(GPIOC, a);
      //500ms 지연
      Delay(250);
      //따라서 한 주기는 1s이며  주파수는 1hz이다.
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
