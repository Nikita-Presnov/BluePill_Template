#ifndef __USART_H__
#define __USART_H__

void InitUSART(int baudrate);
char ReadByte(void);
int ReadStr(char *string);
void SendByte(char byte);
void SendStr(char *string);
void Send(char *data, int len);

#endif /* __USART_H__ */
