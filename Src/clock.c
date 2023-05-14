#include "stm32f1xx.h"
#include "clock.h"

// uint32_t SystemCoreClock = 8000000;
// const uint8_t AHBPrescallerTable[16U] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
// const uint8_t APBPrescallerTable[8U] =  {0, 0, 0, 0, 1, 2, 3, 4};

#define GetHCLKFreq(__SysCLKFreq, __AHBPrescaller) ((__SysCLKFreq) >> AHBPrescTable[((__AHBPrescaller) & RCC_CFGR_HPRE) >> 4U])
#define GetPCLK1Freq(__HCLKFreq, __APB1Prescaller) ((__HCLKFreq) >> APBPrescTable[(__APB1Prescaller) >> 8U])
#define GetPCLK2Freq(__HCLKFreq, __APB2Prescaller) ((__HCLKFreq) >> APBPrescTable[(__APB2Prescaller) >> 11U])
#define GetPLLCLKFreq(__INPUTFREQ, __PLLMUL) ((__INPUTFREQ) * (((__PLLMUL) >> RCC_CFGR_PLLMULL_Pos) + 2U))

void InitSysClockHSE8(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while (RCC->CR & RCC_CR_HSERDY)
    {
    }
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
    while (RCC->CR & RCC_CR_HSERDY)
    {
    }
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    /* HCLK = SYSCLK */
    RCC->CFGR &= ~RCC_CFGR_HPRE;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till PLL is ready */
    while (!(RCC->CR & RCC_CR_PLLRDY))
    {
    }
    /* Select PLL as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != 0x08)
    {
    }
}

uint32_t __GetFreqDomain(void)
{
    uint32_t pllinputfreq = 0U, pllsource = 0U;

    /* PLL_VCO = (HSE_VALUE, HSI_VALUE or PLL2 / PLL Predivider) * PLL Multiplicator */

    /* Get PLL source */
    pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

    switch (pllsource)
    {
    case 0x00000000U: /* HSI used as PLL clock source */
        pllinputfreq = HSICLK / 2U;
        break;

    case RCC_CFGR_PLLSRC: /* HSE used as PLL clock source */
        pllinputfreq = HSECLK / (((RCC->CFGR & RCC_CFGR_PLLXTPRE) >> RCC_CFGR_PLLXTPRE_Pos) + 1U);
        break;

        // #if defined(RCC_PLL2_SUPPORT)
        //     case LL_RCC_PLLSOURCE_PLL2:       /* PLL2 used as PLL clock source */
        //       pllinputfreq = RCC_PLL2_GetFreqClockFreq() / (LL_RCC_PLL_GetPrediv() + 1U);
        //       break;
        // #endif /* RCC_PLL2_SUPPORT */

    default:
        pllinputfreq = HSECLK / 2U;
        break;
    }
    return GetPLLCLKFreq(pllinputfreq, RCC->CFGR & RCC_CFGR_PLLMULL);
}

uint32_t GetSysClk(void)
{
    switch (RCC->CFGR & RCC_CFGR_SWS)
    {
    case RCC_CFGR_SWS_HSI: /* HSI used as system clock  source */
        return HSICLK;

    case RCC_CFGR_SWS_HSE: /* HSE used as system clock  source */
        return HSECLK;

    case RCC_CFGR_SWS_PLL: /* PLL used as system clock  source */
        return __GetFreqDomain();

    default:
        return HSICLK;
    }
}
uint32_t GetHCLKClkFreq(void)
{
    return GetHCLKFreq(
        GetSysClk(), 
        RCC->CFGR & RCC_CFGR_HPRE);
}

uint32_t GetPCLK1ClkFreq(void)
{
    return GetPCLK1Freq(
        GetHCLKFreq(
            GetSysClk(), 
            RCC->CFGR & RCC_CFGR_HPRE), 
        RCC->CFGR & RCC_CFGR_PPRE1);
}

uint32_t GetPCLK2ClkFreq(void)
{
    return GetPCLK2Freq(
        GetPCLK1Freq(
            GetHCLKFreq(
                GetSysClk(), 
                RCC->CFGR & RCC_CFGR_HPRE), 
            RCC->CFGR & RCC_CFGR_PPRE1), 
        RCC->CFGR & RCC_CFGR_PPRE2);
}

void GetClocks(RCC_ClkTypeDef *Clocks)
{
    Clocks->SYSCLK_Freq = GetSysClk();
    Clocks->HCLK_Freq = GetHCLKFreq(Clocks->SYSCLK_Freq, RCC->CFGR & RCC_CFGR_HPRE);
    Clocks->PCLK1_Freq = GetPCLK1Freq(Clocks->HCLK_Freq, RCC->CFGR & RCC_CFGR_PPRE1);
    Clocks->PCLK2_Freq = GetPCLK2Freq(Clocks->HCLK_Freq, RCC->CFGR & RCC_CFGR_PPRE2);
}
