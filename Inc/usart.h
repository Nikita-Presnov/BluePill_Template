#ifndef __USART_H__
#define __USART_H__

void InitUSART(int baudrate);
char USARTReadByte(void);
int USARTReadStr(char *string);
void USARTSendByte(char byte);
void USARTSendStr(char *string);
void USARTSendData(char *data, int len);

#endif /* __USART_H__ */
