# include "main.h"

int main()
{
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
        GPIO_CRL_CNF3_0
    );
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 &= ~(USART_CR1_PCE | USART_CR1_M);
    USART2->CR2 &= USART_CR2_STOP;
    // LL_RCC_GetSystemClocksFreq
    // LL_USART_SetBaudRate()
    // LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
    USART2->BRR = (FCLK/(BAUDRATE*16) << 4) | FCLK%(BAUDRATE*16);
    USART2->CR1 |= USART_CR1_RXNEIE;
    USART2->CR1 |= USART_CR1_UE;

    TIM2->PSC = PRESCALER - 1;
    TIM2->ARR = AUTORELOAD - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    while(1);
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    uint32_t odr = GPIOC->ODR;
    GPIOC->BSRR = ((odr & (1 << 13)) << 16) | (~odr & (1 << 13));
}

void USART2_IRQHandler()
{
    USART2->SR &= ~USART_SR_RXNE;
    uint32_t data = USART2->DR;
}