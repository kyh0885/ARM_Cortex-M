#ifndef __LCD_H__
#define __LCD_H__
#define TLCD_RS  GPIO_Pin_8                  // LCD RS ��ȣ 
#define TLCD_RW  GPIO_Pin_9                  // LCD RW ��ȣ 
#define TLCD_E   GPIO_Pin_10                  // LCD E ��ȣ 
#define LCDDATAPORT     GPIOC                       // LCD ������ ��ȣ 
#define TLCD_EN { GPIO_ResetBits(LCDDATAPORT, TLCD_E); GPIO_SetBits(LCDDATAPORT, TLCD_E); }

#define ON      1
#define OFF     2

#define RIGHT   1
#define LEFT    2

static void Delay(const uint32_t Count);
static void Delay_us(const uint32_t Count);
void LcdPort_Init(void);
// LCD �� ��� ����� �������� �ݵ�� Enable Pulse�� �־�� �Ѵ�.
void E_Pulse(void);
// LCD�� �����͸� �����ϴ� �Լ��̴�   
void TLCD_DATA(unsigned char data);
void Func_Set(void);
// LCD �ʱ�ȭ �Լ�
void Init_LCD(void);
  /******************************************
     lcd_char() : 1 ���ڸ� ���÷��� �ϴ� �Լ�
  *******************************************/
void lcd_char(char s);
void lcd_disp(char x, char y);
void move_disp(char p );
void disp_ON_OFF(char d, char c, char b);
void clrscr(void);
void lcd(char x, char y, char *str);

#endif