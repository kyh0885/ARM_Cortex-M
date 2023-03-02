#ifndef __UART4_H__ //만약 정의가 안되어있다면 정의해라
#define __UART4_H__

void Serial_Event();
void Serial_Send(unsigned char t);
void Serial_Send_String(char* s); //문자열을 전송하는 함수
int putchar(int ch); //printf를 위함 함수
void print_2d1(double number); //정수두자리 소수 한자리를 위한 함수선언
void UART4_init();

#endif