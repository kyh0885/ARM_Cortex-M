#include "stm32f4xx.h"

#define TLCD_RS  GPIO_Pin_0                  // LCD RS ��ȣ 
#define TLCD_RW  GPIO_Pin_1                  // LCD RW ��ȣ 
#define TLCD_E   GPIO_Pin_2                  // LCD E ��ȣ 
#define TLCD_EN { GPIO_ResetBits(GPIOB, TLCD_E); GPIO_SetBits(GPIOB, TLCD_E); }
#define DATA     GPIOC                       // LCD ������ ��ȣ 
#define ON      1
#define OFF     2
#define RIGHT   1
#define LEFT    2

// delay �Լ�
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

static void Delay_us(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16 * Count); index != 0; index--);
}

void Port_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                             GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
// LCD �� ��� ����� �������� �ݵ�� Enable Pulse�� �־�� �Ѵ�.
void E_Pulse(void)
{
  GPIO_SetBits(GPIOB, TLCD_E);
  Delay(5);
  GPIO_ResetBits(GPIOB, TLCD_E);
}
  
// LCD�� �����͸� �����ϴ� �Լ��̴�   
void TLCD_DATA(unsigned char data)
{
  GPIO_Write(DATA, data);
  TLCD_EN;    // �� �κп�����  E_Pulse() �Լ� ��� ����Ѵ�.
} 

void Func_Set(void)
{
  GPIO_ResetBits(GPIOB, TLCD_RW);        // RW = 0, RS = 0 �� �� IR���� ���
  GPIO_ResetBits(GPIOB, TLCD_RS); // ����� ������ ������ ���ڴ�
  GPIO_Write(DATA, 0x38);               // �ʱ�ȭ�� ���� Function Set
  E_Pulse();                            // Enable Pulse
}

// LCD �ʱ�ȭ �Լ�
void Init_LCD(void)
{
  GPIO_ResetBits(GPIOB, TLCD_E);
  Delay(15);                    // �� 15ms �ð� ����
  Func_Set();
  Delay(10);                    // �� 10ms �ð� ����
  Func_Set();
  Delay_us(150);                // �� 150us �ð� ����
  Func_Set();
  GPIO_Write(DATA, 0x0f);       // ���÷��� ON, Ŀ�� ON 
  E_Pulse();
  GPIO_Write(DATA, 0x06);       // ���ڸ� ���������� ����Ʈ ��Ű�� ��� 
  E_Pulse();
  GPIO_Write(DATA, 0x01);       // ȭ�� Ŭ����
  E_Pulse();
}
  
  /******************************************
     lcd_char() : 1 ���ڸ� ���÷��� �ϴ� �Լ�
  *******************************************/
void lcd_char(char s)
{
  GPIO_SetBits(GPIOB, TLCD_RS);
  GPIO_Write(DATA, s);
  E_Pulse();
}

void lcd_disp(char x, char y)
{
  GPIO_ResetBits(GPIOB, TLCD_RS);
  GPIO_ResetBits(GPIOB, TLCD_RW);
  if(y==0) GPIO_Write(DATA, x+0x80);
  else if(y==1) GPIO_Write(DATA, x+0xc0);
  E_Pulse();
 }
 
void move_disp(char p )
{
  GPIO_ResetBits(GPIOB, TLCD_RS);
  GPIO_ResetBits(GPIOB, TLCD_RW);

  if(p==LEFT ) GPIO_Write(DATA, 0x18);
  else if(p==RIGHT) GPIO_Write(DATA, 0x1c);

  E_Pulse();
}

void disp_ON_OFF(char d, char c, char b)
{
  char disp = 0x08;

  GPIO_ResetBits(GPIOB, TLCD_RS);
  GPIO_ResetBits(GPIOB, TLCD_RW);

  if (d==ON) d = 0x04;
  else         d = 0x00;

  if(c == ON) c = 0x02;
  else          c = 0x00;

  if(b == ON) b = 0x01;
  else          b = 0x00;

  GPIO_Write(DATA, disp | d | c| b);
  E_Pulse();
            
}   
 
void clrscr(void)
{
  GPIO_ResetBits(GPIOB, TLCD_RS);
  GPIO_ResetBits(GPIOB, TLCD_RW);
  GPIO_Write(DATA, 0x01);
  E_Pulse();

  Delay(10);
} 


 /***************************************************************
    LCD �� ��ġ�� �����Ͽ� ���ڿ��� ����� �� �ֵ��� ���� �Լ��̴� 
 ****************************************************************/
void lcd(char x, char y, char *str)
{
  lcd_disp(x, y);
  while(*str) lcd_char(*str++);
}


int main()
{
  int i;
  Port_Init();
  Init_LCD();
  UART2_init();
  
  printf("start main() \n\r");
  while(1)
  {
    clrscr();  //  ȭ�� Ŭ���� 
    lcd(0,0,"HBE-Multi II");    // ���ڿ� ���
    lcd(0,1,"BASE PROGRAM"); // ���ڿ� ���
    Delay(1000);           // �����ð� ����

    // ���÷��� ON/OFF�� ������ ���鼭 5�� ���� 
    for(i = 0; i < 5 ; i++ )
    {
      disp_ON_OFF(OFF, ON, ON);  // ���÷��� OFF 
      Delay(1000);

      disp_ON_OFF(ON, ON, ON);  // ���÷��� ON 
      Delay(1000);
    }  

    // ���ڿ� ��ü�� ������/�������� �� 10ȸ�� �̵� 
    for ( i = 0 ; i < 20 ; i ++) 
    {
      if(i < 10) move_disp(RIGHT); // ���÷��� ������ ����Ʈ 
      else      move_disp(LEFT); // ���÷��� ���� ����Ʈ 
      Delay(1000);                
    }
    lcd_disp(0,0);   // Ŀ����ġ �ʱ�ȭ 

    disp_ON_OFF(ON,ON,OFF); // Ŀ�� ������ 
    Delay(1000);            // ���� �ð� ���� 

    disp_ON_OFF(ON, ON, ON); // Ŀ�� ������ ON
    Delay(1000);             // ���� �ð� ���� 

    clrscr();                     // ȭ�� Ŭ����

    lcd(0,1," End of Demo ");    // ���� ���ڿ� ��� 
    Delay(1000);             // �� 1�ʰ� �ð� ���� 
  }
}

