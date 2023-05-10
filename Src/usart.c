#include "stm32f1xx.h"
#include "define.h"
#include "usart.h"
#include "clock.h"
// #include "stm32f1xx_ll_rcc.h"
// #include <string.h>
// #define WAIT_TXE while(!((USART1->SR & USART_SR_TXE) == USART_SR_TXE)){}

void InitUSART(int baudrate)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10);
    GPIOA->CRH |= GPIO_CRH_CNF10_0 | GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;

    USART1->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
    USART1->CR2 &= ~USART_CR2_STOP;
    // LL_RCC_ClocksTypeDef RCCClocks;
    // LL_RCC_GetSystemClocksFreq(&RCCClocks);
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    uint32_t fclk = GetPCLK2ClkFreq();
    uint16_t mantissa = fclk / (baudrate * 16);
    uint8_t fraction = (fclk / (baudrate * 16.0) - mantissa) * 16;
    USART1->BRR = (mantissa << 4) | fraction;
    USART1->CR1 |= USART_CR1_UE;

    // NVIC_EnableIRQ(USART1_IRQn);
}

void USARTSendByte(char byte)
{
    while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE))
    {
    }
    USART1->DR = byte;
}

void USARTSendData(char *data, int len)
{
    for (int i = 0; i < len; i++)
    {
        while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE))
        {
        }
        USART1->DR = data[i];
    }
}

void USARTSendData16(uint16_t *data, int len)
{
    for (int i = 0; i < len; i++)
    {
        while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE))
        {
        }
        USART1->DR = data[i];
    }
}

void USARTSendStr(char *string)
{
    char *data;
    data = string;
    for (; *data != '\0'; data++)
    {
        while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE))
        {
        }
        USART1->DR = *data;
    }
}

char USARTReadByte(void)
{
    return USART1->DR;
}

int USARTReadStr(char *string)
{
    char *data;
    data = string;
    int i;
    for (i = 0; *data != '\0'; data++, i++)
    {
        while (!((USART1->SR & USART_SR_RXNE) == USART_SR_RXNE))
        {
        }
        *data = USART1->DR;
    }
    return i;
}