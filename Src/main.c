#include "main.h"
#include "define.h"
#include "stm32f1xx.h"
#include "usart.h"
int main()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
    InitUSART();
    TIM2->PSC = PRESCALER - 1;
    TIM2->ARR = AUTORELOAD - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
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
    char data = ReadByte();
    SendByte(data);
    USART1->SR &= ~USART_SR_RXNE;
}