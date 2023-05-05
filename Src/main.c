#include "main.h"

int main()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10);
    GPIOA->CRH |= GPIO_CRH_CNF10_0 | GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;

    USART1->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
    USART1->CR2 &= ~USART_CR2_STOP;
    // LL_RCC_ClocksTypeDef RCCClocks;
    // LL_RCC_GetSystemClocksFreq(&RCCClocks);
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;

    uint16_t mantissa = FCLK/(BAUDRATE*16);
    uint8_t fraction = (FCLK/(BAUDRATE*16.0) - mantissa) * 16;
    USART1->BRR = (mantissa << 4) | fraction;  
    USART1->CR1 |= USART_CR1_UE;

    TIM2->PSC = PRESCALER - 1;
    TIM2->ARR = AUTORELOAD - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE));
    USART1->DR = 'o';
    while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE));
    USART1->DR = 'k';
    while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE));
    USART1->DR = '\n';

    while (1)
    {
    }
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    uint32_t odr = GPIOC->ODR;
    GPIOC->BSRR = ((odr & (1 << 13)) << 16) | (~odr & (1 << 13));
}

void USART1_IRQHandler()
{
    uint32_t data = USART1->DR;
    while (!((USART1->SR & USART_SR_TXE) == USART_SR_TXE));
    USART1->DR = data;
    USART1->SR &= ~USART_SR_RXNE;
}