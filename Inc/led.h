#ifndef __LED_H__
#define __LED_H__
#include "stm32f1xx.h"

#define SETLESHIGH (GPIOC->BSRR = (1 << 13) << 16)
#define SETLEDLOW  (GPIOC->BSRR = (1 << 13))

void InitLed(void);
// void SetLedHigh(void);
// void SetLedLow(void);

void ToggleLed(void);


#endif /* __LED_H__ */
