#ifndef __UART2_H__ //���� ���ǰ� �ȵǾ��ִٸ� �����ض�
#define __UART2_H__

void Serial2_Event();
void Serial2_Send(unsigned char t);
void Serial2_Send_String(char* s);
void UART2_init();
int putchar(int ch);            //printf() ���� �ʿ�
void print_2d1(double number);

#endif