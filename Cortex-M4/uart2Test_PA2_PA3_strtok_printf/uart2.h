#ifndef __UART2_H__ //만약 정의가 안되어있다면 정의해라
#define __UART2_H__

void Serial2_Event();
void Serial2_Send(unsigned char t);
void Serial2_Send_String(char* s);
void UART2_init();
int putchar(int ch);            //printf() 사용시 필요
void print_2d1(double number);

#endif