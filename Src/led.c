#include "stm32f1xx.h"
void InitLed(void)
{
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}
void SetLedHigh(void)
{
    GPIOC->BSRR = (1 << 13) << 16;
}
void SetLedLow(void)
{
    GPIOC->BSRR = (1 << 13);
}
void ToggleLed(void)
{
    uint32_t odr = GPIOC->ODR;
    GPIOC->BSRR = ((odr & (1 << 13)) << 16) | (~odr & (1 << 13));
}