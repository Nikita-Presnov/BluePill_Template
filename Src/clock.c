#include "stm32f1xx.h"

void InitSysClockHSE8(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while(RCC->CR & RCC_CR_HSERDY);
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_0;
    /* HCLK = SYSCLK */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

    /* Select HSE as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSE;

    // RCC->CR |= RCC_CR_HSEON;
    /* Wait till HSE is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
    {
    }
}
void InitSysClockHSE72(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while(RCC->CR & RCC_CR_HSERDY);
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    /* HCLK = SYSCLK */
    RCC->CFGR &=~RCC_CFGR_HPRE;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    RCC->CFGR &=~RCC_CFGR_PPRE2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till PLL is ready */
    while(!(RCC->CR & RCC_CR_PLLRDY));

    /* Select PLL as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != 0x08)
    {
    }
}
