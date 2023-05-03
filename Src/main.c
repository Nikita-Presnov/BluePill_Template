#include "main.h"

// uint16_t testval16;
// uint8_t testval8;
// uint8_t Testfunc8(void)
// {
// 	testval8 = 2;
// 	return testval8;
// }
// void Testfunc(void)
// {
// 	testval16 = 20;
// 	testval16 << 3;
// 	testval16 += Testfunc8();
// }
// void HardFault_Handler(void)
// {
//     while (1)
//     {
//     }
// }
int main()
{
    // Testfunc();
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_USART2EN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    GPIOA->CRL |=
        GPIO_CRL_MODE2_1 |
        GPIO_CRL_CNF2_1 |
        GPIO_CRL_MODE3_1 |
        GPIO_CRL_CNF3_1;
    GPIOA->CRL &= ~(
        GPIO_CRL_MODE2_0 |
        GPIO_CRL_CNF2_0 |
        GPIO_CRL_MODE3_0 |
        GPIO_CRL_CNF3_0);
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
    // LL_USART_SetParity(USART2, LL_USART_PARITY_NONE);
    // LL_USART_SetDataWidth(USART2, LL_USART_DATAWIDTH_8B);
    USART2->CR2 &= ~USART_CR2_STOP;

    uint16_t mantissa = FCLK/(BAUDRATE*16);
    uint8_t fraction = (FCLK/(BAUDRATE*16.0) - mantissa) * 16;
    USART2->BRR = (mantissa << 4) | fraction;

    USART2->CR1 |= USART_CR1_RXNEIE;
    // USART2->CR1 |= USART_CR1_;
    // LL_USART_EnableIT_RXNE(USART2);
    // LL_USART_IsActiveFlag_RXNE
    // while (!((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE))
    // {
    // }
    
    USART2->CR1 |= USART_CR1_UE;

    TIM2->PSC = PRESCALER - 1;
    TIM2->ARR = AUTORELOAD - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    // USART2->SR &= ~USART_SR_RXNE;
    // LL_USART_IsActiveFlag_TXE
    while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
    USART2->DR = 'o';
    while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
    USART2->DR = 'k';
    while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
    USART2->DR = '\n';

    while (1)
    {
        if (((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE))
        {
            while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
            USART2->DR = 'o';
            while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
            USART2->DR = 'k';
            while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
            USART2->DR = '\n';
        }       
    }
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    // uint32_t odr = GPIOC->ODR;
    // GPIOC->BSRR = ((odr & (1 << 13)) << 16) | (~odr & (1 << 13));
}

void USART2_IRQHandler()
{
    uint32_t data = USART2->DR;
    while (!((USART2->SR & USART_SR_TXE) == USART_SR_TXE));
    USART2->DR = data;
    USART2->SR &= ~USART_SR_RXNE;

    uint32_t odr = GPIOC->ODR;
    GPIOC->BSRR = ((odr & (1 << 13)) << 16) | (~odr & (1 << 13));
}