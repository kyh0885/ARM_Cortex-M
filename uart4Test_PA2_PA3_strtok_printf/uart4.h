#ifndef __UART4_H__ //���� ���ǰ� �ȵǾ��ִٸ� �����ض�
#define __UART4_H__

void Serial_Event();
void Serial_Send(unsigned char t);
void Serial_Send_String(char* s); //���ڿ��� �����ϴ� �Լ�
int putchar(int ch); //printf�� ���� �Լ�
void print_2d1(double number); //�������ڸ� �Ҽ� ���ڸ��� ���� �Լ�����
void UART4_init();

#endif