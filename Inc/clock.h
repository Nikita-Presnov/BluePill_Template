#ifndef __CLOCK_H__
#define __CLOCK_H__

#define HSICLK 8000000U
#define HSECLK 8000000U
#include <stdint.h>
typedef struct
{
    uint32_t SYSCLK_Freq; /*!< SYSCLK clock frequency */
    uint32_t HCLK_Freq;   /*!< HCLK clock frequency */
    uint32_t PCLK1_Freq;  /*!< PCLK1 clock frequency */
    uint32_t PCLK2_Freq;  /*!< PCLK2 clock frequency */
} RCC_ClkTypeDef;

void InitSysClockHSE8(void);
void InitSysClockHSE72(void);

uint32_t GetSysClk(void);
uint32_t GetHCLKClkFreq(void);
uint32_t GetPCLK1ClkFreq(void);
uint32_t GetPCLK2ClkFreq(void);
void GetClocks(RCC_ClkTypeDef *Clocks);
#endif /* __CLOCK_H__ */
