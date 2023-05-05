#include "stm32f1xx.h"
#define AUTORELOAD 8000UL
#define PRESCALER  500UL
void InitTIM2(void)
{
    TIM2->PSC = PRESCALER - 1;
    TIM2->ARR = AUTORELOAD - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    TIM2->CR1 |= TIM_CR1_CEN;
}