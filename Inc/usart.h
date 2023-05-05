#ifndef __USART_H__
#define __USART_H__

void InitUSART(void);
char ReadByte(void);
void SendByte(char byte);
void SendStr(char *string);
void Send(char *data, int len);

#endif /* __USART_H__ */
