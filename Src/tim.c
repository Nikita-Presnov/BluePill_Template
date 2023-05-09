#include "stm32f1xx.h"
#include "tim.h"

void InitTIM2(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = PRESCALER - 1;
    TIM2->ARR = AUTORELOAD - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    TIM2->CR1 |= TIM_CR1_CEN;
}