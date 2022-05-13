# include "main.h"

int main()
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_13,LL_GPIO_MODE_OUTPUT);
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetPrescaler(TIM2, 500 - 1);
    LL_TIM_SetAutoReload(TIM2, 8000 - 1);
    LL_TIM_EnableIT_UPDATE(TIM2);
    LL_TIM_EnableCounter(TIM2);
    // LL_GPIO_SetOutputPin(GPIOG,LL_GPIO_PIN_13,LL_GPIO_OUTPUT_PUSHPULL);
    NVIC_EnableIRQ(TIM2_IRQn);
    while(1);
}

void TIM2_IRQHandler()
{
    LL_TIM_ClearFlag_UPDATE(TIM2);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);
}
