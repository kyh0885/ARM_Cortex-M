#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_tim.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "key.h"
#include "lcd.h"
#include "led.h"

#define CMD_SIZE 50
#define ARR_CNT 5  

extern volatile unsigned long systick_sec;            //1sec
extern volatile int systick_secFlag;
extern volatile unsigned char rx2Flag;
extern char rx2Data[50];
extern volatile unsigned char rx4Flag;
extern char rx4Data[50];
extern int key;
extern volatile char uart4_rxdata[5][100];
long map(long x, long in_min, long in_max, long out_min, long out_max);

typedef struct //시간관련 구조체
  {
    int msec;
    int sec;
    int min;
    int hour;
    char apm;
  }TIME;
 
 void clock_calc(void) //윗줄... normal  상태
{
    int time[10] = {0,};
    TIME.hour= h_cnt;
    TIME.min = m_cnt;
    TIME.sec = s_cnt;
   
     if(h_cnt!= NULL)
    {
      h_cnt++;
      
   
      //min++;
      //sec++;
      //msec++;
      
      hour = atoi();
      
      lcd(0,0,hour);
    }
}  

void clock_set(void)
{
  
  
  if(key != 0)
    {
      if(key == 1) 
      {
        
      }
      else if(key == 2) //motor left
      {
        
      }
      else if(key == 3) //motor stop 
      {
        
      }
      else if(key == 4)
      {
        i
      }
      key = 0;
    } 
  
}
void clock_display(){
  
}
    
int main()
{
  Key_Init();
  LcdPort_Init(); // 교수님이 주신 LCD 시간 출력
  clock_calc(); 
  clock_display();
  clock_set();
}
